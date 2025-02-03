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
	inline Socket& getUDPSocket() { return m_socketSender; }

private:

	static const uint16_t serverBasePort = 27014;
	static const uint16_t serverSecondaryPort = 27015;

	Socket m_socketListener;
	Socket m_socketSender;
	ClientConnection m_clients[MAX_CLIENTS];

};
