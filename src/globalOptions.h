#pragma once

#include "parameter/Parameter.h"


auto listDeviceFiles(std::vector<std::string> const& _str) -> std::pair<bool, std::set<std::string>>;
auto listTypicalBaudrates(std::vector<std::string> const& _str) -> std::pair<bool, std::set<std::string>>;

auto getDefaultSerialPort() -> std::string;

inline auto device    = parameter::Parameter<std::string>(getDefaultSerialPort(), "device", "the usb2dynamixel device", {}, &listDeviceFiles);
inline auto id        = parameter::Parameter<int>(0, "id", "the target Id");
inline auto baudrate  = parameter::Parameter<int>(1000000, "baudrate", "baudrate to use", {}, &listTypicalBaudrates);
