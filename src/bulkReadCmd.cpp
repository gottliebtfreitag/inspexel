#include "parameter/Parameter.h"
#include "usb2dynamixel/USB2Dynamixel.h"
#include "usb2dynamixel/MotorMetaInfo.h"

#define TERM_RED                        "\033[31m"
#define TERM_GREEN                      "\033[32m"
#define TERM_RESET                      "\033[0m"

namespace {

auto device    = parameter::Parameter<std::string>("/dev/ttyUSB0", "device", "the usb2dynamixel device");
auto baudrate  = parameter::Parameter<int>(1000000, "baudrate", "baudrate to use");
auto timeout   = parameter::Parameter<int>(10000, "timeout", "timeout in us");

void runBulkRead();
auto bulkReadCmd = parameter::Command{"bulk_read", "bulk read from motors", runBulkRead};
auto optBulk        = bulkReadCmd.Parameter<std::string>("", "bulk", "bulk in format 'id:baseRegister:length'");
auto optContinues   = bulkReadCmd.Parameter<bool>(false, "continues", "continues update parameter?");

auto split(std::string str, std::string const& del) -> std::vector<std::string> {
	std::vector<std::string> retList;
	while (str.length() > 0) {
		auto p = str.find(del);
		retList.push_back(str.substr(0, p));
		if (p == std::string::npos) {
			return retList;
		}
		str.erase(0, p + del.size());
	}
	return retList;
}


void runBulkRead() {
	std::vector<std::tuple<dynamixel::MotorID, int, uint8_t>> request;
	std::map<uint32_t, std::string> mIDs;
	for (auto const& x : split(optBulk, ",")) {
		auto v = split(x, ":");
		if (v.size() == 2) {
			request.push_back(std::make_tuple(stoi(v[0]), 0, stoi(v[1])));
		} else {
			std::transform(begin(v[0]), end(v[0]), begin(v[0]), ::toupper);
			request.push_back(std::make_tuple(stoi(v[1]), stoi(v[2]), stoi(v[3])));
			mIDs[stoi(v[1])] = v[0];
		}
	}

	auto usb2dyn = dynamixel::USB2Dynamixel(baudrate, {device.get()});
	auto start = std::chrono::high_resolution_clock::now();
	int count = 0;
	int failures = 0;
	do {
		++count;
		auto now = std::chrono::high_resolution_clock::now();
		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
		std::cout << failures << "/" << count  << " - " << double(count) / diff.count() * 1000.<< " bulk_reads per second\n";

		auto response = usb2dyn.bulk_read(request, std::chrono::microseconds{timeout});
		std::cout << "address (length): value (initial value) - RW - name - description\n";
		if (response.size() != request.size()) {
			failures += 1;
		}
		for (auto const& [id, data] : response) {
			auto const& [baseRegister, rxBuf] = data;
			std::cout << "motor " << int(id) << "\n";;
			std::optional<std::tuple<int, std::string>> modelType;
			if (not mIDs.empty()) {
				modelType = std::make_tuple(baseRegister, mIDs.at(id));
			}
			dynamixel::visitBuffer(rxBuf, modelType, [](dynamixel::RegisterData const& reg, auto&& x) {
				std::string mem = reg.romArea?"ROM":"RAM";
				std::string initValue = "-";
				if (reg.initialValue) {
					initValue = std::to_string(reg.initialValue.value());
				}
				std::cout << mem << " " << std::setw(3) << reg.baseRegister << "(" << reg.length << ") : ";
				if (reg.initialValue) {
					if (reg.initialValue.value() != x) {
						std::cout << TERM_RED;
					} else {
						std::cout << TERM_GREEN;
					}
				}
				std::cout << std::setw(5) << int(x) << TERM_RESET;


				std::cout << " (" << std::setw(5) << initValue << ") - " << std::setw(2) << to_string(reg.access) << " - " << reg.description << " - " << reg.dataName << "\n";
			});
		}
	} while (optContinues.get());
}

}
