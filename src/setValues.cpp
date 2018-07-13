#include "parameter/Parameter.h"
#include "usb2dynamixel/USB2Dynamixel.h"

#include <iomanip>

namespace
{
auto device    = parameter::Parameter<std::string>("/dev/ttyUSB0", "device", "the usb2dynamixel device");
auto id        = parameter::Parameter<int>(0, "id", "the target Id");
auto baudrate  = parameter::Parameter<int>(1000000, "baudrate", "baudrate to use");

void runSetAngle();
parameter::Command detectCmd{"set_angle", "set the angle of a motor", runSetAngle};
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
	dynamixel::parameter params;
	params.push_back((angle >> 0) & 0xff);
	params.push_back((angle >> 8) & 0xff);
	dynamixel::USB2Dynamixel(baudrate, {device.get()}).write(id, static_cast<int>(dynamixel::v1::Register::GOAL_POSITION), params);
}


void runSetValue();
parameter::Command setAngleCmd{"set_register", "set registers of a motor", runSetValue};
auto reg       = setAngleCmd.Parameter<int>(0, "register", "register to write to");
auto values    = setAngleCmd.Parameter<std::vector<uint8_t>>({}, "values", "values to write to the register");

void runSetValue() {
	bool error = false;
	if (not id.isSpecified()) {
		std::cout << "need to specify the target id!" << std::endl;
		error = true;
	}
	if (not reg.isSpecified()) {
		std::cout << "target angle has to be specified!" << std::endl;
		error = true;
	}
	if (not values.isSpecified()) {
		std::cout << "values to be written to the registers have to be specified!" << std::endl;
		error = true;
	}
	if (error) {
		exit(-1);
	}
	dynamixel::USB2Dynamixel(baudrate, {device.get()}).write(id, reg, values);
}

void runGetValue();
parameter::Command getRegisterCmd{"get_register", "get register(s) of a motor", runGetValue};
auto read_reg = getRegisterCmd.Parameter<int>(0, "register", "register to read from");
auto count    = getRegisterCmd.Parameter<int>(1, "count", "amount of registers to read");
auto timeout   = getRegisterCmd.Parameter<int>(10000, "timeout", "timeout in us");

void runGetValue() {
	bool error = false;
	if (not id.isSpecified()) {
		std::cout << "need to specify the target id!" << std::endl;
		error = true;
	}
	if (not read_reg.isSpecified()) {
		std::cout << "target register has to be specified!" << std::endl;
		error = true;
	}
	if (error) {
		exit(-1);
	}
	dynamixel::USB2Dynamixel(baudrate, {device.get()}).read(id, read_reg, count, std::chrono::microseconds{timeout},
	[](dynamixel::motorID motor, const uint8_t* receiveBuffer, uint8_t rxPayloadLen) {
		if (rxPayloadLen == count) {
			std::cout << "motor " << static_cast<int>(motor) << "\n";
			std::cout << "registers:\n";
			for (int idx{0}; idx < rxPayloadLen; ++idx) {
				std::cout << "  " << std::setw(3) << std::setfill(' ') << std::dec << idx << ": " <<
						std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(receiveBuffer[idx]) << "\n";
			}
			std::cout << "\n";
		}
	});
}

}
