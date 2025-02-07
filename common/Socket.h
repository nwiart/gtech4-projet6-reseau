#pragma once

#include <stdint.h>
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

class Socket {
public:
    SOCKET mSocket;

    Socket();
    Socket(SOCKET s);
    ~Socket();

    void createSocketUDP();
    void createSocketTCP();

    int connectTCP(const char* ip, uint16_t port);
    int listenTCP(uint16_t port);
    bool acceptTCP(Socket& outSocket);

    int bindUDP(uint16_t port);

    int sendUDP(const void* data, int dataSize, sockaddr_in& destAddr);
    int receiveUDP(void* buffer, int bufferSize, sockaddr_in& senderAddr);

    bool isValid() const;
    void closeSocket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

private:
    sockaddr_in address;
};
