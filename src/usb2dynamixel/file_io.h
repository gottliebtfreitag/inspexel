#pragma once

#include <vector>
#include <cstddef>

namespace dynamixel::file_io {
auto read(int _fd, size_t maxReadBytes) -> std::vector<std::byte>;
size_t flushRead(int _fd);
void write(int _fd, std::vector<std::byte> const& txBuf);
}