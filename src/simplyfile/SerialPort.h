#pragma once

#include "FileDescriptor.h"

namespace simplyfile {

struct SerialPort : FileDescriptor
{
	SerialPort() = default;
	SerialPort(std::string const& name, int baudrate);
	SerialPort(SerialPort&&);
	SerialPort& operator=(SerialPort&&);
	virtual ~SerialPort() = default;

	void setBaudrate(int baudrate);
};

}
