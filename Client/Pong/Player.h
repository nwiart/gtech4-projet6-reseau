#ifndef PLAYER_H
#define PLAYER_H

#include "SFMLInclude.h"

/// <summary>
/// Represents a player in the Pong game, managing paddle movement and rendering.
/// </summary>
class Player {
public:
	/// <summary>
	/// Default constructor initializing a player without key bindings.
	/// </summary>
	Player();

	/// <summary>
	/// Constructs a player with a specified X position ratio and movement keys.
	/// </summary>
	/// <param name="xRatio">The horizontal position ratio of the paddle.</param>
	/// <param name="upKey">Key to move the paddle up.</param>
	/// <param name="downKey">Key to move the paddle down.</param>
	Player(float xRatio, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey);

	/// <summary>
	/// Draws the player's paddle on the screen.
	/// </summary>
	/// <param name="window">Reference to the render window.</param>
	void draw(sf::RenderWindow& window);

	/// <summary>
	/// Retrieves the current Y position of the paddle.
	/// </summary>
	/// <returns>The Y coordinate of the paddle.</returns>
	float getPositionY() const;

	/// <summary>
	/// Sets the Y position of the paddle.
	/// </summary>
	/// <param name="y">New Y coordinate.</param>
	void setPosition(float y);

	/// <summary>
	/// Sets the X position of the paddle.
	/// </summary>
	/// <param name="x">New X coordinate.</param>
	void setX(float x);

	/// <summary>
	/// Retrieves the key assigned to move the paddle up.
	/// </summary>
	/// <returns>The SFML key assigned for upward movement.</returns>
	sf::Keyboard::Key getUpKey() const;

	/// <summary>
	/// Retrieves the key assigned to move the paddle down.
	/// </summary>
	/// <returns>The SFML key assigned for downward movement.</returns>
	sf::Keyboard::Key getDownKey() const;

private:
	sf::RectangleShape paddle; ///< Graphical representation of the player's paddle.
	sf::Keyboard::Key upKey; ///< Key used to move the paddle up.
	sf::Keyboard::Key downKey; ///< Key used to move the paddle down.
};

#endif // PLAYER_H
