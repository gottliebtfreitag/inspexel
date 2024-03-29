#include "LayoutMX_V2.h"

namespace dynamixel::mx_v2 {

auto MotorLayoutInfo::getInfos() -> meta::Layout<Register> const& {
	using A = meta::LayoutField::Access;
	static auto data = meta::Layout<Register> {
		{Register::MODEL_NUMBER          , {2, true,  A:: R, "Model Number", "Model Number"}},
		{Register::MODEL_INFORMATION     , {4, true,  A:: R, "Model Information", "Model Information"}},
		{Register::VERSION_FIRMWARE      , {1, true,  A:: R, "Version of Firmware", "Firmware Version"}},
		{Register::ID                    , {1, true,  A::RW, "ID", "Dynamixel ID"}},
		{Register::BAUD_RATE             , {1, true,  A::RW, "Baud Rate", "Communication Baud Rate"}},
		{Register::RETURN_DELAY_TIME     , {1, true,  A::RW, "Return Delay Time", "Response Delay Time"}},
		{Register::DRIVE_MODE            , {1, true,  A::RW, "Drive Mode", "Drive Mode"}},
		{Register::OPERATING_MODE        , {1, true,  A::RW, "Operating Mode", "Operating Mode"}},
		{Register::SECONDARY_ID          , {1, true,  A::RW, "Secondary(Shadow) ID", "Secondary(Shadow) ID"}},
		{Register::PROTOCOL_VERSION      , {1, true,  A::RW, "Protocol Version", "Protocol Version"}},
		{Register::HOMING_OFFSET         , {4, true,  A::RW, "Homing Offset", "Home Position Offset"}},
		{Register::MOVING_THRESHOLD      , {4, true,  A::RW, "Moving Threshold", "Velocity Threshold for Movement Detection"}},
		{Register::TEMPERATURE_LIMIT     , {1, true,  A::RW, "Temperature Limit", "Maximum Internal Temperature Limit"}},
		{Register::MAX_VOLTAGE_LIMIT     , {2, true,  A::RW, "Max Voltage Limit", "Maximum Voltage Limit"}},
		{Register::MIN_VOLTAGE_LIMIT     , {2, true,  A::RW, "Min Voltage Limit", "Minimum Voltage Limit"}},
		{Register::PWM_LIMIT             , {2, true,  A::RW, "PWM Limit", "Maximum PWM Limit"}},
		{Register::CURRENT_LIMIT         , {2, true,  A::RW, "Current Limit", "Maximum Current Limit"}},
		{Register::ACCELERATION_LIMIT    , {4, true,  A::RW, "Acceleration Limit", "Maximum Acceleration Limit"}},
		{Register::VELOCITY_LIMIT        , {4, true,  A::RW, "Velocity Limit", "Maximum Velocity Limit"}},
		{Register::MAX_POSITION_LIMIT    , {4, true,  A::RW, "Max Position Limit", "Maximum Position Limit"}},
		{Register::MIN_POSITION_LIMIT    , {4, true,  A::RW, "Min Position Limit", "Minimum Position Limit"}},
		{Register::SHUTDOWN              , {1, true,  A::RW, "Shutdown", "Shutdown Dynamixel"}},
		{Register::TORQUE_ENABLE         , {1, false, A::RW, "Torque Enable", "Motor Torque On/Off"}},
		{Register::LED                   , {1, false, A::RW, "LED", "Status LED On/Off"}},
		{Register::STATUS_RETURN_LEVEL   , {1, false, A::RW, "Status Return Level", "Select Types of Status Return"}},
		{Register::REGISTERED_INSTRUCTION, {1, false, A:: R, "Registered Instruction", "Check Reception of Instruction"}},
		{Register::HARDWARE_ERROR_STATUS , {1, false, A:: R, "Hardware Error Status", "Hardware Error Status"}},
		{Register::VELOCITY_I_GAIN       , {2, false, A::RW, "Velocity I Gain", "I Gain of Velocity"}},
		{Register::VELOCITY_P_GAIN       , {2, false, A::RW, "Velocity P Gain", "P Gain of Velocity"}},
		{Register::POSITION_D_GAIN       , {2, false, A::RW, "Position D Gain", "D Gain of Position"}},
		{Register::POSITION_I_GAIN       , {2, false, A::RW, "Position I Gain", "I Gain of Position"}},
		{Register::POSITION_P_GAIN       , {2, false, A::RW, "Position P Gain", "P Gain of Position"}},
		{Register::FEEDFORWARD_2ND_GAIN  , {2, false, A::RW, "Feedforward 2nd Gain", "2nd Gain of Feed-Forward"}},
		{Register::FEEDFORWARD_1ST_GAIN  , {2, false, A::RW, "Feedforward 1st Gain", "1st Gain of Feed-Forward"}},
		{Register::BUS_WATCHDOG          , {1, false, A::RW, "Bus Watchdog", "Dynamixel Bus Watchdog"}},
		{Register::GOAL_PWM              , {2, false, A::RW, "Goal PWM", "Target PWM Value"}},
		{Register::GOAL_CURRENT          , {2, false, A::RW, "Goal Current", "Target Current Value"}},
		{Register::GOAL_VELOCITY         , {4, false, A::RW, "Goal Velocity", "Target Velocity Value"}},
		{Register::PROFILE_ACCELERATION  , {4, false, A::RW, "Profile Acceleration", "Acceleration Value of Profile"}},
		{Register::PROFILE_VELOCITY      , {4, false, A::RW, "Profile Velocity", "Velocity Value of Profile"}},
		{Register::GOAL_POSITION         , {4, false, A::RW, "Goal Position", "Target Position Value"}},
		{Register::REALTIME_TICK         , {2, false, A:: R, "Realtime Tick", "Count Time in millisecond"}},
		{Register::MOVING                , {1, false, A:: R, "Moving", "Movement Status"}},
		{Register::MOVING_STATUS         , {1, false, A:: R, "Moving Status", "Detailed Information of Movement Status"}},
		{Register::PRESENT_PWM           , {2, false, A:: R, "Present PWM", "Current PWM Value"}},
		{Register::PRESENT_CURRENT       , {2, false, A:: R, "Present Current", "Current Current Value"}},
		{Register::PRESENT_VELOCITY      , {4, false, A:: R, "Present Velocity", "Current Velocity Value"}},
		{Register::PRESENT_POSITION      , {4, false, A:: R, "Present Position", "Current Position Value"}},
		{Register::VELOCITY_TRAJECTORY   , {4, false, A:: R, "Velocity Trajectory", "Target Velocity Trajectory Generated by Profile"}},
		{Register::POSITION_TRAJECTORY   , {4, false, A:: R, "Position Trajectory", "Target Position Trajectory Generated by Profile"}},
		{Register::PRESENT_INPUT_VOLTAGE , {2, false, A:: R, "Present Input Voltage", "Current Input Voltage"}},
		{Register::PRESENT_TEMPERATURE   , {1, false, A:: R, "Present Temperature", "Current Internal Temperature"}},
		{Register::INDIRECT_ADDRESS_BLOCK1, {56, false, A::RW, "Indirect Addresses Block 1", "Indirect Addresses Block 1"}},
		{Register::INDIRECT_DATA_BLOCK1,    {28, false, A::RW, "Indirect Data Block 1", "Indirect Data Block 1"}},
		{Register::INDIRECT_ADDRESS_BLOCK2, {56, false, A::RW, "Indirect Addresses Block 2", "Indirect Addresses Block 2"}},
		{Register::INDIRECT_DATA_BLOCK2,    {28, false, A::RW, "Indirect Data Block 2", "Indirect Data Block 2"}},
	};
	return data;
}

auto MotorLayoutInfo::getDefaults() -> std::map<uint32_t, meta::Info<Register>> const& {
	static auto data = []() {
		auto convertPosition    = meta::buildConverter("r", (2.*M_PI)/4095, 2048);
		auto convertSpeed       = meta::buildConverter("r/s", (235.18/60*2.*M_PI)/1023.);
		auto convertTemperature = meta::buildConverter("C", 1.);
		auto convertVoltage     = meta::buildConverter("V", 16./160);
		auto convertPID_P       = meta::buildConverter("", 1./128., 0, 0, 16383);
		auto convertPID_I       = meta::buildConverter("", 1./65'536., 0, 0, 16383);
		auto convertPID_D       = meta::buildConverter("", 1/16., 0, 0, 16383);
		auto convertCurrent     = meta::buildConverter("A", 3.36/1000., 0.);

		auto data = std::map<uint32_t, meta::Info<Register>> {
			{1020, {
				1020,
				LayoutType::MX_V2,
				"XM430-W350",
				{"XM430-W350-T", "XM430-W350-R"}, {
					{Register::MODEL_NUMBER           , {         1020, {}}},
					{Register::MODEL_INFORMATION      , { std::nullopt, {}}},
					{Register::VERSION_FIRMWARE       , { std::nullopt, {}}},
					{Register::ID                     , {            1, {}}},
					{Register::BAUD_RATE              , {            1, {}}},
					{Register::RETURN_DELAY_TIME      , {          250, {}}},
					{Register::DRIVE_MODE             , {            0, {}}},
					{Register::OPERATING_MODE         , {            3, {}}},
					{Register::SECONDARY_ID           , {          255, {}}},
					{Register::PROTOCOL_VERSION       , {            2, {}}},
					{Register::HOMING_OFFSET          , {            0, {}}},
					{Register::MOVING_THRESHOLD       , {           10, {}}},
					{Register::TEMPERATURE_LIMIT      , {           80, convertTemperature}},
					{Register::MAX_VOLTAGE_LIMIT      , {          160, convertVoltage}},
					{Register::MIN_VOLTAGE_LIMIT      , {           95, convertVoltage}},
					{Register::PWM_LIMIT              , {          885, {}}},
					{Register::CURRENT_LIMIT          , {         1193, convertCurrent}},
					{Register::ACCELERATION_LIMIT     , {        32767, {}}},
					{Register::VELOCITY_LIMIT         , {          200, convertSpeed}},
					{Register::MAX_POSITION_LIMIT     , {         4095, convertPosition}},
					{Register::MIN_POSITION_LIMIT     , {            0, convertPosition}},
					{Register::SHUTDOWN               , {           52, {}}},
					{Register::TORQUE_ENABLE          , {            0, {}}},
					{Register::LED                    , {            0, {}}},
					{Register::STATUS_RETURN_LEVEL    , {            2, {}}},
					{Register::REGISTERED_INSTRUCTION , {            0, {}}},
					{Register::HARDWARE_ERROR_STATUS  , {            0, {}}},
					{Register::VELOCITY_I_GAIN        , {         1920, {}}},
					{Register::VELOCITY_P_GAIN        , {          100, {}}},
					{Register::POSITION_D_GAIN        , {            0, convertPID_D}},
					{Register::POSITION_I_GAIN        , {            0, convertPID_I}},
					{Register::POSITION_P_GAIN        , {          800, convertPID_P}},
					{Register::FEEDFORWARD_2ND_GAIN   , {            0, {}}},
					{Register::FEEDFORWARD_1ST_GAIN   , {            0, {}}},
					{Register::BUS_WATCHDOG           , {            0, {}}},
					{Register::GOAL_PWM               , { std::nullopt, {}}},
					{Register::GOAL_CURRENT           , { std::nullopt, convertCurrent}},
					{Register::GOAL_VELOCITY          , { std::nullopt, convertSpeed}},
					{Register::PROFILE_ACCELERATION   , {            0, {}}},
					{Register::PROFILE_VELOCITY       , {            0, convertSpeed}},
					{Register::GOAL_POSITION          , { std::nullopt, convertPosition}},
					{Register::REALTIME_TICK          , { std::nullopt, {}}},
					{Register::MOVING                 , {            0, {}}},
					{Register::MOVING_STATUS          , {            0, {}}},
					{Register::PRESENT_PWM            , { std::nullopt, {}}},
					{Register::PRESENT_CURRENT        , { std::nullopt, convertCurrent}},
					{Register::PRESENT_VELOCITY       , { std::nullopt, convertSpeed}},
					{Register::PRESENT_POSITION       , { std::nullopt, convertPosition}},
					{Register::VELOCITY_TRAJECTORY    , { std::nullopt, convertSpeed}},
					{Register::POSITION_TRAJECTORY    , { std::nullopt, convertPosition}},
					{Register::PRESENT_INPUT_VOLTAGE  , { std::nullopt, convertVoltage}},
					{Register::PRESENT_TEMPERATURE    , { std::nullopt, convertTemperature}},
				}
			}}
		};

		auto newMotor = [&](int number, std::string shortName, std::vector<std::string> names) -> meta::Info<Register>& {
			auto& m = data[number];
			m = data.at(1020);
			m.modelNumber = number;
			m.shortName = std::move(shortName);
			m.motorNames = std::move(names);
			std::get<0>(m.defaultLayout[Register::MODEL_NUMBER]) = number;
			return m;
		};

		{
			auto& m = newMotor(321, "MX106-V2", {"MX-106T-V2", "MX-106R-V2"});
			std::get<0>(m.defaultLayout[Register::CURRENT_LIMIT])      = 2047;
			std::get<0>(m.defaultLayout[Register::VELOCITY_LIMIT])     = 360;
			std::get<0>(m.defaultLayout[Register::POSITION_P_GAIN])    = 850;
			std::get<0>(m.defaultLayout[Register::ACCELERATION_LIMIT]) = 32767;
		}

		{
			auto& m = newMotor(311, "MX64-V2", {"MX-64T-V2", "MX-64R-V2", "MX-64AT-V2", "MX-64AR-V2"});
			std::get<0>(m.defaultLayout[Register::CURRENT_LIMIT])      = 1941;
			std::get<0>(m.defaultLayout[Register::VELOCITY_LIMIT])     = 435;
			std::get<0>(m.defaultLayout[Register::POSITION_P_GAIN])    = 850;
			std::get<0>(m.defaultLayout[Register::ACCELERATION_LIMIT]) = 32767;
		}

		{
			auto& m = newMotor(30, "MX28-V2", {"MX-28T-V2", "MX-28R-V2", "MX-28AT-V2", "MX-28AR-V2"});
			m.defaultLayout.erase(Register::CURRENT_LIMIT);

			std::get<0>(m.defaultLayout[Register::CURRENT_LIMIT]) = 1941;
			std::get<0>(m.defaultLayout[Register::VELOCITY_LIMIT]) = 380;
			std::get<0>(m.defaultLayout[Register::POSITION_P_GAIN]) = 850;
			std::get<0>(m.defaultLayout[Register::ACCELERATION_LIMIT]) = 32767;
		}
		{
			auto& m = newMotor( 1'000, "XH430-W350", {"XH430-W350-T", "XH430-W350-R"});
			m.defaultLayout.erase(Register::ACCELERATION_LIMIT);
			std::get<0>(m.defaultLayout[Register::CURRENT_LIMIT]) = 689;
			std::get<0>(m.defaultLayout[Register::VELOCITY_LIMIT]) = 280;
			std::get<0>(m.defaultLayout[Register::POSITION_P_GAIN]) = 900;
		}
		{
			auto& m = newMotor( 1'010, "XH430-W210", {"XH430-W210-T", "XH430-W210-R"});
			m.defaultLayout.erase(Register::ACCELERATION_LIMIT);
			std::get<0>(m.defaultLayout[Register::CURRENT_LIMIT]) = 689;
			std::get<0>(m.defaultLayout[Register::CURRENT_LIMIT]) = 689;
			std::get<0>(m.defaultLayout[Register::VELOCITY_LIMIT]) = 210;
			std::get<0>(m.defaultLayout[Register::POSITION_P_GAIN]) = 900;
		}
		{
			auto& m = newMotor( 1'030, "XM430-W210", {"XM430-W210-T", "XM430-W210-R"});
			m.defaultLayout.erase(Register::ACCELERATION_LIMIT);
			std::get<0>(m.defaultLayout[Register::VELOCITY_LIMIT]) = 300;
		}
		{
			auto& m = newMotor( 1'040, "XH430-V350", {"XH430-V350"});
			m.defaultLayout.erase(Register::ACCELERATION_LIMIT);
			std::get<0>(m.defaultLayout[Register::CURRENT_LIMIT]) = 689;
			std::get<0>(m.defaultLayout[Register::VELOCITY_LIMIT]) = 135;
		}
		{
			auto& m = newMotor(1050, "XH430-V210", {"XH430-V210"});
			m.defaultLayout.erase(Register::ACCELERATION_LIMIT);
			std::get<0>(m.defaultLayout[Register::MAX_VOLTAGE_LIMIT]) = 300;
			std::get<0>(m.defaultLayout[Register::MIN_VOLTAGE_LIMIT]) = 110;
			std::get<0>(m.defaultLayout[Register::VELOCITY_LIMIT]) = 230;
		}
		{
			auto& m = newMotor( 1'060, "XL430-W250", {"XL430-W250"});
			m.defaultLayout.erase(Register::CURRENT_LIMIT);
			m.defaultLayout.erase(Register::ACCELERATION_LIMIT);

			std::get<0>(m.defaultLayout[Register::TEMPERATURE_LIMIT]) = 72;
			std::get<0>(m.defaultLayout[Register::MAX_VOLTAGE_LIMIT]) = 140;
			std::get<0>(m.defaultLayout[Register::MIN_VOLTAGE_LIMIT]) =  60;
			std::get<0>(m.defaultLayout[Register::VELOCITY_LIMIT]) =  265;

			std::get<0>(m.defaultLayout[Register::VELOCITY_I_GAIN]) =  1000;
			std::get<0>(m.defaultLayout[Register::VELOCITY_P_GAIN]) =  100;
			std::get<0>(m.defaultLayout[Register::POSITION_D_GAIN]) =  4000;
			std::get<0>(m.defaultLayout[Register::POSITION_P_GAIN]) =  640;
		}
		{
			auto& m = newMotor( 1'120, "XM540-W150", {"XM540-W150-T", "XM540-W150-R"});
			m.defaultLayout.erase(Register::ACCELERATION_LIMIT);

			std::get<0>(m.defaultLayout[Register::CURRENT_LIMIT]) = 2047;
			std::get<0>(m.defaultLayout[Register::VELOCITY_LIMIT]) = 230;
			std::get<0>(m.defaultLayout[Register::EXTERNAL_PORT_MODE_1]) = 1;
			std::get<0>(m.defaultLayout[Register::EXTERNAL_PORT_MODE_2]) = 1;
			std::get<0>(m.defaultLayout[Register::EXTERNAL_PORT_MODE_3]) = 1;
		}
		{
			auto& m = newMotor( 1'130, "XM540-W270", {"XM540-W270-T", "XM540-W270-R"});
			m.defaultLayout.erase(Register::ACCELERATION_LIMIT);

			std::get<0>(m.defaultLayout[Register::CURRENT_LIMIT]) = 2047;
			std::get<0>(m.defaultLayout[Register::VELOCITY_LIMIT]) = 128;
			std::get<0>(m.defaultLayout[Register::EXTERNAL_PORT_MODE_1]) = 1;
			std::get<0>(m.defaultLayout[Register::EXTERNAL_PORT_MODE_2]) = 1;
			std::get<0>(m.defaultLayout[Register::EXTERNAL_PORT_MODE_3]) = 1;
		}
		return data;
	}();
	return data;
};

}
