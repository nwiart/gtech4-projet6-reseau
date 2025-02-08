#include "Networking.h"
#include <iostream>

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


size_t network::sendPacketTCP(Socket& s, const void* buf, size_t size) {
    const char* data = static_cast<const char*>(buf);
    size_t totalSent = 0;

    while (totalSent < size) {
        int bytesSent = send(s.mSocket, data + totalSent, size - totalSent, 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "send failed! WSA Error: " << WSAGetLastError() << std::endl;
            return 0;
        }
        totalSent += bytesSent;
    }
    return totalSent;
}

size_t network::sendPacketUDP(Socket& s, const sockaddr* addr, const void* buf, size_t size)
{
    int iResult = sendto(s.mSocket, (const char*)buf, size, 0, addr, sizeof(sockaddr_in));
    if (iResult == SOCKET_ERROR) {
        return 0;
    }

    return size;
}

bool network::fetchTCPID(Socket& s, uint32_t& packetID) {
    char buffer[MAX_PACKET_SIZE];
    int receivedBytes = recv(s.mSocket, buffer, sizeof(buffer), 0);

    if (receivedBytes == SOCKET_ERROR) {
        std::cerr << "Error receiving packet TCP! WSA Error: " << WSAGetLastError() << std::endl;
        return false;
    }

    if (receivedBytes < sizeof(uint32_t)) {
        std::cerr << "Invalid packet received (too small)\n";
        return false;
    }

    memcpy(&packetID, buffer, sizeof(uint32_t));
    packetID = ntohl(packetID);

    return true;
}