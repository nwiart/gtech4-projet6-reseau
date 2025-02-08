#pragma once

#include <stdint.h>
#include <winsock2.h>


class Socket {
public:
    uint64_t mSocket;

    Socket();
    Socket(uint64_t s);
    ~Socket();

    void createSocketUDP();
    void createSocketTCP();

    int connectTCP(const char* ip, uint16_t port);
    int listenTCP(uint16_t port);
    bool acceptTCP(Socket& outSocket);

    int bindUDP(uint16_t port);


    bool isValid() const;
    void closeSocket();
};
