#include "Socket.h"
#include "Networking.h"
#include <iostream>

void Socket::initClient()
{
    network::initializeWinsock();
}

void Socket::createSocketUDP()
{
    mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (mSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        network::cleanupWinsock();
        return;
    }
}

void Socket::createSocketTCP()
{
    mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        network::cleanupWinsock();
        return;
    }
}
