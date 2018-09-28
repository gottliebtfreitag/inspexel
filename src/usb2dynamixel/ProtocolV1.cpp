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

[[nodiscard]]
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


Parameter ProtocolV1::synchronizeOnHeader(Timeout timeout, MotorID expectedMotorID, std::size_t numParameters, simplyfile::SerialPort const& port) const {
	Parameter preambleBuffer;
	struct __attribute__((packed)) Header {
		std::array<std::byte, 2> syncMarker;
		uint8_t id;
		uint8_t length;
		uint8_t error;
	};
	std::array<std::byte, 2> syncMarker = {std::byte{0xff}, std::byte{0xff}};
	auto startTime = std::chrono::high_resolution_clock::now();
	while (not ((timeout.count() != 0) and (std::chrono::high_resolution_clock::now() - startTime >= timeout))) {
		// figure out how many bytes have to be read
		int indexOfSyncMarker = 0;
		for (;indexOfSyncMarker <= static_cast<int>(preambleBuffer.size())-static_cast<int>(sizeof(syncMarker)); ++indexOfSyncMarker) {
			if (0 == std::memcmp(&preambleBuffer[indexOfSyncMarker], syncMarker.data(), sizeof(syncMarker))) {
				break;
			}
		}
		preambleBuffer.erase(preambleBuffer.begin(), preambleBuffer.begin()+indexOfSyncMarker);
		int bytesToRead = std::max(1, static_cast<int>(sizeof(Header)+1) - static_cast<int>(preambleBuffer.size()));
		auto buffer = read(port, bytesToRead);
		preambleBuffer.insert(preambleBuffer.end(), buffer.begin(), buffer.end());
		if (preambleBuffer.size() >= sizeof(Header)) {
			// test if this preamble contains the header of the packet we were looking for
			Header header;
			std::memcpy(&header, preambleBuffer.data(), sizeof(Header));
			if (header.syncMarker == syncMarker and header.length+2 >= static_cast<int>(numParameters)) {
				// found a synchronization token and a "matching" packet
				if (expectedMotorID != 0xfe) {
					if (expectedMotorID == header.id) {
						return preambleBuffer;
					}
					// received an unexpected header -> flush this header and continue reading
					preambleBuffer.clear();
				} else {
					return preambleBuffer;
				}
			}
		}
	}
	return {};
}

auto ProtocolV1::readPacket(Timeout timeout, MotorID expectedMotorID, std::size_t numParameters, simplyfile::SerialPort const& port) const -> std::tuple<bool, MotorID, ErrorCode, Parameter> {
	bool timeoutFlag = false;
	auto startTime = std::chrono::high_resolution_clock::now();

	while (not timeoutFlag) {
		// read a header (headers consist of 5 bytes)
		Parameter preambleBuffer;
		auto testTimeout = [&]{ return (timeout.count() != 0) and (std::chrono::high_resolution_clock::now() - startTime >= timeout);};
		Parameter rxBuf = synchronizeOnHeader(timeout, expectedMotorID, numParameters, port);
		if (rxBuf.size() < 5) { // if we could not synchronize on a header bail out
			break;
		}
		std::size_t incomingLength = numParameters + 6;
		while (rxBuf.size() < incomingLength and not timeoutFlag) {
			auto buffer = read(port, incomingLength - rxBuf.size());
			rxBuf.insert(rxBuf.end(), buffer.begin(), buffer.end());
			timeoutFlag = testTimeout();
		};
		if (timeoutFlag) {
			break;
		}
		auto  [motorID, errorCode, payload] = extractPayload(rxBuf);
		if (payload.size() != numParameters or motorID != expectedMotorID) {
			continue;
		}
		return std::make_tuple(false, motorID, errorCode, payload);
	}
	flushRead(port);
	return std::make_tuple(true, MotorIDInvalid, ErrorCode{}, Parameter{});
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
