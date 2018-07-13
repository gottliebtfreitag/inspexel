#pragma once

#include "Registry.h"

#include <functional>
#include <string>
#include <memory>

namespace factory {

template<typename Base, typename KeyType>
struct Factory;

template<typename Base, typename KeyType=std::string>
struct Creator {
	using RegisterableType = registry::UniqueKeyedRegisterable<KeyType, Creator<Base, KeyType>>;
	using FactoryType = Factory<Base, KeyType>;
	using CreatorFunc = std::function<std::unique_ptr<Base>(void)>;

	Creator(KeyType const& _key, CreatorFunc && _creator)
	: registerable(_key, this)
	, creator(std::move(_creator))
	, key(_key) {}
	~Creator() = default;

	typename CreatorFunc::result_type create() const {
		return creator();
	}

protected:
	RegisterableType registerable;
	CreatorFunc creator;
	KeyType key;
};


template<typename Base, typename KeyType=std::string>
struct Factory {
private:
	using RegistryType = typename Creator<Base, KeyType>::RegisterableType::RegistryType;
public:
	static auto getCreatables() -> typename RegistryType::ContainerType const& {
		return RegistryType::getInstance().getInstances();
	}

	static std::unique_ptr<Base> build(KeyType const& name) {
		auto const& items = RegistryType::getInstance().getInstances();
		auto it = items.find(name);
		if (it == items.end()) {
			throw std::runtime_error("cannot build something with name \""  + name + "\" as there is no creator for it");
		}
		return it->second->create();
	}
};

}
