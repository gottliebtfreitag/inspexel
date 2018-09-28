#pragma once

#include "dynamixel.h"
#include "ProtocolBase.h"
#include <simplyfile/SerialPort.h>

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

enum class Protocol : int {
	V1 = 1,
	V2 = 2,
};

struct USB2Dynamixel {
	using Timeout = std::chrono::microseconds;

	USB2Dynamixel(int baudrate, std::string const& device, Protocol protocol = Protocol::V1);
	~USB2Dynamixel();

	[[nodiscard]] bool ping(MotorID motor, Timeout timeout) const;
	[[nodiscard]] auto read(MotorID motor, int baseRegister, size_t length, Timeout timeout) const -> std::tuple<bool, MotorID, ErrorCode, Parameter>;
	[[nodiscard]] auto bulk_read(std::vector<std::tuple<MotorID, int, size_t>> const& motors, Timeout timeout) const -> std::vector<std::tuple<MotorID, int, ErrorCode, Parameter>>;

	void write(MotorID motor, int baseRegister, Parameter const& txBuf) const;
	auto writeRead(MotorID motor, int baseRegister, Parameter const& txBuf, Timeout timeout) const -> std::tuple<bool, MotorID, ErrorCode, Parameter>;

	void sync_write(std::map<MotorID, Parameter> const& motorParams, int baseRegister) const;

	void reset(MotorID motor) const;
	void reboot(MotorID motor)const;

	template <auto baseRegister, size_t length>
	[[nodiscard]] auto read(MotorID motor, Timeout timeout) -> std::tuple<bool, MotorID, ErrorCode, Layout<baseRegister, size_t(length)>> {
		using RType = Layout<baseRegister, length>;
		static_assert(length == sizeof(RType));

		auto [timeoutFlag, motorID, errorCode, rxBuf] = read(motor, int(baseRegister), length, timeout);
		if (timeoutFlag) {
			return std::make_tuple(true, MotorIDInvalid, errorCode, RType{});
		} else if (motorID == MotorIDInvalid) {
			return std::make_tuple(false, MotorIDInvalid, errorCode, RType{});
		}
		return std::make_tuple(false, motorID, errorCode, RType(rxBuf));
	}

	template <auto baseRegister, size_t length, typename ...Extras>
	[[nodiscard]] auto bulk_read(std::vector<std::tuple<MotorID, Extras...>> const& motors, USB2Dynamixel::Timeout timeout) -> std::vector<std::tuple<MotorID, Extras..., ErrorCode, Layout<baseRegister, length>>> {
		if (motors.empty()) return {};

		std::vector<std::tuple<MotorID, int, size_t>> request;
		for (auto data : motors) {
			auto id = std::get<0>(data);
			request.push_back(std::make_tuple(id, int(baseRegister), size_t(length)));
		}
		auto list = bulk_read(request, timeout);

		std::vector<std::tuple<MotorID, Extras..., ErrorCode, Layout<baseRegister, length>>> response;
		auto iter = begin(motors);
		for (auto const& [id, _reg, errorCode, params] : list) {
			response.push_back(std::tuple_cat(*iter, std::make_tuple(errorCode, Layout<baseRegister, length>{params})));
			++iter;
		}
		return response;
	}

	template <auto baseRegister, size_t length>
	void write(MotorID motor, Layout<baseRegister, length> layout) const {
		std::vector<std::byte> txBuf(sizeof(layout));
		memcpy(txBuf.data(), &layout, sizeof(layout));
		write(motor, int(baseRegister), txBuf);
	}

	template <auto baseRegister, size_t length>
	auto writeRead(MotorID motor, Layout<baseRegister, length> layout, Timeout timeout) const {
		std::vector<std::byte> txBuf(sizeof(layout));
		memcpy(txBuf.data(), &layout, sizeof(layout));
		return writeRead(motor, int(baseRegister), txBuf, timeout);
	}



	template <template<auto, size_t> class Layout, auto baseRegister, size_t Length>
	void sync_write(std::map<MotorID, Layout<baseRegister, Length>> const& params) {
		if (params.empty()) return;

		std::map<MotorID, Parameter> motorParams;
		for (auto const& [id, layout] : params) {
			auto& buffer = motorParams[id];
			buffer.resize(Length);
			memcpy(buffer.data(), &layout, Length);
		}
		sync_write(motorParams, int(baseRegister));
	}

	template <typename Layout, typename ...Extras>
	void sync_sync_write(std::vector<std::tuple<MotorID, Extras...>> const& motors, Layout const& layout) {
		if (motors.empty()) return;

		std::map<MotorID, Layout> motorParams;
		for (auto m : motors) {
			motorParams[std::get<0>(m)] = layout;
		}
		sync_write(motorParams);
	}

private:
	std::unique_ptr<ProtocolBase> mProtocol;
	mutable std::mutex mMutex;

	simplyfile::SerialPort mPort;
};


}
