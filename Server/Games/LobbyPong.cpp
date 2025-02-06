#include "LobbyPong.h"

#include "Networking.h"
#include "PongPackets.h"


LobbyPong::LobbyPong(bool twoPlayersTeam)
	: m_pong(1280, 720), m_twoPlayerTeams(twoPlayersTeam)
{

}

void LobbyPong::start()
{
    Server_GameStart p;
    p.started = true;

    for (auto p : m_players) {
        network::sendPacketTCP(p.second, (uint32_t)ServerPackets::GameStart, p);
    }

    gameStarted = true;
    std::cout << "Le match de Pong commence !" << std::endl;
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

uint32_t LobbyPong::getPlayerID(uint64_t socket) const
{
    for (auto p : m_players) {
        if (p.second.mSocket == socket) return p.first;
    }
    return -1;
}

void LobbyPong::update(float dt) {
    std::cout << "[DEBUG] gameStarted l'update est arrêtée !" << gameStarted << std::endl;

    if (!gameStarted) {
       std::cout << "[DEBUG] gameStarted est FAUX, l'update est arrêtée !" << std::endl;
       return;
    }
    std::cout << "[DEBUG] gameStarted est VRAI, update continue." << std::endl;

    m_pong.update(dt);
    m_ball.update(dt, &m_pong);
    int scoreJ1 = m_pong.getScoreP1();
    int scoreJ2 = m_pong.getScoreP2();
    std::cout << "[INFO] Score - Joueur 1 : " << scoreJ1
        << " | Joueur 2 : " << scoreJ2 << std::endl;

    std::cout << "[INFO] Position balle : X=" << m_ball.getPosition().x
        << " | Y=" << m_ball.getPosition().y << std::endl;
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
