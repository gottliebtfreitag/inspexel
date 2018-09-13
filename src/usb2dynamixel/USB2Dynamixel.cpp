#include "USB2Dynamixel.h"

#include <cstdio>
#include <cstring>

#include <condition_variable>
#include <sstream>
#include <thread>

#include <simplyfile/SerialPort.h>
#include "ProtocolV1.h"
#include "ProtocolV2.h"

namespace dynamixel {

USB2Dynamixel::USB2Dynamixel(int baudrate, std::string const& device, Protocol protocol)
	: mPort(device, baudrate)
	, protocolVersion{protocol}
{
	simplyfile::flushRead(mPort);
	if (protocol == Protocol::V1) {
		m_pimpl = std::make_unique<ProtocolV1>();
	} else {
		m_pimpl = std::make_unique<ProtocolV2>();
	}
}

USB2Dynamixel::~USB2Dynamixel() {
}

bool USB2Dynamixel::ping(MotorID motor, Timeout timeout) const {
	auto g = std::lock_guard(mMutex);
	simplyfile::write(mPort, m_pimpl->createPacket(motor, Instruction::PING, {}));
	auto [validFlag, motorID, errorCode, rxBuf] = m_pimpl->validateRawPacket(m_pimpl->readPacket(timeout, 0, mPort));
	return not validFlag and motorID != MotorIDInvalid;
}

auto USB2Dynamixel::read(MotorID motor, int baseRegister, size_t length, Timeout timeout) const -> std::tuple<bool, MotorID, ErrorCode, Parameter> {
	std::vector<std::byte> txBuf;
	//!TODO looks protocol v1 dependent
	for (auto b : m_pimpl->convertAddress(baseRegister)) {
		txBuf.push_back(b);
	}
	for (auto b : m_pimpl->convertLength(length)) {
		txBuf.push_back(b);
	}

	auto g = std::lock_guard(mMutex);
	simplyfile::write(mPort, m_pimpl->createPacket(motor, Instruction::READ, txBuf));
	auto [validFlag, motorID, errorCode, rxBuf] = m_pimpl->validateRawPacket(m_pimpl->readPacket(timeout, length, mPort));
	if (not validFlag) {
		motorID = MotorIDInvalid;
	}

	return std::make_tuple(not validFlag, motorID, errorCode, std::move(rxBuf));
}

auto USB2Dynamixel::bulk_read(std::vector<std::tuple<MotorID, int, size_t>> const& motors, Timeout timeout) const -> std::vector<std::tuple<MotorID, int, ErrorCode, Parameter>> {
	std::vector<std::byte> txBuf;

	std::vector<std::tuple<MotorID, int, ErrorCode, Parameter>> resList;
	resList.reserve(motors.size());

	if (protocolVersion == Protocol::V1) {
		txBuf.reserve(motors.size()*3+1);
		txBuf.push_back(std::byte{0x00});
		for (auto const& [id, baseRegister, length] : motors) {
			for (auto b : m_pimpl->convertLength(length)) {
				txBuf.push_back(b);
			}
			txBuf.push_back(std::byte{id});
			for (auto b : m_pimpl->convertAddress(baseRegister)) {
				txBuf.push_back(b);
			}
		}
	}
	if (protocolVersion == Protocol::V2) {
		txBuf.reserve(motors.size()*5);
		for (auto const& [id, baseRegister, length] : motors) {
			txBuf.push_back(std::byte{id});
			for (auto b : m_pimpl->convertAddress(baseRegister)) {
				txBuf.push_back(b);
			}
			for (auto b : m_pimpl->convertLength(length)) {
				txBuf.push_back(b);
			}
		}
	}
	auto g = std::lock_guard(mMutex);
	simplyfile::write(mPort, m_pimpl->createPacket(BroadcastID, Instruction::BULK_READ, txBuf));

	for (auto const& [id, baseRegister, length] : motors) {
		auto [validFlag, motorID, errorCode, rxBuf] = m_pimpl->validateRawPacket(m_pimpl->readPacket(timeout, length, mPort));
		if (not validFlag or motorID == MotorIDInvalid or motorID != id) {
			break;
		}
		resList.push_back(std::make_tuple(id, baseRegister, errorCode, rxBuf));
	}
	return resList;
}

void USB2Dynamixel::write(MotorID motor, int baseRegister, Parameter const& txBuf) const {
	std::vector<std::byte> parameters;
	for (auto b : m_pimpl->convertAddress(baseRegister)) {
		parameters.push_back(b);
	}
	parameters.insert(parameters.end(), txBuf.begin(), txBuf.end());
	auto g = std::lock_guard(mMutex);
	simplyfile::write(mPort, m_pimpl->createPacket(motor, Instruction::WRITE, parameters));
}

void USB2Dynamixel::sync_write(std::map<MotorID, Parameter> const& motorParams, int baseRegister) const {
	auto g = std::lock_guard(mMutex);

	if (motorParams.empty()) {
		throw std::runtime_error("sync_write: motorParams can't be empty");
	}

	const size_t len = motorParams.begin()->second.size();
	bool const okay = std::all_of(begin(motorParams), end(motorParams), [&](auto param) {
		return param.second.size() == len;
	});

	if (len <= 0 or not okay) {
		throw std::runtime_error("sync_write: data is not consistent");
	}

	Parameter txBuf;
	for (auto b : m_pimpl->convertAddress(baseRegister)) {
		txBuf.push_back(b);
	}
	for (auto b : m_pimpl->convertLength(len)) {
		txBuf.push_back(b);
	}

	for (auto const& [id, params] : motorParams) {
		txBuf.push_back(std::byte{id});
		txBuf.insert(txBuf.end(), params.begin(), params.end());
	}

	m_pimpl->createPacket(BroadcastID, Instruction::SYNC_WRITE, txBuf);
}

void USB2Dynamixel::reset(MotorID motor) const {
	auto g = std::lock_guard(mMutex);
	m_pimpl->createPacket(motor, Instruction::RESET, {});
}

void USB2Dynamixel::reboot(MotorID motor) const {
	auto g = std::lock_guard(mMutex);
	m_pimpl->createPacket(motor, Instruction::REBOOT, {});
}

}
