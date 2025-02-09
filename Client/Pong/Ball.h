#ifndef BALL_H
#define BALL_H

#include "SFMLInclude.h"

/// <summary>
/// Represents the ball in the Pong game, handling movement, rendering, and network updates.
/// </summary>
class Ball {
public:
	/// <summary>
	/// Default constructor initializing the ball's properties.
	/// </summary>
	Ball();

	/// <summary>
	/// Sets the position of the ball.
	/// </summary>
	/// <param name="x">New X coordinate.</param>
	/// <param name="y">New Y coordinate.</param>
	void setPosition(float x, float y);

	/// <summary>
	/// Sets the velocity of the ball.
	/// </summary>
	/// <param name="velocity">New velocity vector.</param>
	void setVelocity(const sf::Vector2f& velocity);

	/// <summary>
	/// Updates the ball's position based on its velocity.
	/// </summary>
	/// <param name="dt">Time delta for movement calculations.</param>
	void update(float dt);

	/// <summary>
	/// Updates the ball's state based on data received from the server.
	/// </summary>
	/// <param name="x">New X position.</param>
	/// <param name="y">New Y position.</param>
	/// <param name="xDir">New X direction.</param>
	/// <param name="yDir">New Y direction.</param>
	/// <param name="speed">New speed of the ball.</param>
	void updateFromServer(float x, float y, float xDir, float yDir, float speed);

	/// <summary>
	/// Draws the ball on the given window.
	/// </summary>
	/// <param name="window">Reference to the render window.</param>
	void draw(sf::RenderWindow& window);

private:
	sf::CircleShape shape; ///< Graphical representation of the ball.
	sf::Vector2f velocity; ///< Velocity vector of the ball.
};

#endif // BALL_H
