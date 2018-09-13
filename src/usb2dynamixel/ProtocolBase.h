#pragma once

#include "dynamixel.h"

#include <chrono>

namespace dynamixel {

enum class ErrorCode : uint8_t {
	InpultVoltage = 0x01,
	AngleLimit    = 0x02,
	Overheating   = 0x04,
	Range         = 0x08,
	Checksum      = 0x10,
	Overload      = 0x20,
	Instruction   = 0x40
};


struct ProtocolBase {

	virtual ~ProtocolBase() {}

	using Timeout = std::chrono::microseconds;

	virtual void writePacket(MotorID motorID, Instruction instr, Parameter data) const = 0;

	[[nodiscard]] virtual auto readPacket(size_t incomingLength, Timeout timeout) const -> std::tuple<bool, MotorID, ErrorCode, Parameter> = 0;

	virtual auto convertLength(size_t len) const -> Parameter = 0;
	virtual auto convertAddress(int addr) const -> Parameter = 0;

};

}
