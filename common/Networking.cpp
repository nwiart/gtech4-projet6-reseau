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

void network::initializeWinsock() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed with error: " << iResult << std::endl;
        exit(1);
    }
}

void network::cleanupWinsock() {
    WSACleanup();
}

int network::getServerAddressUDP() {
    struct addrinfo* result = nullptr, * ptr = nullptr, hints;

    // Set up hints for getaddrinfo
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;  // UDP
    hints.ai_protocol = IPPROTO_UDP;

    int iResult = getaddrinfo(serverAddress.c_str(), UDPPort, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed with error: " << iResult << std::endl;
        return SOCKET_ERROR;
    }

    for (ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        memcpy(&serverAddr, ptr->ai_addr, sizeof(*(&serverAddr)));
    }

    freeaddrinfo(result);
    return 0;
}

int network::getServerAddressTCP()
{
    struct addrinfo* result = nullptr, * ptr = nullptr, hints;

    // Set up hints for getaddrinfo
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_STREAM;  // UDP
    hints.ai_protocol = IPPROTO_TCP;

    int iResult = getaddrinfo(serverAddress.c_str(), TCPPort, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed with error: " << iResult << std::endl;
        return SOCKET_ERROR;
    }

    for (ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        memcpy(&serverAddr, ptr->ai_addr, sizeof(*(&serverAddr)));
    }

    freeaddrinfo(result);
    return 0;
}

void network::sendSocketUDP(std::string message)
{
    int iResult;
    Socket clientSocket;
    clientSocket.createSocketUDP();

    iResult = sendto(clientSocket.mSocket, message.c_str(), message.length(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
        cleanupWinsock();
        return;
    }
}

void network::sendSocketTCP(std::string message)
{
    int iResult;
    Socket clientSocket;
    clientSocket.createSocketTCP();

    iResult = send(clientSocket.mSocket, message.c_str(), message.length(), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
        cleanupWinsock();
        return;
    }
}
