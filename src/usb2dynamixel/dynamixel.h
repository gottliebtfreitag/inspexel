#pragma once

#include <cstdint>
#include <vector>

namespace dynamixel {

	using MotorID = uint8_t;

	constexpr MotorID MotorIDInvalid = 0xFF;
	constexpr MotorID BroadcastID    = 0xFE;

	using Parameter = std::vector<uint8_t>;

	enum class Instruction : uint8_t
	{
		PING       = 0x01,
		READ       = 0x02,
		WRITE      = 0x03,
		REG_WRITE  = 0x04,
		ACTION     = 0x05,
		RESET      = 0x06,
		SYNC_WRITE = 0x83,
	};

	namespace v1 {
		enum class Register : uint8_t
		{
			GOAL_POSITION       = 0x1e,
		};
	}
	inline uint32_t baudIndexToBaudrate(uint8_t baudIdx)
	{
		if (baudIdx < 250) {
			return (2000000 / (baudIdx + 1));
		} else {
			switch (baudIdx) {
				case 250:
					return 2250000;
					break;
				case 251:
					return 2500000;
					break;
				case 252:
					return 3000000;
					break;
				default:
					break;
			}
		}
		return 57600; // return factory default
	}
}
