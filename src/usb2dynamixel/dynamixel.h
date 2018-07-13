/*
 * dynamixelMotor.h
 *
 *  Created on: 17.02.2015
 *      Author: lutz
 */

#ifndef DYNAMIXEL_H_
#define DYNAMIXEL_H_

#include <stdint.h>
#include <inttypes.h>
#include <vector>

namespace dynamixel
{
	typedef uint8_t motorID;

	static constexpr uint8_t broadcastID = 0xfe;

	typedef uint8_t length;
	typedef std::vector<uint8_t> parameter;
	typedef uint8_t checksum;

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
			MODEL_NUMBER        = 0x00,
			VERSION_FIRMWARE    = 0x02,
			ID                  = 0x03,
			BAUD_RATE           = 0x04,
			RETURN_DELAY_TIME   = 0x05,
			CW_ANGLE_LIMIT      = 0x06,
			CCW_ANGLE_LIMIT     = 0x08,
			TEMPERATURE_LIMIT   = 0x0b,
			VOLTAGE_LIMIT_LOW   = 0x0c,
			VOLTAGE_LIMIT_HIGH  = 0x0d,
			MAX_TORQUE          = 0x0e,
			STATUS_RETURN_LEVEL = 0x10,
			ALARM_LED           = 0x11,
			ALARM_SHUTDOWN      = 0x12,
			MULTI_TURN_OFFSET   = 0x14,
			RESOLUTION_DIVIDER  = 0x16,
			TORQUE_ENABLE       = 0x18,
			LED                 = 0x19,
			GAIN                = 0x13,
			D_GAIN              = 0x1a,
			I_GAIN              = 0x1b,
			P_GAIN              = 0x1c,
			GOAL_POSITION       = 0x1e,
			MOVING_SPEED        = 0x20,
			TORQUE_LIMIT        = 0x22,
			PRESENT_POSITION    = 0x24,
			PRESENT_SPEED       = 0x26,
			PRESENT_LOAD        = 0x28,
			PRESENT_VOLTAGE     = 0x2a,
			PRESENT_TEMPERATURE = 0x2b,
			REGISTERED          = 0x2c,
			MOVING              = 0x2e,
			LOCK                = 0x2f,
			PUNCH               = 0x30,
			CURRENT             = 0x44,
			GOAL_ACCELERATION   = 0x49,
		};
	}
	namespace v2 {
		enum class Register : uint8_t
		{
			// EEPROM AREA
			MODEL_NUMBER        = 0x00,
			MODEL_INFORMATION   = 0x02,
			VERSION_FIRMWARE    = 0x06,
			ID                  = 0x07,
			BAUD_RATE           = 0x08,
			RETURN_DELAY_TIME   = 0x09,
			DRIVE_MODE          = 0x0a,
			OPERATING_MODE      = 0x0b,
			SECONDARY_ID        = 12,
			PROTOCOL_VERSION    = 13,
			HOMING_OFFSET       = 20,
			MOVING_THRESHOLD    = 24,
			TEMPERATURE_LIMIT   = 31,
			MAX_VOLTAGE_LIMIT   = 32,
			MIN_VOLTAGE_LIMIT   = 34,
			PWM_LIMIT           = 36,
			CURRENT_LIMIT       = 38,
			ACCELERATION_LIMIT  = 40,
			VELOCITY_LIMIT      = 44,
			MAX_POSITION_LIMIT  = 48,
			MIN_POSITION_LIMIT  = 52,
			SHUTDOWN            = 63,

			// RAM AREA
			TORQUE_ENABLE       = 64,
			LED                 = 65,
			STATUS_RETURN_LEVEL = 68,
			REGISTERED_INSTRUCTION = 69,
			HARDWARE_ERROR_STATUS  = 70,
			VELOCITY_I_GAIN        = 76,
			VELOCITY_P_GAIN        = 78,
			POSITION_D_GAIN        = 80,
			POSITION_I_GAIN        = 82,
			POSITION_P_GAIN        = 84,
			FEEDFORWARD_2ND_GAIN   = 88,
			FEEDFORWARD_1ST_GAIN   = 90,
			BUS_WATCHDOG           = 98,
			GOAL_PWM               = 100,
			GOAL_CURRENT           = 102,
			GOAL_VELOCITY          = 104,
			PROFILE_ACCELERATION   = 108,
			PROFILE_VELOCITY       = 112,
			GOAL_POSITION          = 116,
			REALTIME_TICK          = 120,
			MOVING                 = 122,
			MOVING_STATUS          = 123,
			PRESENT_PWM            = 124,
			PRESENT_CURRENT        = 126,
			PRESENT_VELOCITY       = 128,
			PRESENT_POSITION       = 132,

			VELOCITY_TRAJECTORY    = 136,
			POSITION_TRAJECTORY    = 140,
			PRESENT_INPUT_VOLTAGE  = 144,
			PRESENT_TEMPERATURE    = 146
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



#endif /* DYNAMIXEL_H_ */
