#pragma once

#include <map>
#include <optional>
#include <stdint.h>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <functional>
#include "Layout.h"

namespace dynamixel::meta {

struct LayoutField {
	uint16_t   length;
	bool       romArea;
	enum class Access { R = 0x01, W = 0x02, RW = 0x03 };
	Access      access;
	std::string name;
	std::string description;
};

enum class LayoutType { None, V1, V2, Pro, XL320, AX };

inline auto to_string(LayoutType layout) -> std::string {
	switch(layout) {
	case LayoutType::V1:    return "V1";
	case LayoutType::V2:    return "V2";
	case LayoutType::Pro:   return "Pro";
	case LayoutType::XL320: return "XL320";
	case LayoutType::AX:    return "AX";
	default:
		throw std::runtime_error("unknown layout");
	}
}

template <typename Reg>
using Layout = std::map<Reg, LayoutField>;

inline auto to_string(LayoutField::Access access) -> std::string {
	switch(access) {
	case LayoutField::Access::R: return "R";
	case LayoutField::Access::W: return "W";
	case LayoutField::Access::RW: return "RW";
	}
	throw std::runtime_error("unknown access value");
}

auto getLayoutV1Infos()  -> Layout<v1::Register> const&;
auto getLayoutV2Infos()  -> Layout<v2::Register> const&;
auto getLayoutProInfos() -> Layout<pro::Register> const&;
auto getLayoutXL320Infos() -> Layout<xl320::Register> const&;
auto getLayoutAXInfos() -> Layout<ax::Register> const&;

struct ConverterFunctions {
	std::function<int(double)>      toMotorPosition;
	std::function<double(int)>      fromMotorPosition;
	std::function<int(double)>      toMotorSpeed;
	std::function<double(int)>      fromMotorSpeed;
};

template <typename Reg>
using DefaultLayout = std::map<Reg, std::optional<uint32_t>>;

template <typename Register>
struct Info {
	uint16_t modelNumber;
	LayoutType layout;

	std::string shortName;
	std::vector<std::string> motorNames;

	ConverterFunctions converterFunctions;

	DefaultLayout<Register> defaultLayout;
};

auto getLayoutV1Defaults() -> std::map<uint32_t, Info<v1::Register>> const&;
auto getLayoutV2Defaults() -> std::map<uint32_t, Info<v2::Register>> const&;
auto getLayoutProDefaults() -> std::map<uint32_t, Info<pro::Register>> const&;
auto getLayoutXL320Defaults() -> std::map<uint32_t, Info<xl320::Register>> const&;
auto getLayoutAXDefaults() -> std::map<uint32_t, Info<ax::Register>> const&;


template <LayoutType type> struct LayoutInfo;

template <> struct LayoutInfo<LayoutType::V1> {
	static constexpr LayoutType Type{LayoutType::V1};
	using FullLayout = v1::FullLayout;

	static auto const& getDefaults() {
		return getLayoutV1Defaults();
	}
	static auto const& getInfos() {
		return getLayoutV1Infos();
	}
};

template <> struct LayoutInfo<LayoutType::V2> {
	static constexpr LayoutType Type{LayoutType::V2};
	using FullLayout = v2::FullLayout;

	static auto const& getDefaults() {
		return getLayoutV2Defaults();
	}
	static auto const& getInfos() {
		return getLayoutV2Infos();
	}
};

template <> struct LayoutInfo<LayoutType::Pro> {
	static constexpr LayoutType Type{LayoutType::Pro};
	using FullLayout = pro::FullLayout;

	static auto const& getDefaults() {
		return getLayoutProDefaults();
	}
	static auto const& getInfos() {
		return getLayoutProInfos();
	}
};

template <> struct LayoutInfo<LayoutType::XL320> {
	static constexpr LayoutType Type{LayoutType::XL320};
	using FullLayout = xl320::FullLayout;

	static auto const& getDefaults() {
		return getLayoutXL320Defaults();
	}
	static auto const& getInfos() {
		return getLayoutXL320Infos();
	}
};

template <> struct LayoutInfo<LayoutType::AX> {
	static constexpr LayoutType Type{LayoutType::AX};
	using FullLayout = ax::FullLayout;

	static auto const& getDefaults() {
		return getLayoutAXDefaults();
	}
	static auto const& getInfos() {
		return getLayoutAXInfos();
	}
};

template <LayoutType Head, LayoutType... Types, typename CB>
void forLayoutTypes(CB&& cb) {
	cb(LayoutInfo<Head>{});
	if constexpr (sizeof...(Types) > 0) {
		forLayoutTypes<Types...>(std::forward<CB>(cb));
	}
};

template <typename CB>
void forAllLayoutTypes(CB&& cb) {
	forLayoutTypes<LayoutType::V1, LayoutType::V2, LayoutType::Pro, LayoutType::XL320, LayoutType::AX>(std::forward<CB>(cb));
};


struct MotorInfo {
	uint16_t                        modelNumber;
	LayoutType layout;

	std::string                     shortName;
	std::vector<std::string>        motorNames;

	ConverterFunctions              converterFunctions;
};

auto getMotorInfo(uint16_t _modelNumber) -> MotorInfo const*;
auto getMotorInfo(std::string const& _name) -> MotorInfo const*;

}
