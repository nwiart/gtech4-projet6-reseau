#pragma once

#include "Socket.h"
#include "PongPlayer.h"

#define MAX_CLIENTS 4


class ClientConnection
{
public:

	

private:

	PongPlayer m_player;
	Socket m_socket;
};


class Server
{
public:

	Server();

	void open(uint16_t tcpPort, uint16_t udpPort);


private:

	ClientConnection m_clients[MAX_CLIENTS];
};
