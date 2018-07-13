#pragma once

#include <sstream>
#include <iomanip>
#include <type_traits>
#include <vector>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <optional>
#include "utils/demangle.h"

namespace parameter
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
		throw ParseError{};
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
			throw ParseError{};
		}
		// if we didnt parse everything -> throw
		if (int(nextIdx) != strEnd - strBegin) {
			throw ParseError{};
		}
		return ret;
	}

	// parse everything else
	std::stringstream ss{str};
	ss >> ret;
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
			return std::accumulate(std::next(t.begin()), t.end(), stringify(*t.begin()), [](std::string const& left, auto const& p) {
				return left + ", " + stringify(p);
			});
		}
		return "{}";
	} else if constexpr (detail::is_optional_v<T>) {
		if (t) {
			return stringify(*t);
		}
		return "{}";
	} else if constexpr (std::is_same_v<std::string, T>) {
		return "\"" + t + "\"";
	} else if constexpr (std::is_same_v<bool, T>) {
		return t?"true":"false";
	} else {
		return std::to_string(t);
	}
}

}
}


