#pragma once

#include "ProtocolBase.h"

namespace dynamixel {

struct ProtocolV2 : public ProtocolBase {
	[[nodiscard]] auto createPacket(MotorID motorID, Instruction instr, Parameter data) const -> Parameter override;
	[[nodiscard]] auto readPacket(std::chrono::high_resolution_clock::duration timeout, std::size_t numParameters, simplyfile::SerialPort const& port) const -> std::tuple<bool, MotorID, ErrorCode, Parameter> override;
	[[nodiscard]] auto extractPayload(Parameter const& raw_packet) const -> std::tuple<MotorID, ErrorCode, Parameter>;

	auto convertLength(size_t len) const -> Parameter override;
	auto convertAddress(int addr)  const -> Parameter override;

	auto buildBulkReadPackage(std::vector<std::tuple<MotorID, int, size_t>> const& motors) const -> std::vector<std::byte> override;
};

}
