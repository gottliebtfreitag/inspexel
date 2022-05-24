#pragma once

#include "FileDescriptor.h"

#include <string>

namespace simplyfile
{

struct SerialPort : FileDescriptor
{
	SerialPort() = default;
	SerialPort(std::string const& name, int baudrate);
	SerialPort(SerialPort&&) noexcept = default;
	SerialPort& operator=(SerialPort&&) noexcept = default;
	virtual ~SerialPort() = default;

	void setBaudrate(int baudrate);
};


}
