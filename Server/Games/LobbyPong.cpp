#include "LobbyPong.h"
#include <iostream>

LobbyPong::LobbyPong(uint32_t id, bool twoPlayersTeam)
    : Lobby(id), m_pong(1280, 720), m_twoPlayerTeams(twoPlayersTeam) {}

uint32_t LobbyPong::addPlayer(uint64_t socket) {
    if (m_players.size() >= getMaxPlayers()) {
        std::cout << "Lobby complet !" << std::endl;
        return -1;
    }

    uint32_t playerID = m_players.size() + 1;
    m_players[playerID] = Socket(socket);
    m_paddles[socket] = playerID;

    std::cout << "Joueur " << playerID << " ajouté !" << std::endl;

    if (m_players.size() == getMaxPlayers()) {
        start();
    }

    return playerID;
}

uint32_t LobbyPong::getPlayerID(uint64_t socket) const {
    auto it = m_paddles.find(socket);
    if (it != m_paddles.end()) {
        return it->second;
    }
    return -1;
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
}

void LobbyPong::sendGameState() {
    float ballX, ballY, ballRadius;
    float paddle1Y, paddle2Y;
    int score1, score2;

    getGameState(ballX, ballY, ballRadius, paddle1Y, paddle2Y, score1, score2);

    for (auto& player : m_players) {
        Socket& socket = player.second;

        socket.sendData(&ballX, sizeof(ballX));
        socket.sendData(&ballY, sizeof(ballY));
        socket.sendData(&ballRadius, sizeof(ballRadius));
        socket.sendData(&paddle1Y, sizeof(paddle1Y));
        socket.sendData(&paddle2Y, sizeof(paddle2Y));
        socket.sendData(&score1, sizeof(score1));
        socket.sendData(&score2, sizeof(score2));
    }
}


void LobbyPong::getGameState(float& ballX, float& ballY, float& ballRadius,
    float& paddle1Y, float& paddle2Y,
    int& score1, int& score2) const {
    m_pong.getGameState(ballX, ballY, ballRadius, paddle1Y, paddle2Y, score1, score2);
}

