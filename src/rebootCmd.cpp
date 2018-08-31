#include "parameter/Parameter.h"
#include "usb2dynamixel/USB2Dynamixel.h"

#include "globalOptions.h"

namespace {

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
