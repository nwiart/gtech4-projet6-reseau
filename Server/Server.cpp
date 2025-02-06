#include "Server.h"

#include "Games/Lobby.h"

#include "Networking.h"

#include <WinSock2.h>
#include <stdlib.h>


Server::Server()
	: m_clientUID(0)
{

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
	if (m_clients.find(clientSocketTCP.mSocket) != m_clients.end()) {
		return false;
	}

	auto it = m_clients.insert(std::pair<uint64_t, ClientConnection>((uint64_t)clientSocketTCP.mSocket, ClientConnection()));

	ClientConnection& conn = it.first->second;
	conn.m_id = -1;
	conn.m_name = "PLAYER_CONNECTING";
	conn.m_socket = clientSocketTCP;

	return true;
}

void Server::notifyDisconnect(Socket& clientSocketTCP)
{
	auto it = m_clients.find(clientSocketTCP.mSocket);

	// No matching client! This is a critical error!
	if (it == m_clients.end()) {
		__debugbreak();
	}

	ClientConnection& conn = it->second;
	if (conn.m_lobby) {
		conn.m_lobby->disconnectPlayer(conn.m_id);
	}
	
	// TODO : close socket.

	m_clients.erase(it);
}

uint32_t Server::confirmClient(Socket clientSocketTCP, const std::string& playerName)
{
	sockaddr_in addr;
	int addrSize = sizeof(addr);
	getpeername(clientSocketTCP.mSocket, (sockaddr*)&addr, &addrSize);
	uint32_t clientIP = addr.sin_addr.s_addr;

	ClientConnection& conn = m_clients.at(clientSocketTCP.mSocket);

	conn.m_id = m_clientUID;
	conn.m_name = playerName;
	conn.setIP(clientIP);

	m_clientUID++;

	return conn.m_id;
}



#include "PongPackets.h"

void Server::notifyReceiveTCP(SOCKET clientSocketTCP)
{
	if (m_clients.find(clientSocketTCP) == m_clients.end()) {
		std::cerr << "Client socket not found in server!" << std::endl;
		return;
	}

	ClientConnection& conn = m_clients.at(clientSocketTCP);
	uint32_t packetID;
	char buf[1020];

	if (clientSocketTCP == INVALID_SOCKET) {
		std::cerr << "Error: Attempted to receive on an invalid socket!\n";
		return;
	}

	int receivedBytes = recv(clientSocketTCP, reinterpret_cast<char*>(&packetID), sizeof(packetID), 0);

	if (receivedBytes == 0) {
		std::cerr << "Client disconnected gracefully.\n";
		notifyDisconnect(conn.getSocket());
		return;
	}
	else if (receivedBytes == SOCKET_ERROR) {
		int error = WSAGetLastError();
		std::cerr << "Error receiving packet ID. WSA Error: " << error << std::endl;

		if (error == WSAECONNRESET) {
			std::cerr << "Client forcibly closed the connection.\n";
		}
		else if (error == WSAETIMEDOUT) {
			std::cerr << "Connection timed out.\n";
		}
		else if (error == WSAENOTSOCK) {
			std::cerr << "Invalid socket error (10038). Check if the socket is closed or uninitialized.\n";
		}

		notifyDisconnect(conn.getSocket());
		return;
	}

	int e = WSAGetLastError();

	switch ((ClientPackets)packetID)
	{
	case ClientPackets::PlayerConnect: {
		recv(clientSocketTCP, buf, sizeof(Client_PlayerConnect), 0);
		uint32_t playerID = confirmClient(conn.getSocket(), reinterpret_cast<Client_PlayerConnect*>(buf)->playerName);

		Server_ConnectResult p;
		p.success = true;
		p.playerID = playerID;
		network::sendPacketTCP(conn.getSocket(), (uint32_t)ServerPackets::ConnectResult, p);
		std::cout << "Successfully connected Player " << playerID << " with name \"" << reinterpret_cast<Client_PlayerConnect*>(buf)->playerName << "\"." << std::endl;
	}
		break;

	case ClientPackets::GetLobbies: {
		for (Lobby* lobby : m_games) {
			Server_GetLobbies p;
			strcpy(p.lobbyName, lobby->getName().c_str());
			p.numPlayers = lobby->getNumPlayers();
			p.maxPlayers = lobby->getMaxPlayers();
			network::sendPacketTCP(conn.getSocket(), (uint32_t)ServerPackets::GetLobbies, p);
		}
	}
	break;

	case ClientPackets::CreateLobby:
		break;

	case ClientPackets::JoinLobby:
		break;
	}
}

void Server::receiveUDPPackets() {
	sockaddr_in senderAddr;
	uint32_t packetID;

	while (network::receivePacketUDP(m_socketUDP, &senderAddr, packetID)) {
		uint32_t senderIP = senderAddr.sin_addr.s_addr;

		// Find the client that matches the IP
		auto it = std::find_if(m_clients.begin(), m_clients.end(), [&](const auto& pair) {
			return pair.second.getIP() == senderIP;
			});

		if (it == m_clients.end()) {
			std::cerr << "Received UDP packet from unknown IP"  << "\n";
			continue; // Ignore packets from unknown clients
		}

		int playerID = it->second.m_id;
		handleUDPPacket(packetID, playerID);
	}
}

void Server::handleUDPPacket(uint32_t packetID, int playerID) {
	switch (static_cast<ClientPackets>(packetID)) {
	case ClientPackets::PlayerMove: {
		Client_PlayerMove packet;
		if (network::receivePacketUDP(m_socketUDP, nullptr, packet)) {
			std::cout << "Player " << playerID << " move to " << packet.position << std::endl;
		}
	} break;


	default:
		std::cerr << "Unknown UDP packet received: " << packetID << std::endl;
		break;
	}
	/////////ADD THEM ALL!!!!!///////// eventually
}
