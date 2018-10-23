#include "LayoutAX.h"

namespace dynamixel::ax {

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
		{Register::TEMPERATURE_LIMIT    , {1,  true, A::RW, "Highest Limit Temperature", "Internal Limit Temperature"}},
		{Register::MIN_VOLTAGE_LIMIT    , {1,  true, A::RW, "Min Limit Voltage", "Min Limit Voltage"}},
		{Register::MAX_VOLTAGE_LIMIT    , {1,  true, A::RW, "Max Limit Voltage", "Max Limit Voltage"}},
		{Register::MAX_TORQUE           , {2,  true, A::RW, "Max Torque", "Max. Torque"}},
		{Register::STATUS_RETURN_LEVEL  , {1,  true, A::RW, "Status Return Level", "Status Return Level"}},
		{Register::ALARM_LED            , {1,  true, A::RW, "Alarm LED", "LED for Alarm"}},
		{Register::SHUTDOWN             , {1,  true, A::RW, "Shutdown", "Shutdown for Alarm"}},
		{Register::TORQUE_ENABLE        , {1, false, A::RW, "Torque Enable", "Torque On/Off"}},
		{Register::LED                  , {1, false, A::RW, "LED", "LED On/Off"}},
		{Register::CW_COMPLIANCE_MARGIN , {1, false, A::RW, "CW Compliance Margin", "CW Compliance Margin"}},
		{Register::CCW_COMPLIANCE_MARGIN, {1, false, A::RW, "CCW Compliance Margin", "CCW Compliance Margin"}},
		{Register::CW_COMPLIANCE_SLOPE  , {1, false, A::RW, "CW Compliance Slope", "CW Compliance Slope"}},
		{Register::CCW_COMPLIANCE_SLOPE , {1, false, A::RW, "CCW Compliance Slope", "CCW Compliance Slope"}},
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
		{Register::LOCK                 , {1, false, A::RW, "Lock", "Locking EEPROM"}},
		{Register::PUNCH                , {2, false, A::RW, "Punch", "Punch"}},
	};
	return data;
}


auto MotorLayoutInfo::getDefaults() -> std::map<uint32_t, meta::Info<Register>> const& {
	 static auto data = []() {
		auto convertPosition    = meta::buildConverter("r", (2.*M_PI)/1023.*300./360., 512);
		auto convertSpeed       = meta::buildConverter("r/s", 0.111/60*2.*M_PI);
		auto convertTemperature = meta::buildConverter("C", 1.);
		auto convertVoltage     = meta::buildConverter("V", 16./160);
		auto convertTorque      = meta::buildConverter("%", 100./1023., 0);

		auto data = std::map<uint32_t, meta::Info<Register>> {
			{300, {
				300,
				LayoutType::AX,
				"AX-12W",
				{"AX-12W"}, {
					{Register::MODEL_NUMBER         , {   300, {}}},
					{Register::FIRMWARE_VERSION     , {    {}, {}}},
					{Register::ID                   , {     1, {}}},
					{Register::BAUD_RATE            , {     1, {}}},
					{Register::RETURN_DELAY_TIME    , {   250, {}}},
					{Register::CW_ANGLE_LIMIT       , {     0, convertPosition}},
					{Register::CCW_ANGLE_LIMIT      , {0x03ff, convertPosition}},
					{Register::TEMPERATURE_LIMIT    , {    70, convertTemperature}},
					{Register::MIN_VOLTAGE_LIMIT    , {    60, convertVoltage}},
					{Register::MAX_VOLTAGE_LIMIT    , {   140, convertVoltage}},
					{Register::MAX_TORQUE           , {0x03ff, convertTorque}},
					{Register::STATUS_RETURN_LEVEL  , {     2, {}}},
					{Register::ALARM_LED            , {    36, {}}},
					{Register::SHUTDOWN             , {    36, {}}},
					{Register::TORQUE_ENABLE        , {     0, {}}},
					{Register::LED                  , {     0, {}}},
					{Register::CW_COMPLIANCE_MARGIN , {     4, {}}},
					{Register::CCW_COMPLIANCE_MARGIN, {     4, {}}},
					{Register::CW_COMPLIANCE_SLOPE  , {    64, {}}},
					{Register::CCW_COMPLIANCE_SLOPE , {    64, {}}},
					{Register::GOAL_POSITION        , {    {}, convertPosition}},
					{Register::MOVING_SPEED         , {    {}, convertSpeed}},
					{Register::TORQUE_LIMIT         , {    {}, convertTorque}},
					{Register::PRESENT_POSITION     , {    {}, convertPosition}},
					{Register::PRESENT_SPEED        , {    {}, convertSpeed}},
					{Register::PRESENT_LOAD         , {    {}, {}}},
					{Register::PRESENT_VOLTAGE      , {    {}, convertVoltage}},
					{Register::PRESENT_TEMPERATURE  , {    {}, convertTemperature}},
					{Register::REGISTERED           , {     0, {}}},
					{Register::MOVING               , {     0, {}}},
					{Register::LOCK                 , {     0, {}}},
					{Register::PUNCH                , {    32, {}}},
				}
			}}
		};

		auto newMotor = [&](int number, std::string shortName, std::vector<std::string> names) -> meta::Info<Register>& {
			auto& m = data[number];
			m = data.at(300);
			m.modelNumber = number;
			m.shortName = std::move(shortName);
			m.motorNames = std::move(names);
			std::get<0>(m.defaultLayout[Register::MODEL_NUMBER]) = number;
			return m;
		};

		{
			auto& m = newMotor(12, "AX-12A", {"AX-12A"});
			std::get<0>(m.defaultLayout[Register::CW_COMPLIANCE_MARGIN])  = 1;
			std::get<0>(m.defaultLayout[Register::CCW_COMPLIANCE_MARGIN]) = 1;
			std::get<0>(m.defaultLayout[Register::CW_COMPLIANCE_SLOPE])   = 32;
			std::get<0>(m.defaultLayout[Register::CCW_COMPLIANCE_SLOPE])  = 32;
		}

		{
			auto& m = newMotor(18, "AX-18A", {"AX-18A"});
			std::get<0>(m.defaultLayout[Register::TEMPERATURE_LIMIT])     = 75;
			std::get<0>(m.defaultLayout[Register::MAX_TORQUE])            = 983;
			std::get<0>(m.defaultLayout[Register::CW_COMPLIANCE_MARGIN])  = 1;
			std::get<0>(m.defaultLayout[Register::CCW_COMPLIANCE_MARGIN]) = 1;
			std::get<0>(m.defaultLayout[Register::CW_COMPLIANCE_SLOPE])   = 32;
			std::get<0>(m.defaultLayout[Register::CCW_COMPLIANCE_SLOPE])  = 32;
		}
		return data;
	}();
	return data;
};

}
