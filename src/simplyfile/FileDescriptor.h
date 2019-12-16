#pragma once

#include <fcntl.h>
#include <functional>
#include <map>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <unistd.h>
#include <vector>

namespace simplyfile {

struct FileDescriptor {

	FileDescriptor(int _fd=-1) : fd(_fd) {}

	FileDescriptor(FileDescriptor&& other) noexcept {
		fd = other.fd;
		other.fd = -1;
	}
	FileDescriptor& operator=(FileDescriptor&& other) noexcept {
		close();
		std::swap(fd, other.fd);
		return *this;
	}

	FileDescriptor& operator=(int _fd) noexcept {
		close();
		fd = _fd;
		return *this;
	}

	virtual ~FileDescriptor() {
		close();
	}

	bool valid() const noexcept {
		return fd >= 0;
	}

	operator int() const noexcept {
		return fd;
	}

	void close() noexcept {
		if (valid()) {
			::close(*this);
			fd = -1;
		}
	}

	void setFlags(int flags) noexcept {
		fcntl(*this, F_SETFL, flags | getFlags());
	}
	void clearFlags(int flags) noexcept {
		fcntl(*this, F_SETFL, ~flags & getFlags());
	}
	int getFlags() noexcept {
		return ::fcntl(*this, F_GETFL, 0);
	}

private:
	int fd;
};

}
