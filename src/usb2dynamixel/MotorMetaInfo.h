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

template <LayoutType Head, LayoutType... Types, typename CB>
void forLayoutTypes(CB&& cb) {
	cb(MotorLayoutInfo<Head>{});
	if constexpr (sizeof...(Types) > 0) {
		forLayoutTypes<Types...>(std::forward<CB>(cb));
	}
};

template <typename CB>
void forAllLayoutTypes(CB&& cb) {
	forLayoutTypes<LayoutType::MX_V1, LayoutType::MX_V2, LayoutType::Pro, LayoutType::XL320, LayoutType::AX>(std::forward<CB>(cb));
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
