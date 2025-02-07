#include "LobbyPong.h"
#include <iostream>

#include "Networking.h"
#include "Server.h"

#include "PongPackets.h"
#include "Pong/Ball.h"


LobbyPong::LobbyPong(uint32_t id, bool twoPlayersTeam)
    : Lobby(id), m_pong(1280, 720), m_twoPlayerTeams(twoPlayersTeam), gameStarted(false) {}

uint32_t LobbyPong::addPlayer(uint64_t socket) {
    if (m_players.size() >= getMaxPlayers()) {
        std::cout << "Lobby complet !" << std::endl;
        return -1;
    }

    uint32_t playerID = m_players.size();
    m_players[playerID] = Socket(socket);
    m_paddles[socket] = playerID;

    std::cout << "Joueur " << playerID << " ajouté !" << std::endl;

    return playerID;
}

uint32_t LobbyPong::getPlayerID(uint64_t socket) const {
    auto it = m_paddles.find(socket);
    if (it != m_paddles.end()) {
        return it->second;
    }
    return -1;
}

bool LobbyPong::hasGameStarted() const
{
    return gameStarted;
}


void LobbyPong::start() {
    if (gameStarted) return;
    gameStarted = true;
    m_pong.startGame();
    std::cout << "La partie commence !" << std::endl;
}

void LobbyPong::update(float dt) {
    if (!gameStarted) return;

    m_pong.update(dt);
    sendGameState();
}

void LobbyPong::receivePlayerMove(uint32_t playerID, float positionY) {
    m_pong.receivePlayerMove(playerID, positionY);

    Server_PlayerMove p;
    p.playerID = playerID;
    p.position = positionY;
    for (auto& p : m_players) {
        const sockaddr* clientAddr = Server::m_instance->getClientBySocket(p.second.mSocket)->getIP();
        network::sendPacketUDP(Server::m_instance->getUDPSocket(), clientAddr, (uint32_t)ServerPackets::PlayerMove, p);
    }
}

void LobbyPong::sendGameState() {
    float ballX, ballY, ballRadius;
    float paddle1Y, paddle2Y;
    int score1, score2;

    getGameState(ballX, ballY, ballRadius, paddle1Y, paddle2Y, score1, score2);

    const sf::Vector2f& p = m_pong.getBall().getPosition();
    const sf::Vector2f& v = m_pong.getBall().getVelocity();
    Server_BallInfo packet;
    packet.xPos = p.x; packet.yPos = p.y;
    packet.xVel = v.x; packet.yVel = v.y;

    for (auto& player : m_players) {
        const sockaddr* clientAddr = Server::m_instance->getClientBySocket(player.second.mSocket)->getIP();
        network::sendPacketUDP(Server::m_instance->getUDPSocket(), clientAddr, (uint32_t)ServerPackets::BallInfo, packet);
    }
}


void LobbyPong::getGameState(float& ballX, float& ballY, float& ballRadius,
    float& paddle1Y, float& paddle2Y,
    int& score1, int& score2) const {
    m_pong.getGameState(ballX, ballY, ballRadius, paddle1Y, paddle2Y, score1, score2);
}

