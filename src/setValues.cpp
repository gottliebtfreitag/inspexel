#include "usb2dynamixel/USB2Dynamixel.h"

#include "globalOptions.h"


namespace {

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
	params.push_back(std::byte{dynamixel::Register::GOAL_POSITION});
	params.push_back(std::byte((angle >> 0) & 0xff));
	params.push_back(std::byte((angle >> 8) & 0xff));
	dynamixel::USB2Dynamixel(baudrate, {device.get()}).write(id, params);
}


void runSetValue();
auto setAngleCmd = parameter::Command{"set_register", "set registers of a motor", runSetValue};
auto reg         = setAngleCmd.Parameter<int>(0, "register", "register to write to");
auto values      = setAngleCmd.Parameter<std::vector<uint8_t>>({}, "values", "values to write to the register");
auto ids         = setAngleCmd.Parameter<std::vector<int>>({}, "ids", "use this if you want to set multiple devices at once");

void runSetValue() {
	if (not id.isSpecified() and not ids.isSpecified()) throw std::runtime_error("need to specify the target id!");
	if (not reg.isSpecified()) throw std::runtime_error("target angle has to be specified!");
	if (not values.isSpecified()) throw std::runtime_error("values to be written to the register have to be specified!");

	auto f = [&](int id) {
		std::cout << "set register " << reg << " of motor " << id << " to";
		for (uint8_t v : std::vector<uint8_t>(values)) {
			std::cout << " " << int(v);
		}
		std::cout << "\n";
		dynamixel::Parameter txBuf {std::byte(int(reg))};
		for (auto x : values.get()) {
			txBuf.push_back(std::byte{x});
		}
		dynamixel::USB2Dynamixel(baudrate, {device.get()}).write(id, txBuf);
	};
	if (id.isSpecified()) {
		f(id);
	}
	if (ids.isSpecified()) {
		for (auto id : ids.get()) {
			f(id);
		}
	}
}

void runGetValue();
auto getRegisterCmd = parameter::Command{"get_register", "get register(s) of a motor", runGetValue};
auto read_reg       = getRegisterCmd.Parameter<int>(0, "register", "register to read from");
auto count          = getRegisterCmd.Parameter<int>(1, "count", "amount of registers to read");
auto timeout        = getRegisterCmd.Parameter<int>(10000, "timeout", "timeout in us");

void runGetValue() {
	if (not id.isSpecified()) throw std::runtime_error("need to specify the target id!");
	if (not read_reg.isSpecified()) throw std::runtime_error("target angle has to be specified!");

	auto [timeoutFlag, valid, errorCode, rxBuf] = dynamixel::USB2Dynamixel(baudrate, {device.get()}).read(id, read_reg, count, std::chrono::microseconds{timeout});
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
