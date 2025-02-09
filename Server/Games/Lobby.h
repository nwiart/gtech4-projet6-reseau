#pragma once

#include <stdint.h>
#include <string>
#include <map>

class ClientConnection;

/// <summary>
/// Represents a game lobby where players can join, interact, and start a game session.
/// </summary>
class Lobby
{
public:
	/// <summary>
	/// Structure representing a player inside the lobby.
	/// </summary>
	struct Player
	{
		uint32_t m_inLobbyID; ///< Unique ID of the player within the lobby.
		ClientConnection* m_client; ///< Pointer to the client's connection.
	};

public:
	/// <summary>
	/// Constructs a lobby with a unique ID.
	/// </summary>
	/// <param name="id">Unique identifier of the lobby.</param>
	Lobby(uint32_t id) : m_lobbyID(id) {}

	/// <summary>
	/// Initializes the lobby with a given name.
	/// </summary>
	/// <param name="name">The name of the lobby.</param>
	void init(const std::string& name);

	/// <summary>
	/// Updates the lobby state. This method is virtual and must be implemented by derived classes.
	/// </summary>
	/// <param name="dt">Time delta for smooth updates.</param>
	virtual void update(float dt) = 0;

	/// <summary>
	/// Gets the number of players currently in the lobby.
	/// </summary>
	/// <returns>Number of players in the lobby.</returns>
	virtual int getNumPlayers() const = 0;

	/// <summary>
	/// Gets the maximum number of players allowed in the lobby.
	/// </summary>
	/// <returns>Maximum number of players.</returns>
	virtual int getMaxPlayers() const = 0;

	/// <summary>
	/// Starts the game session within the lobby.
	/// </summary>
	virtual void start() = 0;

	/// <summary>
	/// Checks whether the game has started in this lobby.
	/// </summary>
	/// <returns>True if the game has started, false otherwise.</returns>
	virtual bool hasGameStarted() const = 0;

	/// <summary>
	/// Adds a player to the lobby.
	/// </summary>
	/// <param name="conn">Pointer to the client connection.</param>
	/// <returns>Assigned player ID in the lobby.</returns>
	virtual uint32_t addPlayer(ClientConnection* conn);

	/// <summary>
	/// Removes a player from the lobby.
	/// </summary>
	/// <param name="conn">Pointer to the client connection being removed.</param>
	virtual void disconnectPlayer(ClientConnection* conn);

	/// <summary>
	/// Retrieves the list of players in the lobby.
	/// </summary>
	/// <returns>Constant reference to the map of players.</returns>
	const std::map<uint64_t, Player>& getPlayers() const { return m_players; }

	/// <summary>
	/// Retrieves the player ID based on their socket.
	/// </summary>
	/// <param name="socket">The socket identifier.</param>
	/// <returns>The player's ID or -1 if not found.</returns>
	virtual uint32_t getPlayerID(uint64_t socket) const { return -1; }

	/// <summary>
	/// Retrieves the unique ID of the lobby.
	/// </summary>
	/// <returns>The lobby ID.</returns>
	uint32_t getLobbyID() const { return m_lobbyID; }

	/// <summary>
	/// Retrieves the name of the lobby.
	/// </summary>
	/// <returns>Constant reference to the lobby name string.</returns>
	inline const std::string& getName() const { return m_name; }

protected:
	uint32_t m_lobbyID; ///< Unique identifier for the lobby.
	std::string m_name; ///< Name of the lobby.

	std::map<uint64_t, Player> m_players; ///< Map storing player connections in the lobby.
};
