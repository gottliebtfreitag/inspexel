#pragma once

#include "FileDescriptor.h"

namespace simplyfile
{

struct SerialPort : FileDescriptor
{
	SerialPort() = default;
	SerialPort(std::string const& name, int baudrate);
	SerialPort(SerialPort&&);
	SerialPort& operator=(SerialPort&&);
	virtual ~SerialPort() = default;

	void setBaudrate(int baudrate);
};

void write(SerialPort const& _port, std::vector<std::byte> const& txBuf);
[[nodiscard]] auto read(SerialPort const& _port, size_t maxReadBytes) -> std::vector<std::byte>;
size_t flushRead(SerialPort const& _port);

}
