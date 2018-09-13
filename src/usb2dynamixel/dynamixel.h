#pragma once

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <vector>

namespace dynamixel {

	using MotorID = uint8_t;

	constexpr MotorID MotorIDInvalid = 0xFF;
	constexpr MotorID BroadcastID    = 0xFE;

	using Parameter = std::vector<std::byte>;

	enum class Instruction : std::underlying_type_t<std::byte>
	{
		PING       = 0x01,
		READ       = 0x02,
		WRITE      = 0x03,
		REG_WRITE  = 0x04,
		ACTION     = 0x05,
		RESET      = 0x06,
		REBOOT     = 0x08,
		STATUS     = 0x55,
		SYNC_READ  = 0x82,
		SYNC_WRITE = 0x83,
		BULK_READ  = 0x92,
		BULK_WRITE = 0x93,
	};

	inline uint32_t baudIndexToBaudrate(uint8_t baudIdx) {
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
		throw std::runtime_error("no valid baud index given");
	}

	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
	template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
}
