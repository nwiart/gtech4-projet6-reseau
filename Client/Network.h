#pragma once
#include "Networking.h"
#include "Socket.h"
#include "SFMLInclude.h"

class Network
{
public:

    Network(): m_playerID(-1) 
    { }

    int connect(const char* ip,char playerName[32]);

    int sendPosition(float posY);

private:

    static const uint16_t serverBasePort = 27014;
    static const uint16_t serverSecondaryPort = serverBasePort+1;

    Socket m_socketTCP;
    Socket m_socketUDP;
    sockaddr_in serverUDPAddr;
    int m_playerID;

};