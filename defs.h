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
    TEMPERATURE = 21,
    MAX_ACTIONS
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
    PRE_LOOP,
    LOOPING,
    STOPPING,
    STOPED,
    IDLE,
    FAILED
};

#endif // DEFS_H

/**
 * #!/bin/sh
# My system IP/set ip address of server
SERVER_IP="195.189.227.70"
# Flushing all rules
iptables -F
iptables -X
# Setting default filter policy
iptables -P INPUT ACCEPT
iptables -P OUTPUT ACCEPT
iptables -P FORWARD ACCEPT
#DROP DEFAULT
# Allow unlimited traffic on loopback
iptables -A INPUT -i 127.0.0.1 -j ACCEPT
iptables -A OUTPUT -o 127.0.0.1 -j ACCEPT
 
# Allow incoming 22 and 80 ports only
iptables -A INPUT  -p tcp -m multiport --dports 22,80 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A OUTPUT -p tcp -m multiport --sports 22,80 -m state --state ESTABLISHED     -j ACCEPT

# Allow all icmp
iptables -A OUTPUT -p icmp -j ACCEPT
iptables -A INPUT -p icmp -j ACCEPT

# Allow outgoing icmp
#iptables -A OUTPUT -p icmp --icmp-type 8 -s $SERVER_IP -d 0/0 -m state --state NEW,ESTABLISHED,RELATED -j ACCEPT
#iptables -A INPUT -p icmp --icmp-type 0 -s 0/0 -d $SERVER_IP -m state --state ESTABLISHED,RELATED -j ACCEPT

# Allow outgoing port 80
iptables -A OUTPUT -p tcp --dport 80 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A INPUT -p tcp --sport 80 -m state --state ESTABLISHED -j ACCEPT

#Allow DNS lookups
iptables -A OUTPUT -p udp --dport 53 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A INPUT  -p udp --sport 53 -m state --state ESTABLISHED     -j ACCEPT
iptables -A OUTPUT -p tcp --dport 53 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A INPUT  -p tcp --sport 53 -m state --state ESTABLISHED     -j ACCEPT

#pptpd
iptables -I INPUT -p tcp --dport 1723 -m state --state NEW -j ACCEPT
iptables -I INPUT -p gre -j ACCEPT
iptables -t nat -I POSTROUTING -o eth0 -j MASQUERADE
iptables -I FORWARD -p tcp --tcp-flags SYN,RST SYN -s 10.0.0.0/24 -j TCPMSS  --clamp-mss-to-pmtu

# make sure nothing comes or goes out of this box
iptables -A INPUT -j ACCEPT
iptables -A OUTPUT -j ACCEPT
**/
