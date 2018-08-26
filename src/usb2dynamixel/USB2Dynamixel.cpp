#include "USB2Dynamixel.h"

#include <cstdio>
#include <cstring>

#include <condition_variable>
#include <sstream>
#include <thread>

#include <simplyfile/SerialPort.h>
#include "ProtocolV1.h"

namespace dynamixel {

USB2Dynamixel::USB2Dynamixel(int baudrate, std::vector<std::string> const& deviceNames)
	: m_pimpl {std::make_unique<ProtocolV1>([&] () -> simplyfile::SerialPort {
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
	}())}
{}

USB2Dynamixel::~USB2Dynamixel() {
}

bool USB2Dynamixel::ping(MotorID motor, Timeout timeout)
{
	auto g = std::lock_guard(mMutex);
	m_pimpl->writePacket(motor, Instruction::PING, {});
	auto [timeoutFlag, valid, rxBuf] = m_pimpl->readPacket(6, timeout);
	return not timeoutFlag and valid;
}

auto USB2Dynamixel::read(MotorID motor, int baseRegister, uint8_t length, Timeout timeout) -> std::tuple<bool, bool, Parameter> {
	auto g = std::lock_guard(mMutex);
	m_pimpl->writePacket(motor, Instruction::READ, {std::byte(baseRegister), std::byte{length}});
	auto [timeoutFlag, valid, rxBuf] = m_pimpl->readPacket(6+length, timeout);

	return std::make_tuple(timeoutFlag, valid, std::move(rxBuf));
}

void USB2Dynamixel::write(MotorID motor, Parameter const& txBuf) {
	auto g = std::lock_guard(mMutex);
	m_pimpl->writePacket(motor, Instruction::WRITE, txBuf);
}

void USB2Dynamixel::reset(MotorID motor) {
	auto g = std::lock_guard(mMutex);
	m_pimpl->writePacket(motor, Instruction::RESET, {});
}

void USB2Dynamixel::sync_write(std::map<MotorID, Parameter> const& motorParams, int baseRegister) {
	auto g = std::lock_guard(mMutex);

	if (motorParams.empty() > 0) {
		throw std::runtime_error("sync_write: motorParams can't be empty");
	}

	const uint8_t len = motorParams.begin()->second.size();
	bool const okay = std::all_of(begin(motorParams), end(motorParams), [&](auto param) {
		return param.second.size() == len;
	});

	if (len <= 0 or not okay) {
		throw std::runtime_error("sync_write: data is not consistent");
	}

	Parameter txBuf;
	txBuf.push_back(std::byte(baseRegister));
	txBuf.push_back(std::byte{len});
	for (auto const& [id, params] : motorParams) {
		txBuf.push_back(std::byte{id});
		txBuf.insert(txBuf.end(), params.begin(), params.end());
	}

	m_pimpl->writePacket(BroadcastID, Instruction::SYNC_WRITE, txBuf);
}

}
