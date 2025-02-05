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
	ClientConnection& conn = m_clients.at(clientSocketTCP.mSocket);

	conn.m_id = m_clientUID;
	conn.m_name = playerName;

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

	case ClientPackets::CreateLobby:
		break;

	case ClientPackets::JoinLobby:
		break;

	case ClientPackets::PlayerMove:
		break;
	}
}

void Server::ReceiveUDP()
{
	sockaddr_in senderAddr;
	int senderAddrSize = sizeof(senderAddr);
	char buffer[MAX_PACKET_SIZE];

	int bytesReceived = recvfrom(m_socketUDP.mSocket, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&senderAddr), &senderAddrSize);

	if (bytesReceived == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK)
		{
			std::cerr << "UDP receive failed: " << error << std::endl;
		}
		return;
	}

	if (bytesReceived < 4)
		return;

	uint32_t packetID = *((uint32_t*)buffer);

	// Process packet based on ID
	switch ((ClientPackets)packetID)
	{
	case ClientPackets::CreateLobby:
		break;
	case ClientPackets::JoinLobby:
		break;
	case ClientPackets::PlayerMove:

		break;
	}

}
