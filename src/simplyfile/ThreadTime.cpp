#include "ThreadTime.h"
#include <time.h>
#include <stdexcept>

namespace simplyfile
{

std::chrono::nanoseconds getThreadTime()
{
	struct timespec ts{};
	if (0 != clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts)) {
		throw std::runtime_error("cannot call clock_gettime for CLOCK_THREAD_CPUTIME_ID");
	}
	return std::chrono::nanoseconds{ts.tv_sec*1000000000} + std::chrono::nanoseconds{ts.tv_nsec};
}
}
