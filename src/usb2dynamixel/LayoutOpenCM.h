#pragma once

#include "LayoutPart.h"

namespace dynamixel::opencm {

enum class Register : int {
	MODEL_NUMBER                    =  0x00,
	VERSION_FIRMWARE                =  0x06,
	ID                              =  0x07,
	BAUD_RATE                       =  0x08,
	RETURN_DELAY_TIME               =  0x09,
	STATUS_RETURN_LEVEL             =  0x0a,
	DXL_BAUD_RATE                   =  0x0c,
	MODE_NUMBER                     =  0x15,
	BUTTON_STATUS                   =  0x1a,
	MOTION_PLAY_PAGE                =  0x42,
	MOTION_PLAY_STATUS              =  0x44,
	TIMER_VALUE_128MS               =  0x49,
	TIMER_VALUE_1MS                 =  0x4a,
	RANDOM_NUMBER                   =  0x4d,
	GREEN_LED                       =  0x4f,
	MOTION_LED                      =  0x52,
	PORT_1_IR_SENSOR_VALUE          = 0x168,
	PORT_4_IR_SENSOR_VALUE          = 0x16e,
	PORT_1_DMS_SENSOR_VALUE         = 0x170,
	PORT_2_DMS_SENSOR_VALUE         = 0x172,
	PORT_3_DMS_SENSOR_VALUE         = 0x174,
	PORT_4_DMS_SENSOR_VALUE         = 0x176,
	PORT_1_TOUCH_SENSOR_VALUE       = 0x178,
	PORT_2_TOUCH_SENSOR_VALUE       = 0x179,
	PORT_3_TOUCH_SENSOR_VALUE       = 0x17a,
	PORT_4_TOUCH_SENSOR_VALUE       = 0x17b,
	PORT_2_LED_MODULE_VALUE         = 0x17d,
	PORT_3_LED_MODULE_VALUE         = 0x17e,
	PORT_2_USER_DEVICE_VALUE        = 0x182,
	PORT_3_USER_DEVICE_VALUE        = 0x184,
	PORT_1_TEMPERATURE_SENSOR_VALUE = 0x188,
	PORT_2_TEMPERATURE_SENSOR_VALUE = 0x189,
	PORT_3_TEMPERATURE_SENSOR_VALUE = 0x18a,
	PORT_4_TEMPERATURE_SENSOR_VALUE = 0x18b,
	PORT_1_ULTRASONIC_SENSOR_VALUE  = 0x18c,
	PORT_2_ULTRASONIC_SENSOR_VALUE  = 0x18d,
	PORT_3_ULTRASONIC_SENSOR_VALUE  = 0x18e,
	PORT_4_ULTRASONIC_SENSOR_VALUE  = 0x18f,
	PORT_1_MAGNETIC_SENSOR_VALUE    = 0x190,
	PORT_2_MAGNETIC_SENSOR_VALUE    = 0x191,
	PORT_3_MAGNETIC_SENSOR_VALUE    = 0x192,
	PORT_4_MAGNETIC_SENSOR_VALUE    = 0x193,
	PORT_1_MOTION_SENSOR_VALUE      = 0x194,
	PORT_2_MOTION_SENSOR_VALUE      = 0x195,
	PORT_3_MOTION_SENSOR_VALUE      = 0x196,
	PORT_4_MOTION_SENSOR_VALUE      = 0x197,
	PORT_2_COLOR_SENSOR_VALUE       = 0x199,
	PORT_3_COLOR_SENSOR_VALUE       = 0x19a,
};

/*
MODE_NUMBER                     =  21 	1 	RW 	-
BUTTON_STATUS                   =  26 	1 	R 	0
MOTION_PLAY_PAGE                =  66 	2 	RW 	0
MOTION_PLAY_STATUS              =  68 	1 	R 	-
TIMER_VALUE_128MS               =  73 	1 	RW 	0
TIMER_VALUE_1MS                 =  74 	2 	RW 	0
RANDOM_NUMBER                   =  77 	1 	RW 	-
GREEN_LED                       =  79 	1 	RW 	0
MOTION_LED                      =  82 	1 	RW 	0
PORT_1_IR_SENSOR_VALUE          = 360 	2 	R 	-
PORT_4_IR_SENSOR_VALUE          = 366 	2 	R 	-
PORT_1_DMS_SENSOR_VALUE         = 368 	2 	R 	-
PORT_2_DMS_SENSOR_VALUE         = 370 	2 	R 	-
PORT_3_DMS_SENSOR_VALUE         = 372 	2 	R 	-
PORT_4_DMS_SENSOR_VALUE         = 374 	2 	R 	-
PORT_1_TOUCH_SENSOR_VALUE       = 376 	1 	R 	-
PORT_2_TOUCH_SENSOR_VALUE       = 377 	1 	R 	-
PORT_3_TOUCH_SENSOR_VALUE       = 378 	1 	R 	-
PORT_4_TOUCH_SENSOR_VALUE       = 379 	1 	R 	-
PORT_2_LED_MODULE_VALUE         = 381 	1 	RW 	0
PORT_3_LED_MODULE_VALUE         = 382 	1 	RW 	0
PORT_2_USER_DEVICE_VALUE        = 386 	2 	RW 	0
PORT_3_USER_DEVICE_VALUE        = 388 	2 	RW 	0
PORT_1_TEMPERATURE_SENSOR_VALUE = 392 	1 	R 	-
PORT_2_TEMPERATURE_SENSOR_VALUE = 393 	1 	R 	-
PORT_3_TEMPERATURE_SENSOR_VALUE = 394 	1 	R 	-
PORT_4_TEMPERATURE_SENSOR_VALUE = 395 	1 	R 	-
PORT_1_ULTRASONIC_SENSOR_VALUE  = 396 	1 	R 	-
PORT_2_ULTRASONIC_SENSOR_VALUE  = 397 	1 	R 	-
PORT_3_ULTRASONIC_SENSOR_VALUE  = 398 	1 	R 	-
PORT_4_ULTRASONIC_SENSOR_VALUE  = 399 	1 	R 	-
PORT_1_MAGNETIC_SENSOR_VALUE    = 400 	1 	R 	-
PORT_2_MAGNETIC_SENSOR_VALUE    = 401 	1 	R 	-
PORT_3_MAGNETIC_SENSOR_VALUE    = 402 	1 	R 	-
PORT_4_MAGNETIC_SENSOR_VALUE    = 403 	1 	R 	-
PORT_1_MOTION_SENSOR_VALUE      = 404 	1 	R 	-
PORT_2_MOTION_SENSOR_VALUE      = 405 	1 	R 	-
PORT_3_MOTION_SENSOR_VALUE      = 406 	1 	R 	-
PORT_4_MOTION_SENSOR_VALUE      = 407 	1 	R 	-
PORT_2_COLOR_SENSOR_VALUE       = 409 	1 	R 	-
PORT_3_COLOR_SENSOR_VALUE       = 410 	1 	R 	-
*/


constexpr Register operator+(Register t1, size_t t2) {
	return Register(size_t(t1) + t2);
}

struct MotorLayoutInfo {
	static constexpr LayoutType Type{LayoutType::OpenCM};
	using FullLayout = Layout<Register::MODEL_NUMBER, 74>;

	static auto getInfos()    -> meta::Layout<Register> const&;
	static auto getDefaults() -> std::map<uint32_t, meta::Info<Register>> const&;
};

using FullLayout = MotorLayoutInfo::FullLayout;

}
namespace dynamixel {

template <> struct meta::MotorLayoutInfo<LayoutType::OpenCM> : opencm::MotorLayoutInfo {};


#pragma pack(push, 1)
DynamixelLayoutPart(opencm::Register::MODEL_NUMBER                    , uint16_t, model_number                   );
DynamixelLayoutPart(opencm::Register::VERSION_FIRMWARE                ,  uint8_t, version_firmware               );
DynamixelLayoutPart(opencm::Register::ID                              ,  uint8_t, id                             );
DynamixelLayoutPart(opencm::Register::BAUD_RATE                       ,  uint8_t, baud_rate                      );
DynamixelLayoutPart(opencm::Register::RETURN_DELAY_TIME               ,  uint8_t, return_delay_time              );
DynamixelLayoutPart(opencm::Register::STATUS_RETURN_LEVEL             ,  uint8_t, status_return_level            );
DynamixelLayoutPart(opencm::Register::DXL_BAUD_RATE                   ,  uint8_t, dxl_baud_rate                  );
DynamixelLayoutPart(opencm::Register::BUTTON_STATUS                   ,  uint8_t, button_status                  );
DynamixelLayoutPart(opencm::Register::MOTION_PLAY_PAGE                , uint16_t, motion_play_page               );
DynamixelLayoutPart(opencm::Register::MOTION_PLAY_STATUS              ,  uint8_t, motion_play_status             );
DynamixelLayoutPart(opencm::Register::TIMER_VALUE_128MS               ,  uint8_t, timer_value_128ms              );
DynamixelLayoutPart(opencm::Register::TIMER_VALUE_1MS                 , uint16_t, timer_value_1ms                );
DynamixelLayoutPart(opencm::Register::RANDOM_NUMBER                   ,  uint8_t, random_number                  );
DynamixelLayoutPart(opencm::Register::GREEN_LED                       ,  uint8_t, green_led                      );
DynamixelLayoutPart(opencm::Register::MOTION_LED                      ,  uint8_t, motion_led                     );
DynamixelLayoutPart(opencm::Register::PORT_1_IR_SENSOR_VALUE          , uint16_t, port_1_ir_sensor_value         );
DynamixelLayoutPart(opencm::Register::PORT_4_IR_SENSOR_VALUE          , uint16_t, port_4_ir_sensor_value         );
DynamixelLayoutPart(opencm::Register::PORT_1_DMS_SENSOR_VALUE         , uint16_t, port_1_dms_sensor_value        );
DynamixelLayoutPart(opencm::Register::PORT_2_DMS_SENSOR_VALUE         , uint16_t, port_2_dms_sensor_value        );
DynamixelLayoutPart(opencm::Register::PORT_3_DMS_SENSOR_VALUE         , uint16_t, port_3_dms_sensor_value        );
DynamixelLayoutPart(opencm::Register::PORT_4_DMS_SENSOR_VALUE         , uint16_t, port_4_dms_sensor_value        );
DynamixelLayoutPart(opencm::Register::PORT_1_TOUCH_SENSOR_VALUE       ,  uint8_t, port_1_touch_sensor_value      );
DynamixelLayoutPart(opencm::Register::PORT_2_TOUCH_SENSOR_VALUE       ,  uint8_t, port_2_touch_sensor_value      );
DynamixelLayoutPart(opencm::Register::PORT_3_TOUCH_SENSOR_VALUE       ,  uint8_t, port_3_touch_sensor_value      );
DynamixelLayoutPart(opencm::Register::PORT_4_TOUCH_SENSOR_VALUE       ,  uint8_t, port_4_touch_sensor_value      );
DynamixelLayoutPart(opencm::Register::PORT_2_LED_MODULE_VALUE         ,  uint8_t, port_2_led_module_value        );
DynamixelLayoutPart(opencm::Register::PORT_3_LED_MODULE_VALUE         ,  uint8_t, port_3_led_module_value        );
DynamixelLayoutPart(opencm::Register::PORT_2_USER_DEVICE_VALUE        , uint16_t, port_2_user_device_value       );
DynamixelLayoutPart(opencm::Register::PORT_3_USER_DEVICE_VALUE        , uint16_t, port_3_user_device_value       );
DynamixelLayoutPart(opencm::Register::PORT_1_TEMPERATURE_SENSOR_VALUE ,  uint8_t, port_1_temperature_sensor_value);
DynamixelLayoutPart(opencm::Register::PORT_2_TEMPERATURE_SENSOR_VALUE ,  uint8_t, port_2_temperature_sensor_value);
DynamixelLayoutPart(opencm::Register::PORT_3_TEMPERATURE_SENSOR_VALUE ,  uint8_t, port_3_temperature_sensor_value);
DynamixelLayoutPart(opencm::Register::PORT_4_TEMPERATURE_SENSOR_VALUE ,  uint8_t, port_4_temperature_sensor_value);
DynamixelLayoutPart(opencm::Register::PORT_1_ULTRASONIC_SENSOR_VALUE  ,  uint8_t, port_1_ultrasonic_sensor_value );
DynamixelLayoutPart(opencm::Register::PORT_2_ULTRASONIC_SENSOR_VALUE  ,  uint8_t, port_2_ultrasonic_sensor_value );
DynamixelLayoutPart(opencm::Register::PORT_3_ULTRASONIC_SENSOR_VALUE  ,  uint8_t, port_3_ultrasonic_sensor_value );
DynamixelLayoutPart(opencm::Register::PORT_4_ULTRASONIC_SENSOR_VALUE  ,  uint8_t, port_4_ultrasonic_sensor_value );
DynamixelLayoutPart(opencm::Register::PORT_1_MAGNETIC_SENSOR_VALUE    ,  uint8_t, port_1_magnetic_sensor_value   );
DynamixelLayoutPart(opencm::Register::PORT_2_MAGNETIC_SENSOR_VALUE    ,  uint8_t, port_2_magnetic_sensor_value   );
DynamixelLayoutPart(opencm::Register::PORT_3_MAGNETIC_SENSOR_VALUE    ,  uint8_t, port_3_magnetic_sensor_value   );
DynamixelLayoutPart(opencm::Register::PORT_4_MAGNETIC_SENSOR_VALUE    ,  uint8_t, port_4_magnetic_sensor_value   );
DynamixelLayoutPart(opencm::Register::PORT_1_MOTION_SENSOR_VALUE      ,  uint8_t, port_1_motion_sensor_value     );
DynamixelLayoutPart(opencm::Register::PORT_2_MOTION_SENSOR_VALUE      ,  uint8_t, port_2_motion_sensor_value     );
DynamixelLayoutPart(opencm::Register::PORT_3_MOTION_SENSOR_VALUE      ,  uint8_t, port_3_motion_sensor_value     );
DynamixelLayoutPart(opencm::Register::PORT_4_MOTION_SENSOR_VALUE      ,  uint8_t, port_4_motion_sensor_value     );
DynamixelLayoutPart(opencm::Register::PORT_2_COLOR_SENSOR_VALUE       ,  uint8_t, port_2_color_sensor_value      );
DynamixelLayoutPart(opencm::Register::PORT_3_COLOR_SENSOR_VALUE       ,  uint8_t, port_3_color_sensor_value      );
#pragma pack(pop)
}
