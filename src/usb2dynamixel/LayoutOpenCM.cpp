#include "LayoutOpenCM.h"

namespace dynamixel::opencm {

auto MotorLayoutInfo::getInfos() -> meta::Layout<Register> const& {
	using A = meta::LayoutField::Access;
	static auto data = meta::Layout<Register> {
		{Register::MODEL_NUMBER                    , {2, true,  A:: R, "Model Number", "Model Number"}},
		{Register::VERSION_FIRMWARE                , {1, true,  A::RW, "Firmware Version", "Firmware Version"}},
		{Register::ID                              , {1, true,  A:: R, "ID", "Controller ID"}},
		{Register::BAUD_RATE                       , {1, true,  A::RW, "Baud Rate", "Communication Baud Rate"}},
		{Register::RETURN_DELAY_TIME               , {1, true,  A::RW, "Return Delay Time", "Response Delay Time"}},
		{Register::STATUS_RETURN_LEVEL             , {1, true,  A::RW, "Status Return Level", "Select Types of Status Return"}},
		{Register::DXL_BAUD_RATE                   , {1, true,  A::RW, "DXL Baud Rate", "Dynamixel Communication Baud Rate"}},
		{Register::MODE_NUMBER                     , {1, false, A::RW, "Mode Number", "Operation Mode"}},
		{Register::BUTTON_STATUS                   , {1, false, A:: R, "Button Status", "Start Button Status"}},
		{Register::MOTION_PLAY_PAGE                , {2, false, A::RW, "Motion Play Page", "Motion Play Page Number"}},
		{Register::MOTION_PLAY_STATUS              , {1, false, A:: R, "Motion Play Status", "Motion Play Status"}},
		{Register::TIMER_VALUE_128MS               , {1, false, A::RW, "128ms Timer Value", "128ms Timer Counter"}},
		{Register::TIMER_VALUE_1MS                 , {2, false, A::RW, "1ms Timer Value", "1ms Timer Counter"}},
		{Register::RANDOM_NUMBER                   , {1, false, A::RW, "Random Number", "Creating Random Number"}},
		{Register::GREEN_LED                       , {1, false, A::RW, "Green LED", "Green LED Status"}},
		{Register::MOTION_LED                      , {1, false, A::RW, "Motion LED", "Motion LED Status"}},
		{Register::PORT_1_IR_SENSOR_VALUE          , {2, false, A:: R, "Port 1 IR Sensor Value", "IR Sensor Value on Port 1"}},
		{Register::PORT_4_IR_SENSOR_VALUE          , {2, false, A:: R, "Port 4 IR Sensor Value", "IR Sensor Value on Port 4"}},
		{Register::PORT_1_DMS_SENSOR_VALUE         , {2, false, A:: R, "Port 1 DMS Sensor Value", "DMS Sensor Value on Port 1"}},
		{Register::PORT_2_DMS_SENSOR_VALUE         , {2, false, A:: R, "Port 2 DMS Sensor Value", "DMS Sensor Value on Port 2"}},
		{Register::PORT_3_DMS_SENSOR_VALUE         , {2, false, A:: R, "Port 3 DMS Sensor Value", "DMS Sensor Value on Port 3"}},
		{Register::PORT_4_DMS_SENSOR_VALUE         , {2, false, A:: R, "Port 4 DMS Sensor Value", "DMS Sensor Value on Port 4"}},
		{Register::PORT_1_TOUCH_SENSOR_VALUE       , {1, false, A:: R, "Port 1 Touch Sensor Value", "Touch Sensor Value on Port 1"}},
		{Register::PORT_2_TOUCH_SENSOR_VALUE       , {1, false, A:: R, "Port 2 Touch Sensor Value", "Touch Sensor Value on Port 2"}},
		{Register::PORT_3_TOUCH_SENSOR_VALUE       , {1, false, A:: R, "Port 3 Touch Sensor Value", "Touch Sensor Value on Port 3"}},
		{Register::PORT_4_TOUCH_SENSOR_VALUE       , {1, false, A:: R, "Port 4 Touch Sensor Value", "Touch Sensor Value on Port 4"}},
		{Register::PORT_2_LED_MODULE_VALUE         , {1, false, A::RW, "Port 2 LED Module Value", "LED Module Control Value on Port 2"}},
		{Register::PORT_3_LED_MODULE_VALUE         , {1, false, A::RW, "Port 3 LED Module Value", "LED Module Control Value on Port 3"}},
		{Register::PORT_2_USER_DEVICE_VALUE        , {2, false, A::RW, "Port 2 User Device Value", "User Device Value on Port 2"}},
		{Register::PORT_3_USER_DEVICE_VALUE        , {2, false, A::RW, "Port 3 User Device Value", "User Device Value on Port 3"}},
		{Register::PORT_1_TEMPERATURE_SENSOR_VALUE , {1, false, A:: R, "Port 1 Temperature Sensor Value", "Temperature Sensor Value on Port 1"}},
		{Register::PORT_3_TEMPERATURE_SENSOR_VALUE , {1, false, A:: R, "Port 3 Temperature Sensor Value", "Temperature Sensor Value on Port 3"}},
		{Register::PORT_1_ULTRASONIC_SENSOR_VALUE  , {1, false, A:: R, "Port 1 Ultrasonic Sensor Value", "Ultrasonic Sensor Value on Port 1"}},
		{Register::PORT_2_ULTRASONIC_SENSOR_VALUE  , {1, false, A:: R, "Port 2 Ultrasonic Sensor Value", "Ultrasonic Sensor Value on Port 2"}},
		{Register::PORT_3_ULTRASONIC_SENSOR_VALUE  , {1, false, A:: R, "Port 3 Ultrasonic Sensor Value", "Ultrasonic Sensor Value on Port 3"}},
		{Register::PORT_4_ULTRASONIC_SENSOR_VALUE  , {1, false, A:: R, "Port 4 Ultrasonic Sensor Value", "Ultrasonic Sensor Value on Port 4"}},
		{Register::PORT_1_MAGNETIC_SENSOR_VALUE    , {1, false, A:: R, "Port 1 Magnetic Sensor Value", "Magnetic Sensor Value on Port 1"}},
		{Register::PORT_2_MAGNETIC_SENSOR_VALUE    , {1, false, A:: R, "Port 2 Magnetic Sensor Value", "Magnetic Sensor Value on Port 2"}},
		{Register::PORT_3_MAGNETIC_SENSOR_VALUE    , {1, false, A:: R, "Port 3 Magnetic Sensor Value", "Magnetic Sensor Value on Port 3"}},
		{Register::PORT_4_MAGNETIC_SENSOR_VALUE    , {1, false, A:: R, "Port 4 Magnetic Sensor Value", "Magnetic Sensor Value on Port 4"}},
		{Register::PORT_1_MOTION_SENSOR_VALUE      , {1, false, A:: R, "Port 1 Motion Sensor Value", "Motion Sensor Value on Port 1"}},
		{Register::PORT_2_MOTION_SENSOR_VALUE      , {1, false, A:: R, "Port 2 Motion Sensor Value", "Motion Sensor Value on Port 2"}},
		{Register::PORT_3_MOTION_SENSOR_VALUE      , {1, false, A:: R, "Port 3 Motion Sensor Value", "Motion Sensor Value on Port 3"}},
		{Register::PORT_4_MOTION_SENSOR_VALUE      , {1, false, A:: R, "Port 4 Motion Sensor Value", "Motion Sensor Value on Port 4"}},
		{Register::PORT_2_COLOR_SENSOR_VALUE       , {1, false, A:: R, "Port 2 Color Sensor Value", "Color Sensor Value on Port 2"}},
		{Register::PORT_3_COLOR_SENSOR_VALUE       , {1, false, A:: R, "Port 3 Color Sensor Value", "Color Sensor Value on Port 3"}},
	};
	return data;
}

auto MotorLayoutInfo::getDefaults() -> std::map<uint32_t, meta::Info<Register>> const& {
	static auto data = []() {
		auto data = std::map<uint32_t, meta::Info<Register>> {
			{400, {
				400,
				LayoutType::OpenCM,
				"OpenCM9.04",
				{"OpenCM9.04 A Type", "OpenCM9.04 B Type", "OpenCM9.04 C Type"}, {
					{Register::MODEL_NUMBER                   , { 400, {}}},
					{Register::VERSION_FIRMWARE               , {  {}, {}}},
					{Register::ID                             , { 200, {}}},
					{Register::BAUD_RATE                      , {   1, {}}},
					{Register::RETURN_DELAY_TIME              , {   0, {}}},
					{Register::STATUS_RETURN_LEVEL            , {   2, {}}},
					{Register::DXL_BAUD_RATE                  , {   3, {}}},
					{Register::MODE_NUMBER                    , {  {}, {}}},
					{Register::BUTTON_STATUS                  , {   0, {}}},
					{Register::MOTION_PLAY_PAGE               , {   0, {}}},
					{Register::MOTION_PLAY_STATUS             , {  {}, {}}},
					{Register::TIMER_VALUE_128MS              , {   0, {}}},
					{Register::TIMER_VALUE_1MS                , {   0, {}}},
					{Register::RANDOM_NUMBER                  , {  {}, {}}},
					{Register::GREEN_LED                      , {  {}, {}}},
					{Register::MOTION_LED                     , {  {}, {}}},
					{Register::PORT_1_IR_SENSOR_VALUE         , {  {}, {}}},
					{Register::PORT_4_IR_SENSOR_VALUE         , {  {}, {}}},
					{Register::PORT_1_DMS_SENSOR_VALUE        , {  {}, {}}},
					{Register::PORT_2_DMS_SENSOR_VALUE        , {  {}, {}}},
					{Register::PORT_3_DMS_SENSOR_VALUE        , {  {}, {}}},
					{Register::PORT_4_DMS_SENSOR_VALUE        , {  {}, {}}},
					{Register::PORT_1_TOUCH_SENSOR_VALUE      , {  {}, {}}},
					{Register::PORT_2_TOUCH_SENSOR_VALUE      , {  {}, {}}},
					{Register::PORT_3_TOUCH_SENSOR_VALUE      , {  {}, {}}},
					{Register::PORT_4_TOUCH_SENSOR_VALUE      , {  {}, {}}},
					{Register::PORT_2_LED_MODULE_VALUE        , {   0, {}}},
					{Register::PORT_3_LED_MODULE_VALUE        , {   0, {}}},
					{Register::PORT_2_USER_DEVICE_VALUE       , {   0, {}}},
					{Register::PORT_3_USER_DEVICE_VALUE       , {   0, {}}},
					{Register::PORT_1_TEMPERATURE_SENSOR_VALUE, {  {}, {}}},
					{Register::PORT_3_TEMPERATURE_SENSOR_VALUE, {  {}, {}}},
					{Register::PORT_1_ULTRASONIC_SENSOR_VALUE , {  {}, {}}},
					{Register::PORT_2_ULTRASONIC_SENSOR_VALUE , {  {}, {}}},
					{Register::PORT_3_ULTRASONIC_SENSOR_VALUE , {  {}, {}}},
					{Register::PORT_4_ULTRASONIC_SENSOR_VALUE , {  {}, {}}},
					{Register::PORT_1_MAGNETIC_SENSOR_VALUE   , {  {}, {}}},
					{Register::PORT_2_MAGNETIC_SENSOR_VALUE   , {  {}, {}}},
					{Register::PORT_3_MAGNETIC_SENSOR_VALUE   , {  {}, {}}},
					{Register::PORT_4_MAGNETIC_SENSOR_VALUE   , {  {}, {}}},
					{Register::PORT_1_MOTION_SENSOR_VALUE     , {  {}, {}}},
					{Register::PORT_2_MOTION_SENSOR_VALUE     , {  {}, {}}},
					{Register::PORT_3_MOTION_SENSOR_VALUE     , {  {}, {}}},
					{Register::PORT_4_MOTION_SENSOR_VALUE     , {  {}, {}}},
					{Register::PORT_2_COLOR_SENSOR_VALUE      , {  {}, {}}},
					{Register::PORT_3_COLOR_SENSOR_VALUE      , {  {}, {}}},
				}
			}}
		};
		return data;
	}();
	return data;
};


}
