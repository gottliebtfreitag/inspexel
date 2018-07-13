#pragma once

#include "Singleton.h"

#include <map>
#include <set>
#include <utility>
#include <stdexcept>

namespace registry
{
template<typename ValueType>
struct Registry : Singleton<Registry<ValueType>>{
	using ContainerType = std::set<ValueType*>;

	void registerInstance(ValueType* inst) {
		instances.emplace(inst);
	}
	void deregisterInstance(ValueType* inst) {
		instances.erase(inst);
	}
	ContainerType const& getInstances() {
		return instances;
	}
protected:
	ContainerType instances;
};

template<typename ValueType>
struct Registerable {
	using RegistryType = Registry<ValueType>;

	Registerable(ValueType* _value)
		: value(_value)
	{
		RegistryType::getInstance().registerInstance(value);
	}
	~Registerable() {
		RegistryType::getInstance().deregisterInstance(value);
	}
private:
	ValueType* value;
};

template<typename ValueType>
struct IndirectRegisterable : ValueType {
	using RegistryType = Registry<ValueType>;
	template<typename... Args>
	IndirectRegisterable(Args &&... args)
	: ValueType(std::forward<Args>(args)...) {
		RegistryType::getInstance().registerInstance(this);
	}
	~IndirectRegisterable() {
		RegistryType::getInstance().deregisterInstance(this);
	}
};


template<typename KeyType, typename ValueType>
struct UniqueKeyedRegistry : Singleton<UniqueKeyedRegistry<KeyType, ValueType>>{
	using ContainerType = std::map<KeyType, ValueType*>;

	void registerInstance(KeyType const& key, ValueType* inst) {
		if (instances.find(key) != instances.end()) {
			throw std::runtime_error("cannot insert multiple items with the same key into a UniqueRegistry");
		}
		instances.emplace(key, inst);
	}
	void deregisterInstance(KeyType const& key) {
		instances.erase(key);
	}
	ContainerType const& getInstances() {
		return instances;
	}
protected:
	ContainerType instances;
};

template<typename KeyType, typename ValueType>
struct UniqueKeyedRegisterable {
	using RegistryType = UniqueKeyedRegistry<KeyType, ValueType>;

	UniqueKeyedRegisterable(KeyType const& _key, ValueType* value)
	: key(_key) {
		RegistryType::getInstance().registerInstance(_key, value);
	}
	~UniqueKeyedRegisterable() {
		RegistryType::getInstance().deregisterInstance(key);
	}
	KeyType const& getKey() const {
		return key;
	}
private:
	KeyType key;
};

template<typename KeyType, typename ValueType>
struct UniqueKeyedInheritedRegisterable : ValueType {
	using RegistryType = UniqueKeyedRegistry<KeyType, ValueType>;

	template<typename... Args>
	UniqueKeyedInheritedRegisterable(KeyType const& _key, Args &&... args)
	: ValueType(std::forward<Args>(args)...), key(_key) {
		RegistryType::getInstance().registerInstance(_key, this);
	}
	~UniqueKeyedInheritedRegisterable() {
		RegistryType::getInstance().deregisterInstance(key);
	}
	KeyType const& getKey() const {
		return key;
	}
private:
	KeyType key;
};


template<typename KeyType, typename ValueType>
struct KeyedRegistry : Singleton<KeyedRegistry<KeyType, ValueType>>{
	using ContainerType = std::multimap<KeyType, ValueType*>;

	void registerInstance(KeyType const& key, ValueType* inst) {
		instances.emplace(key, inst);
	}
	void deregisterInstance(KeyType const& key, ValueType* inst) {
		auto range = instances.equal_range(key);
		for (auto it = range.first; it != range.second; ++it) {
			if (it->second == inst) {
				instances.erase(it);
				break;
			}
		}
		instances.erase(key);
	}
	ContainerType const& getInstances() {
		return instances;
	}
protected:
	ContainerType instances;
};

template<typename KeyType, typename ValueType>
struct KeyedRegisterable {
	using RegistryType = KeyedRegistry<KeyType, ValueType>;
	KeyedRegisterable(KeyType const& _key, ValueType* _value)
	: key(_key)
	, value(_value) {
		RegistryType::getInstance().registerInstance(_key, _value);
	}
	~KeyedRegisterable() {
		RegistryType::getInstance().deregisterInstance(key, this);
	}
	KeyType const& getKey() const {
		return key;
	}
private:
	KeyType key;
	ValueType* value;
};

template<typename KeyType, typename ValueType>
struct KeyedInheritedRegisterable : ValueType {
	using RegistryType = KeyedRegistry<KeyType, ValueType>;
	template<typename... Args>
	KeyedInheritedRegisterable(KeyType const& _key, Args &&... args)
	: ValueType(std::forward<Args>(args)...), key(_key) {
		RegistryType::getInstance().registerInstance(_key, this);
	}
	~KeyedInheritedRegisterable() {
		RegistryType::getInstance().deregisterInstance(key, this);
	}
	KeyType const& getKey() const {
		return key;
	}
private:
	KeyType key;
};

}
