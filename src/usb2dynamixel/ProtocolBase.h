#pragma once

#include "dynamixel.h"

#include <chrono>

namespace dynamixel {

struct ProtocolBase {
	virtual ~ProtocolBase() {}

	using Timeout = std::chrono::microseconds;

	virtual void writePacket(MotorID motorID, Instruction instr, Parameter data) const = 0;

	[[nodiscard]] virtual auto readPacket(uint8_t incomingLength, Timeout timeout) const -> std::tuple<bool, MotorID, Parameter> = 0;
};

}
