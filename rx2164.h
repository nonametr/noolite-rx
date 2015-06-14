/*
 *  Copyright 2015 Igor Kuruch aka nonametr
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
 *  /Note: RX2164 HID documentation - http://www.noo.com.by/assets/files/software/RX2164_HID_API.pdf
 */

#ifndef RX2164_H
#define RX2164_H

#include <stdint.h>
#include <thread>
#include <map>

#include "defs.h"

using namespace std;
struct libusb_device_handle;

class RX2164
{    
public:
    RX2164(int debug_lvl = RX2164_DEFAULT_DEBUG_LVL);
    virtual ~RX2164();

    RX2164_STATE open(uint _vid = RX2163_DEV_VID, uint _pid = RX2163_DEV_PID);
    RX2164_STATE start();
    RX2164_STATE close();

    bool bindChannel(uint channel);
    bool unbindChannel(uint channel);
    bool unbindAll();

private:
    void _run();
    RX2164_STATE _fail();
    void _processCommand(int new_togl, unsigned char *input);

    volatile RX2164_STATE _state = CLOSED;
    libusb_device_handle *_handle = nullptr;
};

#endif // RX2164_H
