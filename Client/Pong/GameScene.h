#pragma once

#include "Scene/Scene.h"
#include "Widget/Score.h"

#include "Ball.h"
#include "Player.h"

/// <summary>
/// Represents the main game scene, managing player movements, ball physics, scores,
/// and network synchronization.
/// </summary>
class GameScene : public Scene
{
public:
	/// <summary>
	/// Constructs the game scene, initializing players, ball, and score.
	/// </summary>
	GameScene();

	/// <summary>
	/// Handles user input events such as keyboard and mouse interactions.
	/// </summary>
	/// <param name="event">SFML event to process.</param>
	/// <param name="window">Reference to the render window.</param>
	void handleEvent(sf::Event event, sf::RenderWindow& window) override;

	/// <summary>
	/// Updates the game state, including ball movement, player movements, and physics.
	/// </summary>
	/// <param name="window">Reference to the render window.</param>
	/// <param name="dt">Time delta for smooth updates.</param>
	void update(sf::RenderWindow& window, double dt) override;

	/// <summary>
	/// Updates the ping value displayed for a specific player.
	/// </summary>
	/// <param name="playerID">The ID of the player.</param>
	/// <param name="ping">The new ping value in milliseconds.</param>
	void updatePlayerPing(uint32_t playerID, int ping);

	/// <summary>
	/// Updates the displayed name for a specific player.
	/// </summary>
	/// <param name="playerID">The ID of the player.</param>
	/// <param name="name">The new name to display.</param>
	void updatePlayerName(uint32_t playerID, const std::string& name);

	/// <summary>
	/// Handles the local player's movement based on input and time.
	/// </summary>
	/// <param name="dt">Time delta for movement calculations.</param>
	void updateLocalPlayerMovement(double dt);

	/// <summary>
	/// Sets a player's position.
	/// </summary>
	/// <param name="id">The player's unique ID.</param>
	/// <param name="p">The new position.</param>
	void setPlayerPos(uint32_t id, int p);

	/// <summary>
	/// Updates the ball's position, direction, and speed based on network data.
	/// </summary>
	/// <param name="x">X coordinate of the ball.</param>
	/// <param name="y">Y coordinate of the ball.</param>
	/// <param name="xDir">X direction of movement.</param>
	/// <param name="yDir">Y direction of movement.</param>
	/// <param name="speed">Speed of the ball.</param>
	void setBallInfo(float x, float y, float xDir, float yDir, float speed);

	/// <summary>
	/// Updates the game score for both teams.
	/// </summary>
	/// <param name="score1">Score of team 1.</param>
	/// <param name="score2">Score of team 2.</param>
	void setScore(int score1, int score2);

	/// <summary>
	/// Draws all game elements including players, ball, score, and UI elements.
	/// </summary>
	/// <param name="window">Reference to the render window.</param>
	void draw(sf::RenderWindow& window) override;

private:
	static constexpr int MAX_PLAYERS = 4; ///< Maximum number of players in the game.

	Player players[MAX_PLAYERS]; ///< Array of players in the game.
	Ball ball; ///< The game ball.
	Score score; ///< Score management system.

	sf::Text player1Text; ///< Display text for player 1's name.
	sf::Text player2Text; ///< Display text for player 2's name.

	bool twoTeams; ///< True if playing a 2v2 mode, false for 1v1.
	int scoreTeam1 = 0; ///< Current score for team 1.
	int scoreTeam2 = 0; ///< Current score for team 2.

	double localTimer; ///< Timer for local player updates.

private:
	std::map<uint32_t, int> playerPings; ///< Stores ping values for each player.
	sf::Text player1PingText; ///< Display text for player 1's ping.
	sf::Text player2PingText; ///< Display text for player 2's ping.
};
