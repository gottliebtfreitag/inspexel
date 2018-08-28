#include "parameter/Parameter.h"
#include "usb2dynamixel/USB2Dynamixel.h"
#include "usb2dynamixel/MotorMetaInfo.h"

namespace {

void runMeta();
auto metaCmd   = parameter::Command{"meta", "list all motors known to this program", runMeta};
auto optMotorName = metaCmd.Parameter<std::string>("", "motor", "motor to give detail specs", {},
	[](std::vector<std::string> const& _str) -> std::pair<bool, std::set<std::string>> {
		auto const& db = dynamixel::getMotorDataBase();
		std::set<std::string> resList;
		if (_str.empty()) {
			for (auto const& [id, data] : db) {
				resList.insert(data.shortName);
			}
		} else {
			for (auto const& [id, data] : db) {
				if (data.shortName.size() < _str.size()) continue;
				auto str = _str.back();
				if (std::equal(begin(str), end(str), begin(data.shortName))) {
					resList.insert(data.shortName);
				}
			}
		}
		return std::make_pair(true, std::move(resList));
	});

auto optJson      = metaCmd.Parameter(false, "json", "print list as json");

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
void printDetailInfoJson(MotorData const& data) {
	std::cout << "name: " << data.shortName << "\n";
	std::cout << "altNames: "<< "[" << join(begin(data.motorNames), end(data.motorNames), ", ") << "]\n";
	std::cout << "registers:\n";
	for (auto [id, info] : data.registerData) {
		std::cout << "  - id: " << id << "\n";
		std::cout << "    address: " << info.baseRegister << "\n";
		std::cout << "    length: " << int(info.length) << "\n";
		std::cout << "    access: "   << to_string(info.access) << "\n";
		std::cout << "    dataName: " << info.dataName << "\n";
		std::cout << "    description: " << info.description << "\n";
		std::cout << "    inRom: " << info.romArea << "\n";
		if (info.initialValue) {
			std::cout << "    initialValue: " << info.initialValue.value() << "\n";
		}
	}
}

void printDetailInfoTable(MotorData const& data) {
	std::cout << " addr | l | Ac | mem | init |                          name | description \n";
	std::cout << "------+---+----+-----+------+-------------------------------+-------------------------------\n";
	for (auto [id, info] : data.registerData) {
		std::cout.width(5);
		std::cout << id << " |";
		std::cout.width(2);
		std::cout << info.length << " |";
		std::cout.width(3);
		std::cout << to_string(info.access) << " |";
		std::cout << (info.romArea?" ROM":" RAM") << " |";
		if (not info.initialValue) {
			std::cout << "    - |";
		} else {
			std::cout.width(5);
			std::cout << info.initialValue.value() << " |";
		}
		std::cout.width(30);
		std::cout << info.dataName << " |";
		std::cout.width(30);
		std::cout << info.description;
		std::cout << "\n";
	}
}


void runMeta() {
	auto const& db = getMotorDataBase();
	if (optMotorName.isSpecified()) {
		auto name = optMotorName.get();
		std::transform(begin(name), end(name), begin(name), ::toupper);

		for (auto const& [id, data] : db) {
			if (data.shortName == name) {
				if (optJson) {
					printDetailInfoJson(data);
				} else {
					printDetailInfoTable(data);
				}
				return;
			}
		}
		throw std::runtime_error("motor with name: " + name + " not found");
	} else {
		std::cout << "motors: \n";
		for (auto const& [id, data] : db) {
			std::cout << data.shortName << ": [";
			std::cout << join(begin(data.motorNames), end(data.motorNames), ", ") << "]\n";
		}
	}
}

}
