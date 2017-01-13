#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <string>
#include <vector>

class TCPServer
{
public:
    TCPServer() = default;
    virtual ~TCPServer() = default;

    void run();
    void broadcast(std::string data);

private:
    int _makeSocket (int port);
    int _readClient (int filedes);

    std::vector<int> _clients;
};

#endif // TCP_SERVER_H
