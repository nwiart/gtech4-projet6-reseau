#pragma once

#include <stdint.h>
#include <string>
#include <map>

/// <summary>
/// Represents a game lobby where players can join, interact, and start a match.
/// Handles both host and guest roles.
/// </summary>
class Lobby
{
public:
	/// <summary>
	/// Represents a player inside the lobby.
	/// </summary>
	class Player
	{
	public:
		std::string m_name; ///< Player's name.
		uint32_t m_playerID; ///< Unique global player ID.
		uint32_t m_idInLobby; ///< Player's ID within the lobby (used for roles/teams).
	};

public:
	/// <summary>
	/// Default constructor initializing an empty lobby.
	/// </summary>
	Lobby();

	/// <summary>
	/// Sets up the local player as the host of the lobby.
	/// </summary>
	/// <param name="myID">The local player's unique ID.</param>
	/// <param name="maxPlayers">Maximum number of players allowed in the lobby.</param>
	void setupHost(uint32_t myID, uint32_t maxPlayers);

	/// <summary>
	/// Sets up the local player as a guest in the lobby.
	/// </summary>
	/// <param name="myID">The local player's unique ID.</param>
	/// <param name="maxPlayers">Maximum number of players allowed in the lobby.</param>
	void setupGuest(uint32_t myID, uint32_t maxPlayers);

	/// <summary>
	/// Leaves the current lobby, clearing all player data.
	/// </summary>
	void leave();

	/// <summary>
	/// Adds a player to the lobby.
	/// </summary>
	/// <param name="playerID">The unique ID of the player.</param>
	/// <param name="inLobbyID">The assigned ID of the player within the lobby.</param>
	/// <param name="playerName">The player's name.</param>
	void listPlayer(uint32_t playerID, uint32_t inLobbyID, const std::string& playerName);

	/// <summary>
	/// Removes a player from the lobby.
	/// </summary>
	/// <param name="playerID">The unique ID of the player to remove.</param>
	void removePlayer(uint32_t playerID);

	/// <summary>
	/// Checks if the lobby is valid (i.e., contains players).
	/// </summary>
	/// <returns>True if the lobby has at least one player, false otherwise.</returns>
	inline bool isValid() const { return !m_players.empty(); }

	/// <summary>
	/// Checks if the local player is the host of the lobby.
	/// </summary>
	/// <returns>True if the local player is the host, false otherwise.</returns>
	inline bool isHost() const { return m_isHost; }

	/// <summary>
	/// Retrieves the number of players currently in the lobby.
	/// </summary>
	/// <returns>The number of players in the lobby.</returns>
	inline int getNumPlayers() const { return m_players.size(); }

	/// <summary>
	/// Retrieves the maximum number of players allowed in the lobby.
	/// </summary>
	/// <returns>The maximum number of players.</returns>
	inline int getMaxPlayers() const { return m_maxPlayers; }

	/// <summary>
	/// Retrieves the local player's unique ID.
	/// </summary>
	/// <returns>The local player's ID.</returns>
	inline uint32_t getLocalPlayerID() const { return m_myID; }

	/// <summary>
	/// Retrieves a list of all players in the lobby.
	/// </summary>
	/// <returns>Constant reference to the map of players.</returns>
	inline const std::map<uint32_t, Player>& getPlayers() const { return m_players; }

private:
	std::map<uint32_t, Player> m_players; ///< Stores all players currently in the lobby.
	uint32_t m_maxPlayers; ///< Maximum number of players allowed in the lobby.
	uint32_t m_myID; ///< The local player's unique ID.
	bool m_isHost; ///< True if the local player is the host, false otherwise.
};
