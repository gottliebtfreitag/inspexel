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

enum class LayoutType { None, V1, V2, Pro, XL320 };

inline auto to_string(LayoutType layout) -> std::string {
	switch(layout) {
	case LayoutType::V1:  return "V1";
	case LayoutType::V2:  return "V2";
	case LayoutType::Pro: return "Pro";
	case LayoutType::XL320: return "XL320";
	default:
		throw std::runtime_error("unknown layout");
	}
}

template <LayoutType TType>
struct LayoutInfo {
	static constexpr LayoutType type {TType};
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
	forLayoutTypes<LayoutType::V1, LayoutType::V2, LayoutType::Pro, LayoutType::XL320>(std::forward<CB>(cb));
};


template <typename Reg>
using Layout = std::map<Reg, LayoutField>;

template <typename Reg>
using DefaultLayout = std::map<Reg, std::optional<uint32_t>>;

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

template <LayoutType type>
auto getLayoutInfos() {
	if constexpr (type == LayoutType::V1) {
		return getLayoutV1Infos();
	} else if constexpr (type == LayoutType::V2) {
		return getLayoutV2Infos();
	} else if constexpr (type == LayoutType::Pro) {
		return getLayoutProInfos();
	} else if constexpr (type == LayoutType::XL320) {
		return getLayoutXL320Infos();
	}

	throw std::runtime_error("unknown layout");
}
auto getLayoutV1Defaults() -> std::map<uint32_t, DefaultLayout<v1::Register>> const&;
auto getLayoutV2Defaults() -> std::map<uint32_t, DefaultLayout<v2::Register>> const&;
auto getLayoutProDefaults() -> std::map<uint32_t, DefaultLayout<pro::Register>> const&;
auto getLayoutXL320Defaults() -> std::map<uint32_t, DefaultLayout<xl320::Register>> const&;

template <LayoutType type>
auto const& getLayoutDefaults() {
	if constexpr(type == LayoutType::V1) {
		return getLayoutV1Defaults();
	} else if constexpr(type == LayoutType::V2) {
		return getLayoutV2Defaults();
	} else if constexpr (type == LayoutType::Pro) {
		return getLayoutProDefaults();
	} else if constexpr (type == LayoutType::XL320) {
		return getLayoutXL320Defaults();
	}

	throw std::runtime_error("unknown layout");
}

template <LayoutType type> struct FullLayout;
template <> struct FullLayout<LayoutType::V1>    { using Layout = v1::FullLayout; };
template <> struct FullLayout<LayoutType::V2>    { using Layout = v2::FullLayout; };
template <> struct FullLayout<LayoutType::Pro  > { using Layout = pro::FullLayout; };
template <> struct FullLayout<LayoutType::XL320> { using Layout = xl320::FullLayout; };



struct ConverterFunctions {
	std::function<int(double)>      toMotorPosition;
	std::function<double(int)>      fromMotorPosition;
	std::function<int(double)>      toMotorSpeed;
	std::function<double(int)>      fromMotorSpeed;
};

struct MotorInfo {
	uint16_t                        modelNumber;
	LayoutType layout;

	std::string                     shortName;
	std::vector<std::string>        motorNames;

	ConverterFunctions              converterFunctions;
};

auto getMotorInfos() -> std::vector<MotorInfo> const&;
auto getMotorInfo(uint16_t _modelNumber) -> MotorInfo const*;
auto getMotorInfo(std::string const& _name) -> MotorInfo const*;

}
