#include "parameter/Parameter.h"
#include "usb2dynamixel/USB2Dynamixel.h"

#include <iomanip>

namespace
{
auto device    = parameter::Parameter<std::string>("/dev/ttyUSB0", "device", "the usb2dynamixel device");
auto id        = parameter::Parameter<int>(0, "id", "the target Id");
auto baudrate  = parameter::Parameter<int>(1000000, "baudrate", "baudrate to use");

void runDetect();
auto detectCmd = parameter::Command{"detect", "detect all dynamixel motors", runDetect};
auto baudrates = detectCmd.Parameter<std::set<int>>({57142}, "other_baudrates", "more baudrates to test");
auto timeout   = detectCmd.Parameter<int>(10000, "timeout", "timeout in us");
auto readAll   = detectCmd.Flag("read_all", "read all registers from the detected motors (instead of just printing the found motors)");

void detectMotor(dynamixel::MotorID motor, dynamixel::USB2Dynamixel& usb2dyn) {

	if (readAll) {
		auto [timeoutFlag, valid, rxBuf] = usb2dyn.read(motor, 0, 74, std::chrono::microseconds{timeout});
		if (valid and not rxBuf.empty()) {
			std::cout << "found motor " << static_cast<int>(motor) << "\n";
			std::cout << "registers:\n";
			for (size_t idx{0}; idx < rxBuf.size(); ++idx) {
				std::cout << "  " << std::setw(3) << std::setfill(' ') << std::dec << idx << ": " <<
						std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(rxBuf.at(idx)) << "\n";
			}
			std::cout << "\n";
		}
	} else {
		if (usb2dyn.ping(motor, std::chrono::microseconds{timeout})) {
			std::cout << "found motor " << static_cast<int>(motor) << "\n";
		}
	}
}

void runDetect() {
	baudrates.get().emplace(baudrate);
	for (auto baudrate : baudrates.get()) {
		auto usb2dyn = dynamixel::USB2Dynamixel(baudrate, {device.get()});
		if (id != 0) {
			detectMotor(id, usb2dyn);
		} else {
			for (int motor{0}; motor < 0xfe; ++motor) {
				detectMotor(motor, usb2dyn);
			}
		}
	}
}

}
