#pragma once

#include "FileDescriptor.h"

#include <chrono>
#include <sys/timerfd.h>

namespace simplyfile {

struct Timer : FileDescriptor {
	using FileDescriptor::FileDescriptor;
	Timer(std::chrono::nanoseconds duration, bool oneShot=false, int flags=TFD_NONBLOCK);

    Timer(Timer&&) noexcept = default;
    Timer& operator=(Timer&&) noexcept = default;

	int getElapsed() const;
	void cancel();
	void reset(std::chrono::nanoseconds duration, bool oneShot=false);
};

}
