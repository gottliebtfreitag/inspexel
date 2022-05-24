#include "LayoutMX_V1.h"

namespace dynamixel::mx_v1 {

auto MotorLayoutInfo::getInfos() -> meta::Layout<Register> const& {
	using A = meta::LayoutField::Access;
	static auto data = meta::Layout<Register> {
		{Register::MODEL_NUMBER        ,{2,  true, A:: R, "Model Number", "model number"}},
		{Register::VERSION_FIRMWARE    ,{1,  true, A:: R, "Version of Firmware", "Information on the version of firmware"}},
		{Register::ID                  ,{1,  true, A::RW, "ID", "ID of Dynamixel"}},
		{Register::BAUD_RATE           ,{1,  true, A::RW, "Baud Rate", "Baud Rate of Dynamixel"}},
		{Register::RETURN_DELAY_TIME   ,{1,  true, A::RW, "Return Delay Time", "Return Delay Time"}},
		{Register::CW_ANGLE_LIMIT      ,{2,  true, A::RW, "CW Angle Limit", "clockwise Angle Limit"}},
		{Register::CCW_ANGLE_LIMIT     ,{2,  true, A::RW, "CCW Angle Limit", "counterclockwise Angle Limit"}},
		{Register::DRIVE_MODE          ,{1,  true, A::RW, "Drive Mode", "Dual Mode Setting"}},
		{Register::TEMPERATURE_LIMIT   ,{1,  true, A::RW, "Highest Limit Temperature", "Internal Limit Temperature"}},
		{Register::VOLTAGE_LIMIT_LOW   ,{1,  true, A::RW, "Lowest Limit Voltage", "Lowest Limit Voltage"}},
		{Register::VOLTAGE_LIMIT_HIGH  ,{1,  true, A::RW, "Highest Limit Voltage", "Highest Limit Voltage"}},
		{Register::MAX_TORQUE          ,{2,  true, A::RW, "Max Torque", "Max. Torque"}},
		{Register::STATUS_RETURN_LEVEL ,{1,  true, A::RW, "Status Return Level", "Status Return Level"}},
		{Register::ALARM_LED           ,{1,  true, A::RW, "Alarm LED", "LED for Alarm"}},
		{Register::ALARM_SHUTDOWN      ,{1,  true, A::RW, "Alarm Shutdown", "Shutdown for Alarm"}},
		{Register::MULTI_TURN_OFFSET   ,{2,  true, A::RW, "Multi Turn Offset", "multi{}turn offset"}},
		{Register::RESOLUTION_DIVIDER  ,{1,  true, A::RW, "Resolution Divider", "Resolution divider"}},
		{Register::TORQUE_ENABLE       ,{1, false, A::RW, "Torque Enable", "Torque On/Off"}},
		{Register::LED                 ,{1, false, A::RW, "LED", "LED On/Off"}},
		{Register::D_GAIN              ,{1, false, A::RW, "D Gain", "Derivative Gain"}},
		{Register::I_GAIN              ,{1, false, A::RW, "I Gain", "Integral Gain"}},
		{Register::P_GAIN              ,{1, false, A::RW, "P Gain", "Proportional Gain"}},
		{Register::GOAL_POSITION       ,{2, false, A::RW, "Goal Position", "Goal Position"}},
		{Register::MOVING_SPEED        ,{2, false, A::RW, "Moving Speed", "Moving Speed (Moving Velocity)"}},
		{Register::TORQUE_LIMIT        ,{2, false, A::RW, "Torque Limit", "Torque Limit (Goal Torque)"}},
		{Register::PRESENT_POSITION    ,{2, false, A:: R, "Present Position", "Current Position (Present Velocity)"}},
		{Register::PRESENT_SPEED       ,{2, false, A:: R, "Present Speed", "Current Speed"}},
		{Register::PRESENT_LOAD        ,{2, false, A:: R, "Present Load", "Current Load"}},
		{Register::PRESENT_VOLTAGE     ,{1, false, A:: R, "Present Voltage", "Current Voltage"}},
		{Register::PRESENT_TEMPERATURE ,{1, false, A:: R, "Present Temperature", "Current Temperature"}},
		{Register::REGISTERED          ,{1, false, A:: R, "Registered", "Means if Instruction is registered"}},
		{Register::MOVING              ,{1, false, A:: R, "Moving", "Means if there is any movement"}},
		{Register::LOCK                ,{1, false, A::RW, "Lock", "Locking EEPROM"}},
		{Register::PUNCH               ,{2, false, A::RW, "Punch", "Punch"}},
		{Register::REALTIME_TICK       ,{2, false, A:: R, "Realtime Tick", "Realtime Tick"}},
		{Register::CURRENT             ,{2, false, A:: R, "Current", "Consuming Current"}},
		{Register::TORQUE_CONTROL_MODE ,{1, false, A::RW, "Torque Control Mode Enable", "Torque control mode on/off"}},
		{Register::GOAL_TORQUE         ,{2, false, A::RW, "Goal Torque", "goal torque value"}},
		{Register::GOAL_ACCELERATION   ,{1, false, A::RW, "Goal Acceleration", "Goal Acceleration"}},
	};
	return data;
}

auto MotorLayoutInfo::getDefaults() -> std::map<uint32_t, meta::Info<Register>> const& {
	static auto data = []() {
		auto convertPosition    = meta::buildConverter("r", (2.*M_PI)/4095, 2048);
		auto convertSpeed       = [](std::string unit, double resolution) {
									return meta::Convert {
										unit,
										[=](double val) { return std::max(1., std::round(val / resolution)); },
										[=](int val)    {
											if (val & (1<<10)) {
												val = - (val & ((1<<10)-1));
											}
											return val * resolution;
										},
									};
								}("r/s", (116.62/60*2.*M_PI)/1023.);
		auto convertTemperature = meta::buildConverter("C", 1.);
		auto convertVoltage     = meta::buildConverter("V", 16./160);
		auto convertPID_P       = meta::buildConverter("", 1./8., 0, 0, 254);
		auto convertPID_I       = meta::buildConverter("", 1000./2048., 0, 0, 254);
		auto convertPID_D       = meta::buildConverter("", 4/1000., 0, 0, 254);
		auto convertCurrent     = meta::buildConverter("A", 4.5/1000., 2048);
		auto convertTorque      = meta::buildConverter("%", 100./1023., 0);

		auto data = std::map<uint32_t, meta::Info<Register>> {
			{320, {
				320,
				LayoutType::MX_V1,
				"MX106",
				{"MX-106T", "MX-106R"}, {
					{Register::MODEL_NUMBER        , {          320, {}}},
					{Register::VERSION_FIRMWARE    , { std::nullopt, {}}},
					{Register::ID                  , {            1, {}}},
					{Register::BAUD_RATE           , {           34, {}}},
					{Register::RETURN_DELAY_TIME   , {          250, {}}},
					{Register::CW_ANGLE_LIMIT      , {            0, convertPosition}},
					{Register::CCW_ANGLE_LIMIT     , {       0x0fff, convertPosition}},
					{Register::DRIVE_MODE          , {            0, {}}},
					{Register::TEMPERATURE_LIMIT   , {           80, convertTemperature}},
					{Register::VOLTAGE_LIMIT_LOW   , {           60, convertVoltage}},
					{Register::VOLTAGE_LIMIT_HIGH  , {          160, convertVoltage}},
					{Register::MAX_TORQUE          , {       0x03ff, convertTorque}},
					{Register::STATUS_RETURN_LEVEL , {            2, {}}},
					{Register::ALARM_LED           , {           36, {}}},
					{Register::ALARM_SHUTDOWN      , {           36, {}}},
					{Register::MULTI_TURN_OFFSET   , {            0, {}}},
					{Register::RESOLUTION_DIVIDER  , {            1, {}}},
					{Register::TORQUE_ENABLE       , {            0, {}}},
					{Register::LED                 , {            0, {}}},
					{Register::D_GAIN              , {            0, convertPID_D}},
					{Register::I_GAIN              , {            0, convertPID_I}},
					{Register::P_GAIN              , {           32, convertPID_P}},
					{Register::GOAL_POSITION       , { std::nullopt, convertPosition}},
					{Register::MOVING_SPEED        , { std::nullopt, convertSpeed}},
					{Register::TORQUE_LIMIT        , { std::nullopt, convertTorque}},
					{Register::PRESENT_POSITION    , { std::nullopt, convertPosition}},
					{Register::PRESENT_SPEED       , { std::nullopt, convertSpeed}},
					{Register::PRESENT_LOAD        , { std::nullopt, {}}},
					{Register::PRESENT_VOLTAGE     , { std::nullopt, convertVoltage}},
					{Register::PRESENT_TEMPERATURE , { std::nullopt, convertTemperature}},
					{Register::REGISTERED          , {            0, {}}},
					{Register::MOVING              , {            0, {}}},
					{Register::LOCK                , {            0, {}}},
					{Register::PUNCH               , {            0, {}}},
					{Register::REALTIME_TICK       , {            0, {}}},
					{Register::CURRENT             , {         2048, convertCurrent}},
					{Register::TORQUE_CONTROL_MODE , {            0, {}}},
					{Register::GOAL_TORQUE         , {            0, convertTorque}},
					{Register::GOAL_ACCELERATION   , {            0, {}}},
				}
			}}
		};
		auto newMotor = [&](int number, std::string shortName, std::vector<std::string> names) -> meta::Info<Register>& {
			auto& m = data[number];
			m = data.at(320);
			m.modelNumber = number;
			m.shortName = std::move(shortName);
			m.motorNames = std::move(names);
			std::get<0>(m.defaultLayout[Register::MODEL_NUMBER]) = number;
			return m;
		};

		{
			auto& m = newMotor(310, "MX64", {"MX-64T", "MX-64R", "MX-64AT", "MX-64AR"});
			m.defaultLayout.erase(Register::DRIVE_MODE);
		}
		{
			auto& m = newMotor(29, "MX28", {"MX-28T", "MX-28R", "MX-28AT", "MX-28AR"});
			m.defaultLayout.erase(Register::DRIVE_MODE);
			m.defaultLayout.erase(Register::TORQUE_CONTROL_MODE);
			m.defaultLayout.erase(Register::CURRENT);
			m.defaultLayout.erase(Register::GOAL_TORQUE);
		}
		{
			auto& m = newMotor(360, "MX12", {"MX-12W"});
			auto convertSpeed       = [](std::string unit, double resolution) {
											return meta::Convert {
												unit,
												[=](double val) { return std::max(1., std::round(val / resolution)); },
												[=](int val)    {
													if (val & (1<<10)) {
														val = - (val & ((1<<10)-1));
													}
													return val * resolution;
												},
											};
										}("r/s", (937.1/60*2.*M_PI)/1023.);

			std::get<1>(m.defaultLayout[Register::MOVING_SPEED])  = convertSpeed;
			std::get<1>(m.defaultLayout[Register::PRESENT_SPEED]) = convertSpeed;

			std::get<0>(m.defaultLayout[Register::MODEL_NUMBER]) = 360;
			std::get<0>(m.defaultLayout[Register::BAUD_RATE])    = 1;
			std::get<0>(m.defaultLayout[Register::D_GAIN])       = 8;
			std::get<0>(m.defaultLayout[Register::P_GAIN])       = 8;
			std::get<0>(m.defaultLayout[Register::PUNCH])        = 32;
			m.defaultLayout.erase(Register::DRIVE_MODE);
			m.defaultLayout.erase(Register::CURRENT);
			m.defaultLayout.erase(Register::TORQUE_CONTROL_MODE);
			m.defaultLayout.erase(Register::GOAL_TORQUE);
		}
		return data;
	}();
	return data;
};


}
