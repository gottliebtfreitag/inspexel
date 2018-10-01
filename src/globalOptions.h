#pragma once

#include <sargparse/Parameter.h>


auto listDeviceFiles(std::vector<std::string> const& _str) -> std::pair<bool, std::set<std::string>>;
auto listTypicalBaudrates(std::vector<std::string> const& _str) -> std::pair<bool, std::set<std::string>>;

auto getDefaultSerialPort() -> std::string;

inline auto g_device          = sargp::Parameter<std::string>(getDefaultSerialPort(), "device", "the usb2dynamixel device (e.g.: /dev/ttyUSB0)", {}, &listDeviceFiles);
inline auto g_id              = sargp::Parameter<int>(0, "id", "the target Id (values: 0x00 - 0xfd)");
inline auto g_baudrate        = sargp::Parameter<int>(1000000, "baudrate", "baudrate to use (e.g.: 1m)", {}, &listTypicalBaudrates);
inline auto g_timeout         = sargp::Parameter<int>(10000, "timeout", "timeout in us");
inline auto g_protocolVersion = sargp::Parameter<int>(1, "protocol_version", "the dynamixel protocol version (values: 1, 2)");
