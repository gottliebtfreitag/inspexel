#include "parameter/Parameter.h"
#include "usb2dynamixel/USB2Dynamixel.h"

namespace {

auto device    = parameter::Parameter<std::string>("/dev/ttyUSB0", "device", "the usb2dynamixel device");
auto id        = parameter::Parameter<int>(0, "id", "the target Id");
auto baudrate  = parameter::Parameter<int>(1000000, "baudrate", "baudrate to use");

void runReboot();
auto rebootCmd = parameter::Command{"reboot", "reboot device with specified id", runReboot};

void runReboot() {
	if (not id.isSpecified()) {
		throw std::runtime_error("must specify a id");
	}
	auto usb2dyn = dynamixel::USB2Dynamixel(baudrate, {device.get()});
	usb2dyn.reboot(id);
}

}
