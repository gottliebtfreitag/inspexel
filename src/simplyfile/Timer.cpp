#include "Timer.h"

#include <cstring>
#include <time.h>


namespace simplyfile {

namespace {
void normalize(struct timespec& time) {
	while (time.tv_nsec > 1000000000) {
		time.tv_nsec -= 1000000000;
		time.tv_sec += 1;
	}
	while (time.tv_nsec < -1000000000) {
		time.tv_nsec += 1000000000;
		time.tv_sec -= 1;
	}
}
}


Timer::Timer(std::chrono::nanoseconds duration, bool oneShot, int flags)
	: FileDescriptor(::timerfd_create(CLOCK_MONOTONIC, flags))
{
	reset(duration, oneShot);
}
int Timer::getElapsed() const {
	uint64_t elapsed {0};
	::read(*this, &elapsed, sizeof(elapsed));
	return elapsed;
}

void Timer::cancel() {
	struct itimerspec new_value{};
	if (timerfd_settime(*this, TFD_TIMER_ABSTIME, &new_value, NULL) == -1) {
		throw std::runtime_error("cannot cancel timer " + std::string(strerror(errno)));
	}
}

void Timer::reset(std::chrono::nanoseconds duration, bool oneShot) {
	cancel();
	struct itimerspec new_value {};
	struct timespec now;
	if (clock_gettime(CLOCK_MONOTONIC, &now) == -1) {
		throw std::runtime_error("cannot get the current time");
	}

	int64_t seconds     = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
	int64_t nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration - std::chrono::seconds(seconds)).count();

	new_value.it_value.tv_sec = now.tv_sec + seconds;
	new_value.it_value.tv_nsec = now.tv_nsec + nanoSeconds;
	normalize(new_value.it_value);

	if (not oneShot) {
		new_value.it_interval.tv_sec = seconds;
		new_value.it_interval.tv_nsec = nanoSeconds;
		normalize(new_value.it_interval);
	}

	if (timerfd_settime(*this, TFD_TIMER_ABSTIME, &new_value, NULL) == -1) {
		throw std::runtime_error("cannot set timeout for timerfd " + std::string(strerror(errno)));
	}
}


}

