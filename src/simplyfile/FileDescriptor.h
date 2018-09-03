#pragma once

#include <cstring>
#include <fcntl.h>
#include <functional>
#include <map>
#include <unistd.h>
#include <vector>

namespace simplyfile
{

struct FileDescriptor {

	FileDescriptor(int _fd=-1) : fd(_fd) {}

	FileDescriptor(FileDescriptor&& other) {
		fd = other.fd;
		other.fd = -1;
	}
	FileDescriptor& operator=(FileDescriptor&& other) {
		close();
		std::swap(fd, other.fd);
		return *this;
	}

	FileDescriptor& operator=(int _fd) {
		close();
		fd = _fd;
		return *this;
	}

	virtual ~FileDescriptor() {
		close();
	}

	bool valid() const {
		return fd >= 0;
	}

	operator int() const {
		return fd;
	}

	void close() {
		if (valid()) {
			::close(*this);
			fd = -1;
		}
	}

	void setFlags(int flags) {
		fcntl(*this, F_SETFL, flags | getFlags());
	}
	void clearFlags(int flags) {
		fcntl(*this, F_SETFL, ~flags & getFlags());
	}
	int getFlags() {
		return ::fcntl(*this, F_GETFL, 0);
	}

private:
	int fd;
};

void write(FileDescriptor const& _port, std::vector<std::byte> const& txBuf);
[[nodiscard]] auto read(FileDescriptor const& _port, size_t maxReadBytes, bool singleRead=false) -> std::vector<std::byte>;

template <typename T>
[[nodiscard]] T read(FileDescriptor const& _port) {
	auto data = read(_port, sizeof(T));
	if (data.size() != sizeof(T)) {
		throw std::runtime_error("some reading error, who knows :/");
	}
	T ret;
	memcpy(&ret, data.data(), sizeof(T));
	return ret;
}
size_t flushRead(FileDescriptor const& _port);

}
