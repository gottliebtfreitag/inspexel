#include "MotorMetaInfo.h"

namespace dynamixel::meta {

auto getLayoutV1Infos() -> std::map<v1::Register, LayoutField> const& {
	using A = LayoutField::Access;
	static std::map<v1::Register, LayoutField> data {
		{v1::Register::MODEL_NUMBER        ,{2,  true, A:: R, "Model Number", "model number"}},
		{v1::Register::VERSION_FIRMWARE    ,{1,  true, A:: R, "Version of Firmware", "Information on the version of firmware"}},
		{v1::Register::ID                  ,{1,  true, A::RW, "ID", "ID of Dynamixel"}},
		{v1::Register::BAUD_RATE           ,{1,  true, A::RW, "Baud Rate", "Baud Rate of Dynamixel"}},
		{v1::Register::RETURN_DELAY_TIME   ,{1,  true, A::RW, "Return Delay Time", "Return Delay Time"}},
		{v1::Register::CW_ANGLE_LIMIT      ,{2,  true, A::RW, "CW Angle Limit", "clockwise Angle Limit"}},
		{v1::Register::CCW_ANGLE_LIMIT     ,{2,  true, A::RW, "CCW Angle Limit", "counterclockwise Angle Limit"}},
		{v1::Register::DRIVE_MODE          ,{1,  true, A::RW, "Drive Mode", "Dual Mode Setting"}},
		{v1::Register::TEMPERATURE_LIMIT   ,{1,  true, A::RW, "Highest Limit Temperature", "Internal Limit Temperature"}},
		{v1::Register::VOLTAGE_LIMIT_LOW   ,{1,  true, A::RW, "Lowest Limit Voltage", "Lowest Limit Voltage"}},
		{v1::Register::VOLTAGE_LIMIT_HIGH  ,{1,  true, A::RW, "Highest Limit Voltage", "Highest Limit Voltage"}},
		{v1::Register::MAX_TORQUE          ,{2,  true, A::RW, "Max Torque", "Max. Torque"}},
		{v1::Register::STATUS_RETURN_LEVEL ,{1,  true, A::RW, "Status Return Level", "Status Return Level"}},
		{v1::Register::ALARM_LED           ,{1,  true, A::RW, "Alarm LED", "LED for Alarm"}},
		{v1::Register::ALARM_SHUTDOWN      ,{1,  true, A::RW, "Alarm Shutdown", "Shutdown for Alarm"}},
		{v1::Register::MULTI_TURN_OFFSET   ,{2,  true, A::RW, "Multi Turn Offset", "multi{}turn offset"}},
		{v1::Register::RESOLUTION_DIVIDER  ,{1,  true, A::RW, "Resolution Divider", "Resolution divider"}},
		{v1::Register::TORQUE_ENABLE       ,{1, false, A::RW, "Torque Enable", "Torque On/Off"}},
		{v1::Register::LED                 ,{1, false, A::RW, "LED", "LED On/Off"}},
		{v1::Register::D_GAIN              ,{1, false, A::RW, "D Gain", "Derivative Gain"}},
		{v1::Register::I_GAIN              ,{1, false, A::RW, "I Gain", "Integral Gain"}},
		{v1::Register::P_GAIN              ,{1, false, A::RW, "P Gain", "Proportional Gain"}},
		{v1::Register::GOAL_POSITION       ,{2, false, A::RW, "Goal Position", "Goal Position"}},
		{v1::Register::MOVING_SPEED        ,{2, false, A::RW, "Moving Speed", "Moving Speed (Moving Velocity)"}},
		{v1::Register::TORQUE_LIMIT        ,{2, false, A::RW, "Torque Limit", "Torque Limit (Goal Torque)"}},
		{v1::Register::PRESENT_POSITION    ,{2, false, A:: R, "Present Position", "Current Position (Present Velocity)"}},
		{v1::Register::PRESENT_SPEED       ,{2, false, A:: R, "Present Speed", "Current Speed"}},
		{v1::Register::PRESENT_LOAD        ,{2, false, A:: R, "Present Load", "Current Load"}},
		{v1::Register::PRESENT_VOLTAGE     ,{1, false, A:: R, "Present Voltage", "Current Voltage"}},
		{v1::Register::PRESENT_TEMPERATURE ,{1, false, A:: R, "Present Temperature", "Current Temperature"}},
		{v1::Register::REGISTERED          ,{1, false, A:: R, "Registered", "Means if Instruction is registered"}},
		{v1::Register::MOVING              ,{1, false, A:: R, "Moving", "Means if there is any movement"}},
		{v1::Register::LOCK                ,{1, false, A::RW, "Lock", "Locking EEPROM"}},
		{v1::Register::PUNCH               ,{2, false, A::RW, "Punch", "Punch"}},
		{v1::Register::CURRENT             ,{2, false, A::RW, "Current", "Consuming Current"}},
		{v1::Register::TORQUE_CONTROL_MODE ,{1, false, A::RW, "Torque Control Mode Enable", "Torque control mode on/off"}},
		{v1::Register::GOAL_TORQUE         ,{2, false, A::RW, "Goal Torque", "goal torque value"}},
		{v1::Register::GOAL_ACCELERATION   ,{1, false, A::RW, "Goal Acceleration", "Goal Acceleration"}},
	};
	return data;
}

auto getLayoutV2Infos() -> std::map<v2::Register, LayoutField> const& {
	using A = LayoutField::Access;
	static std::map<v2::Register, LayoutField> data {
		{v2::Register::MODEL_NUMBER          , {2, true,  A:: R, "Model Number", "Model Number"}},
		{v2::Register::MODEL_INFORMATION     , {4, true,  A:: R, "Model Information", "Model Information"}},
		{v2::Register::VERSION_FIRMWARE      , {1, true,  A:: R, "Version of Firmware", "Firmware Version"}},
		{v2::Register::ID                    , {1, true,  A::RW, "ID", "Dynamixel ID"}},
		{v2::Register::BAUD_RATE             , {1, true,  A::RW, "Baud Rate", "Communication Baud Rate"}},
		{v2::Register::RETURN_DELAY_TIME     , {1, true,  A::RW, "Return Delay Time", "Response Delay Time"}},
		{v2::Register::DRIVE_MODE            , {1, true,  A::RW, "Drive Mode", "Drive Mode"}},
		{v2::Register::OPERATING_MODE        , {1, true,  A::RW, "Operating Mode", "Operating Mode"}},
		{v2::Register::SECONDARY_ID          , {1, true,  A::RW, "Secondary(Shadow) ID", "Secondary(Shadow) ID"}},
		{v2::Register::PROTOCOL_VERSION      , {1, true,  A::RW, "Protocol Version", "Protocol Version"}},
		{v2::Register::HOMING_OFFSET         , {4, true,  A::RW, "Homing Offset", "Home Position Offset"}},
		{v2::Register::MOVING_THRESHOLD      , {4, true,  A::RW, "Moving Threshold", "Velocity Threshold for Movement Detection"}},
		{v2::Register::TEMPERATURE_LIMIT     , {1, true,  A::RW, "Temperature Limit", "Maximum Internal Temperature Limit"}},
		{v2::Register::MAX_VOLTAGE_LIMIT     , {2, true,  A::RW, "Max Voltage Limit", "Maximum Voltage Limit"}},
		{v2::Register::MIN_VOLTAGE_LIMIT     , {2, true,  A::RW, "Min Voltage Limit", "Minimum Voltage Limit"}},
		{v2::Register::PWM_LIMIT             , {2, true,  A::RW, "PWM Limit", "Maximum PWM Limit"}},
		{v2::Register::CURRENT_LIMIT         , {2, true,  A::RW, "Current Limit", "Maximum Current Limit"}},
		{v2::Register::ACCELERATION_LIMIT    , {4, true,  A::RW, "Acceleration Limit", "Maximum Acceleration Limit"}},
		{v2::Register::VELOCITY_LIMIT        , {4, true,  A::RW, "Velocity Limit", "Maximum Velocity Limit"}},
		{v2::Register::MAX_POSITION_LIMIT    , {4, true,  A::RW, "Max Position Limit", "Maximum Position Limit"}},
		{v2::Register::MIN_POSITION_LIMIT    , {4, true,  A::RW, "Min Position Limit", "Minimum Position Limit"}},
		{v2::Register::SHUTDOWN              , {1, true,  A::RW, "Shutdown", "Shutdown Dynamixel"}},
		{v2::Register::TORQUE_ENABLE         , {1, false, A::RW, "Torque Enable", "Motor Torque On/Off"}},
		{v2::Register::LED                   , {1, false, A::RW, "LED", "Status LED On/Off"}},
		{v2::Register::STATUS_RETURN_LEVEL   , {1, false, A::RW, "Status Return Level", "Select Types of Status Return"}},
		{v2::Register::REGISTERED_INSTRUCTION, {1, false, A:: R, "Registered Instruction", "Check Reception of Instruction"}},
		{v2::Register::HARDWARE_ERROR_STATUS , {1, false, A:: R, "Hardware Error Status", "Hardware Error Status"}},
		{v2::Register::VELOCITY_I_GAIN       , {2, false, A::RW, "Velocity I Gain", "I Gain of Velocity"}},
		{v2::Register::VELOCITY_P_GAIN       , {2, false, A::RW, "Velocity P Gain", "P Gain of Velocity"}},
		{v2::Register::POSITION_D_GAIN       , {2, false, A::RW, "Position D Gain", "D Gain of Position"}},
		{v2::Register::POSITION_I_GAIN       , {2, false, A::RW, "Position I Gain", "I Gain of Position"}},
		{v2::Register::POSITION_P_GAIN       , {2, false, A::RW, "Position P Gain", "P Gain of Position"}},
		{v2::Register::FEEDFORWARD_2ND_GAIN  , {2, false, A::RW, "Feedforward 2nd Gain", "2nd Gain of Feed-Forward"}},
		{v2::Register::FEEDFORWARD_1ST_GAIN  , {2, false, A::RW, "Feedforward 1st Gain", "1st Gain of Feed-Forward"}},
		{v2::Register::BUS_WATCHDOG          , {1, false, A::RW, "Bus Watchdog", "Dynamixel Bus Watchdog"}},
		{v2::Register::GOAL_PWM              , {2, false, A::RW, "Goal PWM", "Target PWM Value"}},
		{v2::Register::GOAL_CURRENT          , {2, false, A::RW, "Goal Current", "Target Current Value"}},
		{v2::Register::GOAL_VELOCITY         , {4, false, A::RW, "Goal Velocity", "Target Velocity Value"}},
		{v2::Register::PROFILE_ACCELERATION  , {4, false, A::RW, "Profile Acceleration", "Acceleration Value of Profile"}},
		{v2::Register::PROFILE_VELOCITY      , {4, false, A::RW, "Profile Velocity", "Velocity Value of Profile"}},
		{v2::Register::GOAL_POSITION         , {4, false, A::RW, "Goal Position", "Target Position Value"}},
		{v2::Register::REALTIME_TICK         , {2, false, A:: R, "Realtime Tick", "Count Time in millisecond"}},
		{v2::Register::MOVING                , {1, false, A:: R, "Moving", "Movement Status"}},
		{v2::Register::MOVING_STATUS         , {1, false, A:: R, "Moving Status", "Detailed Information of Movement Status"}},
		{v2::Register::PRESENT_PWM           , {2, false, A:: R, "Present PWM", "Current PWM Value"}},
		{v2::Register::PRESENT_CURRENT       , {2, false, A:: R, "Present Current", "Current Current Value"}},
		{v2::Register::PRESENT_VELOCITY      , {4, false, A:: R, "Present Velocity", "Current Velocity Value"}},
		{v2::Register::PRESENT_POSITION      , {4, false, A:: R, "Present Position", "Current Position Value"}},
		{v2::Register::VELOCITY_TRAJECTORY   , {4, false, A:: R, "Velocity Trajectory", "Target Velocity Trajectory Generated by Profile"}},
		{v2::Register::POSITION_TRAJECTORY   , {4, false, A:: R, "Position Trajectory", "Target Position Trajectory Generated by Profile"}},
		{v2::Register::PRESENT_INPUT_VOLTAGE , {2, false, A:: R, "Present Input Voltage", "Current Input Voltage"}},
		{v2::Register::PRESENT_TEMPERATURE   , {1, false, A:: R, "Present Temperature", "Current Internal Temperature"}}
	};
	return data;
}


auto getLayoutV1Defaults() -> std::map<uint32_t, std::map<v1::Register, std::optional<uint32_t>>> const& {
	static std::map<uint32_t, std::map<v1::Register, std::optional<uint32_t>>> data {
		{320, { // mx 106
			{v1::Register::MODEL_NUMBER        ,    320},
			{v1::Register::VERSION_FIRMWARE    ,     {}},
			{v1::Register::ID                  ,      1},
			{v1::Register::BAUD_RATE           ,     34},
			{v1::Register::RETURN_DELAY_TIME   ,    250},
			{v1::Register::CW_ANGLE_LIMIT      ,      0},
			{v1::Register::CCW_ANGLE_LIMIT     , 0x0fff},
			{v1::Register::DRIVE_MODE          ,      0},
			{v1::Register::TEMPERATURE_LIMIT   ,     80},
			{v1::Register::VOLTAGE_LIMIT_LOW   ,     60},
			{v1::Register::VOLTAGE_LIMIT_HIGH  ,    160},
			{v1::Register::MAX_TORQUE          , 0x03ff},
			{v1::Register::STATUS_RETURN_LEVEL ,      2},
			{v1::Register::ALARM_LED           ,     36},
			{v1::Register::ALARM_SHUTDOWN      ,     36},
			{v1::Register::MULTI_TURN_OFFSET   ,      0},
			{v1::Register::RESOLUTION_DIVIDER  ,      1},
			{v1::Register::TORQUE_ENABLE       ,      0},
			{v1::Register::LED                 ,      0},
			{v1::Register::D_GAIN              ,      0},
			{v1::Register::I_GAIN              ,      0},
			{v1::Register::P_GAIN              ,     32},
			{v1::Register::GOAL_POSITION       ,     {}},
			{v1::Register::MOVING_SPEED        ,     {}},
			{v1::Register::TORQUE_LIMIT        ,     {}},
			{v1::Register::PRESENT_POSITION    ,     {}},
			{v1::Register::PRESENT_SPEED       ,     {}},
			{v1::Register::PRESENT_LOAD        ,     {}},
			{v1::Register::PRESENT_VOLTAGE     ,     {}},
			{v1::Register::PRESENT_TEMPERATURE ,     {}},
			{v1::Register::REGISTERED          ,      0},
			{v1::Register::MOVING              ,      0},
			{v1::Register::LOCK                ,      0},
			{v1::Register::PUNCH               ,      0},
			{v1::Register::CURRENT             ,      0},
			{v1::Register::TORQUE_CONTROL_MODE ,      0},
			{v1::Register::GOAL_TORQUE         ,      0},
			{v1::Register::GOAL_ACCELERATION   ,      0},
		}}
	};
	static bool firstRun{true};
	if (firstRun) {
		firstRun = false;
		auto& m310 = data[310]; // mx64
		m310 = data.at(320);
		m310[v1::Register::MODEL_NUMBER] = 310;
		m310.erase(v1::Register::DRIVE_MODE);

		auto& m29 = data[29]; // mx28
		m29 = data.at(310);
		m29[v1::Register::MODEL_NUMBER] = 29;
		m29.erase(v1::Register::TORQUE_CONTROL_MODE);
		m29.erase(v1::Register::GOAL_TORQUE);
		m29.erase(v1::Register::DRIVE_MODE);
	}
	return data;
};

auto getLayoutV2Defaults() -> std::map<uint32_t, std::map<v2::Register, std::optional<uint32_t>>> const& {
	static std::map<uint32_t, std::map<v2::Register, std::optional<uint32_t>>> data {
		{1020, { // xm430-w350
			{v2::Register::MODEL_NUMBER           ,  1020},
			{v2::Register::MODEL_INFORMATION      ,    {}},
			{v2::Register::VERSION_FIRMWARE       ,    {}},
			{v2::Register::ID                     ,     1},
			{v2::Register::BAUD_RATE              ,     1},
			{v2::Register::RETURN_DELAY_TIME      ,   250},
			{v2::Register::DRIVE_MODE             ,     0},
			{v2::Register::OPERATING_MODE         ,     3},
			{v2::Register::SECONDARY_ID           ,   255},
			{v2::Register::PROTOCOL_VERSION       ,     2},
			{v2::Register::HOMING_OFFSET          ,     0},
			{v2::Register::MOVING_THRESHOLD       ,    10},
			{v2::Register::TEMPERATURE_LIMIT      ,    80},
			{v2::Register::MAX_VOLTAGE_LIMIT      ,   160},
			{v2::Register::MIN_VOLTAGE_LIMIT      ,    95},
			{v2::Register::PWM_LIMIT              ,   885},
			{v2::Register::CURRENT_LIMIT          ,  1193},
			{v2::Register::ACCELERATION_LIMIT     , 32767},
			{v2::Register::VELOCITY_LIMIT         ,   350},
			{v2::Register::MAX_POSITION_LIMIT     ,  4095},
			{v2::Register::MIN_POSITION_LIMIT     ,     0},
			{v2::Register::SHUTDOWN               ,    52},
			{v2::Register::TORQUE_ENABLE          ,     0},
			{v2::Register::LED                    ,     0},
			{v2::Register::STATUS_RETURN_LEVEL    ,     2},
			{v2::Register::REGISTERED_INSTRUCTION ,     0},
			{v2::Register::HARDWARE_ERROR_STATUS  ,     0},
			{v2::Register::VELOCITY_I_GAIN        ,  1920},
			{v2::Register::VELOCITY_P_GAIN        ,   100},
			{v2::Register::POSITION_D_GAIN        ,     0},
			{v2::Register::POSITION_I_GAIN        ,     0},
			{v2::Register::POSITION_P_GAIN        ,   800},
			{v2::Register::FEEDFORWARD_2ND_GAIN   ,     0},
			{v2::Register::FEEDFORWARD_1ST_GAIN   ,     0},
			{v2::Register::BUS_WATCHDOG           ,     0},
			{v2::Register::GOAL_PWM               ,    {}},
			{v2::Register::GOAL_CURRENT           ,    {}},
			{v2::Register::GOAL_VELOCITY          ,    {}},
			{v2::Register::PROFILE_ACCELERATION   ,     0},
			{v2::Register::PROFILE_VELOCITY       ,     0},
			{v2::Register::GOAL_POSITION          ,    {}},
			{v2::Register::REALTIME_TICK          ,    {}},
			{v2::Register::MOVING                 ,     0},
			{v2::Register::MOVING_STATUS          ,     0},
			{v2::Register::PRESENT_PWM            ,    {}},
			{v2::Register::PRESENT_CURRENT        ,    {}},
			{v2::Register::PRESENT_VELOCITY       ,    {}},
			{v2::Register::PRESENT_POSITION       ,    {}},
			{v2::Register::VELOCITY_TRAJECTORY    ,    {}},
			{v2::Register::POSITION_TRAJECTORY    ,    {}},
			{v2::Register::PRESENT_INPUT_VOLTAGE  ,    {}},
			{v2::Register::PRESENT_TEMPERATURE    ,    {}},
		}}
	};
	static bool firstRun{true};
	if (firstRun) {
		firstRun = false;

		auto& m321 = data[321]; // mx106-v2
		m321 = data.at(1020);
		m321[v2::Register::MODEL_NUMBER] = 321;
		m321[v2::Register::CURRENT_LIMIT] = 2047;
		m321[v2::Register::VELOCITY_LIMIT] = 360;
		m321[v2::Register::POSITION_P_GAIN] = 850;

		auto& m311 = data[311]; // mx64-v2
		m311 = data.at(1020);
		m311[v2::Register::MODEL_NUMBER] = 311;
		m311[v2::Register::CURRENT_LIMIT] = 1941;
		m311[v2::Register::VELOCITY_LIMIT] = 435;
		m311[v2::Register::POSITION_P_GAIN] = 850;

		auto& m30 = data[30]; // mx28-v2
		m30 = data.at(1020);
		m30[v2::Register::MODEL_NUMBER] = 30;
		m30.erase(v2::Register::CURRENT_LIMIT);
		m30[v2::Register::VELOCITY_LIMIT] = 380;
		m30[v2::Register::POSITION_D_GAIN] = 850;

		data[1000] = data[1020]; // XH430
		data[1000][v2::Register::MODEL_NUMBER] = 1000;
	}
	return data;
};

auto getMotorInfos() -> std::vector<MotorInfo> const& {
	static std::vector<MotorInfo> data {
		{  29, LayoutType::V1, "MX28",       {"MX-28T", "MX-28R", "MX-28AT", "MX-28AR"}},
		{ 310, LayoutType::V1, "MX64",       {"MX-64T", "MX-64R", "MX-64AT", "MX-64AR"}},
		{ 320, LayoutType::V1, "MX106",      {"MX-106T", "MX-106R"}},
		{  30, LayoutType::V2, "MX28-V2",    {"MX-28T-V2", "MX-28R-V2", "MX-28AT-V2", "MX-28AR-V2"}},
		{ 311, LayoutType::V2, "MX64-V2",    {"MX-64T-V2", "MX-64R-V2", "MX-64AT-V2", "MX-64AR-V2"}},
		{ 321, LayoutType::V2, "MX106-V2",   {"MX-106T-V2", "MX-106R-V2"}},
		{1020, LayoutType::V2, "XM430-W350", {"XM430-W350-T", "XM430-W350-R"}},
		{1000, LayoutType::V2, "XH430-W350", {"XH430-W350-T", "XH430-W350-R"}},
	};
	return data;
}
auto getMotorInfo(uint16_t _modelNumber) -> MotorInfo const* {
	for (auto const& motorInfo : getMotorInfos()) {
		if (motorInfo.modelNumber == _modelNumber) {
			return &motorInfo;
		}
	}
	return nullptr;

}
auto getMotorInfo(std::string const& _name) -> MotorInfo const* {
	for (auto const& motorInfo : getMotorInfos()) {
		if (motorInfo.shortName == _name) {
			return &motorInfo;
		}
		for (auto const& _other : motorInfo.motorNames) {
			if (_other == _name) {
				return &motorInfo;
			}
		}
	}
	return nullptr;
}
}
