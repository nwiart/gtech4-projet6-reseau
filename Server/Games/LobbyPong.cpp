#include "LobbyPong.h"

#include "Networking.h"
#include "PongPackets.h"


LobbyPong::LobbyPong(uint32_t id, bool twoPlayersTeam)
	: Lobby(id), m_pong(1280, 720), m_twoPlayerTeams(twoPlayersTeam)
{

}

void LobbyPong::start()
{
    Server_GameStart p;
    p.started = true;

    for (auto pl : m_players) {
        network::sendPacketTCP(pl.second, (uint32_t)ServerPackets::GameStart, p);
    }

    gameStarted = true;
    m_pong.startGame();

    std::cout << "Le match de Pong commence !" << std::endl;
}

uint32_t LobbyPong::addPlayer(uint64_t playerSocket) {
    if (m_players.size() >= this->getMaxPlayers()) {
        return -1;
    }

    uint32_t playerID = m_players.size();

    m_players[playerID] = playerSocket;
    m_paddles[playerSocket] = playerID;

    std::cout << "Joueur " << playerSocket << " assigné au paddle " << playerID << std::endl;

    return playerID;
}


uint32_t LobbyPong::getPlayerID(uint64_t socket) const
{
    for (auto p : m_players) {
        if (p.second.mSocket == socket) return p.first;
    }
    return -1;
}

void LobbyPong::update(float dt) {
    if (!gameStarted) {
       return;
    }

    m_pong.update(dt);

    int scoreJ1 = m_pong.getScoreP1();
    int scoreJ2 = m_pong.getScoreP2();
    //std::cout << "[INFO] Score - Joueur 1 : " << scoreJ1
    //    << " | Joueur 2 : " << scoreJ2 << std::endl;

    //std::cout << "[INFO] Position balle : X=" << m_pong.getBall().getPosition().x
    //    << " | Y=" << m_pong.getBall().getPosition().y << std::endl;
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

void LobbyPong::sendGameState() {
    for (auto& player : m_players) {
        // Get player-specific address and send the necessary packets


        // For example, send PlayerMove
        Server_PlayerMove playerMovePacket;
        playerMovePacket.playerID = player.first; // Player ID
        playerMovePacket.position = player.second.getPosition(); // Player's paddle position
        network::sendPacketUDP(player.second, &player.second.mSocket, (uint32_t)ServerPackets::PlayerMove, playerMovePacket);

        // Send Ball info
        Server_BallInfo ballInfoPacket;
        ballInfoPacket.xPos = m_pong.getBall().getPosition().x;
        ballInfoPacket.yPos = m_pong.getBall().getPosition().y;
        ballInfoPacket.xVel = m_pong.getBall().getVelocity().x;
        ballInfoPacket.yVel = m_pong.getBall().getVelocity().y;
        network::sendPacketUDP(player.second, &player.second.mSocket, (uint32_t)ServerPackets::BallInfo, ballInfoPacket);

        // Send Score
        Server_Score scorePacket;
        scorePacket.team = m_pong.getScoreP1();  // Or use appropriate score calculation
        network::sendPacketUDP(player.second, &player.second.mSocket, (uint32_t)ServerPackets::Score, scorePacket);
    }
}