#include "ProtocolV1.h"

#include <cstring>
#include <stdexcept>


namespace dynamixel {
namespace {

[[nodiscard]]
std::byte calculateChecksum(Parameter const& packet) {
	uint32_t checkSum = 0;
	for (size_t i(2); i < packet.size(); ++i) {
		checkSum += uint8_t(packet[i]);
	}
	return std::byte(~checkSum);
}


bool validatePacket(Parameter const& rxBuf) {
	if (rxBuf.size() > 255) {
		return false;
	}
	if (rxBuf.size() < 4) {
		return false;
	}

	bool success = 0xff == uint8_t(rxBuf[0]);
	success &= 0xff == uint8_t(rxBuf[1]);
	success &= 0xff != uint8_t(rxBuf[2]);
	success &= (rxBuf.size() - 4) == uint8_t(rxBuf[3]);
	uint8_t checkSum = 0;
	for (size_t i(2); i < rxBuf.size(); ++i) {
		checkSum += uint8_t(rxBuf[i]);
	}
	success &= 0xff == uint8_t(checkSum);
	return success;
}

}

auto ProtocolV1::createPacket(MotorID motorID, Instruction instr, Parameter data) const -> Parameter {
	if (data.size() > 253) {
		throw std::runtime_error("packet is longer than 255 bytes, not supported in protocol v1");
	}
	uint8_t length = 2 + data.size();

	Parameter txBuf {
		std::byte{0xff}, std::byte{0xff}, std::byte{motorID}, std::byte{length}, std::byte(instr)
	};
	txBuf.insert(txBuf.end(), data.begin(), data.end());

	txBuf.push_back(calculateChecksum(txBuf));

	return txBuf;
}

auto ProtocolV1::readPacket(std::chrono::high_resolution_clock::duration timeout, std::size_t numParameters, simplyfile::SerialPort const& port) const -> std::tuple<bool, MotorID, ErrorCode, Parameter> {
	std::size_t incomingLength = numParameters + 6;
	bool timeoutFlag = false;
	Parameter rxBuf;
	auto startTime = std::chrono::high_resolution_clock::now();
	while (rxBuf.size() < incomingLength and not timeoutFlag) {
		auto buffer = read(port, incomingLength - rxBuf.size());
		rxBuf.insert(rxBuf.end(), buffer.begin(), buffer.end());
		timeoutFlag = (timeout.count() != 0) and (std::chrono::high_resolution_clock::now() - startTime >= timeout);
	};
	if (timeoutFlag) {
		rxBuf.clear();
		flushRead(port);
		return std::make_tuple(true, MotorIDInvalid, ErrorCode{}, Parameter{});
	}
	auto  [motorID, errorCode, payload] = extractPayload(rxBuf);
	return std::make_tuple(false, motorID, errorCode, payload);
}

auto ProtocolV1::extractPayload(Parameter const& raw_packet) const -> std::tuple<MotorID, ErrorCode, Parameter> {
	if (not validatePacket(raw_packet)) {
		return std::make_tuple(MotorIDInvalid, ErrorCode{}, Parameter{});
	}

	auto motorID = MotorID(raw_packet[2]);
	auto errorCode = ErrorCode(raw_packet[4]);
	int len = static_cast<int>(raw_packet[3]);
	Parameter payload;
	payload.insert(payload.end(), std::next(raw_packet.begin(), 5), std::next(raw_packet.begin(), 5+len-2));

	return std::make_tuple(motorID, errorCode, std::move(payload));
}

auto ProtocolV1::convertLength(size_t len) const -> Parameter {
	if (len > 255) {
		throw std::runtime_error("packet is longer than 255 bytes, not supported in protocol v1");
	}

	return {std::byte(len)};
}

auto ProtocolV1::convertAddress(int addr) const -> Parameter {
	if (addr > 255) {
		throw std::runtime_error("baseRegister above 255 are not supported in protocol v1");
	}

	return {std::byte(addr)};
}

auto ProtocolV1::buildBulkReadPackage(std::vector<std::tuple<MotorID, int, size_t>> const& motors) const -> std::vector<std::byte> {
	std::vector<std::byte> txBuf;

	txBuf.reserve(motors.size()*3+1);
	txBuf.push_back(std::byte{0x00});
	for (auto const& [id, baseRegister, length] : motors) {
		for (auto b : convertLength(length)) {
			txBuf.push_back(b);
		}
		txBuf.push_back(std::byte{id});
		for (auto b : convertAddress(baseRegister)) {
			txBuf.push_back(b);
		}
	}

	return txBuf;
}


}
