#pragma once

namespace {
	auto device    = parameter::Parameter<std::string>("/dev/ttyUSB0", "device", "the usb2dynamixel device");
	auto id        = parameter::Parameter<int>(0, "id", "the target Id");
	auto baudrate  = parameter::Parameter<int>(1000000, "baudrate", "baudrate to use");
}
