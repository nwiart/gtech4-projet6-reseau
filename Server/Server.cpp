#include "Server.h"

#include "Games/Lobby.h"
#include "Games/LobbyPong.h"

#include "PongPackets.h"

#include "Networking.h"

#include <WinSock2.h>
#include <stdlib.h>


Server* Server::m_instance = 0;


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
	m_socketUDP.bindUDP(serverSecondaryPort);
}

bool Server::notifyConnect(Socket& clientSocketTCP)
{
	if (m_clients.find(clientSocketTCP.mSocket) != m_clients.end())
	{
		return false;
	}

	auto it = m_clients.emplace(clientSocketTCP.mSocket, ClientConnection());
	ClientConnection& conn = it.first->second;

	conn.m_id = -1;
	conn.m_name = "PLAYER_CONNECTING";
	conn.m_socket = std::move(clientSocketTCP);

	return true;
}

void Server::notifyDisconnect(Socket& clientSocketTCP) {
	auto it = m_clients.find(clientSocketTCP.mSocket);

	if (it == m_clients.end()) {
		std::cerr << "Erreur: Tentative de deconnexion d’un client inconnu !" << std::endl;
		return;
	}

	ClientConnection& conn = it->second;

	if (conn.m_lobby) {
		conn.m_lobby->disconnectPlayer(conn.m_id);
		std::cout << "Joueur " << conn.m_id << " retiré du lobby " << conn.m_lobby->getLobbyID() << std::endl;
	}

	shutdown(clientSocketTCP.mSocket, SD_BOTH);
	closesocket(clientSocketTCP.mSocket);

	std::cout << "Client " << conn.m_id << " (" << conn.m_name << ") déconnecté.\n";

	conn.m_socket.closeSocket();
	m_clients.erase(it);
}


uint32_t Server::confirmClient(Socket& clientSocketTCP, const std::string& playerName)
{
	sockaddr_in addr;
	int addrSize = sizeof(addr);

	if (getpeername(clientSocketTCP.mSocket, (sockaddr*)&addr, &addrSize) == SOCKET_ERROR) {
		std::cerr << "Erreur: Impossible de récupérer l'adresse du client." << std::endl;
		return -1;
	}

	uint32_t clientIP = addr.sin_addr.s_addr;

	auto it = m_clients.find(clientSocketTCP.mSocket);
	if (it == m_clients.end()) {
		std::cerr << "Erreur: Tentative de confirmation d'un client inconnu !" << std::endl;
		return -1;
	}

	ClientConnection& conn = it->second;
	conn.m_id = m_clientUID;
	conn.m_name = playerName;
	conn.setIP(addr);

	m_clientUID++;

	return conn.m_id;
}


void Server::createLobby(Socket& initiator, const std::string &name, GameMode gm)
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

void Server::joinLobby(Socket& player, Lobby* l) {
	ClientConnection& conn = m_clients[player.mSocket];

	// Lobby is non-existent.
	if (l == 0) return;

	// Player already in a lobby.
	if (conn.getLobby()) return;

	// Game already started.
	if (l->hasGameStarted()) {
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

	LobbyPong* pongGame = dynamic_cast<LobbyPong*>(l);
	if (pongGame) {
		Server_GameState state;
		pongGame->getGameState(state.ballX, state.ballY, state.ballRadius,
			state.paddle1Y, state.paddle2Y,
			state.scoreP1, state.scoreP2);

		network::sendPacketTCP(player, (uint32_t)ServerPackets::GameState, state);
	}
}


void Server::notifyReceiveTCP(SOCKET clientSocketTCP)
{
	if (m_clients.find(clientSocketTCP) == m_clients.end())
	{
		std::cerr << "Client socket not found in server!" << std::endl;
		return;
	}

	ClientConnection &conn = m_clients.at(clientSocketTCP);
	uint32_t packetID;
	char buf[1020];

	if (clientSocketTCP == INVALID_SOCKET)
	{
		std::cerr << "Error: Attempted to receive on an invalid socket!\n";
		return;
	}

	int receivedBytes = recv(clientSocketTCP, reinterpret_cast<char *>(&packetID), sizeof(packetID), 0);

	if (receivedBytes == 0)
	{
		std::cerr << "Client disconnected gracefully.\n";
		notifyDisconnect(conn.getSocket());
		return;
	}
	else if (receivedBytes == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		std::cerr << "Error receiving packet ID. WSA Error: " << error << std::endl;

		if (error == WSAECONNRESET)
		{
			std::cerr << "Client forcibly closed the connection.\n";
		}
		else if (error == WSAETIMEDOUT)
		{
			std::cerr << "Connection timed out.\n";
		}
		else if (error == WSAENOTSOCK)
		{
			std::cerr << "Invalid socket error (10038). Check if the socket is closed or uninitialized.\n";
		}

		notifyDisconnect(conn.getSocket());
		return;
	}

	int e = WSAGetLastError();

	switch ((ClientPackets)packetID)
	{
	case ClientPackets::PlayerConnect:
	{
		recv(clientSocketTCP, buf, sizeof(Client_PlayerConnect), 0);
		uint32_t playerID = confirmClient(conn.getSocket(), reinterpret_cast<Client_PlayerConnect *>(buf)->playerName);

		Server_ConnectResult p;
		p.success = true;
		p.playerID = playerID;
		network::sendPacketTCP(conn.getSocket(), (uint32_t)ServerPackets::ConnectResult, p);
		std::cout << "Successfully connected Player " << playerID << " with name \"" << reinterpret_cast<Client_PlayerConnect *>(buf)->playerName << "\"." << std::endl;
	}
	break;

	case ClientPackets::GetLobbies:
	{
		for (Lobby *lobby : m_games)
		{
			Server_GetLobbies p;
			strcpy(p.lobbyName, lobby->getName().c_str());
			p.numPlayers = lobby->getNumPlayers();
			p.maxPlayers = lobby->getMaxPlayers();
			p.lobbyID = lobby->getLobbyID();
			network::sendPacketTCP(conn.getSocket(), (uint32_t)ServerPackets::GetLobbies, p);
		}
	}
	break;

	case ClientPackets::CreateLobby:
	{
		recv(clientSocketTCP, buf, sizeof(Client_CreateLobby), 0);
		Socket& playerSocket = conn.getSocket();
		createLobby(playerSocket,
			reinterpret_cast<Client_CreateLobby*>(buf)->lobbyName,
			reinterpret_cast<Client_CreateLobby*>(buf)->gamemode);

	}
	break;

	case ClientPackets::JoinLobby:
	{
		recv(clientSocketTCP, buf, sizeof(Client_JoinLobby), 0);
		Socket& playerSocket = conn.getSocket();
		joinLobby(playerSocket, getLobbyByID(reinterpret_cast<Client_JoinLobby*>(buf)->lobbyID));

	}
	break;

	case ClientPackets::StartGame:
	{
		// Not in lobby or not owner.
		if (conn.getLobby() == 0 || conn.getLobby()->getPlayerID(clientSocketTCP) != 0)
		{
			Server_GameStart p;
			p.started = false;
			network::sendPacketTCP(conn.getSocket(), (uint32_t)ServerPackets::GameStart, p);
			return;
		}

		conn.getLobby()->start();
	}
	break;
	}
}


void Server::notifyReceiveUDP()
{
	uint32_t packetID;
	sockaddr clientAddr;
	int addrlen;
	int rec = recvfrom(m_socketUDP.mSocket, (char*)  & packetID, 4, 0, &clientAddr, &addrlen);

	if (rec != 4) {
		return;
	}

	ClientConnection* conn = getClientByAddress(clientAddr);
	if (!conn) {
		std::cout << "Received UDP data, but the client could not be resolved...\n";
		return;
	}

	handleUDPPacket(packetID, conn);
}

void Server::receiveUDPPackets() {
	sockaddr_in senderAddr;
	uint32_t packetID;

	while (network::receivePacketUDP(m_socketUDP, &senderAddr, packetID))
	{
		uint32_t senderIP = senderAddr.sin_addr.s_addr;

		// Find the client that matches the IP
		/*auto it = std::find_if(m_clients.begin(), m_clients.end(), [&](const auto& pair)
							   { return pair.second.getIP() == senderIP; });

		if (it == m_clients.end())
		{
			std::cerr << "Received UDP packet from unknown IP" << "\n";
			continue; // Ignore packets from unknown clients
		}

		int playerID = it->second.m_id;
		handleUDPPacket(packetID, 0);*/
	}
}

void Server::handleUDPPacket(uint32_t packetID, ClientConnection* conn)
{
	switch (static_cast<ClientPackets>(packetID))
	{
	case ClientPackets::PlayerMove:
	{
		Client_PlayerMove packet;
		network::receivePacketUDP(m_socketUDP, nullptr, packet);
		//std::cout << "Player " << playerID << " moved to " << packet.position << std::endl;

		LobbyPong* pong = dynamic_cast<LobbyPong*>(conn->getLobby());
		pong->receivePlayerMove(pong->getPlayerID(conn->getSocket().mSocket), packet.position);
	}
	break;

	default:
		std::cerr << "Unknown UDP packet received: " << packetID << std::endl;
		break;
	}
}

void Server::updateGames(float dt) {
	for (auto it = m_games.begin(); it != m_games.end();) {
		(*it)->update(dt);

		if ((*it)->getNumPlayers() == 0) {
			std::cout << "Lobby " << (*it)->getLobbyID() << " supprimé (aucun joueur actif)" << std::endl;
			delete* it;
			it = m_games.erase(it);
		}
		else {
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

ClientConnection* Server::getClientBySocket(SOCKET s)
{
	auto it = m_clients.find(s);
	if (it == m_clients.end()) return 0;
	return &it->second;
}

ClientConnection* Server::getClientByAddress(const sockaddr& addr)
{
	const sockaddr_in& inetaddr = reinterpret_cast<const sockaddr_in&>(addr);
	for (auto &p : m_clients) {
		if (p.second.m_addr.sin_addr.s_addr == inetaddr.sin_addr.s_addr && p.second.m_addr.sin_port == inetaddr.sin_port) {
			return &p.second;
		}
	}
	return 0;
}
