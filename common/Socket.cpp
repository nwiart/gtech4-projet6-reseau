#include "Socket.h"
#include "Networking.h"
#include <iostream>


Socket::Socket()
    : mSocket(INVALID_SOCKET)
{

}

Socket::~Socket() {
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

int Socket::connectTCP(const char* ip, uint16_t port)
{
    sockaddr addr;
    network::getServerAddressTCP(&addr, ip, port);

    return connect(mSocket, &addr, sizeof(sockaddr));
}

int Socket::listenTCP(uint16_t port)
{
    sockaddr addr;
    network::getServerAddressTCP(&addr, "127.0.0.1", port);

    bind(mSocket, &addr, sizeof(sockaddr));
    return listen(mSocket, SOMAXCONN);
}

int Socket::acceptTCP()
{
    sockaddr clientAddr;
    int addrLen;

    SOCKET so = accept(mSocket, &clientAddr, &addrLen);
    return 1;
}

int Socket::bindUDP(uint16_t port)
{
    sockaddr addr;
    network::getServerAddressUDP(&addr, "127.0.0.1", port);  // Bind to all available interfaces

    if (bind(mSocket, &addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "UDP Bind failed: " << WSAGetLastError() << std::endl;
        return SOCKET_ERROR;
    }

    return 0; // Success
}

int Socket::receiveUDP(std::string& outMessage)
{
    char buffer[512];
    sockaddr_in senderAddr;
    int senderAddrSize = sizeof(senderAddr);

    int bytesReceived = recvfrom(mSocket, buffer, sizeof(buffer) - 1, 0, reinterpret_cast<sockaddr*>(&senderAddr), &senderAddrSize);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "UDP receive failed: " << WSAGetLastError() << std::endl;
        return SOCKET_ERROR;
    }

    buffer[bytesReceived] = '\0';
    outMessage = buffer;
    return bytesReceived;
}
