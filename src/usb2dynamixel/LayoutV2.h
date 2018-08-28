#pragma once

#include <bitset>

namespace dynamixel {

namespace v2 {
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
	PRESENT_TEMPERATURE    = 146
};

using Type = Register;

constexpr Type operator+(Type t1, size_t t2) {
	return Type(size_t(t1) + t2);
}

#pragma pack(push, 1)

template <Type type>
struct LayoutPart {
	LayoutPart() = default;
	LayoutPart(LayoutPart const& _other) = default;
	auto operator=(LayoutPart const& _other) -> LayoutPart& = default;
	LayoutPart(uint8_t value)
		: _oneByte {std::move(value)}
	{}
private:
	uint8_t _oneByte;
};

#define LayoutPart(enum, type, name) \
template <> \
struct LayoutPart<enum> { \
	LayoutPart() = default; \
	LayoutPart(LayoutPart const& _other) = default; \
	auto operator=(LayoutPart const& _other) -> LayoutPart& = default; \
	LayoutPart(type value) \
		: name {std::move(value)} \
	{} \
	type name; \
};

LayoutPart(Type::MODEL_NUMBER           , uint16_t, model_number           );
LayoutPart(Type::MODEL_INFORMATION      , uint32_t, model_information      );
LayoutPart(Type::VERSION_FIRMWARE       , uint8_t , version_firmware       );
LayoutPart(Type::ID                     , uint8_t , id                     );
LayoutPart(Type::BAUD_RATE              , uint8_t , baud_rate              );
LayoutPart(Type::RETURN_DELAY_TIME      , uint8_t , return_delay_time      );
LayoutPart(Type::DRIVE_MODE             , uint8_t , drive_mode             );
LayoutPart(Type::OPERATING_MODE         , uint8_t , operating_mode         );
LayoutPart(Type::SECONDARY_ID           , uint8_t , secondary_id           );
LayoutPart(Type::PROTOCOL_VERSION       , uint8_t , protocol_version       );
LayoutPart(Type::HOMING_OFFSET          ,  int32_t, homing_offset          );
LayoutPart(Type::MOVING_THRESHOLD       ,  int32_t, moving_threshold       );
LayoutPart(Type::TEMPERATURE_LIMIT      , uint8_t , temperature_limit      );
LayoutPart(Type::MAX_VOLTAGE_LIMIT      , uint16_t, max_voltage_limit      );
LayoutPart(Type::MIN_VOLTAGE_LIMIT      , uint16_t, min_voltage_limit      );
LayoutPart(Type::PWM_LIMIT              , uint16_t, pwm_limit              );
LayoutPart(Type::CURRENT_LIMIT          , uint16_t, current_limit          );
LayoutPart(Type::ACCELERATION_LIMIT     , uint32_t, acceleration_limit     );
LayoutPart(Type::VELOCITY_LIMIT         , uint32_t, velocity_limit         );
LayoutPart(Type::MAX_POSITION_LIMIT     , uint32_t, max_position_limit     );
LayoutPart(Type::MIN_POSITION_LIMIT     , uint32_t, min_position_limit     );
LayoutPart(Type::SHUTDOWN               , uint8_t , shutdown               );
LayoutPart(Type::TORQUE_ENABLE          ,    bool , torque_enable          );
LayoutPart(Type::LED                    ,    bool , led                    );
LayoutPart(Type::STATUS_RETURN_LEVEL    , uint8_t , status_return_level    );
LayoutPart(Type::REGISTERED_INSTRUCTION , uint8_t , registered_instruction );
LayoutPart(Type::HARDWARE_ERROR_STATUS  , std::bitset<8> , hardware_error_status  );
LayoutPart(Type::VELOCITY_I_GAIN        , uint16_t, velocity_i_gain        );
LayoutPart(Type::VELOCITY_P_GAIN        , uint16_t, velocity_p_gain        );
LayoutPart(Type::POSITION_D_GAIN        , uint16_t, position_d_gain        );
LayoutPart(Type::POSITION_I_GAIN        , uint16_t, position_i_gain        );
LayoutPart(Type::POSITION_P_GAIN        , uint16_t, position_p_gain        );
LayoutPart(Type::FEEDFORWARD_2ND_GAIN   , uint16_t, feedforward_2nd_gain   );
LayoutPart(Type::FEEDFORWARD_1ST_GAIN   , uint16_t, feedforward_1st_gain   );
LayoutPart(Type::BUS_WATCHDOG           , uint8_t , bus_watchdog           );
LayoutPart(Type::GOAL_PWM               , uint16_t, goal_pwm               );
LayoutPart(Type::GOAL_CURRENT           ,  int16_t, goal_current           );
LayoutPart(Type::GOAL_VELOCITY          ,  int32_t, goal_velocity          )
LayoutPart(Type::PROFILE_ACCELERATION   ,  int32_t, profile_acceleration   )
LayoutPart(Type::PROFILE_VELOCITY       ,  int32_t, profile_velocity       )
LayoutPart(Type::GOAL_POSITION          ,  int32_t, goal_position          )
LayoutPart(Type::REALTIME_TICK          , uint16_t, realtime_tick          )
LayoutPart(Type::MOVING                 ,    bool , moving                 )
LayoutPart(Type::MOVING_STATUS          , uint8_t , moving_status          )
LayoutPart(Type::PRESENT_PWM            , uint16_t, present_pwm            )
LayoutPart(Type::PRESENT_CURRENT        ,  int16_t, present_current        )
LayoutPart(Type::PRESENT_VELOCITY       ,  int32_t, present_velocity       )
LayoutPart(Type::PRESENT_POSITION       ,  int32_t, present_position       )
LayoutPart(Type::VELOCITY_TRAJECTORY    ,  int32_t, velocity_trajectory    )
LayoutPart(Type::POSITION_TRAJECTORY    ,  int32_t, position_trajectory    )
LayoutPart(Type::PRESENT_INPUT_VOLTAGE  , uint16_t, present_input_voltage  );
LayoutPart(Type::PRESENT_TEMPERATURE    , uint8_t , present_temperature    );


template <Type type, size_t L>
struct Layout : LayoutPart<type> , Layout<type+sizeof(LayoutPart<type>), L-sizeof(LayoutPart<type>)> {
	Layout() = default;
	Layout(Layout const& _other) = default;
	auto operator=(Layout const& _other) -> Layout& = default;

	template <typename Head, typename ...Args>
	Layout(Head&& head, Args&&...next)
		: LayoutPart<type>{std::forward<Head>(head)}
		, Layout<type+sizeof(LayoutPart<type>), L-sizeof(LayoutPart<type>)>{std::forward<Args>(next)...}
	{}

	static_assert(L >= sizeof(LayoutPart<type>), "must fit layout size");
	static constexpr size_t BaseRegister {size_t(type)};
	static constexpr size_t Length {L};

	template <Type type2>
	struct Has {
		static constexpr bool value = type <= type2 and int(type) + L > int(type2);
	};
	template <Type type2>
	static constexpr bool has = Has<type2>::value;

};

template <Type type>
struct Layout<type, 0> {};

#pragma pack(pop)
}

#undef LayoutPart
}
