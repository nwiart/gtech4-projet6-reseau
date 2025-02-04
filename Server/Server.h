#pragma once

#include "Socket.h"

#include <vector>
#include <map>

class Lobby;


class ClientConnection
{
	friend class Server;

public:
	ClientConnection() : m_id(-1), m_lobby(0), m_hasUDP(false)
	{
	}

	void connect(Socket socket, const std::string& playerName)
	{
		m_socket = socket;
		m_name = playerName;
	}

	Socket& getSocket() { return m_socket; }
	bool hasUDP() const { return m_hasUDP; }

private:

	uint32_t m_id;
	Socket m_socket;
	std::string m_name;

	Lobby* m_lobby;

	bool m_hasUDP;
};


class Server
{
public:

	Server();

	void open();

	inline Socket& getListenSocket() { return m_socketListener; }
	inline Socket& getUDPSocket() { return m_socketUDP; }

	bool notifyConnect(Socket clientSocketTCP);
	void notifyDisconnect(Socket clientSocketTCP);

	uint32_t confirmClient(Socket clientSocketTCP, const std::string& playerName);

	void notifyReceiveTCP(SOCKET clientSocketTCP);
	void ReceiveUDP();



private:

	static const uint16_t serverBasePort = 27014;
	static const uint16_t serverSecondaryPort = serverBasePort+1;

	Socket m_socketListener;
	Socket m_socketUDP;

	uint32_t m_clientUID;
	std::map<uint64_t, ClientConnection> m_clients;
	std::vector<Lobby*> m_games;
};
