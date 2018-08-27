#pragma once

#include <map>
#include <optional>
#include <stdint.h>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace dynamixel {

struct RegisterData {
	uint32_t baseRegister;
	uint32_t length;
	bool     romArea;
	enum class Access { R = 0x01, W = 0x02, RW = 0x03 };
	Access access;
	std::optional<uint32_t> initialValue;

	std::string dataName;
	std::string description;
};

inline auto to_string(RegisterData::Access access) -> std::string {
	switch(access) {
	case RegisterData::Access::R: return "R";
	case RegisterData::Access::W: return "W";
	case RegisterData::Access::RW: return "RW";
	}
	throw std::runtime_error("unknown access value");
}

struct MotorData {
	std::string                     shortName;
	std::vector<std::string>        motorNames;
	std::map<uint32_t, RegisterData> registerData;
};

[[nodiscard]] auto getMotorDataBase() -> std::map<int, MotorData> const&;
[[nodiscard]] inline auto getMotorDataBase(std::string const& _name) -> MotorData const& {
	auto const& db = getMotorDataBase();
	for (auto const& [id, data] : db) {
		if (_name == data.shortName) {
			return data;
		}
		for (auto const& name : data.motorNames) {
			if (name == _name) {
				return data;
			}
		}
	}
	throw std::runtime_error("couldn't find any info for motor " + _name);
}

[[nodiscard]] inline auto getMotorDataBase(uint32_t modelNumber) -> MotorData const* {
	auto const& db = getMotorDataBase();
	for (auto const& [key, value] : db) {
		if (value.registerData.at(0).initialValue == modelNumber) {
			return &value;
		}
	}
	return nullptr;	
}


inline auto readValue(std::byte const* _data, int length) -> uint32_t {
	uint32_t value {0};
	for (int i{0}; i<length; ++i) {
		value = value | uint32_t(_data[i]) << (8*i);
	}
	return value;
}
inline auto readValueVariant(std::byte const* _data, int _length) -> std::variant<uint8_t, uint16_t, uint32_t> {
	uint32_t value {0};
	for (int i{0}; i<_length; ++i) {
		value = value | uint32_t(_data[i]) << (8*i);
	}
	if (_length == 1) {
		return {uint8_t(value)};
	} else if (_length == 2) {
		return {uint16_t(value)};
	} else if (_length == 4) {
		return {uint32_t(value)};
	}
	throw std::runtime_error("unknown value _length: " + std::to_string(_length));
}


template <typename CB>
void visitBuffer(std::vector<std::byte> const& _buffer, std::optional<std::tuple<int, std::string>> modelType, CB cb) {
	if (not modelType) { // if no base Register given, we assume that _buffer has model information at the beginning
		auto modelNumber = readValue(_buffer.data(), 2);
		auto modelPtr = getMotorDataBase(modelNumber);
		if (not modelPtr) {
			throw std::runtime_error("unknown model " + std::to_string(modelNumber));
		}
		modelType = std::make_tuple(0, modelPtr->shortName);
	}
	auto [baseRegister, shortName] = modelType.value();
	auto data = getMotorDataBase(shortName);

	auto iter = data.registerData.find(baseRegister);
	int currentRegister = baseRegister;
	while (currentRegister + iter->second.length <= _buffer.size()) {
		auto value = readValueVariant(&_buffer.at(currentRegister), iter->second.length);
		std::visit([&](auto&& data) {
			cb(iter->second, data);
		}, value);
		++iter;
		currentRegister = iter->second.baseRegister - baseRegister;
	}
}

template <typename T>
auto readValue(std::string_view::iterator iter) -> T {
	T retValue{0};
	for (size_t i{0}; i<sizeof(T); ++i) {
		retValue = retValue | (T(*iter) << (i*8));
		++iter;
	}
	return retValue;
}


template <typename CB>
void visitBuffer(std::string motorType, std::string_view data, uint8_t baseRegister, CB cb) {
	auto const& db = getMotorDataBase(motorType).registerData;

	auto iter = begin(data);
	while (iter != end(data)) {
		auto rd = db.at(baseRegister);
		if (rd.length == 1) {
			auto value = readValue<uint8_t>(iter);
			cb(rd, value);
		} else if (rd.length == 2) {
			auto value = readValue<uint16_t>(iter);
			cb(rd, value);
		} else if (rd.length == 4) {
			auto value = readValue<uint32_t>(iter);
			cb(rd, value);
		}

		std::advance(iter, rd.length);
		baseRegister += rd.length;
	}
}
}
