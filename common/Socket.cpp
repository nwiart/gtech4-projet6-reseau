#include "Socket.h"
#include "Networking.h"
#include <iostream>


Socket::Socket()
    : mSocket(INVALID_SOCKET)
{

}

Socket::Socket(SOCKET s)
    : mSocket(s)
{

}

Socket::~Socket() {
    if (mSocket != INVALID_SOCKET) {
        //closesocket(mSocket);
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

void Socket::acceptTCP(Socket& outSocket)
{
    sockaddr clientAddr;
    int addrLen = sizeof(clientAddr);

    SOCKET clientSocket = accept(mSocket, &clientAddr, &addrLen);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "TCP accept failed: " << WSAGetLastError() << std::endl;
        outSocket.mSocket = INVALID_SOCKET;
        return;
    }

    std::cout << "Accepted new client socket: " << clientSocket << std::endl;
    outSocket.mSocket = clientSocket; // Store the new socket
}

int Socket::bindUDP(uint16_t port)
{
    sockaddr addr;
    network::getServerAddressUDP(&addr, "0.0.0.0", port);

    if (bind(mSocket, &addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "UDP Bind failed: " << WSAGetLastError() << std::endl;
        return SOCKET_ERROR;
    }

    return 0;
}


bool Socket::isValid() const
{
    return mSocket != INVALID_SOCKET;
}

//std::string Socket::receiveUDP()
//{
//    char buffer[512];
//    sockaddr_in senderAddr;
//    int senderAddrSize = sizeof(senderAddr);
//
//    int bytesReceived = recvfrom(mSocket, buffer, sizeof(buffer) - 1, 0, reinterpret_cast<sockaddr*>(&senderAddr), &senderAddrSize);
//    if (bytesReceived == SOCKET_ERROR) {
//        std::cerr << "UDP receive failed: " << WSAGetLastError() << std::endl;
//        return "SOCKET_ERROR";
//    }
//
//    buffer[bytesReceived] = '\0';
//    return std::string(buffer);
//}
