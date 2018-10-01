#include "usb2dynamixel/USB2Dynamixel.h"
#include "usb2dynamixel/Layout.h"
#include "usb2dynamixel/MotorMetaInfo.h"

#include "globalOptions.h"


namespace {

void runSetAngle();
auto setAngleCmd = sargp::Command{"set_angle", "set the angle of a motor", runSetAngle};
auto angle       = setAngleCmd.Parameter<int>(0, "angle", "the goal angle (raw register value)");

void runSetAngle() {
	bool error = false;
	if (not g_id.isSpecified()) {
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

	auto usb2dyn = dynamixel::USB2Dynamixel(g_baudrate, g_device.get(), dynamixel::Protocol(g_protocolVersion.get()));
	auto [timeoutFlag, motorID, errorCode, layout] = usb2dyn.read<dynamixel::v1::Register::MODEL_NUMBER, 2>(dynamixel::MotorID(g_id), std::chrono::microseconds{g_timeout});
	if (timeoutFlag) {
		std::cout << "the specified motor is not present" << std::endl;
		exit(-1);
	}
	auto modelPtr = dynamixel::meta::getMotorInfo(layout.model_number);
	if (not modelPtr) {
		std::cout << "the specified motor has an unknown register layout" << std::endl;
		exit(-1);
	}
	if (modelPtr->layout == dynamixel::meta::LayoutType::V1) {
		usb2dyn.write<dynamixel::v1::Register::GOAL_POSITION, 2>(g_id, {int16_t(angle)});
	} else if (modelPtr->layout == dynamixel::meta::LayoutType::V2) {
		usb2dyn.write<dynamixel::v2::Register::GOAL_POSITION, 4>(g_id, {int32_t(angle)});
	} else if (modelPtr->layout == dynamixel::meta::LayoutType::Pro) {
		usb2dyn.write<dynamixel::pro::Register::GOAL_POSITION, 4>(g_id, {int32_t(angle)});
	}

}


void runSetRegister();
auto setRegisterCmd = sargp::Command{"set_register", "set registers of a motor", runSetRegister};
auto reg            = setRegisterCmd.Parameter<int>(0, "register", "register to write to");
auto values         = setRegisterCmd.Parameter<std::vector<uint8_t>>({}, "values", "values to write to the register");
auto ids            = setRegisterCmd.Parameter<std::vector<int>>({}, "ids", "use this if you want to set multiple devices at once");

void runSetRegister() {
	if (not g_id.isSpecified() and not ids.isSpecified()) throw std::runtime_error("need to specify the target g_id!");
	if (not reg.isSpecified()) throw std::runtime_error("target register has to be specified!");
	if (not values.isSpecified()) throw std::runtime_error("values to be written to the register have to be specified!");

	auto f = [&](int id) {
		std::cout << "set register " << reg << " of motor " << id << " to";
		for (uint8_t v : std::vector<uint8_t>(values)) {
			std::cout << " " << int(v);
		}
		std::cout << "\n";
		dynamixel::Parameter txBuf;
		for (auto x : values.get()) {
			txBuf.push_back(std::byte{x});
		}
		auto usb2dyn = dynamixel::USB2Dynamixel(g_baudrate, g_device.get(), dynamixel::Protocol(g_protocolVersion.get()));
		usb2dyn.write(id, int(reg), txBuf);
	};
	if (g_id.isSpecified()) {
		f(g_id);
	}
	if (ids.isSpecified()) {
		for (auto id : ids.get()) {
			f(id);
		}
	}
}

void runGetValue();
auto getRegisterCmd = sargp::Command{"get_register", "get register(s) of a motor", runGetValue};
auto read_reg       = getRegisterCmd.Parameter<int>(0, "register", "register to read from");
auto count          = getRegisterCmd.Parameter<int>(1, "count", "amount of registers to read");
auto timeout        = getRegisterCmd.Parameter<int>(10000, "timeout", "timeout in us");

void runGetValue() {
	if (not g_id.isSpecified()) throw std::runtime_error("need to specify the target g_id!");
	if (not read_reg.isSpecified()) throw std::runtime_error("target angle has to be specified!");

	auto usb2dyn = dynamixel::USB2Dynamixel(g_baudrate, g_device.get(), dynamixel::Protocol(g_protocolVersion.get()));
	auto [timeoutFlag, valid, errorCode, rxBuf] = usb2dyn.read(g_id, read_reg, count, std::chrono::microseconds{timeout});
	if (valid) {
		std::cout << "motor " << static_cast<int>(g_id) << "\n";
		std::cout << "registers:\n";
		for (size_t idx{0}; idx < rxBuf.size(); ++idx) {
			std::cout << "  " << std::setw(3) << std::setfill(' ') << std::dec << idx << ": " <<
					std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(rxBuf[idx]) << "\n";
		}
		std::cout << "\n";
	}
}

}
