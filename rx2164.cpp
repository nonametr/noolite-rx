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

#include "rx2164.h"
#include "common.h"
#include "loger.h"
#include "defs_to_str.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

#include <map>

RX2164::RX2164()
{
    if(libusb_init(nullptr))
    {
        perror("Can't init. libusb");
    }
}

RX2164::~RX2164()
{
    close();
}

void RX2164::init(map<channelId, map<actionId, RxActionData>> v_channel_action, std::function<void(int, int, int, int)> &callback, int debug_lvl)
{
    _channel_actions = v_channel_action;
    _callback_func = callback;
    ASSERT_WITH_CODE(debug_lvl, return);
    libusb_set_debug(NULL, debug_lvl);
}

RX2164_STATE RX2164::close()
{
    if(_state == LOOPING)
    {
        _state = STOPPING;
        while(_state == STOPPING)
        {
            sleep(1);
        }
    }

    if(_handle)
    {
        libusb_close(_handle);
        _handle = nullptr;
    }

    return _state;
}

bool RX2164::waitForEvent(RX2164_ACTION_TYPE action_type, uint ms_timeout)
{
    _wait_for_event = true;

    while(_wait_for_event && _last_action != action_type && ms_timeout > RX2163_POOLING_PERIOD)
    {
        usleep(RX2163_POOLING_PERIOD);
        ms_timeout -= RX2163_POOLING_PERIOD;
    }

    return true;
}

bool RX2164::stopWaitForEvent()
{
    _wait_for_event = false;

    return true;
}

bool RX2164::stopBind()
{
    ASSERT_WITH_CODE(_state == OPENED || _state == LOOPING, "RX2164 not ready! Open new device!", return false);

    unsigned char buf[8] = {'\0'};
    buf[0] = 2;

    libusb_control_transfer(_handle, LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT, 0x9, 0x300, 0, buf, 8, 100);

    return true;
}

bool RX2164::bindChannel(const int channel)
{
    ASSERT_WITH_CODE(_state == OPENED || _state == LOOPING, "RX2164 not ready! Open new device!", return false);

    if(channel >= RX2164_MAX_CHANNELS)
    {
        traceNotice("RX2164 doesn't support channel #%u.", channel);
        return false;
    }

    unsigned char buf[8] = {'\0'};
    buf[0] = 1;
    buf[1] = channel;

    libusb_control_transfer(_handle, LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT, 0x9, 0x300, 0, buf, 8, 100);

    return true;
}

bool RX2164::unbindChannel(const int channel)
{
    ASSERT_WITH_CODE(_state == OPENED || _state == LOOPING, "RX2164 not ready! Open new device!", return false);

    if(channel >= RX2164_MAX_CHANNELS)
    {
        traceNotice("RX2164 doesn't support channel #%u.", channel);
        return false;
    }

    unsigned char buf[8] = {'\0'};
    buf[0] = 3;
    buf[1] = channel;

    libusb_control_transfer(_handle, LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT, 0x9, 0x300, 0, buf, 8, 100);

    return true;
}

bool RX2164::unbindAll()
{
    ASSERT_WITH_CODE(_state == OPENED || _state == LOOPING, "RX2164 not ready! Open new device!", return false);

    unsigned char buf[8] = {'\0'};
    buf[0] = 4;

    libusb_control_transfer(_handle, LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT, 0x9, 0x300, 0, buf, 8, 100);

    return true;
}

RX2164_STATE RX2164::start()
{
    ASSERT_WITH_CODE(_state == OPENED, "RX2164 not ready! Open new device!", return FAILED);

    _state = PRE_LOOP;
    std::thread thread(&RX2164::_run, this);
    thread.detach();

    while(_state == PRE_LOOP)
        usleep(RX2163_POOLING_PERIOD);

    return _state;
}

RX2164_STATE RX2164::state()
{
    return _state;
}

RX2164_STATE RX2164::open(uint _vid, uint _pid)
{
    ASSERT_WITH_CODE(_state == CLOSED, "RX2164 not ready! Open new device!", return FAILED);

    _handle = libusb_open_device_with_vid_pid(NULL, _vid, _pid);

    if(!_handle)
    {
        traceError("Can't open RX2164");
        return _fail();
    }

    if(libusb_kernel_driver_active(_handle, 0))
    {
        libusb_detach_kernel_driver(_handle, 0);
    }

    if(libusb_set_configuration(_handle, 1) < 0)
    {
        traceError("Can't configure RX2164");
        return _fail();
    }

    if (libusb_claim_interface(_handle, 0) < 0)
    {
        traceError("Failed to claim RX2164 interface!");
        return _fail();
    }

    return _state = OPENED;
}

void RX2164::_handleEvent(int new_togl, unsigned char *input)
{
    int channel = input[1];
    int action = input[2];
    RX2164_DATA_FORMAT data_format = (RX2164_DATA_FORMAT)input[3];

    traceNotice("RX2164 received command");
    traceNotice("TOGL: %u ", new_togl);
    traceNotice("Action: %s[%u]", _actionToStr(action).c_str(), action);
    traceNotice("Channel: %u", channel);
    traceNotice("Data Size: %u", _formatToDataSize(data_format));

    switch (action)
    {
        case SET_LEVEL:
            traceNotice("Lvl: %u", input[4]);
            break;
        case BIND:
            if(data_format == DATA_FORMAT_1)//if it's sensor we can get it's type
            {
                SensorType sensor_type = (SensorType)(input[4] & 0xff);                
                traceNotice("SensorType: %u", sensorTypeToStr(sensor_type).c_str());
            }
            break;
        case TEMPERATURE:
        {
            int intTemp = ((input[5] & 0x0f) << 8) + (input[4] & 0xff);

            if (intTemp >= 0x800)
            {
                intTemp = intTemp - 0x1000;
            }

            double temperature          = (double)intTemp / 10.0f;
            double humidity             = input[6] & 0xff;
            double analog_sensor        = input[7] & 0xff;
            BatteryState batery_state   = (BatteryState)((input[5] >> 7) & 1);
            SensorType sensor_type      = (SensorType)((input[5] >> 4) & 7);

            traceNotice("Temperature: %f\n", temperature);
            traceNotice("Humidity: %f\n", humidity);
            traceNotice("Analog sensor: %f\n", analog_sensor);
            traceNotice("Battery: %s\n", batteryToStr(batery_state).c_str());
            traceNotice("SensorType: %f\n", sensorTypeToStr(sensor_type).c_str());
        }
            break;
        default:
            break;
    }


    _callback_func(new_togl, action, channel, data_format);
    _last_action = (RX2164_ACTION_TYPE)action;

    auto it_channel = _channel_actions.find(channel);
    if(it_channel != _channel_actions.end())
    {
        auto it_action = it_channel->second.find(action);
        if(it_action != it_channel->second.end())
        {
            string command = it_action->second.script + " -action " + std::to_string(action) +
                    " -channel " + std::to_string(channel);

            if(it_action->second.fw)
            {
                command += " -togl " + std::to_string(new_togl) + " -data_format " + std::to_string(data_format);
            }
            if(it_action->second.fw_ext)
            {
                command += " -arg_1 " + std::to_string(input[4]) + " -arg_2 " + std::to_string(input[5]) + " -arg_3 " + std::to_string(input[6]) + " -arg_4 " + std::to_string(input[7]);
            }

            system(command.c_str());
        }
    }
}

void RX2164::_processEvents()
{
    static int new_togl;
    static int prev_togl = -10000;

    unsigned char buf[8];
    libusb_control_transfer(_handle, LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_IN, 0x9, 0x300, 0, buf, 8, 1000);

    new_togl = buf[0] & 63;

    if (new_togl != prev_togl)//got new command
    {
        _handleEvent(new_togl, buf);
    }

    prev_togl = new_togl;
}

void RX2164::_run()
{
    if(_state == PRE_LOOP)
    {
        _processEvents();
        _state = LOOPING;

        do
        {
            _processEvents();
            usleep(RX2163_POOLING_PERIOD);//1000000 - 1s
        } while(_state == LOOPING);
    }
    _state = STOPED;
}

RX2164_STATE RX2164::_fail()
{
    close();
    return FAILED;
}
