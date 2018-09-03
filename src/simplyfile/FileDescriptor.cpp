#include "FileDescriptor.h"

#include <cstring>
#include <cerrno>

namespace simplyfile {

void write(FileDescriptor const& _port, std::vector<std::byte> const& txBuf) {
	uint32_t bytesWritten = 0;
	const size_t count = txBuf.size();
	do {
		ssize_t w = ::write(_port, txBuf.data() + bytesWritten, count - bytesWritten);

		if (w == -1) {
			throw std::runtime_error(std::string{"write to the dyanmixel bus failed: "} + strerror(errno) + " (" + std::to_string(errno) + ")");
		}
		bytesWritten += w;
	} while (bytesWritten < count);
}

auto read(FileDescriptor const& _port, size_t maxReadBytes, bool singleRead) -> std::vector<std::byte> {
	std::vector<std::byte> rxBuf(maxReadBytes);

	size_t bytesRead = 0;

	do {
		ssize_t r = ::read(_port, rxBuf.data() + bytesRead, rxBuf.size() - bytesRead);
		if (r == -1) {
			if (errno == EAGAIN) {
				break;
			}
			throw std::runtime_error(std::string{"unexpected read error: "} + strerror(errno) + " (" + std::to_string(errno) + ")");
		}
		bytesRead += r;
	} while (bytesRead < maxReadBytes and not singleRead);
	rxBuf.resize(bytesRead);
	return rxBuf;
}

size_t flushRead(FileDescriptor const& _port) {
	size_t bytesRead {0};
	uint8_t dummy;
	ssize_t r {0};
	while (r != -1) {
		bytesRead += r;
		r = ::read(_port, &dummy, sizeof(dummy)); // read flush
	}
	return bytesRead;
}

}
