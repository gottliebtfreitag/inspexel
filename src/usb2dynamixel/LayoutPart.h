#pragma once

#include <cstddef>
#include <cstring>
#include <stdexcept>

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

}
