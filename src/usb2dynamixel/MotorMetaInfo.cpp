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

auto getLayoutXL320Infos() -> std::map<xl320::Register, LayoutField> const& {
	using A = LayoutField::Access;
	static std::map<xl320::Register, LayoutField> data {
		{xl320::Register::MODEL_NUMBER         , {2,  true, A:: R, "Model Number", "model number"}},
		{xl320::Register::FIRMWARE_VERSION     , {1,  true, A:: R, "Version of Firmware", "Information on the version of firmware"}},
		{xl320::Register::ID                   , {1,  true, A::RW, "ID", "ID of Dynamixel"}},
		{xl320::Register::BAUD_RATE            , {1,  true, A::RW, "Baud Rate", "Baud Rate of Dynamixel"}},
		{xl320::Register::RETURN_DELAY_TIME    , {1,  true, A::RW, "Return Delay Time", "Return Delay Time"}},
		{xl320::Register::CW_ANGLE_LIMIT       , {2,  true, A::RW, "CW Angle Limit", "clockwise Angle Limit"}},
		{xl320::Register::CCW_ANGLE_LIMIT      , {2,  true, A::RW, "CCW Angle Limit", "counterclockwise Angle Limit"}},
		{xl320::Register::CONTROL_MODE         , {1,  true, A::RW, "Control Mode", "Dual Mode Setting"}},
		{xl320::Register::TEMPERATURE_LIMIT    , {1,  true, A::RW, "Highest Limit Temperature", "Internal Limit Temperature"}},
		{xl320::Register::MIN_VOLTAGE_LIMIT    , {1,  true, A::RW, "Min Limit Voltage", "Min Limit Voltage"}},
		{xl320::Register::MAX_VOLTAGE_LIMIT    , {1,  true, A::RW, "Max Limit Voltage", "Max Limit Voltage"}},
		{xl320::Register::MAX_TORQUE           , {2,  true, A::RW, "Max Torque", "Max. Torque"}},
		{xl320::Register::STATUS_RETURN_LEVEL  , {1,  true, A::RW, "Status Return Level", "Status Return Level"}},
		{xl320::Register::SHUTDOWN             , {1,  true, A::RW, "Shutdown", "Shutdown for Alarm"}},
		{xl320::Register::TORQUE_ENABLE        , {1, false, A::RW, "Torque Enable", "Torque On/Off"}},
		{xl320::Register::LED                  , {1, false, A::RW, "LED", "LED On/Off"}},
		{xl320::Register::D_GAIN               , {1, false, A::RW, "D Gain", "Derivative Gain"}},
		{xl320::Register::I_GAIN               , {1, false, A::RW, "I Gain", "Integral Gain"}},
		{xl320::Register::P_GAIN               , {1, false, A::RW, "P Gain", "Proportional Gain"}},
		{xl320::Register::GOAL_POSITION        , {2, false, A::RW, "Goal Position", "Goal Position"}},
		{xl320::Register::MOVING_SPEED         , {2, false, A::RW, "Moving Speed", "Moving Speed (Moving Velocity)"}},
		{xl320::Register::TORQUE_LIMIT         , {2, false, A::RW, "Torque Limit", "Torque Limit (Goal Torque)"}},
		{xl320::Register::PRESENT_POSITION     , {2, false, A:: R, "Present Position", "Current Position (Present Velocity)"}},
		{xl320::Register::PRESENT_SPEED        , {2, false, A:: R, "Present Speed", "Current Speed"}},
		{xl320::Register::PRESENT_LOAD         , {2, false, A:: R, "Present Load", "Current Load"}},
		{xl320::Register::PRESENT_VOLTAGE      , {1, false, A:: R, "Present Voltage", "Current Voltage"}},
		{xl320::Register::PRESENT_TEMPERATURE  , {1, false, A:: R, "Present Temperature", "Current Temperature"}},
		{xl320::Register::REGISTERED           , {1, false, A:: R, "Registered", "Means if Instruction is registered"}},
		{xl320::Register::MOVING               , {1, false, A:: R, "Moving", "Means if there is any movement"}},
		{xl320::Register::HARDWARE_ERROR_STATUS, {1, false, A:: R, "Hardware Error Status", "Hardware Error Status"}},
		{xl320::Register::PUNCH                , {2, false, A::RW, "Punch", "Punch"}},
	};
	return data;
}

auto getLayoutAXInfos() -> std::map<ax::Register, LayoutField> const& {
	using A = LayoutField::Access;
	static std::map<ax::Register, LayoutField> data {
		{ax::Register::MODEL_NUMBER         , {2,  true, A:: R, "Model Number", "model number"}},
		{ax::Register::FIRMWARE_VERSION     , {1,  true, A:: R, "Version of Firmware", "Information on the version of firmware"}},
		{ax::Register::ID                   , {1,  true, A::RW, "ID", "ID of Dynamixel"}},
		{ax::Register::BAUD_RATE            , {1,  true, A::RW, "Baud Rate", "Baud Rate of Dynamixel"}},
		{ax::Register::RETURN_DELAY_TIME    , {1,  true, A::RW, "Return Delay Time", "Return Delay Time"}},
		{ax::Register::CW_ANGLE_LIMIT       , {2,  true, A::RW, "CW Angle Limit", "clockwise Angle Limit"}},
		{ax::Register::CCW_ANGLE_LIMIT      , {2,  true, A::RW, "CCW Angle Limit", "counterclockwise Angle Limit"}},
		{ax::Register::TEMPERATURE_LIMIT    , {1,  true, A::RW, "Highest Limit Temperature", "Internal Limit Temperature"}},
		{ax::Register::MIN_VOLTAGE_LIMIT    , {1,  true, A::RW, "Min Limit Voltage", "Min Limit Voltage"}},
		{ax::Register::MAX_VOLTAGE_LIMIT    , {1,  true, A::RW, "Max Limit Voltage", "Max Limit Voltage"}},
		{ax::Register::MAX_TORQUE           , {2,  true, A::RW, "Max Torque", "Max. Torque"}},
		{ax::Register::STATUS_RETURN_LEVEL  , {1,  true, A::RW, "Status Return Level", "Status Return Level"}},
		{ax::Register::ALARM_LED            , {1,  true, A::RW, "Alarm LED", "LED for Alarm"}},
		{ax::Register::SHUTDOWN             , {1,  true, A::RW, "Shutdown", "Shutdown for Alarm"}},
		{ax::Register::TORQUE_ENABLE        , {1, false, A::RW, "Torque Enable", "Torque On/Off"}},
		{ax::Register::LED                  , {1, false, A::RW, "LED", "LED On/Off"}},
		{ax::Register::CW_COMPLIANCE_MARGIN , {1, false, A::RW, "CW Compliance Margin", "CW Compliance Margin"}},
		{ax::Register::CCW_COMPLIANCE_MARGIN, {1, false, A::RW, "CCW Compliance Margin", "CCW Compliance Margin"}},
		{ax::Register::CW_COMPLIANCE_SLOPE  , {1, false, A::RW, "CW Compliance Slope", "CW Compliance Slope"}},
		{ax::Register::CCW_COMPLIANCE_SLOPE , {1, false, A::RW, "CCW Compliance Slope", "CCW Compliance Slope"}},
		{ax::Register::GOAL_POSITION        , {2, false, A::RW, "Goal Position", "Goal Position"}},
		{ax::Register::MOVING_SPEED         , {2, false, A::RW, "Moving Speed", "Moving Speed (Moving Velocity)"}},
		{ax::Register::TORQUE_LIMIT         , {2, false, A::RW, "Torque Limit", "Torque Limit (Goal Torque)"}},
		{ax::Register::PRESENT_POSITION     , {2, false, A:: R, "Present Position", "Current Position (Present Velocity)"}},
		{ax::Register::PRESENT_SPEED        , {2, false, A:: R, "Present Speed", "Current Speed"}},
		{ax::Register::PRESENT_LOAD         , {2, false, A:: R, "Present Load", "Current Load"}},
		{ax::Register::PRESENT_VOLTAGE      , {1, false, A:: R, "Present Voltage", "Current Voltage"}},
		{ax::Register::PRESENT_TEMPERATURE  , {1, false, A:: R, "Present Temperature", "Current Temperature"}},
		{ax::Register::REGISTERED           , {1, false, A:: R, "Registered", "Means if Instruction is registered"}},
		{ax::Register::MOVING               , {1, false, A:: R, "Moving", "Means if there is any movement"}},
		{ax::Register::LOCK                 , {1, false, A::RW, "Lock", "Locking EEPROM"}},
		{ax::Register::PUNCH                , {2, false, A::RW, "Punch", "Punch"}},
	};
	return data;
}

ConverterFunctions buildConverters(double angularResolution, int centerVal, double speedResolution) {
	return ConverterFunctions{
		[=](double val) { return std::round(val / (2. * M_PI) * angularResolution + centerVal); },
		[=](int val) { return (static_cast<double>(val) - centerVal) * 2. * M_PI / static_cast<double>(angularResolution); },

		[=](double speed) { return std::round(speed / (2. * M_PI) * 60. / speedResolution); },
		[=](int speed) { return static_cast<double>(speed) * (2. * M_PI) / 60. * speedResolution; }
	};
}

auto getLayoutV1Defaults() -> std::map<uint32_t, Info<v1::Register>> const& {
	static auto data = std::map<uint32_t, Info<v1::Register>> {
		{320, {
			320,
			LayoutType::V1,
			"MX106",
			{"MX-106T", "MX-106R"},
			buildConverters(4096, 2048, .114), {
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
			}
		}}
	};
	auto newMotor = [&](int number, std::string shortName, std::vector<std::string> names) -> Info<v1::Register>& {
		auto& m = data[number];
		m = data.at(320);
		m.modelNumber = number;
		m.shortName = std::move(shortName);
		m.motorNames = std::move(names);
		m.defaultLayout[v1::Register::MODEL_NUMBER] = number;
		return m;
	};
	static bool firstRun{true};
	if (firstRun) {
		firstRun = false;
		{
			auto& m = newMotor(310, "MX64", {"MX-64T", "MX-64R", "MX-64AT", "MX-64AR"});
			m.defaultLayout.erase(v1::Register::DRIVE_MODE);
		}
		{
			auto& m = newMotor(29, "MX28", {"MX-28T", "MX-28R", "MX-28AT", "MX-28AR"});
			m.defaultLayout.erase(v1::Register::DRIVE_MODE);
			m.defaultLayout.erase(v1::Register::TORQUE_CONTROL_MODE);
			m.defaultLayout.erase(v1::Register::CURRENT);
			m.defaultLayout.erase(v1::Register::GOAL_TORQUE);
		}
		{
			auto& m = newMotor(360, "MX12", {"MX-12W"});
			m.converterFunctions = buildConverters(4096, 2048, 0.916);
			m.defaultLayout[v1::Register::MODEL_NUMBER] = 360;
			m.defaultLayout[v1::Register::BAUD_RATE]    = 1;
			m.defaultLayout[v1::Register::D_GAIN]       = 8;
			m.defaultLayout[v1::Register::P_GAIN]       = 8;
			m.defaultLayout[v1::Register::PUNCH]        = 32;
			m.defaultLayout.erase(v1::Register::DRIVE_MODE);
			m.defaultLayout.erase(v1::Register::CURRENT);
			m.defaultLayout.erase(v1::Register::TORQUE_CONTROL_MODE);
			m.defaultLayout.erase(v1::Register::GOAL_TORQUE);
		}
	}
	return data;
};
auto getLayoutV2Defaults() -> std::map<uint32_t, Info<v2::Register>> const& {
	static auto data = std::map<uint32_t, Info<v2::Register>> {
		{1020, {
			1020,
			LayoutType::V2,
			"XM430-W350",
			{"XM430-W350-T", "XM430-W350-R"},
			buildConverters(4096, 2048, .229), {
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
			}
		}}
	};

	auto newMotor = [&](int number, std::string shortName, std::vector<std::string> names) -> Info<v2::Register>& {
		auto& m = data[number];
		m = data.at(1020);
		m.modelNumber = number;
		m.shortName = std::move(shortName);
		m.motorNames = std::move(names);
		m.defaultLayout[v2::Register::MODEL_NUMBER] = number;
		return m;
	};

	static bool firstRun{true};
	if (firstRun) {
		firstRun = false;

		{
			auto& m = newMotor(321, "MX106-V2", {"MX-106T-V2", "MX-106R-V2"});
			m.defaultLayout[v2::Register::CURRENT_LIMIT] = 2047;
			m.defaultLayout[v2::Register::VELOCITY_LIMIT] = 360;
			m.defaultLayout[v2::Register::POSITION_P_GAIN] = 850;
			m.defaultLayout[v2::Register::ACCELERATION_LIMIT] = 32767;
		}

		{
			auto& m = newMotor(311, "MX64-V2", {"MX-64T-V2", "MX-64R-V2", "MX-64AT-V2", "MX-64AR-V2"});
			m.defaultLayout[v2::Register::CURRENT_LIMIT] = 1941;
			m.defaultLayout[v2::Register::VELOCITY_LIMIT] = 435;
			m.defaultLayout[v2::Register::POSITION_P_GAIN] = 850;
			m.defaultLayout[v2::Register::ACCELERATION_LIMIT] = 32767;
		}

		{
			auto& m = newMotor(30, "MX28-V2", {"MX-28T-V2", "MX-28R-V2", "MX-28AT-V2", "MX-28AR-V2"});
			m.defaultLayout[v2::Register::CURRENT_LIMIT] = 1941;
			m.defaultLayout.erase(v2::Register::CURRENT_LIMIT);
			m.defaultLayout[v2::Register::VELOCITY_LIMIT] = 380;
			m.defaultLayout[v2::Register::POSITION_P_GAIN] = 850;
			m.defaultLayout[v2::Register::ACCELERATION_LIMIT] = 32767;
		}
		{
			auto& m = newMotor( 1'000, "XH430-W350", {"XH430-W350-T", "XH430-W350-R"});
			m.defaultLayout[v2::Register::CURRENT_LIMIT] = 689;
			m.defaultLayout[v2::Register::VELOCITY_LIMIT] = 280;
			m.defaultLayout[v2::Register::POSITION_P_GAIN] = 900;
			m.defaultLayout.erase(v2::Register::ACCELERATION_LIMIT);
		}
		{
			auto& m = newMotor( 1'010, "XH430-W210", {"XH430-W210-T", "XH430-W210-R"});
			m.defaultLayout[v2::Register::CURRENT_LIMIT] = 689;
			m.defaultLayout[v2::Register::CURRENT_LIMIT] = 689;
			m.defaultLayout[v2::Register::VELOCITY_LIMIT] = 210;
			m.defaultLayout[v2::Register::POSITION_P_GAIN] = 900;
			m.defaultLayout.erase(v2::Register::ACCELERATION_LIMIT);
		}
		{
			auto& m = newMotor( 1'030, "XM430-W210", {"XM430-W210-T", "XM430-W210-R"});
			m.defaultLayout[v2::Register::VELOCITY_LIMIT] = 300;
			m.defaultLayout.erase(v2::Register::ACCELERATION_LIMIT);
		}
		{
			auto& m = newMotor( 1'040, "XH430-V350", {"XH430-V350"});
			m.defaultLayout[v2::Register::CURRENT_LIMIT] = 689;
			m.defaultLayout[v2::Register::VELOCITY_LIMIT] = 135;
			m.defaultLayout.erase(v2::Register::ACCELERATION_LIMIT);
		}
		{
			auto& m = newMotor(1050, "", {});
			m.defaultLayout[v2::Register::MAX_VOLTAGE_LIMIT] = 300;
			m.defaultLayout[v2::Register::MIN_VOLTAGE_LIMIT] = 110;
			m.defaultLayout[v2::Register::VELOCITY_LIMIT] = 230;
			m.defaultLayout.erase(v2::Register::ACCELERATION_LIMIT);
		}
		{
			auto& m = newMotor( 1'060, "XL430-W250", {"XL430-W250"});
			m.defaultLayout[v2::Register::TEMPERATURE_LIMIT] = 72;
			m.defaultLayout[v2::Register::MAX_VOLTAGE_LIMIT] = 140;
			m.defaultLayout[v2::Register::MIN_VOLTAGE_LIMIT] =  60;
			m.defaultLayout[v2::Register::VELOCITY_LIMIT] =  265;

			m.defaultLayout[v2::Register::VELOCITY_I_GAIN] =  1000;
			m.defaultLayout[v2::Register::VELOCITY_P_GAIN] =  100;
			m.defaultLayout[v2::Register::POSITION_D_GAIN] =  4000;
			m.defaultLayout[v2::Register::POSITION_P_GAIN] =  640;

			m.defaultLayout.erase(v2::Register::CURRENT_LIMIT);
			m.defaultLayout.erase(v2::Register::ACCELERATION_LIMIT);
		}
		{
			auto& m = newMotor( 1'120, "XM540-W150", {"XM540-W150-T", "XM540-W150-R"});
			m.defaultLayout[v2::Register::CURRENT_LIMIT] = 2047;
			m.defaultLayout[v2::Register::VELOCITY_LIMIT] = 230;
			m.defaultLayout[v2::Register::EXTERNAL_PORT_MODE_1] = 1;
			m.defaultLayout[v2::Register::EXTERNAL_PORT_MODE_2] = 1;
			m.defaultLayout[v2::Register::EXTERNAL_PORT_MODE_3] = 1;
			m.defaultLayout.erase(v2::Register::ACCELERATION_LIMIT);
		}
		{
			auto& m = newMotor( 1'130, "XM540-W270", {"XM540-W270-T", "XM540-W270-R"});
			m.defaultLayout[v2::Register::CURRENT_LIMIT] = 2047;
			m.defaultLayout[v2::Register::VELOCITY_LIMIT] = 128;
			m.defaultLayout[v2::Register::EXTERNAL_PORT_MODE_1] = 1;
			m.defaultLayout[v2::Register::EXTERNAL_PORT_MODE_2] = 1;
			m.defaultLayout[v2::Register::EXTERNAL_PORT_MODE_3] = 1;
			m.defaultLayout.erase(v2::Register::ACCELERATION_LIMIT);
		}
	}
	return data;
};
auto getLayoutProDefaults() -> std::map<uint32_t, Info<pro::Register>> const& {
	static auto data = std::map<uint32_t, Info<pro::Register>> {
		{46'352, {
			46'352,
			LayoutType::Pro,
			"M54-60-S250-R",
			{"M54-60-S250-R"},
			buildConverters(251'417, 0, 0.00397746), {
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
			}
		}}
	};
	auto newMotor = [&](int number, std::string shortName, std::vector<std::string> names) -> Info<pro::Register>& {
		auto& m = data[number];
		m = data.at(46'352);
		m.modelNumber = number;
		m.shortName = std::move(shortName);
		m.motorNames = std::move(names);
		m.defaultLayout[pro::Register::MODEL_NUMBER] = number;
		return m;
	};

	static bool firstRun{true};
	if (firstRun) {
		firstRun = false;

		{
			auto& m = newMotor(46'096, "M54-40-S250-R", {"M54-40-S250-R"});
			m.converterFunctions = buildConverters(251'417, 0, 0.00397746);
			m.defaultLayout[pro::Register::TORQUE_LIMIT] = 120;
		}
		{
			auto& m = newMotor(43'288, "M42-10-S260-R", {"M42-10-S260-R"}); 
			m.converterFunctions = buildConverters(263'187, 0, 0.00389076);
			m.defaultLayout[pro::Register::TORQUE_LIMIT] = 300;
			m.defaultLayout[pro::Register::MAX_POSITION_LIMIT] =  131'593;
			m.defaultLayout[pro::Register::MIN_POSITION_LIMIT] = -131'593;
			m.defaultLayout[pro::Register::VELOCITY_I_GAIN] =  72;
			m.defaultLayout[pro::Register::VELOCITY_P_GAIN] = 520;
			m.defaultLayout[pro::Register::POSITION_P_GAIN] =  32;
		}
		{
			auto& m = newMotor(54'024, "H54-200-S500-R", {"H54-200-S500-R"});
			m.converterFunctions = buildConverters(501'923, 0, 0.00199234);
			m.defaultLayout[pro::Register::TORQUE_LIMIT] = 620;
			m.defaultLayout[pro::Register::VELOCITY_LIMIT] = 17'000;
			m.defaultLayout[pro::Register::MAX_POSITION_LIMIT] =  250'961;
			m.defaultLayout[pro::Register::MIN_POSITION_LIMIT] = -250'961;
			m.defaultLayout[pro::Register::VELOCITY_I_GAIN] =  14;
			m.defaultLayout[pro::Register::VELOCITY_P_GAIN] = 399;
			m.defaultLayout[pro::Register::POSITION_P_GAIN] =  32;
		}

		{
			auto& m = newMotor(53'768, "H54-100-S500-R", {"H54-100-S500-R"});
			m.converterFunctions = buildConverters(501'923, 0, 0.00199234);
			m.defaultLayout[pro::Register::TORQUE_LIMIT] = 310;
			m.defaultLayout[pro::Register::VELOCITY_LIMIT] = 17'000;
			m.defaultLayout[pro::Register::MAX_POSITION_LIMIT] =  250'961;
			m.defaultLayout[pro::Register::MIN_POSITION_LIMIT] = -250'961;
			m.defaultLayout[pro::Register::VELOCITY_I_GAIN] =  16;
			m.defaultLayout[pro::Register::VELOCITY_P_GAIN] = 256;
			m.defaultLayout[pro::Register::POSITION_P_GAIN] =  32;
		}
		{
			auto& m = newMotor(51'200, "H42-20-S300-R", { "H42-20-S300-R"});
			m.converterFunctions = buildConverters(303'750, 0, 0.00329218);
			m.defaultLayout[pro::Register::ACCELERATION_LIMIT] =255;
			m.defaultLayout[pro::Register::TORQUE_LIMIT] = 465;
			m.defaultLayout[pro::Register::VELOCITY_LIMIT] = 10'300;
			m.defaultLayout[pro::Register::MAX_POSITION_LIMIT] =  151'875;
			m.defaultLayout[pro::Register::MIN_POSITION_LIMIT] = -151'875;
			m.defaultLayout[pro::Register::VELOCITY_I_GAIN] =  40;
			m.defaultLayout[pro::Register::VELOCITY_P_GAIN] = 440;
			m.defaultLayout[pro::Register::POSITION_P_GAIN] =  32;
		}
	}
	return data;
};

auto getLayoutXL320Defaults() -> std::map<uint32_t, Info<xl320::Register>> const& {
	static auto data = std::map<uint32_t, Info<xl320::Register>> {
		{350, {
			350,
			LayoutType::XL320,
			"XL-320",
			{"XL-320"},
			buildConverters(1024*360/300, 512, .111), {
				{xl320::Register::MODEL_NUMBER         ,    350},
				{xl320::Register::FIRMWARE_VERSION     ,     {}},
				{xl320::Register::ID                   ,      1},
				{xl320::Register::BAUD_RATE            ,      3},
				{xl320::Register::RETURN_DELAY_TIME    ,    250},
				{xl320::Register::CW_ANGLE_LIMIT       ,      0},
				{xl320::Register::CCW_ANGLE_LIMIT      , 0x03ff},
				{xl320::Register::CONTROL_MODE         ,      2},
				{xl320::Register::TEMPERATURE_LIMIT    ,     65},
				{xl320::Register::MIN_VOLTAGE_LIMIT    ,     60},
				{xl320::Register::MIN_VOLTAGE_LIMIT    ,     90},
				{xl320::Register::MAX_TORQUE           , 0x03ff},
				{xl320::Register::STATUS_RETURN_LEVEL  ,      2},
				{xl320::Register::SHUTDOWN             ,      3},
				{xl320::Register::TORQUE_ENABLE        ,      0},
				{xl320::Register::LED                  ,      0},
				{xl320::Register::D_GAIN               ,      0},
				{xl320::Register::I_GAIN               ,      0},
				{xl320::Register::P_GAIN               ,     32},
				{xl320::Register::GOAL_POSITION        ,     {}},
				{xl320::Register::MOVING_SPEED         ,     {}},
				{xl320::Register::TORQUE_LIMIT         ,     {}},
				{xl320::Register::PRESENT_POSITION     ,     {}},
				{xl320::Register::PRESENT_SPEED        ,     {}},
				{xl320::Register::PRESENT_LOAD         ,     {}},
				{xl320::Register::PRESENT_VOLTAGE      ,     {}},
				{xl320::Register::PRESENT_TEMPERATURE  ,     {}},
				{xl320::Register::REGISTERED           ,      0},
				{xl320::Register::MOVING               ,      0},
				{xl320::Register::HARDWARE_ERROR_STATUS,      0},
				{xl320::Register::PUNCH                ,     32},
			}
		}}
	};
	return data;
};

auto getLayoutAXDefaults() -> std::map<uint32_t, Info<ax::Register>> const& {
	static auto data = std::map<uint32_t, Info<ax::Register>> {
		{300, {
			300,
			LayoutType::AX,
			"AX-12W",
			{"AX-12W"},
			buildConverters(1024*360/300, 512, .111), {
				{ax::Register::MODEL_NUMBER         ,    300},
				{ax::Register::FIRMWARE_VERSION     ,     {}},
				{ax::Register::ID                   ,      1},
				{ax::Register::BAUD_RATE            ,      1},
				{ax::Register::RETURN_DELAY_TIME    ,    250},
				{ax::Register::CW_ANGLE_LIMIT       ,      0},
				{ax::Register::CCW_ANGLE_LIMIT      , 0x03ff},
				{ax::Register::TEMPERATURE_LIMIT    ,     70},
				{ax::Register::MIN_VOLTAGE_LIMIT    ,     60},
				{ax::Register::MIN_VOLTAGE_LIMIT    ,    140},
				{ax::Register::MAX_TORQUE           , 0x03ff},
				{ax::Register::STATUS_RETURN_LEVEL  ,      2},
				{ax::Register::ALARM_LED            ,     36},
				{ax::Register::SHUTDOWN             ,     36},
				{ax::Register::TORQUE_ENABLE        ,      0},
				{ax::Register::LED                  ,      0},
				{ax::Register::CW_COMPLIANCE_MARGIN ,      4},
				{ax::Register::CCW_COMPLIANCE_MARGIN,      4},
				{ax::Register::CW_COMPLIANCE_SLOPE  ,     64},
				{ax::Register::CCW_COMPLIANCE_SLOPE ,     64},
				{ax::Register::GOAL_POSITION        ,     {}},
				{ax::Register::MOVING_SPEED         ,     {}},
				{ax::Register::TORQUE_LIMIT         ,     {}},
				{ax::Register::PRESENT_POSITION     ,     {}},
				{ax::Register::PRESENT_SPEED        ,     {}},
				{ax::Register::PRESENT_LOAD         ,     {}},
				{ax::Register::PRESENT_VOLTAGE      ,     {}},
				{ax::Register::PRESENT_TEMPERATURE  ,     {}},
				{ax::Register::REGISTERED           ,      0},
				{ax::Register::MOVING               ,      0},
				{ax::Register::LOCK                 ,      0},
				{ax::Register::PUNCH                ,     32},
			}
		}}
	};

	auto newMotor = [&](int number, std::string shortName, std::vector<std::string> names) -> Info<ax::Register>& {
		auto& m = data[number];
		m = data.at(300);
		m.modelNumber = number;
		m.shortName = std::move(shortName);
		m.motorNames = std::move(names);
		m.defaultLayout[ax::Register::MODEL_NUMBER] = number;
		return m;
	};

	static bool firstRun{true};
	if (firstRun) {
		firstRun = false;
		{
			auto& m = newMotor(12, "AX-12A", {"AX-12A"});
			m.defaultLayout[ax::Register::CW_COMPLIANCE_MARGIN] = 1;
			m.defaultLayout[ax::Register::CCW_COMPLIANCE_MARGIN] = 1;
			m.defaultLayout[ax::Register::CW_COMPLIANCE_SLOPE] = 32;
			m.defaultLayout[ax::Register::CCW_COMPLIANCE_SLOPE] = 32;
		}

		{
			auto& m = newMotor(18, "AX-18A", {"AX-18A"});
			m.defaultLayout[ax::Register::TEMPERATURE_LIMIT] = 75;
			m.defaultLayout[ax::Register::MAX_TORQUE] = 983;
			m.defaultLayout[ax::Register::CW_COMPLIANCE_MARGIN] = 1;
			m.defaultLayout[ax::Register::CCW_COMPLIANCE_MARGIN] = 1;
			m.defaultLayout[ax::Register::CW_COMPLIANCE_SLOPE] = 32;
			m.defaultLayout[ax::Register::CCW_COMPLIANCE_SLOPE] = 32;
		}
	}
	return data;
};

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

		// dynamixel xl320
		{   350, LayoutType::XL320, "XL-320", {"XL-320"},   buildConverters(1024*360/300, 512, .111)},

		// dynamixel ax
		{   300, LayoutType::AX, "AX-12W", {"AX-12W"},   buildConverters(1024*360/300, 512, .111)},
		{    12, LayoutType::AX, "AX-12A", {"AX-12A"},   buildConverters(1024*360/300, 512, .111)},
		{    18, LayoutType::AX, "AX-18A", {"AX-18A"},   buildConverters(1024*360/300, 512, .111)},


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
