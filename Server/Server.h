#pragma once

#include "Socket.h"
#include "PongPlayer.h"

#define MAX_CLIENTS 4


class ClientConnection
{
public:
	ClientConnection() : m_isConnected(false), m_player() {}

	bool isConnected() const { return m_isConnected; }
	void connect(Socket socket, const std::string& playerName) {
		m_socket = socket;
		m_player.setName(playerName);
		m_isConnected = true;
	}

	Socket& getSocket() { return m_socket; }
	PongPlayer& getPlayer() { return m_player; }
	

private:
	bool m_isConnected;
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
	void addClient(ClientConnection* pClient);

private:

	static const uint16_t serverBasePort = 27014;
	static const uint16_t serverSecondaryPort = 27015;

	Socket m_socketListener;
	Socket m_socketSender;
	ClientConnection* m_clients[MAX_CLIENTS];

};
