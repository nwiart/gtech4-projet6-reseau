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

	void open();

	inline Socket& getListenSocket() { return m_socketListener; }


private:

	static const uint16_t serverBasePort = 27014;

	Socket m_socketListener;
	ClientConnection m_clients[MAX_CLIENTS];
};
