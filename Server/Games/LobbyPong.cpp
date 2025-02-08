#include "LobbyPong.h"
#include <iostream>

#include "Networking.h"
#include "Server.h"

#include "PongPackets.h"
#include "Pong/Ball.h"

#include "Server.h"

LobbyPong::LobbyPong(uint32_t id, bool twoPlayersTeam)
	: Lobby(id), m_pong(1280, 720), m_twoPlayerTeams(twoPlayersTeam), gameStarted(false)
{
}

uint32_t LobbyPong::addPlayer(ClientConnection *conn)
{
	return Lobby::addPlayer(conn);
}

uint32_t LobbyPong::getPlayerID(uint64_t socket) const
{
	auto it = m_players.find(socket);
	if (it != m_players.end())
	{
		return it->second.m_inLobbyID;
	}
	return -1;
}

bool LobbyPong::hasGameStarted() const
{
	return gameStarted;
}

void LobbyPong::start()
{
	if (gameStarted)
		return;
	gameStarted = true;
	m_pong.startGame();

	// Signal everyone that the game has started.
	Server_GameStart packet;
	packet.started = true;
	for (auto &p : m_players)
	{
		network::sendPacketTCP(p.second.m_client->getSocket(), (uint32_t)ServerPackets::GameStart, packet);
	}

	std::cout << "The game starts!" << std::endl;
}

void LobbyPong::update(float dt)
{
	if (!gameStarted)
		return;

	m_pong.update(dt);
	sendGameState();
}

void LobbyPong::receivePlayerMove(uint32_t playerID, float positionY)
{
	m_pong.receivePlayerMove(playerID, positionY);

	Server_PlayerMove p;
	p.playerID = playerID;
	p.position = positionY;
	for (auto &pl : m_players)
	{
		const sockaddr_in *clientAddr = Server::m_instance->getClientBySocket(pl.second.m_client->getSocket().mSocket)->getUDPAddr();
		network::sendPacketUDP(Server::m_instance->getUDPSocket(), (const sockaddr *)clientAddr, (uint32_t)ServerPackets::PlayerMove, p);
	}
}

void LobbyPong::sendGameState()
{
	float x, y, xDir, yDir, speed;
	int score1, score2;

	m_pong.getBallInfo(x, y, xDir, yDir, speed);
	m_pong.getScoreInfo(score1, score2);

	Server_ScoreInfo scorePacket;
	scorePacket.score1 = score1;
	scorePacket.score2 = score2;

	Server_BallInfo packet;
    packet.xPos = x;
    packet.yPos = y;
	packet.xDir = xDir;
	packet.yDir = yDir;
	packet.speed = speed;

	for (auto &player : m_players)
	{
		const sockaddr *clientAddr = (const sockaddr *)Server::m_instance->getClientBySocket(player.second.m_client->getSocket().mSocket)->getUDPAddr();
		network::sendPacketUDP(Server::m_instance->getUDPSocket(), clientAddr, (uint32_t)ServerPackets::BallInfo, packet);
		network::sendPacketUDP(Server::m_instance->getUDPSocket(), clientAddr, (uint32_t)ServerPackets::ScoreInfo, scorePacket);
	}
}