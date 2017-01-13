#include "tx1132.h"
#include <libusb-1.0/libusb.h>
#include <iostream>
#include <algorithm>

using namespace std;

#define DEV_VID 0x16c0 //0x5824
#define DEV_PID 0x05df //0x1503
#define DEV_CONFIG 1
#define DEV_INTF 0

int Tx1132::runCmd(int argc, char *argv[])
{
    if(argc < 5 || argc % 2 != 0)
    {
        cout << "Not enought arguments";
        return 0;
    }

    int ch = 0;
    int act = 0;
    int v1 = 0;
    int v2 = 0;
    int v3 = 0;

    string p1 = argv[2];
    string p2 = argv[3];
    if(p1 != "-ch" || !std::all_of(p2.begin(), p2.end(), ::isdigit))
    {
        cout << "Unexpected paramenter => " << p1;
        cout << "Usage example => ./tx-app -ch 4 -act 2 -v1 0 -v2 0 -v3 0";
        return 0;
    }
    ch = std::stoi(p2);


    string p3 = argv[4];
    string p4 = argv[5];
    if(p3 != "-act" || !std::all_of(p4.begin(), p4.end(), ::isdigit))
    {
        cout << "Unexpected paramenter => " << p3;
        cout << "Usage example => ./tx-app -ch 4 -act 2 -v1 0 -v2 0 -v3 0";
        return 0;
    }
    act = std::stoi(p4);

    if(argc >= 6)
    {
        string p5 = argv[6];
        string p6 = argv[7];
        if(p5 != "-v1" || !std::all_of(p6.begin(), p6.end(), ::isdigit))
        {
            cout << "Unexpected paramenter => " << p5;
            cout << "Usage example => ./tx-app -ch 4 -act 2 -v1 0 -v2 0 -v3 0";
            return 0;
        }

        v1 = std::stoi(p6);
    }

    if(argc >= 8)
    {
        string p7 = argv[8];
        string p8 = argv[9];
        if(p7 != "-v2" || !std::all_of(p8.begin(), p8.end(), ::isdigit))
        {
            cout << "Unexpected paramenter => " << p7;
            cout << "Usage example => ./tx-app -ch 4 -act 2 -v1 0 -v2 0 -v3 0";
            return 0;
        }

        v2 = std::stoi(p8);
    }

    if(argc == 10)
    {
        string p9 = argv[10];
        string p10 = argv[11];
        if(p9 != "-v3" || !std::all_of(p10.begin(), p10.end(), ::isdigit))
        {
            cout << "Unexpected paramenter => " << p9;
            cout << "Usage example => ./tx-app -ch 4 -act 2 -v1 0 -v2 0 -v3 0";
            return 0;
        }

        v3 = std::stoi(p10);
    }

    Tx1132 tx;
    tx.execute(static_cast<TX_ACTION>(act), ch, v1, v2, v3);

    return 1;
}

bool Tx1132::execute(TX_ACTION tx_action, u_char ch_id, u_char v1, u_char v2, u_char v3)
{
    switch (tx_action) {
    case TX_ON:
            _action[1] = 2;
        break;
    case TX_OFF:
            _action[1] = 0;
        break;
    case TX_SWITCH:
        _action[1] = 4;
        break;
    case TX_SET_LVL:
        _action[1] = 6;
        _action[2] = 1;
        _action[5]= v1;
        break;
    case TX_BIND:
            _action[1] = 15;
        break;
    case TX_UNBIND:
            _action[1] = 9;
        break;
    case TX_LOAD_PRESET:
        _action[1] = 7;
        break;
    case TX_SAVE_PRESET:
        _action[1] = 8;
        break;
    case TX_STOP_CHANGE:
        _action[1] = 10;
        break;
    case TX_ROLL_COLOR:
        _action[1] = 16;
        _action[2] = 4;
        break;
    case TX_SWITCH_COLOR:
        _action[1] = 17;
        _action[2] = 4;
        break;
    case TX_SWITCH_MODE:
        _action[1] = 18;
        _action[2] = 4;
        break;
    case TX_SWITCH_SPEED_CHANGE_COLOR:
        _action[1] = 19;
        _action[2] = 4;
        break;
    case TX_SET_COLOR:
        _action[1] = 6;
        _action[2] = 3;
        _action[5] = v1;
        _action[6] = v2;
        _action[7] = v3;
        break;
    }

    int ret;
    _action[4] = ch_id;

    libusb_init(NULL);
    libusb_set_debug(NULL, 3);
     libusb_device_handle *handle = libusb_open_device_with_vid_pid(NULL, DEV_VID, DEV_PID);
     if (handle == NULL) {
         perror("Can't open noolite `Tx` device");
         libusb_exit(NULL);
         return false;
     }
     if (libusb_kernel_driver_active(handle,DEV_INTF))
          libusb_detach_kernel_driver(handle, DEV_INTF);
     if ((ret = libusb_set_configuration(handle, DEV_CONFIG)) < 0)
     {
          perror("Failed to configure device");
          libusb_close(handle);
          libusb_exit(NULL);
          if (ret == LIBUSB_ERROR_BUSY)
              printf("B\n");
          printf("ret:%i\n", ret);
          return 0;
     }
     if (libusb_claim_interface(handle,  DEV_INTF) < 0)
     {
          perror("Failed to claim interface!");
          libusb_close(handle);
          libusb_exit(NULL);
          return 0;
     }

     ret = libusb_control_transfer(handle, LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_OUT, 0x9, 0x300, 0, _action, 8, 100);

     libusb_attach_kernel_driver(handle, DEV_INTF);
     libusb_close(handle);
     libusb_exit(NULL);
     return true;

}
