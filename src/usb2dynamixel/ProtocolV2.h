#pragma once

#include "ProtocolBase.h"

namespace dynamixel {

struct ProtocolV2 : public ProtocolBase {
	auto createPacket(MotorID motorID, Instruction instr, Parameter data) const -> Parameter override;
	[[nodiscard]] auto readPacket(std::chrono::high_resolution_clock::duration timeout, std::size_t numParameters, simplyfile::SerialPort const& port) const -> Parameter override;
	[[nodiscard]] auto validateRawPacket(Parameter const& raw_packet) const -> std::tuple<bool, MotorID, ErrorCode, Parameter> override;

	auto convertLength(size_t len) const -> Parameter override;
	auto convertAddress(int addr) const -> Parameter override;
};

}
