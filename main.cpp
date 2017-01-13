#include <iostream>
#include <libusb-1.0/libusb.h>

#include "rx2164.h"
#include "tx1132.h"
#include "tcp_server.h"
#include "loger.h"

using namespace std;

static TCPServer server;

void onEvent(int new_togl, int action, int channel, int data_format)
{
    server.broadcast("togl=" + std::to_string(new_togl) + ";act=" + std::to_string(channel) + ";format=" + std::to_string(data_format));
}


int main(int argc, char *argv[])
{
    if(argc >= 2 && string(argv[1]) == "-tx")
    {
        Tx1132 tx;
        return tx.runCmd(argc, argv);
    }

    RX2164 rx;

    std::function<void(int, int, int, int)> callback = std::bind(&onEvent,
                                                std::placeholders::_1, std::placeholders::_2,
                                                std::placeholders::_3, std::placeholders::_4);

    map<channelId, map<actionId, RxActionData>> _channel_actions;
    rx.init(_channel_actions, callback);

    if(rx.open() != RX2164_STATE::OPENED)
    {
        traceError("Failed to open RX device");
        return 0;
    }
    if(rx.start() != RX2164_STATE::LOOPING)
    {
        traceError("Failed to start quering loop on RX device");
        return 0;
    }

    traceNotice("Listening for incomming connections...");
    server.run();

    return 1;
}
