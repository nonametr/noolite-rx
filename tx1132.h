#ifndef TX1132_H
#define TX1132_H

#include <string>

enum TX_ACTION { TX_ON, TX_OFF, TX_SWITCH, TX_SET_LVL, TX_BIND, TX_UNBIND, TX_LOAD_PRESET, TX_SAVE_PRESET, TX_STOP_CHANGE, TX_ROLL_COLOR, TX_SWITCH_COLOR, TX_SWITCH_MODE, TX_SWITCH_SPEED_CHANGE_COLOR, TX_SET_COLOR };

class Tx1132
{
public:
    Tx1132() = default;
    virtual ~Tx1132() = default;

    int runCmd(int argc, char *argv[]);
    bool execute(TX_ACTION action_id, u_char ch_id, u_char v1, u_char v2, u_char v3);

private:
    unsigned char _action[8] = {0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
};

#endif // TX1132_H
