#pragma once
#include "Networking.h"
#include "Socket.h"
#include "SFMLInclude.h"

class Network
{
public:

    Network() { }

    int connect(const char* ip);

    int sendPosition(std::string id, sf::Vector2i m_Position);

private:

    static const uint16_t serverBasePort = 27014;
    static const uint16_t serverSecondaryPort = serverBasePort+1;

    Socket m_socketTCP;
    Socket m_socketUDP;
    sockaddr_in serverUDPAddr;
};