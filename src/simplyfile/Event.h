#pragma once

#include "FileDescriptor.h"

#include <sys/eventfd.h>

namespace simplyfile
{

struct Event : FileDescriptor {
	Event(int flags=0, int initval=0) noexcept 
		: FileDescriptor(eventfd(initval, flags))
	{}

    Event(Event&&) noexcept = default;
    Event& operator=(Event&&) noexcept = default;

	uint64_t get() {
		uint64_t val {0};
		if (::eventfd_read(*this, &val) != 0) {
			throw std::runtime_error("cannot read from eventfd");
		}
		return val;
	}

	void put(uint64_t val) {
		if (::eventfd_write(*this, val) != 0) {
			throw std::runtime_error("cannot write to eventfd");
		}
	}
};

}

