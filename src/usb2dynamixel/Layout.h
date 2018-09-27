#pragma once
#include "LayoutV1.h"
#include "LayoutV2.h"
#include "LayoutPro.h"

namespace dynamixel {

template <size_t L, auto type>
auto getLayoutType(v1::Register) -> v1::Layout<type, L> {
	return {};
}

template <size_t L, auto type>
auto getLayoutType(v2::Register) -> v2::Layout<type, L> {
	return {};
}

template <size_t L, auto type>
auto getLayoutType(pro::Register) -> pro::Layout<type, L> {
	return {};
}


template <auto type, size_t L>
using Layout = std::decay_t<decltype(getLayoutType<L, type>(type))>;

}
