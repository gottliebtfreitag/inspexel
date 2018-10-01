#include "usb2dynamixel/USB2Dynamixel.h"
#include "usb2dynamixel/MotorMetaInfo.h"
#include "globalOptions.h"

#include "commonTasks.h"

#include <algorithm>
#include <numeric>

#define TERM_RED                        "\033[31m"
#define TERM_GREEN                      "\033[32m"
#define TERM_RESET                      "\033[0m"

namespace {

void runDetect();
auto detectCmd  = sargp::Command{{"", "detect"}, "detect all dynamixel motors", runDetect};
auto baudrates  = detectCmd.Parameter<std::set<int>>({57142}, "other_baudrates", "more baudrates to test", {}, &listTypicalBaudrates);
auto readAll    = detectCmd.Flag("read_all", "read all registers from the detected motors (instead of just printing the found motors)");
auto ids        = detectCmd.Parameter<std::set<int>>({}, "ids", "the target Id");
auto optCont    = detectCmd.Flag("continues", "runs bulk read repeatably after detecting motors");


using namespace dynamixel;


auto readDetailedInfosFromUnknown(dynamixel::USB2Dynamixel& usb2dyn, std::vector<std::tuple<MotorID, uint16_t>> const& motors, std::chrono::microseconds timeout, bool _print) -> std::tuple<int, int> {
	int expectedTransactions = 1 + motors.size();
	int successfullTransactions = 0;

	std::vector<std::tuple<MotorID, int, size_t>> request;
	for (auto const [g_id, modelNumber] : motors) {
		request.push_back(std::make_tuple(g_id, int(v1::Register::MODEL_NUMBER), 74));
	}
	auto response = usb2dyn.bulk_read(request, timeout);

	if (not response.empty()) {
		successfullTransactions = 1 + response.size();
	}
	if (not _print) {
		return {successfullTransactions, expectedTransactions};
	}

	if (response.size() != motors.size()) {
		std::cout << "couldn't retrieve detailed information from all motors\n";
	}
	for (auto const& [motorID, baseRegister, errorCode, rxBuf] : response) {
		std::cout << "found motor " << static_cast<int>(motorID) << "\n";
		std::cout << "registers:\n";
		for (size_t idx{0}; idx < rxBuf.size(); ++idx) {
			std::cout << "  " << std::setw(3) << std::setfill(' ') << std::dec << idx << ": " <<
				std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(rxBuf.at(idx)) << std::dec << "\n";
		}
		std::cout << "\n";
	}
	std::cout << "-----------\n";

	return {successfullTransactions, expectedTransactions};
}

template <meta::LayoutType LT, typename Layout>
auto readDetailedInfos(dynamixel::USB2Dynamixel& usb2dyn, std::vector<std::tuple<MotorID, uint16_t>> const& motors, std::chrono::microseconds timeout, bool _print) -> std::tuple<int, int> {
	int expectedTransactions = 1 + motors.size();
	int successfullTransactions = 0;

	auto response = usb2dyn.bulk_read<Layout::BaseRegister, Layout::Length>(motors, timeout);
	if (not response.empty()) {
		successfullTransactions = 1 + response.size();
	}
	if (not _print) {
		return {successfullTransactions, expectedTransactions};
	}

	if (response.size() != motors.size()) {
		std::cout << "couldn't retrieve detailed information from all motors\n";
	}

	std::cout << "             ";
	for (auto const& [g_id, modelNumber, errorCode, layout] : response) {
		auto motorInfoPtr = meta::getMotorInfo(modelNumber);
		std::cout << std::setw(14) << motorInfoPtr->shortName;
	}
	std::cout << "\n";

	auto infos = meta::getLayoutInfos<LT>();
	for (auto const& [reg, info] : infos) {
		std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2) << int(reg) << std::setfill(' ');
		std::cout << std::dec << " " << int(info.length);
		std::cout << " " << std::setw(2) << to_string(info.access);
		std::cout << " " << (info.romArea?"ROM":"RAM");

		for (auto const& [g_id, modelNumber, errorCode, layout] : response) {
			visit([reg=reg, modelNumber=modelNumber](auto _reg, auto&& value) {
				if (_reg != reg) return;
				auto const& defaults = meta::getLayoutDefaults<LT>().at(modelNumber);
				auto iter = defaults.find(reg);

				std::stringstream ss;
				int extra = 0;
				if (iter == defaults.end()) {
					ss << std::boolalpha;
					ss << "-na-";
				} else {
					if (iter->second) {
						extra = 9;
						if (int(value) != int(iter->second.value())) {
							ss << " " TERM_RED << int(value) << TERM_RESET "(";
						} else {
							ss << " " << TERM_GREEN << int(value) << TERM_RESET "(";
						}
						ss << int(iter->second.value());
					} else {
						ss << " " << int(value) << "(";
						ss << "-";
					}
					ss << ")";
				}
				std::cout << std::setw(14+extra) << ss.str();
			}, layout);
		}
		std::cout << std::setw(30) << info.name << " - " << info.description << "\n";
	}
	std::cout << "-----------\n";
	return {successfullTransactions, expectedTransactions};
}

void runDetect() {
	baudrates.get().emplace(g_baudrate);
	auto timeout = std::chrono::microseconds{g_timeout};
	auto protocols = std::vector<dynamixel::Protocol>{dynamixel::Protocol::V1, dynamixel::Protocol::V2};
	if (g_protocolVersion.isSpecified()) {
		protocols = {dynamixel::Protocol{int(g_protocolVersion)}};
	}
	for (auto protocolVersion : protocols) {
		std::cout << "# trying protocol version " << int(protocolVersion) << "\n";
		for (auto baudrate : baudrates.get()) {
			std::cout << "## trying baudrate: " << baudrate << "\n";
			auto usb2dyn = dynamixel::USB2Dynamixel(baudrate, g_device.get(), protocolVersion);

			// generate range to check
			std::vector<int> range(0xFD);
			std::iota(begin(range), end(range), 0);
			if (g_id.isSpecified()) {
				range = {MotorID(g_id)};
			} else  if (ids.isSpecified()) {
				range.clear();
				for (auto x : ids.get()) {
					range.push_back(x);
				}
			}

			// ping all motors
			std::map<int, std::vector<std::tuple<MotorID, uint16_t>>> motors;
			for (auto motor : range) {
				auto [layout, modelNumber] = detectMotor(MotorID(motor), usb2dyn, timeout);
				motors[layout].push_back(std::make_tuple(motor, modelNumber));
			}
			motors.erase(-1);
			// read detailed infos if requested
			if ((readAll or optCont) and not motors.empty()) {
				static int count = 0;
				static int successful = 0;
				static int total = 0;
				auto start = std::chrono::high_resolution_clock::now();
				auto lastPrint = start;
				do {
					count += 1;
					auto now = std::chrono::high_resolution_clock::now();
					auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPrint);
					bool print = (diff.count() > 100) or readAll;
					if (not motors[1].empty()) {
						auto [suc, tot] = readDetailedInfos<meta::LayoutType::V1, v1::FullLayout>(usb2dyn, motors[1], timeout, print);
						successful += suc;
						total += tot;
					}
					if (not motors[2].empty()) {
						auto [suc, tot] = readDetailedInfos<meta::LayoutType::V2, v2::FullLayout>(usb2dyn, motors[2], timeout, print);
						successful += suc;
						total += tot;
					}
					if (not motors[3].empty()) { // dynamixel pro
						auto [suc, tot] = readDetailedInfos<meta::LayoutType::Pro, pro::FullLayout>(usb2dyn, motors[3], timeout, print);
						successful += suc;
						total += tot;
					}

					if (not motors[0].empty()) {
						auto [suc, tot] = readDetailedInfosFromUnknown(usb2dyn, motors[0], timeout, print);
						successful += suc;
						total += tot;
					}
					if (print and optCont) {
						lastPrint = now;
						std::cout << successful << "/" << total << " successful/total transactions - ";
						std::cout << count << " loops\n";
						auto now = std::chrono::high_resolution_clock::now();
						auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
						std::cout << double(successful) / diff.count() * 1000. << "/" << double(total) / diff.count() * 1000. << " trans per second -        ";
						std::cout << double(count) / diff.count() * 1000. << " loops per second" << "\n";
					}
				} while (optCont);
			}
		}
	}
}

}
