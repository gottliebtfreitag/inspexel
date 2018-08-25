#include "parameter/Parameter.h"
#include "usb2dynamixel/USB2Dynamixel.h"

#include <iomanip>

namespace
{
auto device    = parameter::Parameter<std::string>("/dev/ttyUSB0", "device", "the usb2dynamixel device");
auto id        = parameter::Parameter<int>(0, "id", "the target Id");
auto baudrate  = parameter::Parameter<int>(1000000, "baudrate", "baudrate to use");

void runSetAngle();
auto detectCmd = parameter::Command{"set_angle", "set the angle of a motor", runSetAngle};
auto angle     = detectCmd.Parameter<int>(0, "angle", "the goal angle (raw register value)");

void runSetAngle() {
	bool error = false;
	if (not id.isSpecified()) {
		std::cout << "need to specify the target id!" << std::endl;
		error = true;
	}
	if (not angle.isSpecified()) {
		std::cout << "target angle has to be specified!" << std::endl;
		error = true;
	}
	if (error) {
		exit(-1);
	}
	dynamixel::Parameter params;
	params.push_back((angle >> 0) & 0xff);
	params.push_back((angle >> 8) & 0xff);
	dynamixel::USB2Dynamixel(baudrate, {device.get()}).write(id, static_cast<int>(dynamixel::v1::Register::GOAL_POSITION), params);
}


void runSetValue();
auto setAngleCmd = parameter::Command{"set_register", "set registers of a motor", runSetValue};
auto reg         = setAngleCmd.Parameter<int>(0, "register", "register to write to");
auto values      = setAngleCmd.Parameter<std::vector<uint8_t>>({}, "values", "values to write to the register");

void runSetValue() {
	if (not id.isSpecified()) throw std::runtime_error("need to specify the target id!");
	if (not reg.isSpecified()) throw std::runtime_error("target angle has to be specified!");
	if (not values.isSpecified()) throw std::runtime_error("values to be written to the register have to be specified!");

	std::cout << "set register " << reg << " of motor " << id << " to";
	for (uint8_t v : std::vector<uint8_t>(values)) {
		std::cout << " " << int(v);
	}
	std::cout << "\n";
	dynamixel::USB2Dynamixel(baudrate, {device.get()}).write(id, reg, values);
}

void runGetValue();
auto getRegisterCmd = parameter::Command{"get_register", "get register(s) of a motor", runGetValue};
auto read_reg       = getRegisterCmd.Parameter<int>(0, "register", "register to read from");
auto count          = getRegisterCmd.Parameter<int>(1, "count", "amount of registers to read");
auto timeout        = getRegisterCmd.Parameter<int>(10000, "timeout", "timeout in us");

void runGetValue() {
	if (not id.isSpecified()) throw std::runtime_error("need to specify the target id!");
	if (not read_reg.isSpecified()) throw std::runtime_error("target angle has to be specified!");

	auto [timeoutFlag, valid, rxBuf] = dynamixel::USB2Dynamixel(baudrate, {device.get()}).read(id, read_reg, count, std::chrono::microseconds{timeout});
	if (valid) {
		std::cout << "motor " << static_cast<int>(id) << "\n";
		std::cout << "registers:\n";
		for (size_t idx{0}; idx < rxBuf.size(); ++idx) {
			std::cout << "  " << std::setw(3) << std::setfill(' ') << std::dec << idx << ": " <<
					std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(rxBuf[idx]) << "\n";
		}
		std::cout << "\n";
	}
}

}
