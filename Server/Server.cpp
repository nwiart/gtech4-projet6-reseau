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
