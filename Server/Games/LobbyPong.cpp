#include "LobbyPong.h"


LobbyPong::LobbyPong(bool twoPlayersTeam)
	: m_pong(1280, 720), m_twoPlayerTeams(twoPlayersTeam)
{

}

void LobbyPong::addPlayer(uint32_t playerID, Socket playerSocket) {
    if (m_players.size() < 2) {
        m_players[playerID] = playerSocket;

        if (m_players.size() == 2) {
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

