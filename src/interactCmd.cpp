#include "usb2dynamixel/USB2Dynamixel.h"
#include "usb2dynamixel/MotorMetaInfo.h"
#include "globalOptions.h"

#include <algorithm>
#include <numeric>

#include <thread>
#include <atomic>
#include <unistd.h>

#define TERM_RED                        "\033[31m"
#define TERM_GREEN                      "\033[32m"
#define TERM_RESET                      "\033[0m"

namespace {

void runInteract();
auto interactCmd  = parameter::Command{"interact", "interact with dynamixel", runInteract};
auto optTimeout   = interactCmd.Parameter<int>(10000, "timeout", "timeout in us");

using namespace dynamixel;

auto checkMotorVersion(dynamixel::MotorID motor, dynamixel::USB2Dynamixel& usb2dyn, std::chrono::microseconds timeout) -> int {
	// only read model information, when model is known read full motor
	auto [timeoutFlag, motorID, errorCode, layout] = read<v1::Register::MODEL_NUMBER, 2>(usb2dyn, motor, timeout);
	if (timeoutFlag or motorID == MotorIDInvalid) {
		throw std::runtime_error("failed checking model number");
	}

	auto modelPtr = meta::getMotorInfo(layout.model_number);
	if (modelPtr) {
		std::cout << int(motor) << " " <<  modelPtr->shortName << " (" << layout.model_number << ") Layout " << to_string(modelPtr->layout) << "\n";
		if (modelPtr->layout == meta::LayoutType::V1) {
			return 1;
		} else {
			return 2;
		}
	}
	throw std::runtime_error("failed, unknown model (" + std::to_string(layout.model_number) + ")\n");
}

template <meta::LayoutType LT, typename Layout>
auto readMotorInfos(dynamixel::USB2Dynamixel& usb2dyn, MotorID motor, std::chrono::microseconds timeout) {
	auto [timeoutFlag, motorID, errorCode, layout] = read<Layout::BaseRegister, Layout::Length>(usb2dyn, motor, timeout);

	if (timeoutFlag or motorID == MotorIDInvalid) {
		throw std::runtime_error("trouble reading the motor");
	}

	if constexpr(LT == meta::LayoutType::V1) {
		return layout;
	} else if constexpr(LT == meta::LayoutType::V2) {
		return layout;
	}
}

void runInteract() {
	std::thread waitForInputThread;
	std::mutex mInputMutex;
	std::atomic_bool detectInput{false};
	std::atomic_bool minValue{false};
	waitForInputThread = std::thread([&]() {
		while(true) {
			std::cin.ignore();
			auto g = std::lock_guard(mInputMutex);
			detectInput = true;
			minValue = not minValue;
		}
	});
	try {
	auto timeout = std::chrono::microseconds{optTimeout};

	auto usb2dyn = dynamixel::USB2Dynamixel(baudrate, {device.get()});

	int layoutVersion = checkMotorVersion(id, usb2dyn, timeout);

	while(true) {
		auto printVals = [&](int minV, int maxV, int val) {
			if (not minValue) std::cout << TERM_GREEN;
			std::cout << "min: " << minV << "\n";
			if (not minValue) std::cout << TERM_RESET;

			if (minValue) std::cout << TERM_GREEN;
			std::cout << "max: " << maxV << "\n";
			if (minValue) std::cout << TERM_RESET;
			std::cout << "cur: " << val << "\n";
			std::cout << "-------\n";
		};
		try {
			if (layoutVersion == 1) {
				auto layout = readMotorInfos<meta::LayoutType::V1, v1::FullLayout>(usb2dyn, id, timeout);
				printVals(layout.cw_angle_limit, layout.ccw_angle_limit, layout.present_position);
				if (detectInput) {
					auto g = std::lock_guard(mInputMutex);
					detectInput = false;
					if (minValue) {
						write<v1::Register::CW_ANGLE_LIMIT, 2>(usb2dyn, id, {layout.present_position});
					} else {
						write<v1::Register::CCW_ANGLE_LIMIT, 2>(usb2dyn, id, {layout.present_position});
					}
					std::cout << TERM_GREEN " writing limits " TERM_RESET "\n";
					usleep(100000);
				}
			} else if (layoutVersion == 2) {
				auto layout = readMotorInfos<meta::LayoutType::V2, v2::FullLayout>(usb2dyn, id, timeout);
				printVals(layout.min_position_limit, layout.max_position_limit, layout.present_position);

				if (detectInput) {
					auto g = std::lock_guard(mInputMutex);
					detectInput = false;
					if (minValue) {
						write<v2::Register::MIN_POSITION_LIMIT, 4>(usb2dyn, id, {layout.present_position});
					} else {
						write<v2::Register::MAX_POSITION_LIMIT, 4>(usb2dyn, id, {layout.present_position});
					}
					std::cout << TERM_GREEN " writing limits " TERM_RESET "\n";
					usleep(100000);
				}
			}
		} catch (std::exception const& e) {
			std::cout << TERM_RED << "exception: " << e.what() << TERM_RESET << "\n"; 
			usleep(1000000);
		}
	}
	} catch (std::exception const& e) {
		std::cout << TERM_RED << "exception: " << e.what() << "\n";
	}
}

}
