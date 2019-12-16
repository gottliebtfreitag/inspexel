#pragma once

#include <cmath>
#include <sstream>
#include <iomanip>
#include <type_traits>
#include <vector>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <optional>

namespace sargp
{
namespace parsing
{
namespace detail
{

struct ParseError : std::invalid_argument {
	ParseError(std::string const& msg="")
	: std::invalid_argument(msg)
	{}
};

template <uint64_t maxValue, typename T>
auto parseSuffixHelper(std::string str, std::string_view suffix) -> T {
	if (str != suffix) {
		return 1;
	}
	if constexpr (maxValue >= uint64_t(std::numeric_limits<T>::max())) {
		throw ParseError{"out of range"};
	} else {
		return maxValue;
	}
}

template<typename T>
auto parseSuffix(std::string_view suffix) -> std::optional<T> {
	auto ret = 1
		* parseSuffixHelper<1000, T>("k", suffix)
		* parseSuffixHelper<1024, T>("ki", suffix)
		* parseSuffixHelper<1000*1000, T>("m", suffix)
		* parseSuffixHelper<1024*1024, T>("mi", suffix)
		* parseSuffixHelper<1000*1000*1000, T>("g", suffix)
		* parseSuffixHelper<1024*1024*1024, T>("gi", suffix)
		* parseSuffixHelper<1000ull*1000*1000*1000, T>("t", suffix)
		* parseSuffixHelper<1024ull*1024*1024*1024, T>("ti", suffix)
		* parseSuffixHelper<1000ull*1000*1000*1000*1000, T>("p", suffix)
		* parseSuffixHelper<1024ull*1024*1024*1024*1024, T>("pi", suffix)
		* parseSuffixHelper<1000ull*1000*1000*1000*1000*1000, T>("e", suffix)
		* parseSuffixHelper<1024ull*1024*1024*1024*1024*1024, T>("ei", suffix)
	;
	if (ret == 1) {
		return std::nullopt;
	}
	return {ret};
}

template<typename T>
T parseFromString(std::string str) {
	if constexpr (std::is_same_v<T, bool>) {
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		if (str == "true" or str == "1") {
			return true;
		}
		if (str == "false" or str == "0") {
			return false;
		}
		throw ParseError{"invalid boolean specifier"};
	}

	if constexpr (std::is_same_v<T, std::string>) {
		return str;
	}

	T ret;
	// parse all integer-like types
	if constexpr (std::numeric_limits<T>::is_exact) {
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		int base=0;
		char const* strBegin = str.data();
		char const* strEnd   = str.data() + str.size();
		std::size_t nextIdx=0;
		if (str.find("0b") == 0) {
			base=2;
			strBegin += 2;
		}
		try {
			if constexpr (std::is_unsigned_v<T>) {
				ret = std::stoull(strBegin, &nextIdx, base);
			} else {
				ret = std::stoll(strBegin, &nextIdx, base);
			}
		} catch (std::invalid_argument const&) {
			throw ParseError{"not an integer"};
		}
		// if we didnt parse everything check if it has some known suffix
		if (static_cast<int>(nextIdx) != strEnd - strBegin) {
			if constexpr (not std::is_same_v<bool, T>) {
				auto suffix = std::string_view{strBegin + nextIdx};
				auto value = parseSuffix<T>(suffix);
				if (not value) {
					throw ParseError{"unknown suffix"};
				}
				if (__int128_t(ret) * value.value() > std::numeric_limits<T>::max()
					or __int128_t(ret) * value.value() < std::numeric_limits<T>::min()) {
						throw ParseError{"out of range"};
				}
				ret *= value.value();
			}
		}
		return ret;
	}

	// parse everything else
	std::stringstream ss{str};
	if (not (ss >> ret)) {
		throw ParseError{};
	}

	// parse floats/doubles and convert if they are angles
	if constexpr (std::is_floating_point_v<T>) {
		if (not ss.eof()) {
			std::string ending;
			if (not (ss >> ending)) {
				throw ParseError{};
			}
			if (ending == "rad") {
			} else if (ending == "deg") {
				ret = ret / 180. * M_PI;
			} else if (ending == "tau") {
				ret = ret * 2. * M_PI;
			} else {
				throw ParseError{"unknown suffix"};
			}
		}
	}

	if (not ss.eof()) {
		throw ParseError{};
	}

	return ret;
}

template<typename T>
struct is_collection : std::false_type {};
template<typename T>
struct is_collection<std::vector<T>> : std::true_type {};
template<typename T>
struct is_collection<std::set<T>> : std::true_type {};
template<typename T>
inline constexpr bool is_collection_v = is_collection<T>::value;

template<typename T>
struct is_optional : std::false_type {};
template<typename T>
struct is_optional<std::optional<T>> : std::true_type {};
template<typename T>
inline constexpr bool is_optional_v = is_optional<T>::value;

}


template<typename T>
T parse(std::vector<std::string> const& args) {
	if constexpr (detail::is_collection_v<T>) {
			using value_type = typename T::value_type;
			T collection;
			for (auto const& arg : args) {
				if constexpr (std::is_same_v<T, std::vector<value_type>>) {
					collection.emplace_back(detail::parseFromString<value_type>(arg));
				} else {
					collection.emplace(detail::parseFromString<value_type>(arg));
				}
			}
			return collection;
	} else if constexpr (detail::is_optional_v<T>) {
		using value_type = typename T::value_type;
		if (args.empty()) {
			return T{};
		}
		return parse<value_type>(args);
	} else {
		if (args.empty()) {
			if constexpr (std::is_same_v<bool, T>) {
				return true;
			}
		}
		if (args.size() != 1) {
			throw std::invalid_argument("wrong number of arguments given (expect 1, got " + std::to_string(args.size()) + ")");
		}
		return detail::parseFromString<T>(args[0]);
	}
}

template<typename T>
std::string stringify(T const& t) {
	if constexpr (detail::is_collection_v<T>) {
		if (not t.empty()) {
			return "{" + std::accumulate(std::next(t.begin()), t.end(), stringify(*t.begin()), [](std::string const& left, auto const& p) {
				return left + ", " + stringify(p);
			}) + "}";
		}
		return "{}";
	} else if constexpr (detail::is_optional_v<T>) {
		if (t) {
			return stringify(*t);
		}
		return "[]";
	} else if constexpr (std::is_same_v<std::string, T>) {
		return t;
	} else if constexpr (std::is_same_v<bool, T>) {
		return t?"true":"false";
	} else {
		return std::to_string(t);
	}
}

}
}


