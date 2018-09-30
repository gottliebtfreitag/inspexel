#pragma once

#include "usb2dynamixel/USB2Dynamixel.h"
#include <chrono>

auto detectMotor(dynamixel::MotorID motor, dynamixel::USB2Dynamixel& usb2dyn, std::chrono::microseconds timeout) -> std::tuple<int, uint16_t>;

