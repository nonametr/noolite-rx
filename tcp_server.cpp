#include "tcp_server.h"

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdio>
#include <unistd.h>
#include <netdb.h>

#include "tx1132.h"

using namespace std;

#define PORT    5555
#define MAXMSG  512

void TCPServer::broadcast(std::string data)
{
    for(int client : _clients)
    {
        send(client, data.c_str(), data.length(), MSG_DONTWAIT);
    }
}

void TCPServer::run()
{
    int sock;
    fd_set active_fd_set, read_fd_set;
    int i;
    struct sockaddr_in clientname;
    uint size;

    /* Create the socket and set it up to accept connections. */
    sock = _makeSocket (PORT);
    if (listen (sock, 1) < 0)
    {
        perror ("Fail on start listening");
        exit (EXIT_FAILURE);
    }

    /* Initialize the set of active sockets. */
    FD_ZERO (&active_fd_set);
    FD_SET (sock, &active_fd_set);

    while (true)
    {
        read_fd_set = active_fd_set;
        if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
        {
            perror ("Failed on select");
            exit (EXIT_FAILURE);
        }

        for (i = 0; i < FD_SETSIZE; ++i)
        if (FD_ISSET (i, &read_fd_set))
        {
            if (i == sock)
            {
                int new_sock;
                size = sizeof (clientname);
                new_sock = accept (sock, (struct sockaddr *) &clientname, &size);
                if (new_sock < 0)
                {
                    perror ("Failed on accept");
                    exit (EXIT_FAILURE);
                }
                fprintf (stderr, "Server: connect from host %s, port %hd.\n", inet_ntoa (clientname.sin_addr), ntohs (clientname.sin_port));
                FD_SET (new_sock, &active_fd_set);
                _clients.push_back(new_sock);
            }
            else
            {
                if (_readClient (i) < 0)
                {
                    close (i);
                    FD_CLR (i, &active_fd_set);
                }
            }
        }
    }
}

int TCPServer::_makeSocket (int port)
{
    int sock;
    struct sockaddr_in name;

    sock = socket (PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror ("Failed on socket create");
        exit (EXIT_FAILURE);
    }

    name.sin_family = AF_INET;
    name.sin_port = htons ((u_int16_t)port);
    name.sin_addr.s_addr = htonl (INADDR_ANY);
    if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
    {
        perror ("Failed on socket bind");
        exit (EXIT_FAILURE);
    }

    return sock;
}

int TCPServer::_readClient (int filedes)
{
    char buffer[MAXMSG];
    int nbytes;

    nbytes = read (filedes, buffer, MAXMSG);
    if (nbytes < 0)
    {
        perror ("Failed on read");
        exit (EXIT_FAILURE);
    }
    else if (nbytes == 0)
        return -1;
    else
    {
        Tx1132 tx;

        int argc = 1;
        char *argv[16] = { nullptr };
        char *pch = strtok (buffer," ");
        while (pch != NULL)
        {
            argv[++argc] = strtok (NULL, " ");
        }

        tx.runCmd(argc, argv);
        fprintf (stderr, "Server: got message: `%s'\n", buffer);
        return 0;
    }
}
