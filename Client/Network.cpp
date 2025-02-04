#include "Network.h"
#include "PongPackets.h"

int Network::connect(const char* ip)
{
    m_socketTCP.createSocketTCP();
    if (m_socketTCP.connectTCP(ip, serverBasePort) == SOCKET_ERROR) {
        std::cerr << "TCP connection failed.\n";
        return -1;
    }
    m_socketUDP.createSocketUDP();
    if (m_socketUDP.bindUDP(0) == SOCKET_ERROR) {
        std::cerr << "UDP socket bind failed.\n";
        return -1;
    }

    memset(&serverUDPAddr, 0, sizeof(serverUDPAddr));
    serverUDPAddr.sin_family = AF_INET;
    serverUDPAddr.sin_port = htons(serverSecondaryPort);
    inet_pton(AF_INET, ip, &serverUDPAddr.sin_addr);

    return 0;
}

int Network::sendPosition(std::string id, sf::Vector2i m_Position)
{
    struct PositionPacket {
        int x;
        int y;
    } packet{ m_Position.x, m_Position.y };

    bool success = network::sendPacketUDP(m_socketUDP, reinterpret_cast<sockaddr*>(&serverUDPAddr), (uint32_t)ClientPackets::PlayerMove, packet);

    if (!success) {
        std::cerr << "Failed to send position packet.\n";
        return -1;
    }

    return 0;
    
}
