#include "usb2dynamixel/ProtocolV2.h"
#include "usb2dynamixel/MotorMetaInfo.h"

#include "parameter/Parameter.h"

#include <algorithm>

#include <iostream>
#include <unistd.h>

namespace {

void runTest();
auto testCmd  = parameter::Command{{"test"}, "run my test", runTest};
auto params = testCmd.Parameter<std::vector<uint8_t>>({}, "params", "test");

void runTest() {
	dynamixel::ProtocolV2 protocol;

	dynamixel::Parameter sendParams{};
	for (auto p : params.get()) {
		sendParams.emplace_back(std::byte(p));
	}

	auto outPacket = protocol.createPacket(dynamixel::MotorID{1}, dynamixel::Instruction::READ, sendParams);
	auto [MotorID, ErrorCode, payload] = protocol.validateRawPacket(outPacket);

	for (auto byte : outPacket) {
		std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte) << " ";
	}
	std::cout << std::endl;
	for (auto byte : payload) {
		std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte) << " ";
	}
	std::cout << std::endl;


}


}
