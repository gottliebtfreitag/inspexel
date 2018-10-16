#include <sargparse/Parameter.h>
#include "usb2dynamixel/USB2Dynamixel.h"
#include "usb2dynamixel/MotorMetaInfo.h"

namespace {

using namespace dynamixel;

void runMeta();
auto metaCmd   = sargp::Command{"meta", "list all motors known to this program", runMeta};
auto optMotorName = metaCmd.Parameter<std::string>("", "motor", "motor to give detail specs", {},
	[](std::vector<std::string> const& _str) -> std::pair<bool, std::set<std::string>> {
		auto const& db = meta::getMotorInfos();
		std::set<std::string> resList;
		if (_str.empty()) {
			for (auto const& data: db) {
				resList.insert(data.shortName);
			}
		} else {
			for (auto const& data : db) {
				if (data.shortName.size() < _str.size()) continue;
				auto str = _str.back();
				if (std::equal(begin(str), end(str), begin(data.shortName))) {
					resList.insert(data.shortName);
				}
			}
		}
		return std::make_pair(true, std::move(resList));
	}
);

using namespace dynamixel;

template <typename Iter, typename Delim>
auto join(Iter iter, Iter end, Delim const& delim) -> std::decay_t<decltype(*iter)> {
	if (iter == end) {
		return {};
	}
	auto res = *(iter++);
	while (iter != end) {
		res = res + delim + *(iter++);
	}
	return res;
}

void printDetailInfoTable(meta::MotorInfo const& data) {
	std::cout << " addr | l | Ac | mem |  init  |                          name | description \n";
	std::cout << "------+---+----+-----+--------+-------------------------------+-------------------------------\n";

	auto printEntries = [](auto const& layout, auto const& defaults) {
		for (auto [id, info] : layout) {
			auto iter = defaults.find(id);
			if (iter == defaults.end()) continue;

			std::cout.width(5);
			std::cout << int(id) << " |";
			std::cout.width(2);
			std::cout << int(info.length) << " |";
			std::cout.width(3);
			std::cout << to_string(info.access) << " |";
			std::cout << (info.romArea?" ROM":" RAM") << " |";

			if (iter->second) {
				std::cout.width(7);
				std::cout << int(iter->second.value()) << " |";
			} else {
				std::cout << "      - |";
			}
			std::cout.width(30);
			std::cout << info.name << " |";
			std::cout.width(30);
			std::cout << info.description;
			std::cout << "\n";
		}
	};

	if (data.layout == meta::LayoutType::V1) {
		auto const& layout   = meta::getLayoutInfos<meta::LayoutType::V1>();
		auto const& defaults = meta::getLayoutDefaults<meta::LayoutType::V1>().at(data.modelNumber);
		printEntries(layout, defaults);
	} else if (data.layout == meta::LayoutType::V2) {
		auto const& layout   = meta::getLayoutInfos<meta::LayoutType::V2>();
		auto const& defaults = meta::getLayoutDefaults<meta::LayoutType::V2>().at(data.modelNumber);
		printEntries(layout, defaults);
	} else if (data.layout == meta::LayoutType::Pro) {
		auto const& layout   = meta::getLayoutInfos<meta::LayoutType::Pro>();
		auto const& defaults = meta::getLayoutDefaults<meta::LayoutType::Pro>().at(data.modelNumber);
		printEntries(layout, defaults);
	} else {
		throw std::runtime_error("unknown layout type");
	}
}


void runMeta() {
	auto const& db = meta::getMotorInfos();
	if (optMotorName.isSpecified()) {
		auto name = optMotorName.get();
		std::transform(begin(name), end(name), begin(name), ::toupper);
		for (auto const& data : db) {
			if (data.shortName == name) {
				printDetailInfoTable(data);
				return;
			}
		}
		throw std::runtime_error("motor with name: " + name + " not found");
	} else {
		std::cout << "motors: \n";
		for (auto const& data : db) {
			std::cout << data.shortName << ": [";
			std::cout << join(begin(data.motorNames), end(data.motorNames), ", ") << "]\n";
		}
	}
}

}
