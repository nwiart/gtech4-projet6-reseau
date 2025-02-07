#pragma once

#include "Socket.h"

#include "GameMode.h"

#include <WinSock2.h>

#include <vector>
#include <map>

class Lobby;


class ClientConnection
{
	friend class Server;

public:
	ClientConnection() : m_id(-1), m_lobby(0) {}

	Socket& getSocket() { return m_socket; }
	const sockaddr* getIP() const { return (const sockaddr*) & m_addr; }
	const std::string& getName() const { return m_name; }
	Lobby* getLobby() const { return m_lobby; }

	void setIP(const sockaddr_in& a) { m_addr = a; }

private:
	uint32_t m_id;
	Socket m_socket;
	sockaddr_in m_addr;
	std::string m_name;
	Lobby* m_lobby;
};


class Server
{
public:

	static Server* m_instance;

public:

	Server();

	void open();

	inline Socket& getListenSocket() { return m_socketListener; }
	inline Socket& getUDPSocket() { return m_socketUDP; }

	bool notifyConnect(Socket& clientSocketTCP);
	void notifyDisconnect(Socket& clientSocketTCP);

	uint32_t confirmClient(Socket& clientSocketTCP, const std::string& playerName);
	void createLobby(Socket& initiator, const std::string& name, GameMode gm);
	void joinLobby(Socket& player, Lobby* l);

	void notifyReceiveTCP(SOCKET clientSocketTCP);
	void notifyReceiveUDP();
	void receiveUDPPackets();

	void handleUDPPacket(uint32_t packetID, ClientConnection* conn);

	void updateGames(float dt);

	Lobby* getLobbyByID(uint32_t id) const;

	ClientConnection* getClientBySocket(SOCKET s);

	// Used to identify who sent data over UDP.
	ClientConnection* getClientByAddress(const sockaddr& addr);

private:

	static const uint16_t serverBasePort = 27014;
	static const uint16_t serverSecondaryPort = serverBasePort+1;

	std::map<uint64_t, ClientConnection> m_clients;
	Socket m_socketListener;
	Socket m_socketUDP;

	uint32_t m_lobbyUID;
	uint32_t m_clientUID;
	
	std::vector<Lobby*> m_games;
};
