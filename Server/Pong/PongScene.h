#ifndef PONGSCENE_H
#define PONGSCENE_H

#include "Ball.h"
#include "Score.h"

/// <summary>
/// Represents the main game scene for Pong, managing the game logic,
/// including ball movement, player paddles, scoring, and collisions.
/// </summary>
class PongScene {
public:
	/// <summary>
	/// Initializes the Pong game scene with a specific width and height.
	/// </summary>
	/// <param name="sizeX">Width of the game scene.</param>
	/// <param name="sizeY">Height of the game scene.</param>
	PongScene(int sizeX, int sizeY);

	/// <summary>
	/// Starts the game by initializing necessary parameters.
	/// </summary>
	void startGame();

	/// <summary>
	/// Updates the game state, including ball movement and collision checks.
	/// </summary>
	/// <param name="dt">Time delta for smooth movement.</param>
	void update(float dt);

	/// <summary>
	/// Receives and updates a player's paddle position.
	/// </summary>
	/// <param name="playerID">ID of the player (1 or 2).</param>
	/// <param name="positionY">New Y position of the paddle.</param>
	/// <returns>The adjusted Y position of the paddle.</returns>
	float receivePlayerMove(uint32_t playerID, float positionY);

	/// <summary>
	/// Retrieves the current score of both players.
	/// </summary>
	/// <param name="score1">Score of player 1 (output parameter).</param>
	/// <param name="score2">Score of player 2 (output parameter).</param>
	void getScoreInfo(int& score1, int& score2) const;

	/// <summary>
	/// Retrieves the current ball position, direction, and speed.
	/// </summary>
	/// <param name="x">Ball's X position.</param>
	/// <param name="y">Ball's Y position.</param>
	/// <param name="xDir">Ball's X direction (-1 or 1).</param>
	/// <param name="yDir">Ball's Y direction (-1 or 1).</param>
	/// <param name="speed">Current ball speed.</param>
	void getBallInfo(float& x, float& y, float& xDir, float& yDir, float& speed) const;

	/// <summary>
	/// Gets the width of the game scene.
	/// </summary>
	/// <returns>The width of the scene.</returns>
	int getWidth() const { return sizeX; }

	/// <summary>
	/// Gets the height of the game scene.
	/// </summary>
	/// <returns>The height of the scene.</returns>
	int getHeight() const { return sizeY; }

	/// <summary>
	/// Gets the Y position of player 1's paddle.
	/// </summary>
	/// <returns>Player 1's paddle Y position.</returns>
	float getPaddle1Y() const { return paddle1Y; }

	/// <summary>
	/// Gets the Y position of player 2's paddle.
	/// </summary>
	/// <returns>Player 2's paddle Y position.</returns>
	float getPaddle2Y() const { return paddle2Y; }

	/// <summary>
	/// Increments the score for the given player.
	/// </summary>
	/// <param name="player1">True if player 1 scored, false if player 2 scored.</param>
	void addPointToPlayer(bool player1);

	/// <summary>
	/// Resets the game state, including ball position and scores.
	/// </summary>
	void reset();

private:
	/// <summary>
	/// Checks for collisions between the ball, paddles, and walls, and updates the game state accordingly.
	/// </summary>
	void checkCollisions();

	/// <summary>
	/// Distance from the screen edge to the paddles.
	/// </summary>
	static constexpr int PADDLE_MARGIN = 50;

	/// <summary>
	/// Width of the paddles.
	/// </summary>
	static constexpr int PADDLE_WIDTH = 10;

	/// <summary>
	/// Width of the game scene.
	/// </summary>
	int sizeX;

	/// <summary>
	/// Height of the game scene.
	/// </summary>
	int sizeY;

	/// <summary>
	/// Y position of player 1's paddle.
	/// </summary>
	float paddle1Y;

	/// <summary>
	/// Y position of player 2's paddle.
	/// </summary>
	float paddle2Y;

	/// <summary>
	/// Indicates whether the game has started.
	/// </summary>
	bool started;

	/// <summary>
	/// The ball object managing its position and movement.
	/// </summary>
	Ball m_ball;

	/// <summary>
	/// The score tracker for both players.
	/// </summary>
	Score m_score;
};

#endif // PONGSCENE_H
