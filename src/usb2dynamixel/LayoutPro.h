#pragma once

#include "LayoutPart.h"

namespace dynamixel::pro {

enum class Register : int {
	MODEL_NUMBER           =   0,
	MODEL_INFORMATION      =   2,
	FIRMWARE_VERSION       =   6,
	ID                     =   7,
	BAUD_RATE              =   8,
	RETURN_DELAY_TIME      =   9,
	OPERATING_MODE         =  11,
	HOMING_OFFSET          =  13,
	MOVING_THRESHOLD       =  17,
	TEMPERATURE_LIMIT      =  21,
	MAX_VOLTAGE_LIMIT      =  22,
	MIN_VOLTAGE_LIMIT      =  24,
	ACCELERATION_LIMIT     =  26,
	TORQUE_LIMIT           =  30,
	VELOCITY_LIMIT         =  32,
	MAX_POSITION_LIMIT     =  36,
	MIN_POSITION_LIMIT     =  40,
	EXTERNAL_PORT_MODE_1   =  44,
	EXTERNAL_PORT_MODE_2   =  45,
	EXTERNAL_PORT_MODE_3   =  46,
	EXTERNAL_PORT_MODE_4   =  47,
	SHUTDOWN               =  48,
	INDIRECT_ADDRESS_BLOCK =  49,
	TORQUE_ENABLE          = 562,
	LED_RED                = 563,
	LED_GREEN              = 564,
	LED_BLUE               = 565,
	VELOCITY_I_GAIN        = 586,
	VELOCITY_P_GAIN        = 588,
	POSITION_P_GAIN        = 594,
	GOAL_POSITION          = 596,
	GOAL_VELOCITY          = 600,
	GOAL_TORQUE            = 604,
	GOAL_ACCELERATION      = 606,
	MOVING                 = 610,
	PRESENT_POSITION       = 611,
	PRESENT_VELOCITY       = 615,
	PRESENT_CURRENT        = 621,
	PRESENT_INPUT_VOLTAGE  = 623,
	PRESENT_TEMPERATURE    = 625,
	EXTERNAL_PORT_DATA_1   = 626,
	EXTERNAL_PORT_DATA_2   = 628,
	EXTERNAL_PORT_DATA_3   = 630,
	EXTERNAL_PORT_DATA_4   = 632,
	INDIRECT_DATA_BLOCK    = 534,
	REGISTERED_INSTRUCTION = 890,
	STATUS_RETURN_LEVEL    = 891,
	HARDWARE_ERROR_STATUS  = 892,
};

constexpr Register operator+(Register t1, size_t t2) {
	return Register(size_t(t1) + t2);
}

struct MotorLayoutInfo {
	static constexpr LayoutType Type{LayoutType::Pro};
	using FullLayout = Layout<Register::MODEL_NUMBER, 893>;

	static auto getInfos()    -> meta::Layout<Register> const&;
	static auto getDefaults() -> std::map<uint32_t, meta::Info<Register>> const&;
};

using FullLayout = MotorLayoutInfo::FullLayout;

using IndirectAddresses = std::array<uint16_t, 28>;
using IndirectData      = std::array<uint8_t, 28>;

}

namespace dynamixel {

template <> struct meta::MotorLayoutInfo<LayoutType::Pro> : pro::MotorLayoutInfo {};

#pragma pack(push, 1)
DynamixelLayoutPart(pro::Register::MODEL_NUMBER          ,  uint16_t, model_number          );
DynamixelLayoutPart(pro::Register::MODEL_INFORMATION     ,  uint32_t, model_information     );
DynamixelLayoutPart(pro::Register::FIRMWARE_VERSION      ,   uint8_t, firmware_version      );
DynamixelLayoutPart(pro::Register::ID                    ,   uint8_t, id                    );
DynamixelLayoutPart(pro::Register::BAUD_RATE             ,   uint8_t, baud_rate             );
DynamixelLayoutPart(pro::Register::RETURN_DELAY_TIME     ,   uint8_t, return_delay_time     );
DynamixelLayoutPart(pro::Register::OPERATING_MODE        ,   uint8_t, operating_mode        );
DynamixelLayoutPart(pro::Register::HOMING_OFFSET         ,   int32_t, homing_offset         );
DynamixelLayoutPart(pro::Register::MOVING_THRESHOLD      ,  uint32_t, moving_threshold      );
DynamixelLayoutPart(pro::Register::TEMPERATURE_LIMIT     ,   uint8_t, temperature_limit     );
DynamixelLayoutPart(pro::Register::MAX_VOLTAGE_LIMIT     ,  uint16_t, max_voltage_limit     );
DynamixelLayoutPart(pro::Register::MIN_VOLTAGE_LIMIT     ,  uint16_t, min_voltage_limit     );
DynamixelLayoutPart(pro::Register::ACCELERATION_LIMIT    ,  uint32_t, acceleration_limit    );
DynamixelLayoutPart(pro::Register::TORQUE_LIMIT          ,  uint16_t, torque_limit          );
DynamixelLayoutPart(pro::Register::VELOCITY_LIMIT        ,  uint32_t, velocity_limit        );
DynamixelLayoutPart(pro::Register::MAX_POSITION_LIMIT    ,   int32_t, max_position_limit    );
DynamixelLayoutPart(pro::Register::MIN_POSITION_LIMIT    ,   int32_t, min_position_limit    );
DynamixelLayoutPart(pro::Register::EXTERNAL_PORT_MODE_1  ,   uint8_t, external_port_mode_1  );
DynamixelLayoutPart(pro::Register::EXTERNAL_PORT_MODE_2  ,   uint8_t, external_port_mode_2  );
DynamixelLayoutPart(pro::Register::EXTERNAL_PORT_MODE_3  ,   uint8_t, external_port_mode_3  );
DynamixelLayoutPart(pro::Register::EXTERNAL_PORT_MODE_4  ,   uint8_t, external_port_mode_4  );
DynamixelLayoutPart(pro::Register::SHUTDOWN              ,   uint8_t, shutdown              );
DynamixelLayoutPart(pro::Register::INDIRECT_ADDRESS_BLOCK , pro::IndirectAddresses, indirect_address_block);
DynamixelLayoutPart(pro::Register::TORQUE_ENABLE         ,   uint8_t, torque_enable         );
DynamixelLayoutPart(pro::Register::LED_RED               ,   uint8_t, led_red               );
DynamixelLayoutPart(pro::Register::LED_GREEN             ,   uint8_t, led_green             );
DynamixelLayoutPart(pro::Register::LED_BLUE              ,   uint8_t, led_blue              );
DynamixelLayoutPart(pro::Register::VELOCITY_I_GAIN       ,  uint16_t, velocity_i_gain       );
DynamixelLayoutPart(pro::Register::VELOCITY_P_GAIN       ,  uint16_t, velocity_p_gain       );
DynamixelLayoutPart(pro::Register::POSITION_P_GAIN       ,  uint16_t, position_p_gain       );
DynamixelLayoutPart(pro::Register::GOAL_POSITION         ,   int32_t, goal_position         );
DynamixelLayoutPart(pro::Register::GOAL_VELOCITY         ,   int32_t, goal_velocity         );
DynamixelLayoutPart(pro::Register::GOAL_TORQUE           ,   int16_t, goal_torque           );
DynamixelLayoutPart(pro::Register::GOAL_ACCELERATION     ,  uint32_t, goal_acceleration     );
DynamixelLayoutPart(pro::Register::MOVING                ,   uint8_t, moving                );
DynamixelLayoutPart(pro::Register::PRESENT_POSITION      ,   int32_t, present_position      );
DynamixelLayoutPart(pro::Register::PRESENT_VELOCITY      ,   int32_t, present_velocity      );
DynamixelLayoutPart(pro::Register::PRESENT_CURRENT       ,   int16_t, present_current       );
DynamixelLayoutPart(pro::Register::PRESENT_INPUT_VOLTAGE ,   int16_t, present_input_voltage );
DynamixelLayoutPart(pro::Register::PRESENT_TEMPERATURE   ,   uint8_t, present_temperature   );
DynamixelLayoutPart(pro::Register::EXTERNAL_PORT_DATA_1  ,  uint16_t, external_port_data_1  );
DynamixelLayoutPart(pro::Register::EXTERNAL_PORT_DATA_2  ,  uint16_t, external_port_data_2  );
DynamixelLayoutPart(pro::Register::EXTERNAL_PORT_DATA_3  ,  uint16_t, external_port_data_3  );
DynamixelLayoutPart(pro::Register::EXTERNAL_PORT_DATA_4  ,  uint16_t, external_port_data_4  );
DynamixelLayoutPart(pro::Register::INDIRECT_DATA_BLOCK   ,   pro::IndirectData,      indirect_data_block);
DynamixelLayoutPart(pro::Register::REGISTERED_INSTRUCTION,   uint8_t, registered_instruction);
DynamixelLayoutPart(pro::Register::STATUS_RETURN_LEVEL   ,   uint8_t, status_return_level   );
DynamixelLayoutPart(pro::Register::HARDWARE_ERROR_STATUS ,   uint8_t, hardware_error_status );

#pragma pack(pop)

}
