#pragma once

#include <cstddef>
#include <cassert>
#include <cstring>
#include <bitset>
#include <stdexcept>
#include <vector>

namespace dynamixel::v1 {

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
	CURRENT             = 0x44,
	TORQUE_CONTROL_MODE = 0x46,
	GOAL_TORQUE         = 0x47,
	GOAL_ACCELERATION   = 0x49,
};
using Type = Register;
constexpr Type operator+(Type t1, size_t t2) {
	return Type(size_t(t1) + t2);
}


#pragma pack(push, 1)
template <Type type>
struct LayoutPart {
	using PartType = uint8_t;
	LayoutPart() = default;
	LayoutPart(LayoutPart const& _other) = default;
	auto operator=(LayoutPart const& _other) -> LayoutPart& = default;

	LayoutPart(uint8_t value)
		: _oneByte {std::move(value)}
	{}

	uint8_t _oneByte {};
	template <typename L> void visit(L) const {} \
	template <typename L> void visit(L) {} \
};

#define LayoutPart(enum, type, name) \
template <> \
struct LayoutPart<enum> { \
	using PartType = type; \
	LayoutPart() = default; \
	LayoutPart(LayoutPart const& _other) = default; \
	auto operator=(LayoutPart const& _other) -> LayoutPart& = default; \
	LayoutPart(type value) \
		: name {value} \
	{} \
	\
	type name {}; \
	template <typename L> void visit(L l) const { l(enum, name); }\
	template <typename L> void visit(L l) { l(enum, name); }\
};

LayoutPart(Type::MODEL_NUMBER        , uint16_t, model_number       );
LayoutPart(Type::VERSION_FIRMWARE    , uint8_t , version_firmware   );
LayoutPart(Type::ID                  , uint8_t , id                 );
LayoutPart(Type::BAUD_RATE           , uint8_t , baud_rate          );
LayoutPart(Type::RETURN_DELAY_TIME   , uint8_t , return_delay_time  );
LayoutPart(Type::CW_ANGLE_LIMIT      , int16_t , cw_angle_limit     );
LayoutPart(Type::CCW_ANGLE_LIMIT     , int16_t , ccw_angle_limit    );
LayoutPart(Type::DRIVE_MODE          , uint8_t , drive_mode         );
LayoutPart(Type::TEMPERATURE_LIMIT   , uint8_t , temperature_limit  );
LayoutPart(Type::VOLTAGE_LIMIT_LOW   , uint8_t , voltage_limit_low  );
LayoutPart(Type::VOLTAGE_LIMIT_HIGH  , uint8_t , voltage_limit_high );
LayoutPart(Type::MAX_TORQUE          , uint16_t, max_torque         );
LayoutPart(Type::STATUS_RETURN_LEVEL , uint8_t , status_return_level);
LayoutPart(Type::ALARM_LED           , uint8_t , alarm_led          );
LayoutPart(Type::ALARM_SHUTDOWN      , uint16_t, alarm_shutdown     );
LayoutPart(Type::MULTI_TURN_OFFSET   , int16_t , multi_turn_offset  );
LayoutPart(Type::RESOLUTION_DIVIDER  , uint16_t, resolution_divider );
LayoutPart(Type::TORQUE_ENABLE       ,    bool , torque_enable      );
LayoutPart(Type::LED                 ,    bool , led                );
LayoutPart(Type::D_GAIN              , uint8_t , d_gain             );
LayoutPart(Type::I_GAIN              , uint8_t , i_gain             );
LayoutPart(Type::P_GAIN              , uint8_t , p_gain             );
LayoutPart(Type::GOAL_POSITION       , int16_t , goal_position      );
LayoutPart(Type::MOVING_SPEED        , int16_t , moving_speed       );
LayoutPart(Type::TORQUE_LIMIT        , uint16_t, torque_limit       );
LayoutPart(Type::PRESENT_POSITION    , int16_t , present_position   );
LayoutPart(Type::PRESENT_SPEED       , int16_t , present_speed      );
LayoutPart(Type::PRESENT_LOAD        , int16_t , present_load       );
LayoutPart(Type::PRESENT_VOLTAGE     , uint8_t , present_voltage    );
LayoutPart(Type::PRESENT_TEMPERATURE , uint8_t , present_temperature);
LayoutPart(Type::REGISTERED          , uint16_t, registered         );
LayoutPart(Type::MOVING              ,    bool , moving             );
LayoutPart(Type::LOCK                ,    bool , lock               );
LayoutPart(Type::PUNCH               , uint16_t, punch              );
LayoutPart(Type::CURRENT             , int16_t , current            );
LayoutPart(Type::TORQUE_CONTROL_MODE , uint8_t , torque_control_mode);
LayoutPart(Type::GOAL_TORQUE         , uint16_t, goal_torque        );
LayoutPart(Type::GOAL_ACCELERATION   , uint8_t , goal_acceleration  );

template <Type type, size_t L>
struct Layout : LayoutPart<type> , Layout<type+sizeof(LayoutPart<type>), L-sizeof(LayoutPart<type>)> {
	using SuperClass = Layout<type+sizeof(LayoutPart<type>), L-sizeof(LayoutPart<type>)>;
	using Part = LayoutPart<type>;
	using PartType = typename Part::PartType;

	Layout() = default;
	Layout(Layout const& _other) = default;
	auto operator=(Layout const& _other) -> Layout& = default;
	explicit Layout(std::vector<std::byte> const& buffer) {
		if (buffer.size() != sizeof(Layout)) {
			throw std::runtime_error("buffer " + std::to_string(buffer.size()) + " has not same size as layout " + std::to_string(sizeof(Layout)));
		}
		memcpy((void*)this, buffer.data(), sizeof(Layout));
	}

	template <typename ...Args>
	explicit Layout(PartType head, Args...next)
		: LayoutPart<type>{head}
		, SuperClass{next...}
	{}

	static_assert(L >= sizeof(LayoutPart<type>), "must fit layout size");
	static constexpr Type BaseRegister {type};
	static constexpr size_t Length {L};

	template <Type type2>
	struct Has {
		static constexpr bool value = type <= type2 and int(type) + L > int(type2);
	};
	template <Type type2>
	static constexpr bool has = Has<type2>::value;

	template <typename CB>
	void visit(CB cb) const {
		LayoutPart<type>::visit(cb);
		SuperClass::visit(cb);
	}
	template <typename CB>
	void visit(CB cb) {
		LayoutPart<type>::visit(cb);
		SuperClass::visit(cb);
	}
};

template <Type type>
struct Layout<type, 0> {
	template <typename L> void visit(L) const {}
	template <typename L> void visit(L) {}
};

template <typename CB, auto Register, size_t L>
void visit(CB cb, Layout<Register, L> const& o) {
	o.visit(cb);
}

template <typename CB, auto Register, size_t L>
void visit(CB cb, Layout<Register, L>& o) {
	o.visit(cb);
}

using FullLayout = Layout<Register::MODEL_NUMBER, 74>;


#pragma pack(pop)
#undef LayoutPart
}
