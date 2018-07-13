#pragma once

#include <string>
#include <map>
#include <mutex>
#include <functional>

#include "dynamixel.h"
#include <utils/utils.h>


namespace dynamixel
{

struct USB2Dynamixel_protocol2 {
	typedef std::function<void(dynamixel::motorID motor, bool success, uint8_t error, const uint8_t* receiveBuffer, uint8_t rxPayloadLen)> callback;

	USB2Dynamixel_protocol2(int baudrate, std::vector<std::string> const& deviceNames);
	~USB2Dynamixel_protocol2();

	void setBaudrate(uint newBaudrate);

	bool ping(dynamixel::motorID motor, Second timeout, callback cb, std::mutex* mutex = nullptr);

	bool read(dynamixel::motorID motor, int baseRegister, uint8_t length, Second timeout, callback cb, std::mutex* mutex = nullptr);

	bool write(dynamixel::motorID motor, int baseRegister, dynamixel::parameter const& writeBuffer, std::mutex* mutex = nullptr);

	// write reg is not implemented since it's not necessary see sync_write
	// action is not implemented (see above)

	bool reset(dynamixel::motorID motor, std::mutex* mutex = nullptr);

	bool sync_write(std::map<dynamixel::motorID, dynamixel::parameter> const& motorParams, int baseRegister, std::mutex* mutex = nullptr);

private:
	class Pimpl;
	std::unique_ptr<Pimpl> m_pimpl;
	uint8_t calculateChecksum(dynamixel::parameter const& packet) const;
	mutable std::mutex mMutex;
};

}
