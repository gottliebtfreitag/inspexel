#pragma once

#include <cstddef>
#include <cassert>
#include <cstring>
#include <bitset>
#include <stdexcept>
#include <vector>

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
		: _oneByte {value}
	{}

	PartType _oneByte {};
	template <typename L> void visit(L l) const { l(type, _oneByte); }
	template <typename L> void visit(L l) { l(type, _oneByte); }
	[[nodiscard]] bool reserved() const { return true; }
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
	template <typename L> void visit(L l) const { l(enum, name); } \
	template <typename L> void visit(L l) { l(enum, name); } \
	bool reserved() const { return false; } \
};

using IndirectAddresses = std::array<uint16_t, 256>;
using IndirectData      = std::array<uint8_t, 256>;
LayoutPart(Type::MODEL_NUMBER          ,  uint16_t, model_number          );
LayoutPart(Type::MODEL_INFORMATION     ,  uint32_t, model_information     );
LayoutPart(Type::FIRMWARE_VERSION      ,   uint8_t, firmware_version      );
LayoutPart(Type::ID                    ,   uint8_t, id                    );
LayoutPart(Type::BAUD_RATE             ,   uint8_t, baud_rate             );
LayoutPart(Type::RETURN_DELAY_TIME     ,   uint8_t, return_delay_time     );
LayoutPart(Type::OPERATING_MODE        ,   uint8_t, operating_mode        );
LayoutPart(Type::HOMING_OFFSET         ,   int32_t, homing_offset         );
LayoutPart(Type::MOVING_THRESHOLD      ,  uint32_t, moving_threshold      );
LayoutPart(Type::TEMPERATURE_LIMIT     ,   uint8_t, temperature_limit     );
LayoutPart(Type::MAX_VOLTAGE_LIMIT     ,  uint16_t, max_voltage_limit     );
LayoutPart(Type::MIN_VOLTAGE_LIMIT     ,  uint16_t, min_voltage_limit     );
LayoutPart(Type::ACCELERATION_LIMIT    ,  uint32_t, acceleration_limit    );
LayoutPart(Type::TORQUE_LIMIT          ,  uint16_t, torque_limit          );
LayoutPart(Type::VELOCITY_LIMIT        ,  uint32_t, velocity_limit        );
LayoutPart(Type::MAX_POSITION_LIMIT    ,   int32_t, max_position_limit    );
LayoutPart(Type::MIN_POSITION_LIMIT    ,   int32_t, min_position_limit    );
LayoutPart(Type::EXTERNAL_PORT_MODE_1  ,   uint8_t, external_port_mode_1  );
LayoutPart(Type::EXTERNAL_PORT_MODE_2  ,   uint8_t, external_port_mode_2  );
LayoutPart(Type::EXTERNAL_PORT_MODE_3  ,   uint8_t, external_port_mode_3  );
LayoutPart(Type::EXTERNAL_PORT_MODE_4  ,   uint8_t, external_port_mode_4  );
LayoutPart(Type::SHUTDOWN              ,   uint8_t, shutdown              );
LayoutPart(Type::INDIRECT_ADDRESS_BLOCK , IndirectAddresses, indirect_address_block);
LayoutPart(Type::TORQUE_ENABLE         ,   uint8_t, torque_enable         );
LayoutPart(Type::LED_RED               ,   uint8_t, led_red               );
LayoutPart(Type::LED_GREEN             ,   uint8_t, led_green             );
LayoutPart(Type::LED_BLUE              ,   uint8_t, led_blue              );
LayoutPart(Type::VELOCITY_I_GAIN       ,  uint16_t, velocity_i_gain       );
LayoutPart(Type::VELOCITY_P_GAIN       ,  uint16_t, velocity_p_gain       );
LayoutPart(Type::POSITION_P_GAIN       ,  uint16_t, position_p_gain       );
LayoutPart(Type::GOAL_POSITION         ,   int32_t, goal_position         );
LayoutPart(Type::GOAL_VELOCITY         ,   int32_t, goal_velocity         );
LayoutPart(Type::GOAL_TORQUE           ,   int16_t, goal_torque           );
LayoutPart(Type::GOAL_ACCELERATION     ,  uint32_t, goal_acceleration     );
LayoutPart(Type::MOVING                ,   uint8_t, moving                );
LayoutPart(Type::PRESENT_POSITION      ,   int32_t, present_position      );
LayoutPart(Type::PRESENT_VELOCITY      ,   int32_t, present_velocity      );
LayoutPart(Type::PRESENT_CURRENT       ,   int16_t, present_current       );
LayoutPart(Type::PRESENT_INPUT_VOLTAGE ,   int16_t, present_input_voltage );
LayoutPart(Type::PRESENT_TEMPERATURE   ,   uint8_t, present_temperature   );
LayoutPart(Type::EXTERNAL_PORT_DATA_1  ,  uint16_t, external_port_data_1  );
LayoutPart(Type::EXTERNAL_PORT_DATA_2  ,  uint16_t, external_port_data_2  );
LayoutPart(Type::EXTERNAL_PORT_DATA_3  ,  uint16_t, external_port_data_3  );
LayoutPart(Type::EXTERNAL_PORT_DATA_4  ,  uint16_t, external_port_data_4  );
LayoutPart(Type::INDIRECT_DATA_BLOCK   ,   IndirectData,      indirect_data_block);
LayoutPart(Type::REGISTERED_INSTRUCTION,   uint8_t, registered_instruction);
LayoutPart(Type::STATUS_RETURN_LEVEL   ,   uint8_t, status_return_level   );
LayoutPart(Type::HARDWARE_ERROR_STATUS ,   uint8_t, hardware_error_status );

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
	Layout(PartType head, Args...next)
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

using FullLayout = Layout<Register::MODEL_NUMBER, 893>;


#pragma pack(pop)
#undef LayoutPart
}
