#pragma once

#include "dynamixel.h"

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace dynamixel {

struct USB2Dynamixel {
	using Timeout = std::chrono::microseconds;

	USB2Dynamixel(int baudrate, std::vector<std::string> const& deviceNames);
	~USB2Dynamixel();

	[[nodiscard]] bool ping(MotorID motor, Timeout timeout);

	[[nodiscard]] auto read(MotorID motor, int baseRegister, uint8_t length, Timeout timeout) -> std::tuple<bool, bool, std::vector<uint8_t>>;

	void write(MotorID motor, int baseRegister, Parameter const& writeBuffer);

	void reset(MotorID motor);

	void sync_write(std::map<MotorID, Parameter> const& motorParams, int baseRegister);

private:
	struct Pimpl;
	std::unique_ptr<Pimpl> m_pimpl;
};

}
