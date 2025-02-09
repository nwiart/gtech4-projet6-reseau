#include "Server.h"

#include "Games/Lobby.h"
#include "Games/LobbyPong.h"

#include "PongPackets.h"

#include "Networking.h"

#include <WinSock2.h>
#include <stdlib.h>

Server *Server::m_instance = 0;

Server::Server()
	: m_lobbyUID(0), m_clientUID(0)
{
	m_instance = this;
}

void Server::open()
{
	m_socketListener.createSocketTCP();
	m_socketListener.listenTCP(serverBasePort);

	m_socketUDP.createSocketUDP();
	m_socketUDP.bindUDP(serverSecondaryPort - 1);
}

bool Server::notifyConnect(Socket& clientSocketTCP)
{
	if (!clientSocketTCP.isValid()) {
		std::cerr << "notifyConnect: Received an invalid client socket!\n";
		return false;
	}

	if (m_clients.find(clientSocketTCP.mSocket) != m_clients.end()) {
		std::cerr << "notifyConnect: Client socket already exists!\n";
		return false;
	}

	// Store the client socket in m_clients (COPY, don't move)
	ClientConnection conn;
	conn.m_socket = clientSocketTCP;  // Copy the socket instead of moving it
	conn.m_id = -1;
	conn.m_name = "PLAYER_CONNECTING";

	m_clients[clientSocketTCP.mSocket] = conn;  // Store properly
	std::cout << "Client stored. Socket: " << clientSocketTCP.mSocket << "\n";

	return true;
}

void Server::notifyDisconnect(SOCKET clientSocketTCP)
{
	auto it = m_clients.find(clientSocketTCP);
	if (it == m_clients.end()) {
		std::cerr << "Error: Attempted to disconnect an unknown client socket!\n";
		return;
	}

	ClientConnection& conn = it->second;

	if (conn.m_lobby) {
		conn.m_lobby->disconnectPlayer(conn.m_id);
		std::cout << "Player " << conn.m_id << " removed from lobby " << conn.m_lobby->getLobbyID() << std::endl;
	}

	shutdown(clientSocketTCP, SD_BOTH);
	closesocket(clientSocketTCP);

	std::cout << "Client " << conn.m_id << " (" << conn.m_name << ") disconnected.\n";

	m_clients.erase(it);
}

uint32_t Server::confirmClient(Socket &clientSocketTCP, const std::string &playerName)
{
	sockaddr_in addr;
	int addrSize = sizeof(addr);
	if (getpeername(clientSocketTCP.mSocket, reinterpret_cast<sockaddr *>(&addr), &addrSize) == SOCKET_ERROR)
	{
		std::cerr << "getpeername failed with error: " << WSAGetLastError() << std::endl;
	}

	uint32_t clientIP = addr.sin_addr.s_addr;

	ClientConnection &conn = m_clients.at(clientSocketTCP.mSocket);

	conn.m_id = m_clientUID;
	conn.m_name = playerName;
	conn.setIP(addr);

	m_clientUID++;

	return conn.m_id;
}

void Server::createLobby(Socket &initiator, const std::string &name, GameMode gm)
{
	Lobby *lobby = 0;

	auto it = m_clients.find(initiator.mSocket);
	if (it == m_clients.end() || it->second.getLobby() != 0)
	{
		Server_LobbyCreation p;
		p.success = false;
		network::sendPacketTCP(initiator, (uint32_t)ServerPackets::LobbyCreation, p);

		return;
	}

	uint32_t id = m_lobbyUID;
	switch (gm)
	{
	case GameMode::PONG_1v1:
	{
		LobbyPong *pong = new LobbyPong(id, false);
		pong->init(name);
		lobby = pong;
		ClientConnection &conn = m_clients[initiator.mSocket];
		conn.m_lobby = lobby;
	}
	break;
	case GameMode::PONG_2v2:
	{
		LobbyPong *pong = new LobbyPong(id, true);
		pong->init(name);
		lobby = pong;
		ClientConnection &conn = m_clients[initiator.mSocket];
		conn.m_lobby = lobby;
	}
	break;
	}

	m_lobbyUID++;

	if (lobby)
	{
		m_games.push_back(lobby);

		it->second.m_lobby = lobby;
		uint32_t pid = lobby->addPlayer(initiator.mSocket);

		std::cout << "Player \"" << it->second.getName() << "\" created a new lobby \"" << lobby->getName() << "\".\n";

		Server_LobbyCreation p;
		p.success = true;
		p.playerID = pid;
		network::sendPacketTCP(initiator, (uint32_t)ServerPackets::LobbyCreation, p);
	}
	else
	{
		Server_LobbyCreation p;
		p.success = false;
		network::sendPacketTCP(initiator, (uint32_t)ServerPackets::LobbyCreation, p);
	}
}

void Server::joinLobby(Socket &player, Lobby *l)
{
	ClientConnection &conn = m_clients[player.mSocket];

	// Lobby is non-existent.
	if (l == 0)
		return;

	// Player already in a lobby.
	if (conn.getLobby())
		return;

	// Game already started.
	if (l->hasGameStarted())
	{
		Server_DenyJoin p;
		p.reason = ConnectionDenialReason::GAME_STARTED;
		network::sendPacketTCP(player, (uint32_t)ServerPackets::DenyJoin, p);
		return;
	}

	// Everything good, the player can join.
	uint32_t playerID = l->addPlayer(player.mSocket);
	conn.m_lobby = l;

	Server_AcceptJoin p;
	p.playerID = playerID;
	network::sendPacketTCP(player, (uint32_t)ServerPackets::AcceptJoin, p);

	LobbyPong *pongGame = dynamic_cast<LobbyPong *>(l);
	if (pongGame)
	{
		/*Server_GameState state;
		pongGame->getGameState(state.ballX, state.ballY, state.ballRadius,
							   state.paddle1Y, state.paddle2Y,
							   state.scoreP1, state.scoreP2);

		network::sendPacketTCP(player, (uint32_t)ServerPackets::GameState, state);*/
	}
}

void Server::notifyReceiveTCP(SOCKET clientSocketTCP, uint32_t packetID)
{
	if (m_clients.find(clientSocketTCP) == m_clients.end())
	{
		std::cerr << "Client socket not found in server!" << std::endl;
		return;
	}

	ClientConnection& conn = m_clients.at(clientSocketTCP);

	switch ((ClientPackets)packetID)
	{
	case ClientPackets::PlayerConnect:
	{
		Client_PlayerConnect packet;
		if (network::receivePacketTCP(conn.getSocket(), packetID, packet))
		{
			uint32_t playerID = confirmClient(conn.getSocket(), packet.playerName);

			Server_ConnectResult response;
			response.success = true;
			response.playerID = playerID;
			network::sendPacketTCP(conn.getSocket(), (uint32_t)ServerPackets::ConnectResult, response);
		}
	}
	break;

	case ClientPackets::GetLobbies:
	{
		for (Lobby* lobby : m_games)
		{
			Server_GetLobbies response;
			strcpy(response.lobbyName, lobby->getName().c_str());
			response.numPlayers = lobby->getNumPlayers();
			response.maxPlayers = lobby->getMaxPlayers();
			response.lobbyID = lobby->getLobbyID();
			network::sendPacketTCP(conn.getSocket(), (uint32_t)ServerPackets::GetLobbies, response);
		}
	}
	break;

	case ClientPackets::CreateLobby:
	{
		Client_CreateLobby packet;
		if (network::receivePacketTCP(conn.getSocket(), packetID, packet))
		{
			createLobby(conn.getSocket(), packet.lobbyName, packet.gamemode);
		}
	}
	break;

	case ClientPackets::JoinLobby:
	{
		Client_JoinLobby packet;
		if (network::receivePacketTCP(conn.getSocket(), packetID, packet))
		{
			joinLobby(conn.getSocket(), getLobbyByID(packet.lobbyID));
		}
	}
	break;

	case ClientPackets::StartGame:
	{
		Server_GameStart response;
		if (conn.getLobby() == nullptr || conn.getLobby()->getPlayerID(clientSocketTCP) != 0)
		{
			response.started = false;
			network::sendPacketTCP(conn.getSocket(), (uint32_t)ServerPackets::GameStart, response);
			return;
		}
		conn.getLobby()->start();
	}
	break;
	}
}


void Server::notifyReceiveUDP()
{
	char buf[MAX_PACKET_SIZE];
	sockaddr_in clientAddr;
	int addrlen = sizeof(sockaddr);

	int rec = recvfrom(m_socketUDP.mSocket, buf, MAX_PACKET_SIZE, 0, (sockaddr *)&clientAddr, &addrlen);
	int error = WSAGetLastError();

	std::cout << "Receive port " << clientAddr.sin_port << '\n';

	/*ClientConnection* conn = getClientByAddress(*(sockaddr*)&clientAddr);
	if (!conn) {
		std::cout << "Received UDP data, but the client could not be resolved..." << std::endl;
		return;
	}*/

	uint32_t packetID = *((uint32_t *)buf);
	handleUDPPacket(packetID, buf + 4, (sockaddr *)&clientAddr);
}

void Server::handleUDPPacket(uint32_t packetID, char *buf, sockaddr *addr)
{
	switch (static_cast<ClientPackets>(packetID))
	{
	case ClientPackets::PlayerConnectUDP:
	{
		uint32_t id = ((Client_PlayerConnectUDP *)buf)->playerID;
		getClientByID(id)->m_udpAddr = *(sockaddr_in *)addr;
	}
	break;
	case ClientPackets::PlayerMove:
	{
		Client_PlayerMove *packet = reinterpret_cast<Client_PlayerMove *>(buf);
		// std::cout << "Player " << playerID << " moved to " << packet.position << std::endl;

		std::cout << packet->playerID << " : " << packet->position << '\n';

		ClientConnection *conn = getClientByID(packet->playerID);

		LobbyPong *pong = dynamic_cast<LobbyPong *>(conn->getLobby());
		pong->receivePlayerMove(pong->getPlayerID(conn->getSocket().mSocket), packet->position);
	}
	break;

	default:
		std::cerr << "Unknown UDP packet received: " << packetID << std::endl;
		break;
	}
}

void Server::updateGames(float dt)
{
	for (auto it = m_games.begin(); it != m_games.end();)
	{
		(*it)->update(dt);

		if ((*it)->getNumPlayers() == 0)
		{
			std::cout << "Lobby " << (*it)->getLobbyID() << " supprim� (aucun joueur actif)" << std::endl;
			delete *it;
			it = m_games.erase(it);
		}
		else
		{
			++it;
		}
	}
}

Lobby *Server::getLobbyByID(uint32_t id) const
{
	for (Lobby *l : m_games)
	{
		if (l->getLobbyID() == id)
		{
			return l;
		}
	}
	return 0;
}

ClientConnection *Server::getClientByID(uint32_t id)
{
	for (auto &p : m_clients)
	{
		if (p.second.m_id == id)
		{
			return &p.second;
		}
	}
	return 0;
}

ClientConnection *Server::getClientBySocket(SOCKET s)
{
	auto it = m_clients.find(s);
	if (it == m_clients.end())
		return 0;
	return &it->second;
}

ClientConnection *Server::getClientByAddress(const sockaddr &addr)
{
	const sockaddr_in &inetAddr = reinterpret_cast<const sockaddr_in &>(addr);
	for (auto &p : m_clients)
	{
		if (p.second.getAddr().sin_addr.s_addr == inetAddr.sin_addr.s_addr &&
			p.second.getAddr().sin_port == inetAddr.sin_port)
		{
			return &p.second;
		}
	}
	return 0;
}
