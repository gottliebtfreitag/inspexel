#pragma once

#include "LayoutPart.h"

namespace dynamixel::mx_v1 {

enum class Register : int {
	MODEL_NUMBER        = 0x00,
	VERSION_FIRMWARE    = 0x02,
	ID                  = 0x03,
	BAUD_RATE           = 0x04,
	RETURN_DELAY_TIME   = 0x05,
	CW_ANGLE_LIMIT      = 0x06,
	CCW_ANGLE_LIMIT     = 0x08,
	DRIVE_MODE          = 0x0a,
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
	REALTIME_TICK       = 0x32,
	CURRENT             = 0x44,
	TORQUE_CONTROL_MODE = 0x46,
	GOAL_TORQUE         = 0x47,
	GOAL_ACCELERATION   = 0x49,
};
constexpr Register operator+(Register t1, size_t t2) {
	return Register(size_t(t1) + t2);
}

struct MotorLayoutInfo {
	static constexpr LayoutType Type{LayoutType::MX_V1};
	using FullLayout = Layout<Register::MODEL_NUMBER, 74>;

	static auto getInfos()    -> meta::Layout<Register> const&;
	static auto getDefaults() -> std::map<uint32_t, meta::Info<Register>> const&;
};

using FullLayout = MotorLayoutInfo::FullLayout;

}
namespace dynamixel {

template <> struct meta::MotorLayoutInfo<LayoutType::MX_V1> : mx_v1::MotorLayoutInfo {};


#pragma pack(push, 1)
DynamixelLayoutPart(mx_v1::Register::MODEL_NUMBER        , uint16_t, model_number       );
DynamixelLayoutPart(mx_v1::Register::VERSION_FIRMWARE    ,  uint8_t, version_firmware   );
DynamixelLayoutPart(mx_v1::Register::ID                  ,  uint8_t, id                 );
DynamixelLayoutPart(mx_v1::Register::BAUD_RATE           ,  uint8_t, baud_rate          );
DynamixelLayoutPart(mx_v1::Register::RETURN_DELAY_TIME   ,  uint8_t, return_delay_time  );
DynamixelLayoutPart(mx_v1::Register::CW_ANGLE_LIMIT      ,  int16_t, cw_angle_limit     );
DynamixelLayoutPart(mx_v1::Register::CCW_ANGLE_LIMIT     ,  int16_t, ccw_angle_limit    );
DynamixelLayoutPart(mx_v1::Register::DRIVE_MODE          ,  uint8_t, drive_mode         );
DynamixelLayoutPart(mx_v1::Register::TEMPERATURE_LIMIT   ,  uint8_t, temperature_limit  );
DynamixelLayoutPart(mx_v1::Register::VOLTAGE_LIMIT_LOW   ,  uint8_t, voltage_limit_low  );
DynamixelLayoutPart(mx_v1::Register::VOLTAGE_LIMIT_HIGH  ,  uint8_t, voltage_limit_high );
DynamixelLayoutPart(mx_v1::Register::MAX_TORQUE          , uint16_t, max_torque         );
DynamixelLayoutPart(mx_v1::Register::STATUS_RETURN_LEVEL ,  uint8_t, status_return_level);
DynamixelLayoutPart(mx_v1::Register::ALARM_LED           ,  uint8_t, alarm_led          );
DynamixelLayoutPart(mx_v1::Register::ALARM_SHUTDOWN      , uint16_t, alarm_shutdown     );
DynamixelLayoutPart(mx_v1::Register::MULTI_TURN_OFFSET   ,  int16_t, multi_turn_offset  );
DynamixelLayoutPart(mx_v1::Register::RESOLUTION_DIVIDER  , uint16_t, resolution_divider );
DynamixelLayoutPart(mx_v1::Register::TORQUE_ENABLE       ,     bool, torque_enable      );
DynamixelLayoutPart(mx_v1::Register::LED                 ,     bool, led                );
DynamixelLayoutPart(mx_v1::Register::D_GAIN              ,  uint8_t, d_gain             );
DynamixelLayoutPart(mx_v1::Register::I_GAIN              ,  uint8_t, i_gain             );
DynamixelLayoutPart(mx_v1::Register::P_GAIN              ,  uint8_t, p_gain             );
DynamixelLayoutPart(mx_v1::Register::GOAL_POSITION       ,  int16_t, goal_position      );
DynamixelLayoutPart(mx_v1::Register::MOVING_SPEED        ,  int16_t, moving_speed       );
DynamixelLayoutPart(mx_v1::Register::TORQUE_LIMIT        , uint16_t, torque_limit       );
DynamixelLayoutPart(mx_v1::Register::PRESENT_POSITION    ,  int16_t, present_position   );
DynamixelLayoutPart(mx_v1::Register::PRESENT_SPEED       ,  int16_t, present_speed      );
DynamixelLayoutPart(mx_v1::Register::PRESENT_LOAD        ,  int16_t, present_load       );
DynamixelLayoutPart(mx_v1::Register::PRESENT_VOLTAGE     ,  uint8_t, present_voltage    );
DynamixelLayoutPart(mx_v1::Register::PRESENT_TEMPERATURE ,  uint8_t, present_temperature);
DynamixelLayoutPart(mx_v1::Register::REGISTERED          , uint16_t, registered         );
DynamixelLayoutPart(mx_v1::Register::MOVING              ,     bool, moving             );
DynamixelLayoutPart(mx_v1::Register::LOCK                ,     bool, lock               );
DynamixelLayoutPart(mx_v1::Register::PUNCH               , uint16_t, punch              );
DynamixelLayoutPart(mx_v1::Register::REALTIME_TICK       , uint16_t, realtime_tick      );
DynamixelLayoutPart(mx_v1::Register::CURRENT             ,  int16_t, current            );
DynamixelLayoutPart(mx_v1::Register::TORQUE_CONTROL_MODE ,  uint8_t, torque_control_mode);
DynamixelLayoutPart(mx_v1::Register::GOAL_TORQUE         , uint16_t, goal_torque        );
DynamixelLayoutPart(mx_v1::Register::GOAL_ACCELERATION   ,  uint8_t, goal_acceleration  );

#pragma pack(pop)
}
