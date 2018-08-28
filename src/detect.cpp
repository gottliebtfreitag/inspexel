#include "parameter/Parameter.h"
#include "usb2dynamixel/USB2Dynamixel.h"
#include "usb2dynamixel/MotorMetaInfo.h"

#define TERM_RED                        "\033[31m"
#define TERM_GREEN                      "\033[32m"
#define TERM_RESET                      "\033[0m"

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
	// only read model information, when model is known read full motor
	auto [timeoutFlag, valid, rxBuf] = usb2dyn.read(motor, 0, 2, std::chrono::microseconds{timeout});
	if (timeoutFlag) {
		return;
	}
	if (not valid) {
		std::cout << "something answered when pinging " << int(motor) << " but answer was not valid\n";
		return;
	}
	uint32_t modelNumber = uint32_t(rxBuf.at(0)) + (uint32_t(rxBuf.at(1)) << 8);
	auto modelPtr = dynamixel::getMotorDataBase(modelNumber);

	if (readAll) {
		if (modelPtr) {
			auto lastEntry = std::next(modelPtr->registerData.begin(), modelPtr->registerData.size() -1);
			auto v1 = lastEntry->second.baseRegister;
			auto v2 = lastEntry->second.length;

			auto length = v1 + v2;
			auto [timeoutFlag, valid, rxBuf] = usb2dyn.read(motor, 0, length, std::chrono::microseconds{timeout});
			std::cout << "\n" << int(motor) << " " <<  modelPtr->shortName << " (" << modelNumber << ") Layout " << to_string(modelPtr->layout) << "\n";
			if (not valid) {
				std::cout << "couldn't read detailed infos\n";
				return;
			}

			std::cout << "address (length): value (initial value) - RW - name - description\n";
			dynamixel::visitBuffer(rxBuf, std::nullopt, dynamixel::overloaded{
				[](dynamixel::RegisterData const& reg, auto&& x) {
					std::string mem = reg.romArea?"ROM":"RAM";
					std::string initValue = "-";
					if (reg.initialValue) {
						initValue = std::to_string(reg.initialValue.value());
					}
					std::cout << mem << " " << std::setw(3) << reg.baseRegister << "(" << reg.length << ") : ";
					if (reg.initialValue) {
						if (reg.initialValue.value() != x) {
							std::cout << TERM_RED;
						} else {
							std::cout << TERM_GREEN;
						}
					}
					std::cout << std::setw(5) << int(x) << TERM_RESET;


					std::cout << " (" << std::setw(5) << initValue << ") - " << std::setw(2) << to_string(reg.access) << " - " << reg.description << " - " << reg.dataName << "\n";
				}
			});
		} else {
			std::cout << " unknown model (" << modelNumber << ") going to read first 74 bytes\n";
			auto [timeoutFlag, valid, rxBuf] = usb2dyn.read(motor, 0, 74, std::chrono::microseconds{timeout});
			std::cout << "found motor " << static_cast<int>(motor) << "\n";

			std::cout << "registers:\n";
			for (size_t idx{0}; idx < rxBuf.size(); ++idx) {
				std::cout << "  " << std::setw(3) << std::setfill(' ') << std::dec << idx << ": " <<
					std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(rxBuf.at(idx)) << "\n";
			}
			std::cout << "\n";
		}
	} else {
		if (modelPtr) {
			std::cout << int(motor) << " " <<  modelPtr->shortName << " (" << modelNumber << ") Layout " << to_string(modelPtr->layout) << "\n";
		} else {
			std::cout << int(motor) << " unknown model (" << modelNumber << ")\n";
		}
	}
}

void runDetect() {
	baudrates.get().emplace(baudrate);
	for (auto baudrate : baudrates.get()) {
		std::cout << "trying baudrate: " << baudrate << "\n";
		auto usb2dyn = dynamixel::USB2Dynamixel(baudrate, {device.get()});
		if (id != 0) {
			detectMotor(id, usb2dyn);
		} else {
			for (int motor{0}; motor < 0xFD; ++motor) {
				detectMotor(motor, usb2dyn);
			}
		}
	}
}

}
