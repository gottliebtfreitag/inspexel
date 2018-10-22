#pragma once

#include "LayoutPart.h"

namespace dynamixel::mx_v2 {

enum class Register : int {
	MODEL_NUMBER           = 0x00,
	MODEL_INFORMATION      = 0x02,
	VERSION_FIRMWARE       = 0x06,
	ID                     = 0x07,
	BAUD_RATE              = 0x08,
	RETURN_DELAY_TIME      = 0x09,
	DRIVE_MODE             = 0x0a,
	OPERATING_MODE         = 0x0b,
	SECONDARY_ID           = 12,
	PROTOCOL_VERSION       = 13,
	HOMING_OFFSET          = 20,
	MOVING_THRESHOLD       = 24,
	TEMPERATURE_LIMIT      = 31,
	MAX_VOLTAGE_LIMIT      = 32,
	MIN_VOLTAGE_LIMIT      = 34,
	PWM_LIMIT              = 36,
	CURRENT_LIMIT          = 38,
	ACCELERATION_LIMIT     = 40,
	VELOCITY_LIMIT         = 44,
	MAX_POSITION_LIMIT     = 48,
	MIN_POSITION_LIMIT     = 52,
	EXTERNAL_PORT_MODE_1   = 56,
	EXTERNAL_PORT_MODE_2   = 57,
	EXTERNAL_PORT_MODE_3   = 58,
	SHUTDOWN               = 63,
	TORQUE_ENABLE          = 64,
	LED                    = 65,
	STATUS_RETURN_LEVEL    = 68,
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
	PRESENT_TEMPERATURE    = 146,
	INDIRECT_ADDRESS_BLOCK1 = 168,
	INDIRECT_DATA_BLOCK1    = 224,
	INDIRECT_ADDRESS_BLOCK2 = 578,
	INDIRECT_DATA_BLOCK2    = 634,
};
constexpr Register operator+(Register t1, size_t t2) {
	return Register(size_t(t1) + t2);
}

struct MotorLayoutInfo {
	static constexpr LayoutType Type{LayoutType::MX_V2};
	using FullLayout = Layout<Register::MODEL_NUMBER, 147>;

	static auto getInfos()    -> meta::Layout<Register> const&;
	static auto getDefaults() -> std::map<uint32_t, meta::Info<Register>> const&;
};

using FullLayout = MotorLayoutInfo::FullLayout;

using IndirectAddresses = std::array<uint16_t, 28>;
using IndirectData      = std::array<uint8_t, 28>;

}

namespace dynamixel {

template <> struct meta::MotorLayoutInfo<LayoutType::MX_V2> : mx_v2::MotorLayoutInfo {};

#pragma pack(push, 1)
DynamixelLayoutPart(mx_v2::Register::MODEL_NUMBER           , uint16_t, model_number           );
DynamixelLayoutPart(mx_v2::Register::MODEL_INFORMATION      , uint32_t, model_information      );
DynamixelLayoutPart(mx_v2::Register::VERSION_FIRMWARE       ,  uint8_t, version_firmware       );
DynamixelLayoutPart(mx_v2::Register::ID                     ,  uint8_t, id                     );
DynamixelLayoutPart(mx_v2::Register::BAUD_RATE              ,  uint8_t, baud_rate              );
DynamixelLayoutPart(mx_v2::Register::RETURN_DELAY_TIME      ,  uint8_t, return_delay_time      );
DynamixelLayoutPart(mx_v2::Register::DRIVE_MODE             ,  uint8_t, drive_mode             );
DynamixelLayoutPart(mx_v2::Register::OPERATING_MODE         ,  uint8_t, operating_mode         );
DynamixelLayoutPart(mx_v2::Register::SECONDARY_ID           ,  uint8_t, secondary_id           );
DynamixelLayoutPart(mx_v2::Register::PROTOCOL_VERSION       ,  uint8_t, protocol_version       );
DynamixelLayoutPart(mx_v2::Register::HOMING_OFFSET          ,  int32_t, homing_offset          );
DynamixelLayoutPart(mx_v2::Register::MOVING_THRESHOLD       ,  int32_t, moving_threshold       );
DynamixelLayoutPart(mx_v2::Register::TEMPERATURE_LIMIT      ,  uint8_t, temperature_limit      );
DynamixelLayoutPart(mx_v2::Register::MAX_VOLTAGE_LIMIT      , uint16_t, max_voltage_limit      );
DynamixelLayoutPart(mx_v2::Register::MIN_VOLTAGE_LIMIT      , uint16_t, min_voltage_limit      );
DynamixelLayoutPart(mx_v2::Register::PWM_LIMIT              , uint16_t, pwm_limit              );
DynamixelLayoutPart(mx_v2::Register::CURRENT_LIMIT          , uint16_t, current_limit          );
DynamixelLayoutPart(mx_v2::Register::ACCELERATION_LIMIT     , uint32_t, acceleration_limit     );
DynamixelLayoutPart(mx_v2::Register::VELOCITY_LIMIT         , uint32_t, velocity_limit         );
DynamixelLayoutPart(mx_v2::Register::MAX_POSITION_LIMIT     ,  int32_t, max_position_limit     );
DynamixelLayoutPart(mx_v2::Register::MIN_POSITION_LIMIT     ,  int32_t, min_position_limit     );
DynamixelLayoutPart(mx_v2::Register::EXTERNAL_PORT_MODE_1   ,  uint8_t, external_port_mode_1   );
DynamixelLayoutPart(mx_v2::Register::EXTERNAL_PORT_MODE_2   ,  uint8_t, external_port_mode_2   );
DynamixelLayoutPart(mx_v2::Register::EXTERNAL_PORT_MODE_3   ,  uint8_t, external_port_mode_3   );
DynamixelLayoutPart(mx_v2::Register::SHUTDOWN               ,  uint8_t, shutdown               );
DynamixelLayoutPart(mx_v2::Register::TORQUE_ENABLE          ,     bool, torque_enable          );
DynamixelLayoutPart(mx_v2::Register::LED                    ,     bool, led                    );
DynamixelLayoutPart(mx_v2::Register::STATUS_RETURN_LEVEL    ,  uint8_t, status_return_level    );
DynamixelLayoutPart(mx_v2::Register::REGISTERED_INSTRUCTION ,  uint8_t, registered_instruction );
DynamixelLayoutPart(mx_v2::Register::HARDWARE_ERROR_STATUS  ,  uint8_t, hardware_error_status  );
DynamixelLayoutPart(mx_v2::Register::VELOCITY_I_GAIN        , uint16_t, velocity_i_gain        );
DynamixelLayoutPart(mx_v2::Register::VELOCITY_P_GAIN        , uint16_t, velocity_p_gain        );
DynamixelLayoutPart(mx_v2::Register::POSITION_D_GAIN        , uint16_t, position_d_gain        );
DynamixelLayoutPart(mx_v2::Register::POSITION_I_GAIN        , uint16_t, position_i_gain        );
DynamixelLayoutPart(mx_v2::Register::POSITION_P_GAIN        , uint16_t, position_p_gain        );
DynamixelLayoutPart(mx_v2::Register::FEEDFORWARD_2ND_GAIN   , uint16_t, feedforward_2nd_gain   );
DynamixelLayoutPart(mx_v2::Register::FEEDFORWARD_1ST_GAIN   , uint16_t, feedforward_1st_gain   );
DynamixelLayoutPart(mx_v2::Register::BUS_WATCHDOG           ,  uint8_t, bus_watchdog           );
DynamixelLayoutPart(mx_v2::Register::GOAL_PWM               , uint16_t, goal_pwm               );
DynamixelLayoutPart(mx_v2::Register::GOAL_CURRENT           ,  int16_t, goal_current           );
DynamixelLayoutPart(mx_v2::Register::GOAL_VELOCITY          ,  int32_t, goal_velocity          )
DynamixelLayoutPart(mx_v2::Register::PROFILE_ACCELERATION   ,  int32_t, profile_acceleration   )
DynamixelLayoutPart(mx_v2::Register::PROFILE_VELOCITY       ,  int32_t, profile_velocity       )
DynamixelLayoutPart(mx_v2::Register::GOAL_POSITION          ,  int32_t, goal_position          )
DynamixelLayoutPart(mx_v2::Register::REALTIME_TICK          , uint16_t, realtime_tick          )
DynamixelLayoutPart(mx_v2::Register::MOVING                 ,     bool, moving                 )
DynamixelLayoutPart(mx_v2::Register::MOVING_STATUS          ,  uint8_t, moving_status          )
DynamixelLayoutPart(mx_v2::Register::PRESENT_PWM            , uint16_t, present_pwm            )
DynamixelLayoutPart(mx_v2::Register::PRESENT_CURRENT        ,  int16_t, present_current        )
DynamixelLayoutPart(mx_v2::Register::PRESENT_VELOCITY       ,  int32_t, present_velocity       )
DynamixelLayoutPart(mx_v2::Register::PRESENT_POSITION       ,  int32_t, present_position       )
DynamixelLayoutPart(mx_v2::Register::VELOCITY_TRAJECTORY    ,  int32_t, velocity_trajectory    )
DynamixelLayoutPart(mx_v2::Register::POSITION_TRAJECTORY    ,  int32_t, position_trajectory    )
DynamixelLayoutPart(mx_v2::Register::PRESENT_INPUT_VOLTAGE  , uint16_t, present_input_voltage  );
DynamixelLayoutPart(mx_v2::Register::PRESENT_TEMPERATURE    ,  uint8_t, present_temperature    );
DynamixelLayoutPart(mx_v2::Register::INDIRECT_ADDRESS_BLOCK1 , mx_v2::IndirectAddresses, indirect_address_block1);
DynamixelLayoutPart(mx_v2::Register::INDIRECT_DATA_BLOCK1  ,   mx_v2::IndirectData,      indirect_data_block1);
DynamixelLayoutPart(mx_v2::Register::INDIRECT_ADDRESS_BLOCK2 , mx_v2::IndirectAddresses, indirect_address_block2);
DynamixelLayoutPart(mx_v2::Register::INDIRECT_DATA_BLOCK2 ,    mx_v2::IndirectData,      indirect_data_block2);

#pragma pack(pop)
}
