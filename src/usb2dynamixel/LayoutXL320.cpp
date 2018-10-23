#include "LayoutXL320.h"

namespace dynamixel::xl320 {

auto MotorLayoutInfo::getInfos() -> meta::Layout<Register> const& {
	using A = meta::LayoutField::Access;
	static auto data = meta::Layout<Register> {
		{Register::MODEL_NUMBER         , {2,  true, A:: R, "Model Number", "model number"}},
		{Register::FIRMWARE_VERSION     , {1,  true, A:: R, "Version of Firmware", "Information on the version of firmware"}},
		{Register::ID                   , {1,  true, A::RW, "ID", "ID of Dynamixel"}},
		{Register::BAUD_RATE            , {1,  true, A::RW, "Baud Rate", "Baud Rate of Dynamixel"}},
		{Register::RETURN_DELAY_TIME    , {1,  true, A::RW, "Return Delay Time", "Return Delay Time"}},
		{Register::CW_ANGLE_LIMIT       , {2,  true, A::RW, "CW Angle Limit", "clockwise Angle Limit"}},
		{Register::CCW_ANGLE_LIMIT      , {2,  true, A::RW, "CCW Angle Limit", "counterclockwise Angle Limit"}},
		{Register::CONTROL_MODE         , {1,  true, A::RW, "Control Mode", "Dual Mode Setting"}},
		{Register::TEMPERATURE_LIMIT    , {1,  true, A::RW, "Highest Limit Temperature", "Internal Limit Temperature"}},
		{Register::MIN_VOLTAGE_LIMIT    , {1,  true, A::RW, "Min Limit Voltage", "Min Limit Voltage"}},
		{Register::MAX_VOLTAGE_LIMIT    , {1,  true, A::RW, "Max Limit Voltage", "Max Limit Voltage"}},
		{Register::MAX_TORQUE           , {2,  true, A::RW, "Max Torque", "Max. Torque"}},
		{Register::STATUS_RETURN_LEVEL  , {1,  true, A::RW, "Status Return Level", "Status Return Level"}},
		{Register::SHUTDOWN             , {1,  true, A::RW, "Shutdown", "Shutdown for Alarm"}},
		{Register::TORQUE_ENABLE        , {1, false, A::RW, "Torque Enable", "Torque On/Off"}},
		{Register::LED                  , {1, false, A::RW, "LED", "LED On/Off"}},
		{Register::D_GAIN               , {1, false, A::RW, "D Gain", "Derivative Gain"}},
		{Register::I_GAIN               , {1, false, A::RW, "I Gain", "Integral Gain"}},
		{Register::P_GAIN               , {1, false, A::RW, "P Gain", "Proportional Gain"}},
		{Register::GOAL_POSITION        , {2, false, A::RW, "Goal Position", "Goal Position"}},
		{Register::MOVING_SPEED         , {2, false, A::RW, "Moving Speed", "Moving Speed (Moving Velocity)"}},
		{Register::TORQUE_LIMIT         , {2, false, A::RW, "Torque Limit", "Torque Limit (Goal Torque)"}},
		{Register::PRESENT_POSITION     , {2, false, A:: R, "Present Position", "Current Position (Present Velocity)"}},
		{Register::PRESENT_SPEED        , {2, false, A:: R, "Present Speed", "Current Speed"}},
		{Register::PRESENT_LOAD         , {2, false, A:: R, "Present Load", "Current Load"}},
		{Register::PRESENT_VOLTAGE      , {1, false, A:: R, "Present Voltage", "Current Voltage"}},
		{Register::PRESENT_TEMPERATURE  , {1, false, A:: R, "Present Temperature", "Current Temperature"}},
		{Register::REGISTERED           , {1, false, A:: R, "Registered", "Means if Instruction is registered"}},
		{Register::MOVING               , {1, false, A:: R, "Moving", "Means if there is any movement"}},
		{Register::HARDWARE_ERROR_STATUS, {1, false, A:: R, "Hardware Error Status", "Hardware Error Status"}},
		{Register::PUNCH                , {2, false, A::RW, "Punch", "Punch"}},
	};
	return data;
}

auto MotorLayoutInfo::getDefaults() -> std::map<uint32_t, meta::Info<Register>> const& {
	static auto data = std::map<uint32_t, meta::Info<Register>> {
		{350, {
			350,
			LayoutType::XL320,
			"XL-320",
			{"XL-320"},
			meta::buildConverters(1024*360/300, 512, .111), {
				{Register::MODEL_NUMBER         , {   350, {}}},
				{Register::FIRMWARE_VERSION     , {    {}, {}}},
				{Register::ID                   , {     1, {}}},
				{Register::BAUD_RATE            , {     3, {}}},
				{Register::RETURN_DELAY_TIME    , {   250, {}}},
				{Register::CW_ANGLE_LIMIT       , {     0, {}}},
				{Register::CCW_ANGLE_LIMIT      , {0x03ff, {}}},
				{Register::CONTROL_MODE         , {     2, {}}},
				{Register::TEMPERATURE_LIMIT    , {    65, {}}},
				{Register::MIN_VOLTAGE_LIMIT    , {    60, {}}},
				{Register::MIN_VOLTAGE_LIMIT    , {    90, {}}},
				{Register::MAX_TORQUE           , {0x03ff, {}}},
				{Register::STATUS_RETURN_LEVEL  , {     2, {}}},
				{Register::SHUTDOWN             , {     3, {}}},
				{Register::TORQUE_ENABLE        , {     0, {}}},
				{Register::LED                  , {     0, {}}},
				{Register::D_GAIN               , {     0, {}}},
				{Register::I_GAIN               , {     0, {}}},
				{Register::P_GAIN               , {    32, {}}},
				{Register::GOAL_POSITION        , {    {}, {}}},
				{Register::MOVING_SPEED         , {    {}, {}}},
				{Register::TORQUE_LIMIT         , {    {}, {}}},
				{Register::PRESENT_POSITION     , {    {}, {}}},
				{Register::PRESENT_SPEED        , {    {}, {}}},
				{Register::PRESENT_LOAD         , {    {}, {}}},
				{Register::PRESENT_VOLTAGE      , {    {}, {}}},
				{Register::PRESENT_TEMPERATURE  , {    {}, {}}},
				{Register::REGISTERED           , {     0, {}}},
				{Register::MOVING               , {     0, {}}},
				{Register::HARDWARE_ERROR_STATUS, {     0, {}}},
				{Register::PUNCH                , {    32, {}}},
			}
		}}
	};
	return data;
};



}
