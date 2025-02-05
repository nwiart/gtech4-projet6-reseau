#pragma once
#include "Networking.h"
#include "Socket.h"

class Network
{
public:

    Network(): m_playerID(-1) 
    { }

    static void init();
    static void pollEvents();

    static void handleTCPPacket(uint32_t packetID);

    static void receiveUDPPackets();

    static void handleUDPPacket(uint32_t packetID);

    int connect(const char* ip,const char* playerName);

    int sendPosition(float posY);

    static inline Socket& getServerTCP() { return m_socketTCP; }

private:

    static const uint16_t serverBasePort = 27014;
    static const uint16_t serverSecondaryPort = serverBasePort+1;

    static Socket m_socketTCP;
    static Socket m_socketUDP;
    sockaddr_in serverUDPAddr;
    int m_playerID;

};