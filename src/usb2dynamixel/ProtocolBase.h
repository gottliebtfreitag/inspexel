#pragma once

#include "dynamixel.h"

#include <simplyfile/SerialPort.h>

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

	virtual auto createPacket(MotorID motorID, Instruction instr, Parameter data) const -> Parameter = 0;
	/**
	 * receive a packet that contains numParameters bytes of payload
	 * return the whole raw packet or an empty vector if a timeout happened or and invalid packet was received
	 */
	virtual auto readPacket(std::chrono::high_resolution_clock::duration timeout, std::size_t numParameters, simplyfile::SerialPort const& port) const -> Parameter = 0;


	/** process a received packet by validating it and stripping it to the payload
	 *
	 *  return value
	 *  [validFlag, motorID, errorCode, parameters] = readPacket(...);
	 *
	 *  validFlag   indicates if the packet was valid
	 *  motorID     inidcates if parameters form a valid packet, withc MotorIDInvalid
	 *  errorCode   errorCode flags from the return message
	 *  paremeters  is a vector with the actual payload
	 */
	[[nodiscard]] virtual auto validateRawPacket(Parameter const& raw_packet) const -> std::tuple<bool, MotorID, ErrorCode, Parameter> = 0;

	virtual auto convertLength(size_t len) const -> Parameter = 0;
	virtual auto convertAddress(int addr) const -> Parameter = 0;
};

}
