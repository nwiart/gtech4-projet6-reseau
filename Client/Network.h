#pragma once
#include "Networking.h"
#include "Socket.h"

class Network
{
public:

    Network() { }

    int connect(const char* ip);

private:

    static const uint16_t serverBasePort = 27014;
    static const uint16_t serverSecondaryPort = 27015;

    Socket m_serverTCP;
    Socket m_serverUDP;
};