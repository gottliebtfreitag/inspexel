#include "file_io.h"

#include <cerrno>
#include <cstring>
#include <array>

#include <sys/types.h>
#include <unistd.h>


namespace dynamixel::file_io {

auto read(int _fd, size_t maxReadBytes) -> std::vector<std::byte> {
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
	} while (bytesRead < maxReadBytes);
	rxBuf.resize(bytesRead);
	return rxBuf;
}

size_t flushRead(int _fd) {
	size_t bytesRead {0};
	std::array<uint8_t, 4096> dummy;
	ssize_t r {1};
	while (r != -1 and r != 0) {
		bytesRead += r;
		r = ::read(_fd, dummy.data(), dummy.size()); // read flush
	}
	return bytesRead;
}

void write(int _fd, std::vector<std::byte> const& txBuf) {
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
}
