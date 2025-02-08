#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <stdint.h>

#include "Socket.h"

#define MAX_PACKET_SIZE 1024


#define UDPPort 27015
#define TCPPort 27014

namespace network {
    int initializeWinsock();
    void cleanupWinsock();
    int getServerAddressUDP(sockaddr* out, const char* ip, uint16_t port);
    int getServerAddressTCP(sockaddr* out, const char* ip, uint16_t port);
    size_t sendPacketTCP(Socket& s, const void* buf, size_t size);
    size_t sendPacketUDP(Socket& s, const sockaddr* addr, const void* buf, size_t size);
    bool fetchTCPID(Socket& s, uint32_t& packetID);

    template<typename T>
    bool sendPacketTCP(Socket& s, uint32_t packetID, const T& data);

    template<typename T>
    bool sendPacketUDP(Socket& s, const sockaddr* addr, uint32_t packetID, const T& data);

    template<typename T>
    bool receivePacketTCP(Socket& s, uint32_t& packetID, T& data);

    template<typename T>
    bool receivePacketUDP(Socket& s, sockaddr_in* senderAddr, T& data);
}


#include "Networking.inl"
