#pragma once

#include "Socket.h"
#include "PongPlayer.h"

#define MAX_CLIENTS 4


class ClientConnection
{
public:
<<<<<<< Updated upstream

	
=======
	ClientConnection() : m_id(-1), m_lobby(0), m_ip(0) {}

	Socket& getSocket() { return m_socket; }
	uint32_t getIP() const { return m_ip; }
	const std::string& getName() const { return m_name; }
	Lobby* getLobby() const { return m_lobby; }

	void setIP(const sockaddr_in& addr) {
		m_ip = addr.sin_addr.s_addr;
		m_addr = addr; 
	}

	const sockaddr_in& getAddr() const { return m_addr; }
>>>>>>> Stashed changes

private:

	PongPlayer m_player;
	Socket m_socket;
<<<<<<< Updated upstream
=======
	uint32_t m_ip;
	std::string m_name;
	Lobby* m_lobby;
	sockaddr_in m_addr;
>>>>>>> Stashed changes
};


class Server
{
public:

	Server();

	void open(uint16_t tcpPort, uint16_t udpPort);


private:

	ClientConnection m_clients[MAX_CLIENTS];
};
