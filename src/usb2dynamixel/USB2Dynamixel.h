#pragma once

#include <string>
#include <map>
#include <mutex>
#include <memory>
#include <functional>

#include "dynamixel.h"


namespace dynamixel
{

using MotorID = uint8_t;
constexpr MotorID MotorIDInvalid = 0xFF;
constexpr MotorID MotorIDAll     = 0xFE;

struct USB2Dynamixel {
	using timeout_t = std::chrono::microseconds;
	typedef std::function<void(dynamixel::motorID motor, const uint8_t* receiveBuffer, uint8_t rxPayloadLen)> callback;

	USB2Dynamixel(int baudrate, std::vector<std::string> const& deviceNames);
	~USB2Dynamixel();

	void setBaudrate(uint newBaudrate);

	bool ping(dynamixel::motorID motor, timeout_t timeout);

	bool read(dynamixel::motorID motor, int baseRegister, uint8_t length, timeout_t timeout, callback cb);

	bool write(dynamixel::motorID motor, int baseRegister, dynamixel::parameter const& writeBuffer);

	// write reg is not implemented since it's not necessary see sync_write
	// action is not implemented (see above)

	bool reset(dynamixel::motorID motor);

	bool sync_write(std::map<dynamixel::motorID, dynamixel::parameter> const& motorParams, int baseRegister);

private:
	class Pimpl;
	std::unique_ptr<Pimpl> m_pimpl;
	uint8_t calculateChecksum(dynamixel::parameter const& packet) const;
	mutable std::mutex mMutex;
};

}
