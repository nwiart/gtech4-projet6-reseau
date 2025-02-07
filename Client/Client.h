#pragma once

#include "Networking.h"
#include "Socket.h"

#include "GameMode.h"


class Client
{
public:

    static inline Client& getInstance();

public:

    Client(): m_playerID(-1)
    { }

    void init();

    int connect(const char* serverIP, const char* playerName);

    void pollEvents();

    void handleTCPPacket(uint32_t packetID);
    void handleUDPPacket();

    inline Socket& getServerTCP() { return m_socketTCP; }

public:

    void createLobbyPong1v1(const std::string& name);
    void createLobbyPong2v2(const std::string& name);
    void startGame();

    void joinLobby(uint32_t lobbyID);

    int sendPosition(int posY);

private:

    void completeAuthentication(uint32_t playerID);

    void createLobby(GameMode gm, const std::string& name);

private:

    static const uint16_t serverBasePort = 27014;
    static const uint16_t serverSecondaryPort = serverBasePort+1;

    static Socket m_socketTCP;
    static Socket m_socketUDP;
    static sockaddr_in serverUDPAddr;
    uint32_t m_playerID;
};


#include "Client.inl"
