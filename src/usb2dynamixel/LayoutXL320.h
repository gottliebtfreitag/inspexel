#pragma once

#include "LayoutPart.h"

namespace dynamixel::xl320 {

enum class Register : int {
	MODEL_NUMBER          = 0x00,
	FIRMWARE_VERSION      = 0x02,
	ID                    = 0x03,
	BAUD_RATE             = 0x04,
	RETURN_DELAY_TIME     = 0x05,
	CW_ANGLE_LIMIT        = 0x06,
	CCW_ANGLE_LIMIT       = 0x08,
	CONTROL_MODE          = 0x0b,
	TEMPERATURE_LIMIT     = 0x0c,
	MIN_VOLTAGE_LIMIT     = 0x0d,
	MAX_VOLTAGE_LIMIT     = 0x0e,
	MAX_TORQUE            = 0x0f,
	STATUS_RETURN_LEVEL   = 0x11,
	SHUTDOWN              = 0x12,
	TORQUE_ENABLE         = 0x18,
	LED                   = 0x19,
	D_GAIN                = 0x1a,
	I_GAIN                = 0x1b,
	P_GAIN                = 0x1c,
	GOAL_POSITION         = 0x1e,
	MOVING_SPEED          = 0x20,
	TORQUE_LIMIT          = 0x23,
	PRESENT_POSITION      = 0x25,
	PRESENT_SPEED         = 0x27,
	PRESENT_LOAD          = 0x29,
	PRESENT_VOLTAGE       = 0x2d,
	PRESENT_TEMPERATURE   = 0x2e,
	REGISTERED            = 0x2f,
	MOVING                = 0x31,
	HARDWARE_ERROR_STATUS = 0x32,
	PUNCH                 = 0x33,
};

constexpr Register operator+(Register t1, size_t t2) {
	return Register(size_t(t1) + t2);
}

struct MotorLayoutInfo {
	static constexpr LayoutType Type{LayoutType::XL320};
	using FullLayout = Layout<Register::MODEL_NUMBER, 53>;

	static auto getInfos()    -> meta::Layout<Register> const&;
	static auto getDefaults() -> std::map<uint32_t, meta::Info<Register>> const&;
};

using FullLayout = MotorLayoutInfo::FullLayout;

}
namespace dynamixel {

template <> struct meta::MotorLayoutInfo<LayoutType::XL320> : xl320::MotorLayoutInfo {};

#pragma pack(push, 1)
DynamixelLayoutPart(xl320::Register::MODEL_NUMBER         , uint16_t, model_number         );
DynamixelLayoutPart(xl320::Register::FIRMWARE_VERSION     ,  uint8_t, firmware_version     );
DynamixelLayoutPart(xl320::Register::ID                   ,  uint8_t, id                   );
DynamixelLayoutPart(xl320::Register::BAUD_RATE            ,  uint8_t, baud_rate            );
DynamixelLayoutPart(xl320::Register::RETURN_DELAY_TIME    ,  uint8_t, return_delay_time    );
DynamixelLayoutPart(xl320::Register::CW_ANGLE_LIMIT       ,  int16_t, cw_angle_limit       );
DynamixelLayoutPart(xl320::Register::CCW_ANGLE_LIMIT      ,  int16_t, ccw_angle_limit      );
DynamixelLayoutPart(xl320::Register::CONTROL_MODE         ,  uint8_t, control_mode         );
DynamixelLayoutPart(xl320::Register::TEMPERATURE_LIMIT    ,  uint8_t, temperature_limit    );
DynamixelLayoutPart(xl320::Register::MIN_VOLTAGE_LIMIT    ,  uint8_t, min_voltage_limit    );
DynamixelLayoutPart(xl320::Register::MAX_VOLTAGE_LIMIT    ,  uint8_t, max_voltage_limit    );
DynamixelLayoutPart(xl320::Register::MAX_TORQUE           , uint16_t, max_torque           );
DynamixelLayoutPart(xl320::Register::STATUS_RETURN_LEVEL  ,  uint8_t, status_return_level  );
DynamixelLayoutPart(xl320::Register::SHUTDOWN             ,  uint8_t, shutdown             );
DynamixelLayoutPart(xl320::Register::TORQUE_ENABLE        ,     bool, torque_enable        );
DynamixelLayoutPart(xl320::Register::LED                  ,     bool, led                  );
DynamixelLayoutPart(xl320::Register::D_GAIN               ,  uint8_t, d_gain               );
DynamixelLayoutPart(xl320::Register::I_GAIN               ,  uint8_t, i_gain               );
DynamixelLayoutPart(xl320::Register::P_GAIN               ,  uint8_t, p_gain               );
DynamixelLayoutPart(xl320::Register::GOAL_POSITION        ,  int16_t, goal_position        );
DynamixelLayoutPart(xl320::Register::MOVING_SPEED         ,  int16_t, moving_speed         );
DynamixelLayoutPart(xl320::Register::TORQUE_LIMIT         , uint16_t, torque_limit         );
DynamixelLayoutPart(xl320::Register::PRESENT_POSITION     ,  int16_t, present_position     );
DynamixelLayoutPart(xl320::Register::PRESENT_SPEED        ,  int16_t, present_speed        );
DynamixelLayoutPart(xl320::Register::PRESENT_LOAD         ,  int16_t, present_load         );
DynamixelLayoutPart(xl320::Register::PRESENT_VOLTAGE      ,  uint8_t, present_voltage      );
DynamixelLayoutPart(xl320::Register::PRESENT_TEMPERATURE  ,  uint8_t, present_temperature  );
DynamixelLayoutPart(xl320::Register::REGISTERED           ,  uint8_t, registered           );
DynamixelLayoutPart(xl320::Register::MOVING               ,     bool, moving               );
DynamixelLayoutPart(xl320::Register::HARDWARE_ERROR_STATUS,  uint8_t, hardware_error_status);
DynamixelLayoutPart(xl320::Register::PUNCH                , uint16_t, punch                );

#pragma pack(pop)

}
