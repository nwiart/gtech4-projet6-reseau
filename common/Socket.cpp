#include "Socket.h"
#include "Networking.h"
#include <iostream>

Socket::Socket() : mSocket(INVALID_SOCKET)
{
}

Socket::~Socket()
{
    if (mSocket != INVALID_SOCKET) {
        closesocket(mSocket);
    }
}

void Socket::createSocketUDP()
{
    mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (mSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
    }
}

void Socket::createSocketTCP()
{
    mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
    }
}

void Socket::bindSocketUDP(const char* port)
{
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(atoi(port));

    if (bind(mSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "UDP bind failed with error: " << WSAGetLastError() << std::endl;
    }
}
