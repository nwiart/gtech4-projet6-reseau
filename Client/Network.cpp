#include "Network.h"

int Network::connect(const char* ip)
{
    m_serverTCP.createSocketTCP();
    m_serverUDP.createSocketUDP();
    return m_serverTCP.connectTCP(ip, serverBasePort);
}
