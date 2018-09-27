#pragma once

#include <map>
#include <optional>
#include <stdint.h>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include "Layout.h"

namespace dynamixel::meta {

struct LayoutField {
	uint8_t length;
	bool     romArea;
	enum class Access { R = 0x01, W = 0x02, RW = 0x03 };
	Access access;
	std::string name;
	std::string description;
};

enum class LayoutType { V1, V2, Pro };

inline auto to_string(LayoutType layout) -> std::string {
	switch(layout) {
	case LayoutType::V1:  return "V1";
	case LayoutType::V2:  return "V2";
	case LayoutType::Pro: return "Pro";
	}
	throw std::runtime_error("unknown layout");
}

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

template <LayoutType type>
auto getLayoutInfos() {
	if constexpr(type == LayoutType::V1) {
		return getLayoutV1Infos();
	} else if constexpr(type == LayoutType::V2) {
		return getLayoutV2Infos();
	} else {
		return getLayoutProInfos();
	}
}
auto getLayoutV1Defaults() -> std::map<uint32_t, DefaultLayout<v1::Register>> const&;
auto getLayoutV2Defaults() -> std::map<uint32_t, DefaultLayout<v2::Register>> const&;
auto getLayoutProDefaults() -> std::map<uint32_t, DefaultLayout<pro::Register>> const&;

template <LayoutType type>
auto const& getLayoutDefaults() {
	if constexpr(type == LayoutType::V1) {
		return getLayoutV1Defaults();
	} else if constexpr(type == LayoutType::V2) {
		return getLayoutV2Defaults();
	} else {
		return getLayoutProDefaults();
	}
}

struct MotorInfo {
	uint16_t                        modelNumber;
	LayoutType layout;

	std::string                     shortName;
	std::vector<std::string>        motorNames;

};

auto getMotorInfos() -> std::vector<MotorInfo> const&;
auto getMotorInfo(uint16_t _modelNumber) -> MotorInfo const*;
auto getMotorInfo(std::string const& _name) -> MotorInfo const*;

}
