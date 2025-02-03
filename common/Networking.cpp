#include "Networking.h"
#include <iostream>

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

int network::getServerAddressUDP(struct sockaddr* out, PCSTR PORT) {
    struct addrinfo* result = nullptr, * ptr = nullptr, hints;

    // Set up hints for getaddrinfo
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_DGRAM;  // UDP
    hints.ai_protocol = IPPROTO_UDP;

    int iResult = getaddrinfo(serverAddress, PORT, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed with error: " << iResult << std::endl;
        return SOCKET_ERROR;
    }

    for (ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        memcpy(out, ptr->ai_addr, sizeof(sockaddr));
    }

    freeaddrinfo(result);
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


    /*struct addrinfo* result = nullptr, * ptr = nullptr, hints;

    // Set up hints for getaddrinfo
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_STREAM;  // UDP
    hints.ai_protocol = IPPROTO_TCP;

    int iResult = getaddrinfo(serverAddress.c_str(), PORT, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed with error: " << iResult << std::endl;
        return SOCKET_ERROR;
    }

    for (ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        memcpy(out, ptr->ai_addr, sizeof(sockaddr));
    }

    freeaddrinfo(result);
    return 0;*/
}

void network::sendSocketUDP()
{
    int iResult;
    Socket clientSocket;
    clientSocket.createSocketUDP();

    iResult = sendto(clientSocket.mSocket, "Test", 5, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
        cleanupWinsock();
        return;
    }
}

void network::sendSocketTCP()
{
}
