#pragma once

#include "parameter/Parameter.h"


auto listDeviceFiles(std::vector<std::string> const& _str) -> std::pair<bool, std::set<std::string>>;
auto listTypicalBaudrates(std::vector<std::string> const& _str) -> std::pair<bool, std::set<std::string>>;

auto getDefaultSerialPort() -> std::string;

inline auto g_device          = parameter::Parameter<std::string>(getDefaultSerialPort(), "device", "the usb2dynamixel device (e.g.: /dev/ttyUSB0)", {}, &listDeviceFiles);
inline auto g_id              = parameter::Parameter<int>(0, "id", "the target Id (values: 0x00 - 0xfd)");
inline auto g_baudrate        = parameter::Parameter<int>(1000000, "baudrate", "baudrate to use (e.g.: 1m)", {}, &listTypicalBaudrates);
inline auto g_timeout         = parameter::Parameter<int>(10000, "timeout", "timeout in us");
inline auto g_protocolVersion = parameter::Parameter<int>(1, "protocol_version", "the dynamixel protocol version (values: 1, 2)");
