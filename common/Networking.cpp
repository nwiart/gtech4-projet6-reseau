#include "Networking.h"
#include <iostream>

network::network()
{
    initializeWinsock();
}

network::~network()
{
    cleanupWinsock();
}

int network::initializeWinsock() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed with error: " << iResult << std::endl;
    }
    return iResult;
}

void network::cleanupWinsock() {
    WSACleanup();
}

int network::getServerAddressUDP(struct sockaddr* out, const char* ip, uint16_t port)
{
    struct sockaddr_in* inetaddr = reinterpret_cast<sockaddr_in*>(out);
    ZeroMemory(inetaddr->sin_zero, sizeof(inetaddr->sin_zero));
    inetaddr->sin_family = AF_INET;
    inetaddr->sin_addr.S_un.S_addr = inet_addr(ip);
    inetaddr->sin_port = htons(port);

    return 0;
}

int network::getServerAddressTCP(struct sockaddr* out, const char* ip, uint16_t port)
{
    struct sockaddr_in* inetaddr = reinterpret_cast<sockaddr_in*>(out);
    ZeroMemory(inetaddr->sin_zero, sizeof(inetaddr->sin_zero));
    inetaddr->sin_family = AF_INET;
    inetaddr->sin_addr.S_un.S_addr = inet_addr(ip);
    inetaddr->sin_port = htons(port);

    return 0;
}

void network::sendPacketUDP(Socket& s, std::string message)
{
    int iResult;

    iResult = sendto(s.mSocket, message.c_str(), message.length(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
        cleanupWinsock();
        return;
    }
}

void network::sendPacketTCP(Socket& s, std::string message)
{
    int iResult;

    iResult = send(s.mSocket, message.c_str(), message.length(), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
        cleanupWinsock();
        return;
    }
}


size_t network::sendPacketTCP(Socket& s, const void* buf, size_t size)
{
    int iResult;

    iResult = send(s.mSocket, (const char*) buf, size, 0);
    if (iResult == SOCKET_ERROR) {
        return 0;
    }

    return size;
}

size_t network::sendPacketUDP(Socket& s, const sockaddr* addr, const void* buf, size_t size)
{
    int iResult;

    iResult = sendto(s.mSocket, (const char*) buf, size, 0, addr, sizeof(sockaddr));
    if (iResult == SOCKET_ERROR) {
        return 0;
    }

    return size;
}
