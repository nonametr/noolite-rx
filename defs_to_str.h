/*
 *  Copyright 2015 Igor K. aka nonametr
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef DEFS_TO_STR
#define DEFS_TO_STR

#include <map>
#include <string>

#include "common.h"

using namespace std;

static const map<uint, string> _action_to_str =
{
    {TURN_OFF, "TURN_OFF"},
    {SLOW_TURN_OFF, "SLOW_TURN_OFF"},
    {TURN_ON, "TURN_ON"},
    {SLOW_TURN_ON, "SLOW_TURN_ON"},
    {SWITCH, "SWITCH"},
    {REVERT_SLOW_TURN, "REVERT_SLOW_TURN"},
    {SET_LEVEL, "SET_LEVEL"},
    {RUN_SCENE, "RUN_SCENE"},
    {RECORD_SCENE, "RECORD_SCENE"},
    {UNBIND, "UNBIND"},
    {STOP_DIM_BRIGHT, "STOP_DIM_BRIGHT"},
    {BIND, "BIND"},
    {SLOW_RGB_CHANGE, "SLOW_RGB_CHANGE"},
    {SWITCH_COLOR, "SWITCH_COLOR"},
    {SWITCH_MODE, "SWITCH_MODE"},
    {SWITCH_SPEED_MODE, "SWITCH_SPEED_MODE"},
    {BATTERY_LOW, "BATTERY_LOW"},
    {TEMPERATURE, "TEMPERATURE"}
};

static const map<SensorType, string> _sensor_to_str =
{
    {TEMPERATURE_SENSOR, "TEMPERATURE_SENSOR"},
    {TEMPERATURE_AND_HUMIDITY_SENSOR, "TEMPERATURE_AND_HUMIDITY_SENSOR"}
};

static const map<BatteryState, string> _battery_to_str =
{
    {OK, "BATTERY_OK"},
    {REPLACE, "BATTERY_REPLACE"}
};

static const map<RX2164_DATA_FORMAT, uint>  _data_format_to_data_size =
{
    {DATA_FORMAT_1, 0},
    {DATA_FORMAT_2, 1},
    {DATA_FORMAT_3, 2},
    {DATA_FORMAT_4, 4}
};

static uint _formatToDataSize(RX2164_DATA_FORMAT data_format)
{
    auto it_match = _data_format_to_data_size.find(data_format);
    ASSERT_WITH_CODE(it_match != _data_format_to_data_size.end(), "Error! Unknown data format!", return 0);

    return it_match->second;
}

static string _actionToStr(uint action)
{
    auto it_match = _action_to_str.find(action);
    ASSERT_WITH_CODE(it_match != _action_to_str.end(), "Error! Unknown action!", return "Error! Unknown action!");

    return it_match->second;
}

static string batteryToStr(BatteryState battery_state)
{
    auto it_match = _battery_to_str.find(battery_state);
    ASSERT_WITH_CODE(it_match != _battery_to_str.end(), "Error! Unknown battery state!", return 0);

    return it_match->second;
}

static string sensorTypeToStr(SensorType sensor_type)
{
    auto it_match = _sensor_to_str.find(sensor_type);
    ASSERT_WITH_CODE(it_match != _sensor_to_str.end(), "Error! Unknown battery state!", return "");

    return it_match->second;
}

#endif // DEFS_TO_STR

