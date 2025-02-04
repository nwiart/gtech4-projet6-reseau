#include "Network.h"
#include "PongPackets.h"

int Network::connect(const char* ip,char playerName[32])
{
    m_socketTCP.createSocketTCP();
    if (m_socketTCP.connectTCP(ip, serverBasePort) == SOCKET_ERROR) {
        std::cerr << "TCP connection failed.\n";
        return -1;
    }

    Client_PlayerConnect packet{ *playerName };

    bool success = network::sendPacketTCP(m_socketTCP, (uint32_t)ClientPackets::PlayerConnect, packet);
    if (!success) {
        return -1;
    }

    Server_ConnectResult connectResult;
    recv(m_socketTCP.mSocket, (char*)&connectResult, sizeof(connectResult), 0);

    if (!connectResult.success)
    {
        std::cerr << "Connection refused by server!" << std::endl;
        return -1;
    }

    m_playerID = connectResult.playerID;


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

int Network::sendPosition(float position)
{
    Client_PlayerMove packet{ position };

    bool success = network::sendPacketUDP(m_socketUDP, reinterpret_cast<sockaddr*>(&serverUDPAddr), static_cast<uint32_t>(ClientPackets::PlayerMove), packet);

    if (!success)
    {
        std::cerr << "Failed to send position packet.\n";
        return -1;
    }

    return 0;
}
