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

template <typename T>
struct is_array : std::false_type {};

template <typename T, size_t N>
struct is_array<std::array<T, N>> : std::true_type {};

template<typename T>
inline constexpr bool is_array_v = is_array<T>::value;


auto readDetailedInfosFromUnknown(dynamixel::USB2Dynamixel& usb2dyn, std::vector<std::tuple<MotorID, uint16_t>> const& motors, std::chrono::microseconds timeout, bool _print) -> std::tuple<int, int> {
	int expectedTransactions = 1 + motors.size();
	int successfullTransactions = 0;

	std::vector<std::tuple<MotorID, int, size_t>> request;
	for (auto const& [g_id, modelNumber] : motors) {
		request.push_back(std::make_tuple(g_id, int(mx_v1::Register::MODEL_NUMBER), 74));
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

template <LayoutType LT, typename Layout>
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

	using Info = meta::MotorLayoutInfo<LT>;
	auto const& infos = Info::getInfos();
	for (auto const& [reg, info] : infos) {
		std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2) << int(reg) << std::setfill(' ');
		std::cout << std::dec << " " << int(info.length);
		std::cout << " " << std::setw(2) << to_string(info.access);
		std::cout << " " << (info.romArea?"ROM":"RAM");

		for (auto const& [g_id, modelNumber, errorCode, layout] : response) {
			visit([reg=reg, modelNumber=modelNumber](auto _reg, auto& value) {
				if (_reg != reg) return;
				if constexpr (is_array_v<std::decay_t<decltype(value)>>) {
					return;
				} else {
					auto const& defaults = Info::getDefaults().at(modelNumber).defaultLayout;
					auto iter = defaults.find(reg);

					std::stringstream ss;
					int extra = 0;
					if (iter == defaults.end()) {
						ss << std::boolalpha;
						ss << "-na-";
					} else {
						auto const& [optDefault, convert] = iter->second;
						if (optDefault) {
							extra = 9;
							if (int(value) != int(optDefault.value())) {
								ss << " " TERM_RED << int(value) << TERM_RESET "(";
							} else {
								ss << " " << TERM_GREEN << int(value) << TERM_RESET "(";
							}
							ss << int(optDefault.value());
						} else {
							ss << " " << int(value) << "(";
							ss << "-";
						}
						ss << ")";
					}
					std::cout << std::setw(14+extra) << ss.str();
				}
			}, layout);
		}
		std::cout << std::setw(30) << info.name << " - " << info.description << "\n";
	}
	std::cout << "-----------\n";
	return {successfullTransactions, expectedTransactions};
}

void runDetect() {
	baudrates->emplace(*g_baudrate);
	auto timeout = std::chrono::microseconds{*g_timeout};
	auto protocols = std::vector<dynamixel::Protocol>{dynamixel::Protocol::V1, dynamixel::Protocol::V2};
	if (g_protocolVersion) {
		protocols = {dynamixel::Protocol{*g_protocolVersion}};
	}
	for (auto protocolVersion : protocols) {
		std::cout << "# trying protocol version " << int(protocolVersion) << "\n";
		for (auto baudrate : *baudrates) {
			std::cout << "## trying baudrate: " << baudrate << "\n";
			auto usb2dyn = dynamixel::USB2Dynamixel(baudrate, *g_device, protocolVersion);

			// generate range to check
			std::vector<int> range(0xFD);
			std::iota(begin(range), end(range), 0);
			if (g_id) {
				range = {*g_id};
			} else  if (ids) {
				range.clear();
				for (auto x : *ids) {
					range.push_back(x);
				}
			}

			// ping all motors
			std::map<LayoutType, std::vector<std::tuple<MotorID, uint16_t>>> motors;
			for (auto motor : range) {
				auto [layout, modelNumber] = detectMotor(MotorID(motor), usb2dyn, timeout);
				if (modelNumber != 0) {
					motors[layout].push_back(std::make_tuple(motor, modelNumber));
				}
			}
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

					meta::forAllLayoutTypes([&](auto const& info) {
						using Info = std::decay_t<decltype(info)>;
						if (not motors[Info::Type].empty()) {
							using FullLayout = typename Info::FullLayout;
							auto [suc, tot] = readDetailedInfos<Info::Type, FullLayout>(usb2dyn, motors[Info::Type], timeout, print);
							successful += suc;
							total += tot;
						}
					});

					if (not motors[LayoutType::None].empty()) {
						auto [suc, tot] = readDetailedInfosFromUnknown(usb2dyn, motors[LayoutType::None], timeout, print);
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
