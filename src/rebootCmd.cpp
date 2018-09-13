#include "usb2dynamixel/USB2Dynamixel.h"

#include "globalOptions.h"

namespace {

void runReboot();
auto rebootCmd = parameter::Command{"reboot", "reboot device with specified id", runReboot};

void runReboot() {
	if (not g_id.isSpecified()) {
		throw std::runtime_error("must specify a id");
	}
	auto usb2dyn = dynamixel::USB2Dynamixel(g_baudrate, g_device.get(), dynamixel::Protocol(g_protocolVersion.get()));
	usb2dyn.reboot(g_id);
}

}
