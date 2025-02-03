#include "Server.h"


Server::Server()
{

}

void Server::open()
{
	m_socketListener.createSocketTCP();
	m_socketListener.listenTCP(serverBasePort);
}
