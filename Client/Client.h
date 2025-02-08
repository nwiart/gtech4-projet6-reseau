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

public:

    Client(): m_playerID(-1)
    { }

    void init();

    int connect(const char* serverIP, const std::string& playerName);

    void pollEvents();

    void handleTCPPacket(uint32_t packetID);
    void handleUDPPacket();

    inline Socket& getServerTCP() { return m_socketTCP; }

public:

    void signalConnectionLost(int err);
    void signalGameStart();

    void createLobbyPong1v1(const std::string& name);
    void createLobbyPong2v2(const std::string& name);
    void startGame();

    void joinLobby(uint32_t lobbyID);
    void leaveLobby();

    int sendPosition(int posY);

    inline const std::string& getPlayerName() const { return m_playerName; }
    inline uint32_t getPlayerID() const { return m_playerID; }
    inline Lobby& getLobby() { return m_lobby; }

private:

    void completeAuthentication(uint32_t playerID);

    void createLobby(GameMode gm, const std::string& name);

private:

    static const uint16_t serverBasePort = 27014;
    static const uint16_t serverSecondaryPort = serverBasePort+1;

    Lobby m_lobby;

    static Socket m_socketTCP;
    static Socket m_socketUDP;
    static sockaddr_in serverUDPAddr;
    uint32_t m_playerID;
    std::string m_playerName;
};


#include "Client.inl"
