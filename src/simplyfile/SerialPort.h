#pragma once

#include "FileDescriptor.h"

namespace simplyfile
{

struct SerialPort : FileDescriptor {
	SerialPort() = default;
	SerialPort(std::string const& name, std::optional<int> baudrate);
	SerialPort(SerialPort&&);
	SerialPort& operator=(SerialPort&&);
	virtual ~SerialPort() = default;

	bool hasOptionBaudrate() const;
private:
	bool mHasOptionBaudrate{true};
};


}
