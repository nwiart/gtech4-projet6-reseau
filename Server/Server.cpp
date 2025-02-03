#include "Server.h"


Server::Server()
{

}

void Server::open()
{
	m_socketListener.createSocketTCP();
	m_socketListener.listenTCP(serverBasePort);

	m_socketSender.createSocketUDP();
	m_socketSender.bindUDP(serverSecondaryPort);
}

void Server::addClient(ClientConnection* pClient)
{
	for (int i = 0; i < MAX_CLIENTS; ++i) {
		if (m_clients[i] == nullptr) {
			m_clients[i] = pClient;
			return;
		}
	}
	return;
}


