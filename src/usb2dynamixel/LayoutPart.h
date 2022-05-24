#pragma once

#include <cmath>
#include <cstddef>
#include <cstring>
#include <functional>
#include <map>
#include <optional>
#include <stdexcept>
#include <vector>

namespace dynamixel {

template <auto type>
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

#define DynamixelLayoutPart(enum, type, name) \
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

template <auto type, size_t L>
struct Layout : LayoutPart<type> , Layout<type+sizeof(LayoutPart<type>), L-sizeof(LayoutPart<type>)> {
	using SuperClass = Layout<type+sizeof(LayoutPart<type>), L-sizeof(LayoutPart<type>)>;
	using Part = LayoutPart<type>;
	using PartType = typename Part::PartType;
	using Type = std::decay_t<decltype(type)>;

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

template <auto type>
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

enum class LayoutType { None, MX_V1, MX_V2, Pro, XL320, AX };

inline auto to_string(LayoutType layout) -> std::string {
	switch(layout) {
	case LayoutType::MX_V1:    return "MX_V1";
	case LayoutType::MX_V2:    return "MX_V2";
	case LayoutType::Pro:   return "Pro";
	case LayoutType::XL320: return "XL320";
	case LayoutType::AX:    return "AX";
	default:
		throw std::runtime_error("unknown layout");
	}
}


namespace meta {
struct LayoutField {
	uint16_t   length;
	bool       romArea;
	enum class Access { R = 0x01, W = 0x02, RW = 0x03 };
	Access      access;
	std::string name;
	std::string description;
};

inline auto to_string(LayoutField::Access access) -> std::string {
	switch(access) {
	case LayoutField::Access::R: return "R";
	case LayoutField::Access::W: return "W";
	case LayoutField::Access::RW: return "RW";
	}
	throw std::runtime_error("unknown access value");
}

struct Convert {
	std::string unit;
	std::function<int(double)> toMotor;
	std::function<double(int)> fromMotor;
};

template <typename Reg>
using Layout = std::map<Reg, LayoutField>;


template <typename Reg>
using DefaultLayout = std::map<Reg, std::tuple<std::optional<int32_t>, Convert>>;

template <typename Register>
struct Info {
	uint16_t modelNumber;
	LayoutType layout;

	std::string shortName;
	std::vector<std::string> motorNames;

	DefaultLayout<Register> defaultLayout;
};

inline auto buildConverter(std::string unit, double resolution, int centerVal=0, int minValue=std::numeric_limits<int>::min(), int maxValue = std::numeric_limits<int>::max()) -> Convert {
	return Convert{
		unit,
		[=](double val) { return std::clamp(int(std::round(val / resolution + centerVal)), minValue, maxValue); },
		[=](int val)    { return (val - centerVal) * resolution; },
	};
}


template <LayoutType type> struct MotorLayoutInfo;

}

}
