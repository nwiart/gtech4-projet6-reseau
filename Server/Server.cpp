#include "Server.h"

#include "Games/Lobby.h"

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

bool Server::addClient(Socket clientSocketTCP, const std::string& name)
{
	if (m_clients.find(clientSocketTCP.mSocket) != m_clients.end()) {
		return false;
	}

	auto it = m_clients.insert(std::pair<uint64_t, ClientConnection>(clientSocketTCP.mSocket, ClientConnection()));
	ClientConnection& conn = it.first->second;
	conn.m_id = m_clientUID;
	conn.connect(clientSocketTCP, name);

	m_clientUID++;

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
	conn.m_lobby->disconnectPlayer(conn.m_id);
	
	// TODO : close socket.

	m_clients.erase(it);
}
