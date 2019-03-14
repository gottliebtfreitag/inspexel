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

USB2Dynamixel::USB2Dynamixel(std::optional<int> baudrate, std::string const& device, Protocol protocol)
	: mPort(device, baudrate)
{
	simplyfile::flushRead(mPort);
	if (protocol == Protocol::V1) {
		mProtocol = std::make_unique<ProtocolV1>();
	} else {
		mProtocol = std::make_unique<ProtocolV2>();
	}
}

USB2Dynamixel::~USB2Dynamixel() {
}

bool USB2Dynamixel::ping(MotorID motor, Timeout timeout) const {
	auto g = std::lock_guard(mMutex);
	simplyfile::write(mPort, mProtocol->createPacket(motor, Instruction::PING, {}));
	auto [timeoutFlag, motorID, errorCode, rxBuf] = mProtocol->readPacket(timeout, motor, 0, mPort);
	return motorID != MotorIDInvalid;
}

auto USB2Dynamixel::read(MotorID motor, int baseRegister, size_t length, Timeout timeout) const -> std::tuple<bool, MotorID, ErrorCode, Parameter> {
	std::vector<std::byte> txBuf;
	for (auto b : mProtocol->convertAddress(baseRegister)) {
		txBuf.push_back(b);
	}
	for (auto b : mProtocol->convertLength(length)) {
		txBuf.push_back(b);
	}

	auto g = std::lock_guard(mMutex);
	simplyfile::write(mPort, mProtocol->createPacket(motor, Instruction::READ, txBuf));
	return mProtocol->readPacket(timeout, motor, length, mPort);
}

auto USB2Dynamixel::bulk_read(std::vector<std::tuple<MotorID, int, size_t>> const& motors, Timeout timeout) const -> std::vector<std::tuple<MotorID, int, ErrorCode, Parameter>> {

	std::vector<std::tuple<MotorID, int, ErrorCode, Parameter>> resList;
	resList.reserve(motors.size());

	auto txBuf = mProtocol->buildBulkReadPackage(motors);

	auto g = std::lock_guard(mMutex);
	simplyfile::write(mPort, mProtocol->createPacket(BroadcastID, Instruction::BULK_READ, txBuf));

	for (auto const& [id, baseRegister, length] : motors) {
		auto [timeoutFlag, motorID, errorCode, rxBuf] = mProtocol->readPacket(timeout, id, length, mPort);
		if (motorID == MotorIDInvalid or motorID != id) {
			break;
		}
		resList.push_back(std::make_tuple(id, baseRegister, errorCode, rxBuf));
	}
	return resList;
}

void USB2Dynamixel::write(MotorID motor, int baseRegister, Parameter const& txBuf) const {
	std::vector<std::byte> parameters;
	for (auto b : mProtocol->convertAddress(baseRegister)) {
		parameters.push_back(b);
	}
	parameters.insert(parameters.end(), txBuf.begin(), txBuf.end());
	auto g = std::lock_guard(mMutex);
	simplyfile::write(mPort, mProtocol->createPacket(motor, Instruction::WRITE, parameters));
}
auto USB2Dynamixel::writeRead(MotorID motor, int baseRegister, Parameter const& txBuf, Timeout timeout) const -> std::tuple<bool, MotorID, ErrorCode, Parameter> {
	write(motor, baseRegister, txBuf);
	return mProtocol->readPacket(timeout, motor, 0, mPort);
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
	for (auto b : mProtocol->convertAddress(baseRegister)) {
		txBuf.push_back(b);
	}
	for (auto b : mProtocol->convertLength(len)) {
		txBuf.push_back(b);
	}

	for (auto const& [id, params] : motorParams) {
		txBuf.push_back(std::byte{id});
		txBuf.insert(txBuf.end(), params.begin(), params.end());
	}

	simplyfile::write(mPort, mProtocol->createPacket(BroadcastID, Instruction::SYNC_WRITE, txBuf));
}

void USB2Dynamixel::reset(MotorID motor) const {
	auto g = std::lock_guard(mMutex);
	simplyfile::write(mPort, mProtocol->createPacket(motor, Instruction::RESET, {}));

}

void USB2Dynamixel::reboot(MotorID motor) const {
	auto g = std::lock_guard(mMutex);
	simplyfile::write(mPort, mProtocol->createPacket(motor, Instruction::REBOOT, {}));
}

bool USB2Dynamixel::hasOptionBaudrate() const {
	auto g = std::lock_guard(mMutex);
	return mPort.hasOptionBaudrate();
}

}
