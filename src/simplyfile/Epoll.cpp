#include "Epoll.h"
#include "Event.h"

#include "utils/Finally.h"
#include "utils/demangle.h"

#include <map>
#include <vector>
#include <atomic>

#include <errno.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>

namespace simplyfile
{

struct CallbackInfo {
	Epoll::Callback cb;

	bool tearDownFlag {false};
	int numExecutors {0};

	std::mutex mutex;
	std::condition_variable cv {};

	bool startExecution() {
		std::lock_guard lock(mutex);
		if (tearDownFlag) {
			return false;
		}
		++numExecutors;
		return true;
	}

	void stopExecution() {
		std::lock_guard lock(mutex);
		--numExecutors;
		if (numExecutors == 0  and tearDownFlag) {
			cv.notify_all();
		}
	}

	void tearDown(bool blocking) {
		std::unique_lock lock(mutex);
		tearDownFlag = true;
		if (blocking) {
			while (numExecutors > 0) {
				cv.wait(lock);
			}
		}
	}
};

struct Epoll::Pimpl {
	std::shared_mutex infosMutex;
	std::map<int, std::shared_ptr<CallbackInfo>> infos;
	Event eventFD{EFD_SEMAPHORE};
};

Epoll::Epoll()
	: FileDescriptor(epoll_create1(0))
	, pimpl(new Pimpl)
{
	if (not valid()) {
		throw std::runtime_error("cannot create epollfd");
	}
	addFD(pimpl->eventFD, [=](int){
		pimpl->eventFD.get();
		modFD(pimpl->eventFD, EPOLLIN|EPOLLONESHOT);
	}, EPOLLIN|EPOLLONESHOT);
}

Epoll::Epoll(Epoll &&other)
	: FileDescriptor(std::move(other)) {
	std::swap(pimpl, other.pimpl);
}

Epoll& Epoll::operator=(Epoll &&rhs) {
	FileDescriptor::operator =(std::move(rhs));
	std::swap(pimpl, rhs.pimpl);
	return *this;
}

Epoll::~Epoll() {}

void Epoll::addFD(int _fd, Callback const& callback, int epollFlags) {
	std::shared_ptr<CallbackInfo> info;
	{
		std::lock_guard lock{pimpl->infosMutex};
		info = pimpl->infos[_fd] = std::make_shared<CallbackInfo>(); // this and the next line could be a beautiful one liner if...gcc5.4 would not be used
		pimpl->infos.at(_fd)->cb = callback;
	}
	struct epoll_event event {};
	event.events = epollFlags;
	event.data.fd = _fd;
	if (epoll_ctl(*this, EPOLL_CTL_ADD, _fd, &event) == -1) {
		std::lock_guard lock{pimpl->infosMutex};
		pimpl->infos.erase(_fd); // revert the add
		throw std::runtime_error("cannot call epoll_ctl EPOLL_CTL_ADD " + std::string(strerror(errno)));
	}
}

void Epoll::modFD(int _fd, int epollFlags) {
	struct epoll_event event {};
	event.events = epollFlags;
	event.data.fd = _fd;
	if (epoll_ctl(*this, EPOLL_CTL_MOD, _fd, &event) == -1) {
		throw std::runtime_error("cannot call epoll_ctl EPOLL_CTL_MOD " + std::string(strerror(errno)));
	}
}

void Epoll::rmFD(int _fd, bool blocking) {
	std::shared_ptr<CallbackInfo> info;
	{
		std::lock_guard lock{pimpl->infosMutex};
		auto it = pimpl->infos.find(_fd);
		if (it != pimpl->infos.end()) {
			info = it->second;
		}
	}

	if (info) {
		info->tearDown(blocking);
		{
			std::lock_guard lock{pimpl->infosMutex};
			pimpl->infos.erase(_fd);
			if (epoll_ctl(*this, EPOLL_CTL_DEL, _fd, nullptr) == -1) {
				throw std::runtime_error("cannot call epoll_ctl EPOLL_CTL_DEL " + std::string(strerror(errno)));
			}
		}
	}
}

std::vector<struct epoll_event> Epoll::wait(int maxEvents, int timeout_ms) {
	std::vector<struct epoll_event> events(maxEvents);
	int num = epoll_wait(*this, events.data(), events.size(), timeout_ms);
	if (num >= 0) {
		events.resize(num);
	} else {
		events.resize(0);
//		throw std::runtime_error("epoll wait returned -1 " + std::string(std::strerror(errno)));
	}
	return events;
}

void Epoll::dispatch(std::vector<struct epoll_event> const& events) {
	struct Wrapper {
		std::shared_ptr<CallbackInfo> info;
		struct epoll_event event;
	};

	std::vector<Wrapper> wrappers;
	wrappers.reserve(events.size());
	{
		// fetch the callback handles as the container map might be modified while we access it
		std::shared_lock lock{pimpl->infosMutex};
		for (auto const& event : events) {
			auto cbIt = pimpl->infos.find(event.data.fd);
			if (cbIt != pimpl->infos.end()) {
				wrappers.emplace_back(Wrapper{cbIt->second, event});
			}
		}
	}
	for (auto const& wrapper : wrappers) {
		if (wrapper.info->startExecution()) {
			auto finally = makeFinally([=]{wrapper.info->stopExecution();});
			try {
				wrapper.info->cb(wrapper.event.events);
			} catch (...) {
				std::throw_with_nested(std::runtime_error(demangle(wrapper.info->cb.target_type()) + " threw and exception:"));
			}
		}
	}
}

void Epoll::wakeup(uint64_t count) {
	pimpl->eventFD.put(count);
}

}
