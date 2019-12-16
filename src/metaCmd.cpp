#include <sargparse/Parameter.h>
#include "usb2dynamixel/USB2Dynamixel.h"
#include "usb2dynamixel/MotorMetaInfo.h"

namespace {

using namespace dynamixel;

void runMeta();
auto metaCmd   = sargp::Command{"meta", "list all motors known to this program", runMeta};
auto optMotorName = metaCmd.Parameter<std::string>("", "motor", "motor to give detail specs", {},
	[](std::vector<std::string> const& _str) -> std::pair<bool, std::set<std::string>> {
		std::set<std::string> resList;
		meta::forAllLayoutTypes([&](auto const& info) {
			using Info = std::decay_t<decltype(info)>;
			auto const& defaults = Info::getDefaults();
			if (_str.empty()) {
				for (auto const& [id, data] : defaults) {
					resList.insert(data.shortName);
				}
			} else {
				for (auto const& [id, data] : defaults) {
					if (data.shortName.size() < _str.size()) continue;
					auto str = _str.back();
					if (std::equal(begin(str), end(str), begin(data.shortName))) {
						resList.insert(data.shortName);
					}
				}
			}
		});
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
	std::cout << " addr | l | Ac | mem |   init |        unit |                          name | description \n";
	std::cout << "------+---+----+-----+--------+-------------+-------------------------------+-------------------------------\n";

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

			auto const& [optDefault, convert] = iter->second;
			if (optDefault) {
				std::cout.width(7);
				std::cout << int(optDefault.value()) << " |";
			} else {
				std::cout << "      - |";
			}
			if (convert.toMotor and convert.fromMotor and optDefault) {
				std::cout.width(9);
				std::cout << convert.fromMotor(optDefault.value());
				std::cout.width(3);
				std::cout << convert.unit;
				std::cout << " |";
			} else {
				std::cout << "           - |";
			}
			std::cout.width(30);
			std::cout << info.name << " |";
			std::cout.width(30);
			std::cout << info.description;
			std::cout << "\n";
		}
	};

	meta::forAllLayoutTypes([&](auto const& info) {
		using Info = std::decay_t<decltype(info)>;
		if (data.layout == Info::Type) {
			auto const& layout   = Info::getInfos();
			auto const& defaults = Info::getDefaults().at(data.modelNumber).defaultLayout;
			printEntries(layout, defaults);
		}
	});
	if (data.layout == LayoutType::None) {
		throw std::runtime_error("unknown layout type");
	}
}


void runMeta() {
	if (optMotorName) {
		auto name = *optMotorName;
		std::transform(begin(name), end(name), begin(name), ::toupper);
		auto info = meta::getMotorInfo(name);
		if (not info) {
			throw std::runtime_error("motor with name: " + name + " not found");
		}
		printDetailInfoTable(*info);
	} else {
		std::cout << "motors: \n";
		meta::forAllLayoutTypes([&](auto const& info) {
			using Info = std::decay_t<decltype(info)>;
			auto const& defaults = Info::getDefaults();
			for (auto const& [id, data] : defaults) {
				std::cout << data.shortName << ": [";
				std::cout << join(begin(data.motorNames), end(data.motorNames), ", ") << "]\n";
			}
		});
	}
}

}
