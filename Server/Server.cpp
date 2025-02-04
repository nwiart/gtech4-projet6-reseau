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

	m_socketSender.createSocketUDP();
	m_socketSender.bindUDP(serverSecondaryPort);
}

bool Server::notifyConnect(Socket clientSocketTCP)
{
	if (m_clients.find(clientSocketTCP.mSocket) != m_clients.end()) {
		return false;
	}

	auto it = m_clients.insert(std::pair<uint64_t, ClientConnection>(clientSocketTCP.mSocket, ClientConnection()));
	ClientConnection& conn = it.first->second;
	conn.m_id = -1;
	conn.m_name = "PLAYER_CONNECTING";
	conn.m_socket = clientSocketTCP;

	return true;
}

void Server::notifyDisconnect(Socket clientSocketTCP)
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
	ClientConnection& conn = m_clients.at(clientSocketTCP);
	uint32_t packetID;
	char buf[1020];

	recv(clientSocketTCP, (char*) &packetID, 4, 0);

	switch ((ClientPackets) packetID)
	{
	case ClientPackets::PlayerConnect: {
		recv(clientSocketTCP, buf, sizeof(Client_PlayerConnect), 0);
		uint32_t playerID = confirmClient(conn.getSocket(), reinterpret_cast<Client_PlayerConnect*>(buf)->playerName);

		Server_ConnectResult p;
		p.success = true;
		p.playerID = playerID;
		network::sendPacketTCP(conn.getSocket(), (uint32_t)ServerPackets::ConnectResult, p);
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
