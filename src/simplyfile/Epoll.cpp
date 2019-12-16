#include "Epoll.h"
#include "Event.h"

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


#include <cxxabi.h>
#include <iostream>


namespace
{

std::string demangle(std::type_info const& ti) {
	int status;
	char* name_ = abi::__cxa_demangle(ti.name(), 0, 0, &status);
	if (status) {
		throw std::runtime_error("cannot demangle a type!");
	}
	std::string demangledName{ name_ };
	free(name_); // need to use free here :/
	return demangledName;
}

std::string removeAnonNamespace(std::string const& in_str) {
	size_t index = 0;
	std::string str = in_str;
	std::string toReplace = "(anonymous namespace)::";
	while (true) {
	     index = str.find(toReplace, index);
	     if (index == std::string::npos) {
	    	 break;
	     }
	     str.replace(index, toReplace.size(), "");
	     index += toReplace.size();
	}
	return str;
}


template<typename Func>
struct Finally final {
	Finally(Func && f) : _f(f) {}
	~Finally() {_f();}
private:
	Func _f;
};
template<typename Func>
Finally(Func) -> Finally<Func>;

}


namespace simplyfile
{

struct CallbackInfo {
	Epoll::Callback cb;

	bool tearDownFlag {false};
	int numExecutors {0};

	std::mutex mutex;
	std::condition_variable cv {};

	std::string name;
	std::mutex runtimeMutex;
	Epoll::RuntimeInfo accumulatedRuntime;

	CallbackInfo(std::string const& _name) : name{_name} {}

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
	std::map<std::string, std::atomic<std::chrono::nanoseconds>> runtimes;
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
	}, EPOLLIN|EPOLLONESHOT, "self_wakeup");
}

Epoll::Epoll(Epoll &&other) noexcept
	: FileDescriptor(std::move(other)) {
	std::swap(pimpl, other.pimpl);
}

Epoll& Epoll::operator=(Epoll &&rhs) noexcept {
	FileDescriptor::operator =(std::move(rhs));
	std::swap(pimpl, rhs.pimpl);
	return *this;
}

Epoll::~Epoll() {}

void Epoll::addFD(int _fd, Callback callback, int epollFlags, std::string const& name) {
	std::shared_ptr<CallbackInfo> info;
	std::string _name = name;
	if (_name == "") {
		_name = removeAnonNamespace(demangle(callback.target_type()));
	}
	{
		std::lock_guard lock{pimpl->infosMutex};
		info = pimpl->infos[_fd] = std::make_shared<CallbackInfo>(_name); // this and the next line could be a beautiful one liner if...gcc5.4 would not be used
		info->cb = std::move(callback);
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
	std::vector<epoll_event> events(maxEvents, epoll_event{});
	int num = epoll_wait(*this, events.data(), events.size(), timeout_ms);
	if (num >= 0) {
		events.resize(num);
	} else {
		events.resize(0);
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
			try {
				auto start = std::chrono::high_resolution_clock::now();
				Finally finally{[=] {
					auto end = std::chrono::high_resolution_clock::now();
					{
						std::lock_guard lock{wrapper.info->runtimeMutex};
						Epoll::RuntimeInfo delta{end - start, 1};
						wrapper.info->accumulatedRuntime += delta;
					}
					wrapper.info->stopExecution();
				}};
				wrapper.info->cb(wrapper.event.events);
			} catch (...) {
				std::throw_with_nested(std::runtime_error(removeAnonNamespace(demangle(wrapper.info->cb.target_type())) + " threw an exception:"));
			}
		}
	}
}

void Epoll::wakeup(uint64_t count) noexcept {
	pimpl->eventFD.put(count);
}

auto Epoll::getRuntimes() const -> std::map<std::string, RuntimeInfo> {
	std::map<std::string, RuntimeInfo> runtimes;
	std::shared_lock lock{pimpl->infosMutex};
	for (auto const& info : pimpl->infos) {
		std::lock_guard lock{info.second->runtimeMutex};
		runtimes[info.second->name] += info.second->accumulatedRuntime;
	}
	return runtimes;
}


}
