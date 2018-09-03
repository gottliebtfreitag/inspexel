#include "FileDescriptor.h"

#include <stdexcept>
#include <cstring>
#include <sys/ioctl.h>
#include <sys/un.h>

namespace simplyfile {

void write(FileDescriptor const& _fd, std::vector<std::byte> const& txBuf) {
	uint32_t bytesWritten = 0;
	const size_t count = txBuf.size();
	do {
		ssize_t w = ::write(_fd, txBuf.data() + bytesWritten, count - bytesWritten);

		if (w == -1) {
			throw std::runtime_error(std::string{"write to the dyanmixel bus failed: "} + strerror(errno) + " (" + std::to_string(errno) + ")");
		}
		bytesWritten += w;
	} while (bytesWritten < count);
}

auto read(FileDescriptor const& _fd, size_t maxReadBytes, bool singleRead) -> std::vector<std::byte> {
	std::vector<std::byte> rxBuf(maxReadBytes);

	size_t bytesRead = 0;

	do {
		ssize_t r = ::read(_fd, rxBuf.data() + bytesRead, rxBuf.size() - bytesRead);
		if (r == -1) {
			if (errno == EAGAIN) {
				break;
			}
			throw std::runtime_error(std::string{"unexpected read error: "} + strerror(errno) + " (" + std::to_string(errno) + ")");
		} else if (r == 0) {
			break;
		}
		bytesRead += r;
	} while (bytesRead < maxReadBytes and not singleRead);
	rxBuf.resize(bytesRead);
	return rxBuf;
}
auto read(FileDescriptor const& _fd) -> std::vector<std::byte> {
	return read(_fd, getAvailableBytes(_fd));
}

auto getAvailableBytes(FileDescriptor const& _fd) -> size_t {
	int bytesAvailable;
	if (ioctl(_fd, FIONREAD, &bytesAvailable)) {
		throw std::runtime_error("cannot determine how many bytes are available in filedescriptor");
	}
	return bytesAvailable;
}

size_t flushRead(FileDescriptor const& _fd) {
	size_t bytesRead {0};
	uint8_t dummy;
	ssize_t r {0};
	while (r != -1) {
		bytesRead += r;
		r = ::read(_fd, &dummy, sizeof(dummy)); // read flush
	}
	return bytesRead;
}

}
