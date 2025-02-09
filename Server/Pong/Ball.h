#ifndef BALL_H
#define BALL_H

#include <SFML/System.hpp>

class PongScene;

/// <summary>
/// Represents the ball in the Pong game, handling movement, collision, and networking.
/// </summary>
class Ball {
public:
	/// <summary>
	/// Default constructor initializing the ball's properties.
	/// </summary>
	Ball();

	/// <summary>
	/// Updates the ball's position and handles collisions.
	/// </summary>
	/// <param name="dt">Time delta for movement calculations.</param>
	/// <param name="scene">Pointer to the Pong scene to check for collisions.</param>
	void update(float dt, PongScene* scene);

	/// <summary>
	/// Resets the ball to its initial position.
	/// </summary>
	void resetPosition();

	/// <summary>
	/// Reverses the ball's horizontal direction.
	/// </summary>
	void reverseX();

	/// <summary>
	/// Reverses the ball's vertical direction.
	/// </summary>
	void reverseY();

	/// <summary>
	/// Sets the ball's position.
	/// </summary>
	/// <param name="x">New X coordinate.</param>
	/// <param name="y">New Y coordinate.</param>
	void setPosition(float x, float y);

	/// <summary>
	/// Sets the ball's velocity.
	/// </summary>
	/// <param name="vx">New velocity along the X axis.</param>
	/// <param name="vy">New velocity along the Y axis.</param>
	void setVelocity(float vx, float vy);

	/// <summary>
	/// Sets the ball's radius.
	/// </summary>
	/// <param name="r">New radius of the ball.</param>
	void setRadius(float r);

	/// <summary>
	/// Retrieves the current position of the ball.
	/// </summary>
	/// <returns>Reference to the ball's position vector.</returns>
	const sf::Vector2f& getPosition() const { return position; }

	/// <summary>
	/// Retrieves the current velocity of the ball.
	/// </summary>
	/// <returns>Reference to the ball's velocity vector.</returns>
	const sf::Vector2f& getVelocity() const { return velocity; }

	/// <summary>
	/// Retrieves the radius of the ball.
	/// </summary>
	/// <returns>The radius of the ball.</returns>
	float getRadius() const;

	/// <summary>
	/// Retrieves the ball's position and radius for network transmission.
	/// </summary>
	/// <param name="x">Output X coordinate of the ball.</param>
	/// <param name="y">Output Y coordinate of the ball.</param>
	/// <param name="r">Output radius of the ball.</param>
	void getNetworkData(float& x, float& y, float& r) const;

private:
	sf::Vector2f position; ///< Current position of the ball.
	sf::Vector2f velocity; ///< Current velocity of the ball.
	float radius; ///< Radius of the ball.
};

#endif // BALL_H
