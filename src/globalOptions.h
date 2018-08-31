#pragma once

#include "parameter/Parameter.h"


auto listDeviceFiles(std::vector<std::string> const& _str) -> std::pair<bool, std::set<std::string>>;

namespace {
	auto device    = parameter::Parameter<std::string>("/dev/ttyUSB0", "device", "the usb2dynamixel device", {}, &listDeviceFiles);
	auto id        = parameter::Parameter<int>(0, "id", "the target Id");
	auto baudrate  = parameter::Parameter<int>(1000000, "baudrate", "baudrate to use");
}
