#include "MotorMetaInfo.h"

#include <cmath>


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
		{v1::Register::REALTIME_TICK       ,{2, false, A:: R, "Realtime Tick", "Realtime Tick"}},
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
		{v2::Register::PRESENT_TEMPERATURE   , {1, false, A:: R, "Present Temperature", "Current Internal Temperature"}},
		{v2::Register::INDIRECT_ADDRESS_BLOCK1, {56, false, A::RW, "Indirect Addresses Block 1", "Indirect Addresses Block 1"}},
		{v2::Register::INDIRECT_DATA_BLOCK1,    {28, false, A::RW, "Indirect Data Block 1", "Indirect Data Block 1"}},
		{v2::Register::INDIRECT_ADDRESS_BLOCK2, {56, false, A::RW, "Indirect Addresses Block 2", "Indirect Addresses Block 2"}},
		{v2::Register::INDIRECT_DATA_BLOCK2,    {28, false, A::RW, "Indirect Data Block 2", "Indirect Data Block 2"}},
	};
	return data;
}

auto getLayoutProInfos() -> std::map<pro::Register, LayoutField> const& {
	using A = LayoutField::Access;
	static std::map<pro::Register, LayoutField> data {
		{pro::Register::MODEL_NUMBER           , {2, true,  A:: R, "Model Number"           , "Model Number"           }},
		{pro::Register::MODEL_INFORMATION      , {4, true,  A:: R, "Model Information"      , "Model Information"      }},
		{pro::Register::FIRMWARE_VERSION       , {1, true,  A:: R, "Firmware Version"       , "Firmware Version"       }},
		{pro::Register::ID                     , {1, true,  A::RW, "ID"                     , "ID"                     }},
		{pro::Register::BAUD_RATE              , {1, true,  A::RW, "Baud Rate"              , "Baud Rate"              }},
		{pro::Register::RETURN_DELAY_TIME      , {1, true,  A::RW, "Return Delay Time"      , "Return Delay Time"      }},
		{pro::Register::OPERATING_MODE         , {1, true,  A::RW, "Operating Mode"         , "Operating Mode"         }},
		{pro::Register::HOMING_OFFSET          , {4, true,  A::RW, "Homing Offset"          , "Homing Offset"          }},
		{pro::Register::MOVING_THRESHOLD       , {4, true,  A::RW, "Moving Threshold"       , "Moving Threshold"       }},
		{pro::Register::TEMPERATURE_LIMIT      , {1, true,  A::RW, "Temperature Limit"      , "Temperature Limit"      }},
		{pro::Register::MAX_VOLTAGE_LIMIT      , {2, true,  A::RW, "Max Voltage Limit"      , "Max Voltage Limit"      }},
		{pro::Register::MIN_VOLTAGE_LIMIT      , {2, true,  A::RW, "Min Voltage Limit"      , "Min Voltage Limit"      }},
		{pro::Register::ACCELERATION_LIMIT     , {4, true,  A::RW, "Acceleration Limit"     , "Acceleration Limit"     }},
		{pro::Register::TORQUE_LIMIT           , {2, true,  A::RW, "Torque Limit"           , "Torque Limit"           }},
		{pro::Register::VELOCITY_LIMIT         , {4, true,  A::RW, "Velocity Limit"         , "Velocity Limit"         }},
		{pro::Register::MAX_POSITION_LIMIT     , {4, true,  A::RW, "Max Position Limit"     , "Max Position Limit"     }},
		{pro::Register::MIN_POSITION_LIMIT     , {4, true,  A::RW, "Min Position Limit"     , "Min Position Limit"     }},
		{pro::Register::EXTERNAL_PORT_MODE_1   , {1, true,  A::RW, "External Port Mode 1"   , "External Port Mode 1"   }},
		{pro::Register::EXTERNAL_PORT_MODE_2   , {1, true,  A::RW, "External Port Mode 2"   , "External Port Mode 2"   }},
		{pro::Register::EXTERNAL_PORT_MODE_3   , {1, true,  A::RW, "External Port Mode 3"   , "External Port Mode 3"   }},
		{pro::Register::EXTERNAL_PORT_MODE_4   , {1, true,  A::RW, "External Port Mode 4"   , "External Port Mode 4"   }},
		{pro::Register::SHUTDOWN               , {1, true,  A::RW, "Shutdown"               , "Shutdown"               }},
		{pro::Register::INDIRECT_ADDRESS_BLOCK , {512, true, A::RW, "Indirect Addresses Block", "Indirect Addresses Block"}},
		{pro::Register::TORQUE_ENABLE          , {1, false, A::RW, "Torque Enable"          , "Torque Enable"          }},
		{pro::Register::LED_RED                , {1, false, A::RW, "LED Red"                , "LED Red"                }},
		{pro::Register::LED_GREEN              , {1, false, A::RW, "LED Green"              , "LED Green"              }},
		{pro::Register::LED_BLUE               , {1, false, A::RW, "LED Blue"               , "LED Blue"               }},
		{pro::Register::VELOCITY_I_GAIN        , {2, false, A::RW, "Velocity I Gain"        , "Velocity I Gain"        }},
		{pro::Register::VELOCITY_P_GAIN        , {2, false, A::RW, "Velocity P Gain"        , "Velocity P Gain"        }},
		{pro::Register::POSITION_P_GAIN        , {2, false, A::RW, "Position P Gain"        , "Position P Gain"        }},
		{pro::Register::GOAL_POSITION          , {4, false, A::RW, "Goal Position"          , "Goal Position"          }},
		{pro::Register::GOAL_VELOCITY          , {4, false, A::RW, "Goal Velocity"          , "Goal Velocity"          }},
		{pro::Register::GOAL_TORQUE            , {2, false, A::RW, "Goal Torque"            , "Goal Torque"            }},
		{pro::Register::GOAL_ACCELERATION      , {4, false, A::RW, "Goal Acceleration"      , "Goal Acceleration"      }},
		{pro::Register::MOVING                 , {1, false, A:: R, "Moving"                 , "Moving"                 }},
		{pro::Register::PRESENT_POSITION       , {4, false, A:: R, "Present Position"       , "Present Position"       }},
		{pro::Register::PRESENT_VELOCITY       , {4, false, A:: R, "Present Velocity"       , "Present Velocity"       }},
		{pro::Register::PRESENT_CURRENT        , {2, false, A:: R, "Present Current"        , "Present Current"        }},
		{pro::Register::PRESENT_INPUT_VOLTAGE  , {2, false, A:: R, "Present Input_voltage"  , "Present Input_voltage"  }},
		{pro::Register::PRESENT_TEMPERATURE    , {1, false, A:: R, "Present Temperature"    , "Present Temperature"    }},
		{pro::Register::EXTERNAL_PORT_DATA_1   , {2, false, A::RW, "External Port Data 1"   , "External Port Data 1"   }},
		{pro::Register::EXTERNAL_PORT_DATA_2   , {2, false, A::RW, "External Port Data 2"   , "External Port Data 2"   }},
		{pro::Register::EXTERNAL_PORT_DATA_3   , {2, false, A::RW, "External Port Data 3"   , "External Port Data 3"   }},
		{pro::Register::EXTERNAL_PORT_DATA_4   , {2, false, A::RW, "External Port Data 4"   , "External Port Data 4"   }},
		{pro::Register::REGISTERED_INSTRUCTION , {1, false, A:: R, "Registered Instruction" , "Registered Instruction" }},
		{pro::Register::STATUS_RETURN_LEVEL    , {1, false, A::RW, "Status Return Level"    , "Status Return Level"    }},
		{pro::Register::HARDWARE_ERROR_STATUS  , {1, false, A:: R, "Hardware Error Status"  , "Hardware Error Status"  }},
		{pro::Register::INDIRECT_DATA_BLOCK ,    {256, false, A::RW, "Indirect Data Block", "Indirect Data Block"}},
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
			{v1::Register::REALTIME_TICK       ,      0},
			{v1::Register::CURRENT             ,      0},
			{v1::Register::TORQUE_CONTROL_MODE ,      0},
			{v1::Register::GOAL_TORQUE         ,      0},
			{v1::Register::GOAL_ACCELERATION   ,      0},
		}}
	};
	static bool firstRun{true};
	if (firstRun) {
		firstRun = false;

		{ // mx64
			auto& m = data[310];
			m = data.at(320);
			m[v1::Register::MODEL_NUMBER] = 310;
			m.erase(v1::Register::DRIVE_MODE);
		}

		{ // mx28
			auto& m = data[29];
			m = data.at(320);
			m[v1::Register::MODEL_NUMBER] = 29;
			m.erase(v1::Register::DRIVE_MODE);
			m.erase(v1::Register::TORQUE_CONTROL_MODE);
			m.erase(v1::Register::CURRENT);
			m.erase(v1::Register::GOAL_TORQUE);
		}
		{ // mx12
			auto& m = data[360];
			m = data.at(320);
			m[v1::Register::MODEL_NUMBER] = 360;
			m[v1::Register::BAUD_RATE]    = 1;
			m[v1::Register::D_GAIN]       = 8;
			m[v1::Register::P_GAIN]       = 8;
			m[v1::Register::PUNCH]        = 32;
			m.erase(v1::Register::DRIVE_MODE);
			m.erase(v1::Register::CURRENT);
			m.erase(v1::Register::TORQUE_CONTROL_MODE);
			m.erase(v1::Register::GOAL_TORQUE);
		}
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
			{v2::Register::VELOCITY_LIMIT         ,   200},
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

		{ // mx 106-v2
			auto& m = data[321];
			m = data.at(1020);
			m[v2::Register::MODEL_NUMBER] = 321;
			m[v2::Register::CURRENT_LIMIT] = 2047;
			m[v2::Register::VELOCITY_LIMIT] = 360;
			m[v2::Register::POSITION_P_GAIN] = 850;
			m[v2::Register::ACCELERATION_LIMIT] = 32767;
		}

		{ // mx64-v2
			auto& m = data[311];
			m = data.at(1020);
			m[v2::Register::MODEL_NUMBER] = 311;
			m[v2::Register::CURRENT_LIMIT] = 1941;
			m[v2::Register::VELOCITY_LIMIT] = 435;
			m[v2::Register::POSITION_P_GAIN] = 850;
			m[v2::Register::ACCELERATION_LIMIT] = 32767;
		}

		{ // mx28-v2
			auto& m = data[30];
			m = data.at(1020);
			m[v2::Register::MODEL_NUMBER] = 30;
			m.erase(v2::Register::CURRENT_LIMIT);
			m[v2::Register::VELOCITY_LIMIT] = 380;
			m[v2::Register::POSITION_P_GAIN] = 850;
			m[v2::Register::ACCELERATION_LIMIT] = 32767;
		}

		{ // xh430-w350
			auto& m = data[1000];
			m = data.at(1020);
			m[v2::Register::MODEL_NUMBER] = 1000;
			m[v2::Register::CURRENT_LIMIT] = 689;
			m[v2::Register::VELOCITY_LIMIT] = 280;
			m[v2::Register::POSITION_P_GAIN] = 900;
			m.erase(v2::Register::ACCELERATION_LIMIT);
		}

		{ // xh430-w210
			auto& m = data[1010];
			m = data.at(1020);
			m[v2::Register::MODEL_NUMBER] = 1010;
			m[v2::Register::CURRENT_LIMIT] = 689;
			m[v2::Register::VELOCITY_LIMIT] = 210;
			m[v2::Register::POSITION_P_GAIN] = 900;
			m.erase(v2::Register::ACCELERATION_LIMIT);
		}

		{ // xm430-w210
			auto& m = data[1030];
			m = data.at(1020);
			m[v2::Register::MODEL_NUMBER] = 1030;
			m[v2::Register::VELOCITY_LIMIT] = 300;
			m.erase(v2::Register::ACCELERATION_LIMIT);
		}


		{ // xh430-v350
			auto& m = data[1040];
			m = data.at(1020);
			m[v2::Register::MODEL_NUMBER] = 1040;
			m[v2::Register::CURRENT_LIMIT] = 689;
			m[v2::Register::VELOCITY_LIMIT] = 135;
			m.erase(v2::Register::ACCELERATION_LIMIT);
		}

		{ // xh430-v210
			auto& m = data[1050];
			m = data.at(1020);
			m[v2::Register::MODEL_NUMBER] = 1050;
			m[v2::Register::MAX_VOLTAGE_LIMIT] = 300;
			m[v2::Register::MIN_VOLTAGE_LIMIT] = 110;
			m[v2::Register::VELOCITY_LIMIT] = 230;
			m.erase(v2::Register::ACCELERATION_LIMIT);
		}

		{ // xm540-w150
			auto& m = data[1120];
			m = data.at(1020);
			m[v2::Register::MODEL_NUMBER] = 1120;
			m[v2::Register::CURRENT_LIMIT] = 2047;
			m[v2::Register::VELOCITY_LIMIT] = 230;
			m[v2::Register::EXTERNAL_PORT_MODE_1] = 1;
			m[v2::Register::EXTERNAL_PORT_MODE_2] = 1;
			m[v2::Register::EXTERNAL_PORT_MODE_3] = 1;
			m.erase(v2::Register::ACCELERATION_LIMIT);
		}

		{ // xm540-w270
			auto& m = data[1130];
			m = data.at(1020);
			m[v2::Register::MODEL_NUMBER] = 1130;
			m[v2::Register::CURRENT_LIMIT] = 2047;
			m[v2::Register::VELOCITY_LIMIT] = 128;
			m[v2::Register::EXTERNAL_PORT_MODE_1] = 1;
			m[v2::Register::EXTERNAL_PORT_MODE_2] = 1;
			m[v2::Register::EXTERNAL_PORT_MODE_3] = 1;
			m.erase(v2::Register::ACCELERATION_LIMIT);
		}

		{ // xl430-w250
			auto& m = data[1060];
			m = data.at(1020);
			m[v2::Register::MODEL_NUMBER] = 1060;
			m[v2::Register::TEMPERATURE_LIMIT] = 72;
			m[v2::Register::MAX_VOLTAGE_LIMIT] = 140;
			m[v2::Register::MIN_VOLTAGE_LIMIT] =  60;
			m[v2::Register::VELOCITY_LIMIT] =  265;

			m[v2::Register::VELOCITY_I_GAIN] =  1000;
			m[v2::Register::VELOCITY_P_GAIN] =  100;
			m[v2::Register::POSITION_D_GAIN] =  4000;
			m[v2::Register::POSITION_P_GAIN] =  640;

			m.erase(v2::Register::CURRENT_LIMIT);
			m.erase(v2::Register::ACCELERATION_LIMIT);

		}




	}
	return data;
};

auto getLayoutProDefaults() -> std::map<uint32_t, std::map<pro::Register, std::optional<uint32_t>>> const& {
	static std::map<uint32_t, std::map<pro::Register, std::optional<uint32_t>>> data {
		{46352, { // m54-60-s250-r
			{pro::Register::MODEL_NUMBER           ,   46'352},
			{pro::Register::MODEL_INFORMATION      ,       {}},
			{pro::Register::FIRMWARE_VERSION       ,       {}},
			{pro::Register::ID                     ,        1},
			{pro::Register::BAUD_RATE              ,        1},
			{pro::Register::RETURN_DELAY_TIME      ,      250},
			{pro::Register::OPERATING_MODE         ,        3},
			{pro::Register::HOMING_OFFSET          ,        0},
			{pro::Register::MOVING_THRESHOLD       ,       50},
			{pro::Register::TEMPERATURE_LIMIT      ,       80},
			{pro::Register::MAX_VOLTAGE_LIMIT      ,      400},
			{pro::Register::MIN_VOLTAGE_LIMIT      ,      150},
			{pro::Register::ACCELERATION_LIMIT     ,        {}},
			{pro::Register::TORQUE_LIMIT           ,       180},
			{pro::Register::VELOCITY_LIMIT         ,     8'000},
			{pro::Register::MAX_POSITION_LIMIT     ,   125'708},
			{pro::Register::MIN_POSITION_LIMIT     ,  -125'708},
			{pro::Register::EXTERNAL_PORT_MODE_1   ,         0},
			{pro::Register::EXTERNAL_PORT_MODE_2   ,         0},
			{pro::Register::EXTERNAL_PORT_MODE_3   ,         0},
			{pro::Register::EXTERNAL_PORT_MODE_4   ,         0},
			{pro::Register::SHUTDOWN               ,        58},
			{pro::Register::TORQUE_ENABLE          ,         0},
			{pro::Register::LED_RED                ,         0},
			{pro::Register::LED_GREEN              ,         0},
			{pro::Register::LED_BLUE               ,         0},
			{pro::Register::VELOCITY_I_GAIN        ,        16},
			{pro::Register::VELOCITY_P_GAIN        ,       256},
			{pro::Register::POSITION_P_GAIN        ,        32},
			{pro::Register::GOAL_POSITION          ,        {}},
			{pro::Register::GOAL_VELOCITY          ,         0},
			{pro::Register::GOAL_TORQUE            ,         0},
			{pro::Register::GOAL_ACCELERATION      ,         0},
			{pro::Register::MOVING                 ,        {}},
			{pro::Register::PRESENT_POSITION       ,        {}},
			{pro::Register::PRESENT_VELOCITY       ,        {}},
			{pro::Register::PRESENT_CURRENT        ,        {}},
			{pro::Register::PRESENT_INPUT_VOLTAGE  ,        {}},
			{pro::Register::PRESENT_TEMPERATURE    ,        {}},
			{pro::Register::EXTERNAL_PORT_DATA_1   ,         0},
			{pro::Register::EXTERNAL_PORT_DATA_2   ,         0},
			{pro::Register::EXTERNAL_PORT_DATA_3   ,         0},
			{pro::Register::EXTERNAL_PORT_DATA_4   ,         0},
			{pro::Register::REGISTERED_INSTRUCTION ,         0},
			{pro::Register::STATUS_RETURN_LEVEL    ,         2},
			{pro::Register::HARDWARE_ERROR_STATUS  ,         0},
		}}
	};
	static bool firstRun{true};
	if (firstRun) {
		firstRun = false;

		{ // m54-40-s250-r
			auto& m = data[46096];
			m = data.at(46352);
			m[pro::Register::MODEL_NUMBER] = 46096;
			m[pro::Register::TORQUE_LIMIT] = 120;
		}
		{ // m42-10-s260-r
			auto& m = data[43288];
			m = data.at(46352);
			m[pro::Register::MODEL_NUMBER] = 43288;
			m[pro::Register::TORQUE_LIMIT] = 300;
			m[pro::Register::MAX_POSITION_LIMIT] =  131'593;
			m[pro::Register::MIN_POSITION_LIMIT] = -131'593;
			m[pro::Register::VELOCITY_I_GAIN] =  72;
			m[pro::Register::VELOCITY_P_GAIN] = 520;
			m[pro::Register::POSITION_P_GAIN] =  32;
		}

		{ // h54-200-s500-r
			auto& m = data[54'024];
			m = data.at(46352);
			m[pro::Register::MODEL_NUMBER] = 54'024;
			m[pro::Register::TORQUE_LIMIT] = 620;
			m[pro::Register::VELOCITY_LIMIT] = 17'000;
			m[pro::Register::MAX_POSITION_LIMIT] =  250'961;
			m[pro::Register::MIN_POSITION_LIMIT] = -250'961;
			m[pro::Register::VELOCITY_I_GAIN] =  14;
			m[pro::Register::VELOCITY_P_GAIN] = 399;
			m[pro::Register::POSITION_P_GAIN] =  32;
		}

		{ // h54-100-s500-r
			auto& m = data[53'768];
			m = data.at(46352);
			m[pro::Register::MODEL_NUMBER] = 53'768;
			m[pro::Register::TORQUE_LIMIT] = 310;
			m[pro::Register::VELOCITY_LIMIT] = 17'000;
			m[pro::Register::MAX_POSITION_LIMIT] =  250'961;
			m[pro::Register::MIN_POSITION_LIMIT] = -250'961;
			m[pro::Register::VELOCITY_I_GAIN] =  16;
			m[pro::Register::VELOCITY_P_GAIN] = 256;
			m[pro::Register::POSITION_P_GAIN] =  32;
		}

		{ // h42-20-s300-r
			auto& m = data[51'200];
			m = data.at(46352);
			m[pro::Register::MODEL_NUMBER] = 53'768;
			m[pro::Register::ACCELERATION_LIMIT] =255;
			m[pro::Register::TORQUE_LIMIT] = 465;
			m[pro::Register::VELOCITY_LIMIT] = 10'300;
			m[pro::Register::MAX_POSITION_LIMIT] =  151'875;
			m[pro::Register::MIN_POSITION_LIMIT] = -151'875;
			m[pro::Register::VELOCITY_I_GAIN] =  40;
			m[pro::Register::VELOCITY_P_GAIN] = 440;
			m[pro::Register::POSITION_P_GAIN] =  32;
		}
	}
	return data;
};

ConverterFunctions buildConverters(double angularResolution, int centerVal, double speedResolution) {
	return ConverterFunctions{
		[=](double val) { return std::round(val / (2. * M_PI) * angularResolution + centerVal); },
		[=](int val) { return (static_cast<double>(val) - centerVal) * 2. * M_PI / static_cast<double>(angularResolution); },

		[=](double speed) { return std::round(speed / (2. * M_PI) * 60. / speedResolution); },
		[=](int speed) { return static_cast<double>(speed) * (2. * M_PI) / 60. * speedResolution; }
	};
}

auto getMotorInfos() -> std::vector<MotorInfo> const& {
	static std::vector<MotorInfo> data {
		// mx motors
		{    29, LayoutType::V1,  "MX28",          {"MX-28T", "MX-28R", "MX-28AT", "MX-28AR"}, buildConverters(4096, 2048, .114)},
		{   310, LayoutType::V1,  "MX64",          {"MX-64T", "MX-64R", "MX-64AT", "MX-64AR"}, buildConverters(4096, 2048, .114)},
		{   320, LayoutType::V1,  "MX106",         {"MX-106T", "MX-106R"}, buildConverters(4096, 2048, .114)},
		{   360, LayoutType::V1,  "MX12",          {"MX-12W"}, buildConverters(4096, 2048, 0.916)},

		// mx motors with protocol table 2
		{    30, LayoutType::V2,  "MX28-V2",       {"MX-28T-V2", "MX-28R-V2", "MX-28AT-V2", "MX-28AR-V2"}, buildConverters(4096, 2048, .229)},
		{   311, LayoutType::V2,  "MX64-V2",       {"MX-64T-V2", "MX-64R-V2", "MX-64AT-V2", "MX-64AR-V2"}, buildConverters(4096, 2048, .229)},
		{   321, LayoutType::V2,  "MX106-V2",      {"MX-106T-V2", "MX-106R-V2"}, buildConverters(4096, 2048, .229)},

		// dynamixel x
		{ 1'000, LayoutType::V2,  "XH430-W350",    {"XH430-W350-T", "XH430-W350-R"}, buildConverters(4096, 2048, .229)},
		{ 1'010, LayoutType::V2,  "XH430-W210",    {"XH430-W210-T", "XH430-W210-R"}, buildConverters(4096, 2048, .229)},
		{ 1'020, LayoutType::V2,  "XM430-W350",    {"XM430-W350-T", "XM430-W350-R"}, buildConverters(4096, 2048, .229)},
		{ 1'030, LayoutType::V2,  "XM430-W210",    {"XM430-W210-T", "XM430-W210-R"}, buildConverters(4096, 2048, .229)},
		{ 1'040, LayoutType::V2,  "XH430-V350",    {"XH430-V350"}, buildConverters(4096, 2048, .229)},
		{ 1'050, LayoutType::V2,  "XH430-V210",    {"XH430-V210"}, buildConverters(4096, 2048, .229)},
		{ 1'060, LayoutType::V2,  "XL430-W250",    {"XL430-W250"}, buildConverters(4096, 2048, .229)},
		{ 1'120, LayoutType::V2,  "XM540-W150",    {"XM540-W150-T", "XM540-W150-R"}, buildConverters(4096, 2048, .229)},
		{ 1'130, LayoutType::V2,  "XM540-W270",    {"XM540-W270-T", "XM540-W270-R"}, buildConverters(4096, 2048, .229)},

		// dynamixel pro
		{46'352, LayoutType::Pro, "M54-60-S250-R", {"M54-60-S250-R"},   buildConverters(251'417, 0, 0.00397746)},
		{46'096, LayoutType::Pro, "M54-40-S250-R", {"M54-40-S250-R"},   buildConverters(251'417, 0, 0.00397746)},
		{43'288, LayoutType::Pro, "M42-10-S260-R", {"M42-10-S260-R"},   buildConverters(263'187, 0, 0.00389076)},
		{54'024, LayoutType::Pro, "H54-200-S500-R", {"H54-200-S500-R"}, buildConverters(501'923, 0, 0.00199234)},
		{53'768, LayoutType::Pro, "H54-100-S500-R", {"H54-100-S500-R"}, buildConverters(501'923, 0, 0.00199234)},
		{51'200, LayoutType::Pro,  "H42-20-S300-R", { "H42-20-S300-R"}, buildConverters(303'750, 0, 0.00329218)},
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
