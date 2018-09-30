#include "commonTasks.h"
#include "usb2dynamixel/MotorMetaInfo.h"


using namespace dynamixel;
auto detectMotor(MotorID motor, USB2Dynamixel& usb2dyn, std::chrono::microseconds timeout) -> std::tuple<int, uint16_t> {
	// only read model information, when model is known read full motor
	auto [timeoutFlag, motorID, errorCode, layout] = usb2dyn.read<v1::Register::MODEL_NUMBER, 2>(motor, timeout);
	if (timeoutFlag) {
		return std::make_tuple(-1, 0);
	}
	if (motorID == MotorIDInvalid) {
		std::cout << "something answered when pinging " << int(motor) << " but answer was not valid\n";
		return std::make_tuple(-1, 0);
	}
	auto modelPtr = meta::getMotorInfo(layout.model_number);
	if (modelPtr) {
		std::cout << int(motor) << " " <<  modelPtr->shortName << " (" << layout.model_number << ") Layout " << to_string(modelPtr->layout) << "\n";
		if (modelPtr->layout == meta::LayoutType::V1) {
			return std::make_tuple(1, layout.model_number);
		} else if (modelPtr->layout == meta::LayoutType::V2) {
			return std::make_tuple(2, layout.model_number);
		} else if (modelPtr->layout == meta::LayoutType::Pro) {
			return std::make_tuple(3, layout.model_number);
		}
	}

	std::cout << int(motor) << " unknown model (" << layout.model_number << ")\n";
	return std::make_tuple(0, layout.model_number);
}
