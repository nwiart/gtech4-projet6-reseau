#include "Networking.h"
#include <iostream>



network::network()
{
    initializeWinsock();
    udpSocket.createSocketUDP();
    tcpSocket.createSocketTCP();
    udpSocket.bindSocketUDP(UDPPort);
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
    struct addrinfo hints {}, * result = nullptr;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    int iResult = getaddrinfo(serverAddress.c_str(), UDPPort, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed with error: " << iResult << std::endl;
        return SOCKET_ERROR;
    }

    memcpy(&serverAddr, result->ai_addr, result->ai_addrlen);
    freeaddrinfo(result);
    return 0;
}

int network::getServerAddressTCP()
{
    struct addrinfo hints {}, * result = nullptr;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int iResult = getaddrinfo(serverAddress.c_str(), TCPPort, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed with error: " << iResult << std::endl;
        return SOCKET_ERROR;
    }

    memcpy(&serverAddr, result->ai_addr, result->ai_addrlen);
    freeaddrinfo(result);
    return 0;
}

void network::sendSocketUDP(std::string message)
{
    int iResult = sendto(udpSocket.mSocket, message.c_str(), message.length(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
    }
}

void network::sendSocketTCP(std::string message)
{
    int iResult = send(tcpSocket.mSocket, message.c_str(), message.length(), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed with error: " << WSAGetLastError() << std::endl;
    }
}
