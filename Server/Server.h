#pragma once

#include "Socket.h"
#include "GameMode.h"

#include <WinSock2.h>
#include <string>
#include <vector>
#include <map>

class Lobby;

/// <summary>
/// Represents a client connection in the server, managing socket communication,
/// identification, and lobby assignment.
/// </summary>
class ClientConnection
{
	friend class Server;

public:
	/// <summary>
	/// Default constructor initializing the client with no ID and no lobby.
	/// </summary>
	ClientConnection() : m_id(-1), m_lobby(0) {}

	/// <summary>
	/// Retrieves the socket associated with this client.
	/// </summary>
	/// <returns>Reference to the client's socket.</returns>
	Socket& getSocket() { return m_socket; }

	/// <summary>
	/// Retrieves the client's IP address (TCP).
	/// </summary>
	/// <returns>Pointer to the sockaddr structure containing the IP.</returns>
	const sockaddr* getIP() const { return (const sockaddr*)&m_addr; }

	/// <summary>
	/// Retrieves the client's UDP address.
	/// </summary>
	/// <returns>Pointer to the sockaddr_in structure containing the UDP address.</returns>
	const sockaddr_in* getUDPAddr() const { return &m_udpAddr; }

	/// <summary>
	/// Gets the name of the client.
	/// </summary>
	/// <returns>Client's name as a string.</returns>
	const std::string& getName() const { return m_name; }

	/// <summary>
	/// Gets the lobby where the client is currently assigned.
	/// </summary>
	/// <returns>Pointer to the client's assigned lobby.</returns>
	Lobby* getLobby() const { return m_lobby; }

	/// <summary>
	/// Sets the IP address for the client.
	/// </summary>
	/// <param name="addr">Reference to sockaddr_in containing the IP address.</param>
	void setIP(const sockaddr_in& addr) { m_addr = addr; }

	/// <summary>
	/// Retrieves the client's stored IP address.
	/// </summary>
	/// <returns>Reference to the sockaddr_in structure with the address.</returns>
	const sockaddr_in& getAddr() const { return m_addr; }

	uint32_t m_id;          ///< Unique identifier for the client.
private:
	Socket m_socket;        ///< TCP socket for client communication.
	sockaddr_in m_addr;     ///< Client's TCP address.
	sockaddr_in m_udpAddr;  ///< Client's UDP address.
	std::string m_name;     ///< Client's username.
	Lobby* m_lobby;         ///< Pointer to the lobby the client is in.
};

/// <summary>
/// Represents the game server, managing client connections, lobbies, 
/// and game updates.
/// </summary>
class Server
{
public:
	/// <summary>
	/// Singleton instance of the server.
	/// </summary>
	static Server* m_instance;

public:
	/// <summary>
	/// Initializes the server instance.
	/// </summary>
	Server();

	/// <summary>
	/// Opens the server for connections.
	/// </summary>
	void open();

    void createBotLobby();

	/// <summary>
	/// Retrieves the server's listening socket for TCP connections.
	/// </summary>
	/// <returns>Reference to the TCP listening socket.</returns>
	inline Socket& getListenSocket() { return m_socketListener; }

	/// <summary>
	/// Retrieves the UDP socket used for game data transmission.
	/// </summary>
	/// <returns>Reference to the UDP socket.</returns>
	inline Socket& getUDPSocket() { return m_socketUDP; }

	/// <summary>
	/// Handles a new client attempting to connect to the server.
	/// </summary>
	/// <param name="clientSocketTCP">Reference to the client's TCP socket.</param>
	/// <returns>True if the connection is successful.</returns>
	bool notifyConnect(Socket& clientSocketTCP);

	/// <summary>
	/// Handles a client disconnecting from the server.
	/// </summary>
	/// <param name="clientSocketTCP">Reference to the client's TCP socket.</param>
	void notifyDisconnect(Socket& clientSocketTCP);

	/// <summary>
	/// Confirms a client's connection and assigns an ID.
	/// </summary>
	/// <param name="clientSocketTCP">Reference to the client's TCP socket.</param>
	/// <param name="playerName">The player's chosen name.</param>
	/// <returns>The assigned client ID.</returns>
	uint32_t confirmClient(Socket& clientSocketTCP, const std::string& playerName);

	/// <summary>
	/// Creates a new game lobby.
	/// </summary>
	/// <param name="initiator">Socket of the player creating the lobby.</param>
	/// <param name="name">Lobby name.</param>
	/// <param name="gm">Selected game mode.</param>
	void createLobby(Socket& initiator, const std::string& name, GameMode gm);

	/// <summary>
	/// Adds a player to an existing lobby.
	/// </summary>
	/// <param name="player">Socket of the joining player.</param>
	/// <param name="l">Pointer to the lobby.</param>
	void joinLobby(Socket& player, Lobby* l);

	/// <summary>
	/// Removes a player from a lobby.
	/// </summary>
	/// <param name="conn">Pointer to the client connection being removed.</param>
	void leaveLobby(ClientConnection* conn);

	/// <summary>
	/// Handles incoming TCP data from a client.
	/// </summary>
	/// <param name="clientSocketTCP">The socket from which data is received.</param>
	void notifyReceiveTCP(SOCKET clientSocketTCP);

	/// <summary>
	/// Handles incoming UDP data from a client.
	/// </summary>
	void notifyReceiveUDP();

	/// <summary>
	/// Processes an incoming UDP packet.
	/// </summary>
	/// <param name="packetID">ID of the packet.</param>
	/// <param name="buf">Buffer containing packet data.</param>
	/// <param name="addr">Address of the sending client.</param>
	void handleUDPPacket(uint32_t packetID, char* buf, sockaddr* addr);

	/// <summary>
	/// Updates the state of all ongoing games.
	/// </summary>
	/// <param name="dt">Time delta for smooth game updates.</param>
	void updateGames(float dt);

	/// <summary>
	/// Retrieves a lobby by its unique ID.
	/// </summary>
	/// <param name="id">Lobby ID.</param>
	/// <returns>Pointer to the lobby or nullptr if not found.</returns>
	Lobby* getLobbyByID(uint32_t id) const;

	/// <summary>
	/// Retrieves a client connection by its unique ID.
	/// </summary>
	/// <param name="id">Client ID.</param>
	/// <returns>Pointer to the client connection or nullptr if not found.</returns>
	ClientConnection* getClientByID(uint32_t id);

	/// <summary>
	/// Retrieves a client based on their socket.
	/// </summary>
	/// <param name="s">Client's socket.</param>
	/// <returns>Pointer to the client connection or nullptr if not found.</returns>
	ClientConnection* getClientBySocket(SOCKET s);

	/// <summary>
	/// Retrieves a client based on their network address.
	/// Used for identifying clients in UDP communication.
	/// </summary>
	/// <param name="addr">Reference to sockaddr containing the client's address.</param>
	/// <returns>Pointer to the client connection or nullptr if not found.</returns>
	ClientConnection* getClientByAddress(const sockaddr& addr);

private:
	static const uint16_t serverBasePort = 27014; ///< Base TCP port for the server.
	static const uint16_t serverSecondaryPort = serverBasePort + 1; ///< Secondary UDP port.

	std::map<uint64_t, ClientConnection> m_clients; ///< Stores active client connections.
	Socket m_socketListener; ///< TCP listening socket.
	Socket m_socketUDP; ///< UDP socket for game data.

	uint32_t m_lobbyUID; ///< Unique lobby ID counter.
	uint32_t m_clientUID; ///< Unique client ID counter.

	std::vector<Lobby*> m_games; ///< List of active game lobbies.
};
