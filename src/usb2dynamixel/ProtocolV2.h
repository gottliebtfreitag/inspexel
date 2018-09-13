#pragma once

//!TODO this is not protocol 2!!!!!, still needs to be implemented

#include "ProtocolBase.h"

#include <chrono>
#include <cstring>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <simplyfile/SerialPort.h>

namespace dynamixel {

struct ProtocolV2 : public ProtocolBase {

	simplyfile::SerialPort mPort;

	std::chrono::high_resolution_clock::time_point now() const {
		return std::chrono::high_resolution_clock::now();
	}

	ProtocolV2(simplyfile::SerialPort _port)
		: mPort {std::move(_port)}
	{}

	void writePacket(MotorID motorID, Instruction instr, Parameter data) const override {
		uint8_t length = 2 + data.size();

		Parameter txBuf {
			std::byte{0xff}, std::byte{0xff}, std::byte{motorID}, std::byte{length}, std::byte(instr)
		};
		txBuf.insert(txBuf.end(), data.begin(), data.end());

		txBuf.push_back(calculateChecksum(txBuf));

		write(mPort, txBuf);
	}

	/** tries to read up to incomingLength bytes from filedescriptor
	 *
	 * return value
	 *  [timeoutFlag, motorID, errorCode, parameters] = readPacket(...);
	 *
	 *  timeoutFlag indicates if a timeout has occurred
	 *  motorID     inidcates if parameters form a valid packet, withc MotorIDInvalid
	 *  errorCode   errorCode flags from the return message
	 *  paremeters  is a vector with read bytes
	 */
	auto readPacket(size_t incomingLength, Timeout timeout) const -> std::tuple<bool, MotorID, ErrorCode, Parameter> override {
		if (incomingLength > 255) {
			throw std::runtime_error("packet is longer than 255 bytes, not supported in protocol v1");
		}

		auto startTime = now();

		Parameter rxBuf;
		rxBuf.reserve(incomingLength);
		ErrorCode errorCode;

		bool timeoutFlag = false;
		while (rxBuf.size() < incomingLength and not timeoutFlag) {
			auto buffer = read(mPort, incomingLength - rxBuf.size());

			rxBuf.insert(rxBuf.end(), buffer.begin(), buffer.end());
			timeoutFlag = (timeout.count() != 0) and (now() - startTime >= timeout);
		};
		bool valid = rxBuf.size() == incomingLength and validatePacket(rxBuf);

		auto motorID = MotorIDInvalid;
		if (not valid) {
			flushRead(mPort);
			rxBuf.clear();
		} else {
			motorID = MotorID(rxBuf[2]);
			errorCode = ErrorCode(rxBuf[4]);
			rxBuf.erase(rxBuf.begin(), std::next(rxBuf.begin(), 5));
			rxBuf.pop_back();
		}

		return std::make_tuple(timeoutFlag, motorID, errorCode, std::move(rxBuf));
	}

	auto convertLength(size_t len) const -> Parameter override {
		if (len > 255) {
			throw std::runtime_error("packet is longer than 255 bytes, not supported in protocol v1");
		}

		return {std::byte(len)};
	}
	auto convertAddress(int addr) const -> Parameter override {
		if (addr > 255) {
			throw std::runtime_error("baseRegister above 255 are not supported in protocol v1");
		}

		return {std::byte(addr)};
	}




	bool validatePacket(Parameter const& rxBuf) const {
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

	[[nodiscard]]
	std::byte calculateChecksum(Parameter const& packet) const {
		uint32_t checkSum = 0;
		for (size_t i(2); i < packet.size(); ++i) {
			checkSum += uint8_t(packet[i]);
		}
		return std::byte(~checkSum);
	}

};

}
