#pragma once

#include "FileDescriptor.h"

#include <memory>
#include <sys/epoll.h>
#include <chrono>

namespace simplyfile
{

struct Epoll : FileDescriptor {
	using Callback = std::function<void(int)>;

	Epoll();
	Epoll(Epoll &&other);
	Epoll& operator=(Epoll &&rhs);
	~Epoll();

	void addFD(int fd, Callback const& callback, int epollFlags = EPOLLIN|EPOLLET, std::string const& name="");
	void modFD(int fd, int epollFlags = EPOLLIN|EPOLLET);
	void rmFD(int fd, bool blocking);

	void work(int maxEvents=1, int timeout_ms=-1) {
		dispatch(wait(maxEvents, timeout_ms));
	}

	// call epoll_wait internally and return the list of events
	std::vector<struct epoll_event> wait(int maxEvents=32, int timeout_ms=-1);

	// call all callbacks of the event list
	void dispatch(std::vector<struct epoll_event> const&);

	// wakes up count thread that is calling wait
	void wakeup(uint64_t count=1);

	struct RuntimeInfo {
		std::chrono::nanoseconds accumulatedRuntime {0};
		int64_t numExecutions {0};
		RuntimeInfo& operator+=(RuntimeInfo const& rhs) {
			accumulatedRuntime += rhs.accumulatedRuntime;
			numExecutions += rhs.numExecutions;
			return *this;
		}
		RuntimeInfo& operator-=(RuntimeInfo const& rhs) {
			accumulatedRuntime -= rhs.accumulatedRuntime;
			numExecutions -= rhs.numExecutions;
			return *this;
		}

		RuntimeInfo operator+(RuntimeInfo const& rhs) const {
			RuntimeInfo info{*this};
			info.accumulatedRuntime += rhs.accumulatedRuntime;
			info.numExecutions += rhs.numExecutions;
			return info;
		}

		RuntimeInfo operator-(RuntimeInfo const& rhs) const {
			RuntimeInfo info{*this};
			info.accumulatedRuntime -= rhs.accumulatedRuntime;
			info.numExecutions -= rhs.numExecutions;
			return info;
		}
	};
	std::map<std::string, RuntimeInfo> getRuntimes() const;
private:
	struct Pimpl;
	std::unique_ptr<Pimpl> pimpl;
};

}
