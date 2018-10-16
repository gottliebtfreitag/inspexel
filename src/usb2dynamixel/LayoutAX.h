#pragma once

#include "LayoutPart.h"

namespace dynamixel{
namespace ax {

enum class Register : int {
	MODEL_NUMBER          = 0x00,
	FIRMWARE_VERSION      = 0x02,
	ID                    = 0x03,
	BAUD_RATE             = 0x04,
	RETURN_DELAY_TIME     = 0x05,
	CW_ANGLE_LIMIT        = 0x06,
	CCW_ANGLE_LIMIT       = 0x08,
	TEMPERATURE_LIMIT     = 0x0b,
	MIN_VOLTAGE_LIMIT     = 0x0c,
	MAX_VOLTAGE_LIMIT     = 0x0d,
	MAX_TORQUE            = 0x0e,
	STATUS_RETURN_LEVEL   = 0x10,
	ALARM_LED             = 0x11,
	SHUTDOWN              = 0x12,
	TORQUE_ENABLE         = 0x18,
	LED                   = 0x19,
	CW_COMPLIANCE_MARGIN  = 0x1a,
	CCW_COMPLIANCE_MARGIN = 0x1b,
	CW_COMPLIANCE_SLOPE   = 0x1c,
	CCW_COMPLIANCE_SLOPE  = 0x1d,
	GOAL_POSITION         = 0x1e,
	MOVING_SPEED          = 0x20,
	TORQUE_LIMIT          = 0x22,
	PRESENT_POSITION      = 0x24,
	PRESENT_SPEED         = 0x26,
	PRESENT_LOAD          = 0x28,
	PRESENT_VOLTAGE       = 0x2a,
	PRESENT_TEMPERATURE   = 0x2b,
	REGISTERED            = 0x2c,
	MOVING                = 0x2e,
	LOCK                  = 0x2f,
	PUNCH                 = 0x30,
};

constexpr Register operator+(Register t1, size_t t2) {
	return Register(size_t(t1) + t2);
}

using FullLayout = Layout<Register::MODEL_NUMBER, 50>;

}

#pragma pack(push, 1)
DynamixelLayoutPart(ax::Register::MODEL_NUMBER         , uint16_t, model_number         );
DynamixelLayoutPart(ax::Register::FIRMWARE_VERSION     ,  uint8_t, firmware_version     );
DynamixelLayoutPart(ax::Register::ID                   ,  uint8_t, id                   );
DynamixelLayoutPart(ax::Register::BAUD_RATE            ,  uint8_t, baud_rate            );
DynamixelLayoutPart(ax::Register::RETURN_DELAY_TIME    ,  uint8_t, return_delay_time    );
DynamixelLayoutPart(ax::Register::CW_ANGLE_LIMIT       ,  int16_t, cw_angle_limit       );
DynamixelLayoutPart(ax::Register::CCW_ANGLE_LIMIT      ,  int16_t, ccw_angle_limit      );
DynamixelLayoutPart(ax::Register::TEMPERATURE_LIMIT    ,  uint8_t, temperature_limit    );
DynamixelLayoutPart(ax::Register::MIN_VOLTAGE_LIMIT    ,  uint8_t, min_voltage_limit    );
DynamixelLayoutPart(ax::Register::MAX_VOLTAGE_LIMIT    ,  uint8_t, max_voltage_limit    );
DynamixelLayoutPart(ax::Register::MAX_TORQUE           , uint16_t, max_torque           );
DynamixelLayoutPart(ax::Register::STATUS_RETURN_LEVEL  ,  uint8_t, status_return_level  );
DynamixelLayoutPart(ax::Register::ALARM_LED            ,  uint8_t, alarm_led            );
DynamixelLayoutPart(ax::Register::SHUTDOWN             ,  uint8_t, shutdown             );
DynamixelLayoutPart(ax::Register::TORQUE_ENABLE        ,     bool, torque_enable        );
DynamixelLayoutPart(ax::Register::LED                  ,     bool, led                  );
DynamixelLayoutPart(ax::Register::CW_COMPLIANCE_MARGIN ,  uint8_t, cw_compliance_margin );
DynamixelLayoutPart(ax::Register::CCW_COMPLIANCE_MARGIN,  uint8_t, ccw_compliance_margin);
DynamixelLayoutPart(ax::Register::CW_COMPLIANCE_SLOPE  ,  uint8_t, cw_compliance_slope  );
DynamixelLayoutPart(ax::Register::CCW_COMPLIANCE_SLOPE ,  uint8_t, ccw_compliance_slope );
DynamixelLayoutPart(ax::Register::GOAL_POSITION        ,  int16_t, goal_position        );
DynamixelLayoutPart(ax::Register::MOVING_SPEED         ,  int16_t, moving_speed         );
DynamixelLayoutPart(ax::Register::TORQUE_LIMIT         , uint16_t, torque_limit         );
DynamixelLayoutPart(ax::Register::PRESENT_POSITION     ,  int16_t, present_position     );
DynamixelLayoutPart(ax::Register::PRESENT_SPEED        ,  int16_t, present_speed        );
DynamixelLayoutPart(ax::Register::PRESENT_LOAD         ,  int16_t, present_load         );
DynamixelLayoutPart(ax::Register::PRESENT_VOLTAGE      ,  uint8_t, present_voltage      );
DynamixelLayoutPart(ax::Register::PRESENT_TEMPERATURE  ,  uint8_t, present_temperature  );
DynamixelLayoutPart(ax::Register::REGISTERED           ,  uint8_t, registered           );
DynamixelLayoutPart(ax::Register::MOVING               ,     bool, moving               );
DynamixelLayoutPart(ax::Register::LOCK                 ,     bool, lock);
DynamixelLayoutPart(ax::Register::PUNCH                , uint16_t, punch                );

#pragma pack(pop)

}
