#pragma once

#include "Networking.h"
#include "Socket.h"

#include "GameMode.h"
#include "Lobby.h"

#include <string>


class Client
{
public:
    static inline Client& getInstance();

    Client(): m_playerID(-1)
    { }

    void init();

    void pollEvents();

    int connect(const char* serverIP, const std::string& playerName);

    void createLobbyPong1v1(const std::string& name);
    void createLobbyPong2v2(const std::string& name);
    void createLobby(GameMode gm, const std::string& name);

    void handleTCPPacket(uint32_t PacketID);

    Socket& getSocketTCP() { return m_socketTCP; }
    Socket& getSocketUDP() { return m_socketUDP; }
    sockaddr_in& getServerAddr() { return serverUDPAddr; }

    const std::string& getPlayerName() const { return m_playerName; }
    uint32_t getPlayerID() const { return m_playerID; }
    Lobby& getLobby() { return m_lobby; }

private:

    static const uint16_t serverBasePort = 27014;
    static const uint16_t serverSecondaryPort = serverBasePort+1;

    Lobby m_lobby;

    Socket m_socketTCP;
    Socket m_socketUDP;
    sockaddr_in serverUDPAddr;
    uint32_t m_playerID;
    std::string m_playerName;
};


#include "Client.inl"
