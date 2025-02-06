#include "LobbyPong.h"


LobbyPong::LobbyPong(bool twoPlayersTeam)
	: m_pong(1280, 720), m_twoPlayerTeams(twoPlayersTeam)
{

}

uint32_t LobbyPong::addPlayer(uint64_t id)
{
    if (m_players.size() >= this->getMaxPlayers()) {
        return -1;
    }

    uint32_t pid = m_players.size();
    m_players[pid] = id;

    return pid;
}

void LobbyPong::addPlayer(uint32_t playerID, Socket playerSocket) {
    const int maxPlayers = this->getMaxPlayers();
    if (m_players.size() < maxPlayers) {
        m_players[playerID] = playerSocket;

        if (m_players.size() == maxPlayers) {
            startGame();
        }
    }
}

void LobbyPong::startGame() {
    gameStarted = true;
    std::cout << "Le match de Pong commence !" << std::endl;
}

void LobbyPong::update(float dt) {
    if (!gameStarted) return;

    m_pong.update(dt);
}

void LobbyPong::receivePlayerMove(uint32_t playerID, float positionY) {
    if (!gameStarted) return;

    if (playerID == 0) {
        m_pong.setPaddlePositions(positionY, m_pong.getSizeY() / 2);
    }
    else if (playerID == 1) {
        m_pong.setPaddlePositions(m_pong.getSizeY() / 2, positionY);
    }
}

