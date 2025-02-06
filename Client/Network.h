#pragma once
#include "Networking.h"
#include "Socket.h"

#include "GameMode.h"


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

    static int connect(const char* ip,const char* playerName);
    static void createLobbyPong1v1(const std::string& name);
    static void createLobbyPong2v2(const std::string& name);

    int sendPosition(float posY);

    static inline Socket& getServerTCP() { return m_socketTCP; }

private:

    static void createLobby(GameMode gm, const std::string& name);

private:

    static const uint16_t serverBasePort = 27014;
    static const uint16_t serverSecondaryPort = serverBasePort+1;

    static Socket m_socketTCP;
    static Socket m_socketUDP;
    static sockaddr_in serverUDPAddr;
    int m_playerID;

};