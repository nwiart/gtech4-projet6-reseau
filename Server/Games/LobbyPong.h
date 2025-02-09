#pragma once

#include "Lobby.h"
#include "Pong/PongScene.h"
#include "Socket.h"

#include <map>
#include <stdint.h>

class ClientConnection;

/// <summary>
/// Represents a Pong-specific game lobby, managing players, game state, and interactions.
/// Supports both 1v1 and 2v2 game modes.
/// </summary>
class LobbyPong : public Lobby {

public:
	/// <summary>
	/// Constructs a Pong lobby with a unique ID and a mode (1v1 or 2v2).
	/// </summary>
	/// <param name="id">Unique identifier of the lobby.</param>
	/// <param name="twoPlayersTeam">True if the game mode is 2v2, false for 1v1.</param>
	LobbyPong(uint32_t id, bool twoPlayersTeam);

	/// <summary>
	/// Gets the number of players currently in the lobby.
	/// </summary>
	/// <returns>Number of players in the lobby.</returns>
	virtual int getNumPlayers() const override { return m_players.size(); }

	/// <summary>
	/// Gets the maximum number of players allowed in the lobby.
	/// </summary>
	/// <returns>Maximum number of players (2 for 1v1, 4 for 2v2).</returns>
	virtual int getMaxPlayers() const override { return m_twoPlayerTeams ? 4 : 2; }

	/// <summary>
	/// Starts the Pong game within this lobby.
	/// </summary>
	virtual void start() override;

	/// <summary>
	/// Adds a player to the lobby.
	/// </summary>
	/// <param name="conn">Pointer to the client connection.</param>
	/// <returns>Assigned player ID in the lobby.</returns>
	virtual uint32_t addPlayer(ClientConnection* conn) override;

	/// <summary>
	/// Retrieves the player ID based on their socket.
	/// </summary>
	/// <param name="socket">The socket identifier.</param>
	/// <returns>The player's ID or -1 if not found.</returns>
	virtual uint32_t getPlayerID(uint64_t socket) const override;

	/// <summary>
	/// Checks whether the Pong game has started in this lobby.
	/// </summary>
	/// <returns>True if the game has started, false otherwise.</returns>
	virtual bool hasGameStarted() const override;

	/// <summary>
	/// Updates the Pong game state, including ball movement and collisions.
	/// </summary>
	/// <param name="dt">Time delta for smooth game updates.</param>
	void update(float dt) override;

	/// <summary>
	/// Processes a player's paddle movement request.
	/// </summary>
	/// <param name="playerID">ID of the player moving the paddle.</param>
	/// <param name="positionY">New Y position of the paddle.</param>
	void receivePlayerMove(uint32_t playerID, float positionY);

	/// <summary>
	/// Sends the current game state to all players in the lobby.
	/// </summary>
	void sendGameState();

private:
	PongScene m_pong; ///< Instance of the Pong game logic.

	bool m_twoPlayerTeams; ///< True if the game mode is 2v2, false for 1v1.
	bool gameStarted = false; ///< Tracks whether the game has started.
};
