#ifndef DEFS_H
#define DEFS_H

#define RX2164_MAX_CHANNELS 64
#define RX2164_DEFAULT_DEBUG_LVL 3
#define RX2163_DEV_VID 5824
#define RX2163_DEV_PID 1500
#define RX2163_POOLING_PERIOD 100000

enum RX2164_ACTION_TYPE
{
    TURN_OFF = 0,
    SLOW_TURN_OFF =1,
    TURN_ON = 2,
    SLOW_TURN_ON = 3,
    SWITCH = 4,
    REVERT_SLOW_TURN = 5,
    SET_LEVEL = 6,
    RUN_SCENE = 7,
    RECORD_SCENE = 8,
    UNBIND = 9,
    STOP_DIM_BRIGHT = 10,
    BIND = 15,
    SLOW_RGB_CHANGE = 16,
    SWITCH_COLOR = 17,
    SWITCH_MODE = 18,
    SWITCH_SPEED_MODE = 19,
    BATTERY_LOW = 20,
    TEMPERATURE = 21
};

enum RX2164_DATA_FORMAT
{
    DATA_FORMAT_1 = 0,
    DATA_FORMAT_2,
    DATA_FORMAT_3,
    DATA_FORMAT_4,
};

enum SensorType
{
    RESERVED = 0,
    //PT112
    TEMPERATURE_SENSOR,
    //PT111
    TEMPERATURE_AND_HUMIDITY_SENSOR
};

enum BatteryState
{
    OK = 0,
    REPLACE
};

enum RX2164_STATE
{
    CLOSED = 0,
    OPENED,
    LOOPING,
    STOPPING,
    STOPED,
    IDLE,
    FAILED
};

#endif // DEFS_H
