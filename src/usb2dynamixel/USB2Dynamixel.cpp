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

USB2Dynamixel::USB2Dynamixel(int baudrate, std::vector<std::string> const& deviceNames, Protocol protocol)
	: m_pimpl {[&]() -> std::unique_ptr<ProtocolBase> {
		auto&& serialPort = [&] () -> simplyfile::SerialPort {
			for (auto const& dev : deviceNames) {
				try {
					return {dev, baudrate};
				} catch (...) {
				}
			}
			std::stringstream ss;
			ss << "Cannot open any port for usb2dynamixel. Tried: ";
			for (auto const& dev : deviceNames) {
				ss << dev << ", ";
			}
			throw std::runtime_error(ss.str());
		}();
		if (protocol == Protocol::V1) {
			return std::make_unique<ProtocolV1>(std::move(serialPort));
		} else {
			return std::make_unique<ProtocolV2>(std::move(serialPort));
		}
	}()}
{}

USB2Dynamixel::~USB2Dynamixel() {
}

bool USB2Dynamixel::ping(MotorID motor, Timeout timeout) const {
	auto g = std::lock_guard(mMutex);
	m_pimpl->writePacket(motor, Instruction::PING, {});
	auto [timeoutFlag, motorID, errorCode, rxBuf] = m_pimpl->readPacket(6, timeout);
	return not timeoutFlag and motorID != MotorIDInvalid;
}

auto USB2Dynamixel::read(MotorID motor, int baseRegister, size_t length, Timeout timeout) const -> std::tuple<bool, MotorID, ErrorCode, Parameter> {
	auto g = std::lock_guard(mMutex);

	std::vector<std::byte> txBuf;

	//!TODO looks protocol v1 dependent
	for (auto b : m_pimpl->convertAddress(baseRegister)) {
		txBuf.push_back(b);
	}
	for (auto b : m_pimpl->convertLength(length)) {
		txBuf.push_back(b);
	}

	m_pimpl->writePacket(motor, Instruction::READ, txBuf);
	auto [timeoutFlag, motorID, errorCode, rxBuf] = m_pimpl->readPacket(6+length, timeout);
	if (timeoutFlag) {
		motorID = MotorIDInvalid;
	}

	return std::make_tuple(timeoutFlag, motorID, errorCode, std::move(rxBuf));
}

auto USB2Dynamixel::bulk_read(std::vector<std::tuple<MotorID, int, size_t>> const& motors, Timeout timeout) const -> std::vector<std::tuple<MotorID, int, ErrorCode, Parameter>> {
	auto g = std::lock_guard(mMutex);
	std::vector<std::byte> txBuf;
	txBuf.reserve(motors.size()*3+1);
	txBuf.push_back(std::byte{0x00});

	//!TODO looks protocol v1 dependent
	for (auto const& [id, baseRegister, length] : motors) {
		for (auto b : m_pimpl->convertLength(length)) {
			txBuf.push_back(b);
		}
		txBuf.push_back(std::byte{id});
		for (auto b : m_pimpl->convertAddress(baseRegister)) {
			txBuf.push_back(b);
		}
	}

	m_pimpl->writePacket(BroadcastID, Instruction::BULK_READ, txBuf);

	std::vector<std::tuple<MotorID, int, ErrorCode, Parameter>> resList;
	for (auto const& [id, baseRegister, length] : motors) {
		auto const& [timeoutFlag, motorID, errorCode, rxBuf] = m_pimpl->readPacket(6 + length, timeout);
		if (timeoutFlag or motorID == MotorIDInvalid or motorID != id) {
			break;
		}
		resList.push_back(std::make_tuple(id, baseRegister, errorCode, rxBuf));
	}
	return resList;
}

void USB2Dynamixel::write(MotorID motor, Parameter const& txBuf) const {
	auto g = std::lock_guard(mMutex);
	m_pimpl->writePacket(motor, Instruction::WRITE, txBuf);
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
	//!TODO protocol v1 dependent
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

	m_pimpl->writePacket(BroadcastID, Instruction::SYNC_WRITE, txBuf);
}

void USB2Dynamixel::reset(MotorID motor) const {
	auto g = std::lock_guard(mMutex);
	m_pimpl->writePacket(motor, Instruction::RESET, {});
}

void USB2Dynamixel::reboot(MotorID motor) const {
	auto g = std::lock_guard(mMutex);
	m_pimpl->writePacket(motor, Instruction::REBOOT, {});
}

}
