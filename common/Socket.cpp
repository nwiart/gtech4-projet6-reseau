#include "Socket.h"
#include "Networking.h"
#include <iostream>


Socket::Socket()
    : mSocket(INVALID_SOCKET)
{

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

int Socket::connectTCP(const char* ip, uint16_t port)
{
    sockaddr addr;
    network::getServerAddressTCP(&addr, ip, port);

    return connect(mSocket, &addr, sizeof(sockaddr));
}
