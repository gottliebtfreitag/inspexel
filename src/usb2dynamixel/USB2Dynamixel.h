#pragma once

#include "dynamixel.h"
#include "ProtocolBase.h"

#include <cassert>
#include <chrono>
#include <cstring>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>

#include "Layout.h"

#include <iostream>

namespace dynamixel {

struct USB2Dynamixel {
	using Timeout = std::chrono::microseconds;

	USB2Dynamixel(int baudrate, std::vector<std::string> const& deviceNames);
	~USB2Dynamixel();

	[[nodiscard]] bool ping(MotorID motor, Timeout timeout);
	[[nodiscard]] auto read(MotorID motor, int baseRegister, uint8_t length, Timeout timeout) -> std::tuple<bool, MotorID, Parameter>;
	[[nodiscard]] auto bulk_read(std::vector<std::tuple<MotorID, int, uint8_t>> const& motors, Timeout timeout) -> std::vector<std::tuple<MotorID, int, Parameter>>;

	void write(MotorID motor, Parameter const& txBuf);
	void sync_write(std::map<MotorID, Parameter> const& motorParams, int baseRegister);

	void reset(MotorID motor);
	void reboot(MotorID motor);

private:
	std::unique_ptr<ProtocolBase> m_pimpl;
	std::mutex mMutex;
};

template <auto baseRegister, size_t length>
[[nodiscard]] auto read(USB2Dynamixel& dyn, MotorID motor, USB2Dynamixel::Timeout timeout) -> std::tuple<bool, MotorID, Layout<baseRegister, size_t(length)>> {
	using RType = Layout<baseRegister, length>;
	static_assert(length == sizeof(RType));

	auto [timeoutFlag, motorID, rxBuf] = dyn.read(motor, int(baseRegister), length, timeout);
	if (timeoutFlag) {
		return std::make_tuple(true, MotorIDInvalid, RType{});
	}
	return std::make_tuple(false, motorID, RType(rxBuf));
}

template <auto baseRegister, size_t length, typename ...Extras>
[[nodiscard]] auto bulk_read(USB2Dynamixel& dyn, std::vector<std::tuple<MotorID, Extras...>> const& motors, USB2Dynamixel::Timeout timeout) -> std::vector<std::tuple<MotorID, Extras..., Layout<baseRegister, length>>> {
	if (motors.empty()) return {};

	std::vector<std::tuple<MotorID, int, uint8_t>> request;
	for (auto data : motors) {
		auto id = std::get<0>(data);
		request.push_back(std::make_tuple(id, int(baseRegister), uint8_t(length)));
	}
	auto list = dyn.bulk_read(request, timeout);

	std::vector<std::tuple<MotorID, Extras..., Layout<baseRegister, length>>> response;
	auto iter = begin(motors);
	for (auto const& [id, _reg, params] : list) {
		response.push_back(std::tuple_cat(*iter, std::make_tuple(Layout<baseRegister, length>{params})));
		++iter;
	}
	return response;
}

template <template<auto, size_t> class Layout, auto baseRegister, size_t Length>
void sync_write(USB2Dynamixel& dyn, std::map<MotorID, Layout<baseRegister, Length>> const& params) {
	if (params.empty()) return;

	std::map<MotorID, Parameter> motorParams;
	for (auto const& [id, layout] : params) {
		auto& buffer = motorParams[id];
		buffer.resize(Length);
		memcpy(buffer.data(), &layout, Length);
	}
	dyn.sync_write(motorParams, int(baseRegister));
}

template <typename Layout, typename ...Extras>
void sync_sync_write(USB2Dynamixel& dyn, std::vector<std::tuple<MotorID, Extras...>> const& motors, Layout const& layout) {
	if (motors.empty()) return;
	
	std::map<MotorID, Layout> motorParams;
	for (auto m : motors) {
		motorParams[std::get<0>(m)] = layout;
	}
	sync_write(dyn, motorParams);
}


}
