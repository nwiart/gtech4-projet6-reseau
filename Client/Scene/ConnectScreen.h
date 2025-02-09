#pragma once

#include "Scene.h"
#include "Widget/TextField.h"
#include "Widget/Button.h"

/// <summary>
/// Represents the connection screen where players enter their name and server IP
/// before connecting to the game.
/// </summary>
class ConnectScreen : public Scene
{
public:
	/// <summary>
	/// Constructs the connection screen, initializing UI elements.
	/// </summary>
	ConnectScreen();

	/// <summary>
	/// Handles user input events, such as text entry and button clicks.
	/// </summary>
	/// <param name="event">SFML event to process.</param>
	/// <param name="window">Reference to the render window.</param>
	void handleEvent(sf::Event event, sf::RenderWindow& window) override;

	/// <summary>
	/// Draws the connection screen UI elements on the window.
	/// </summary>
	/// <param name="window">Reference to the render window.</param>
	void draw(sf::RenderWindow& window) override;

	/// <summary>
	/// Sets the status message displayed on the screen (e.g., connection errors).
	/// </summary>
	/// <param name="msg">The status message to display.</param>
	void setStatus(const std::string& msg);

private:
	TextField playerNameField; ///< Input field for entering the player's name.
	TextField ipField; ///< Input field for entering the server IP address.
	Button buttonConnect; ///< Button to initiate the connection.

	bool showStatus; ///< Indicates whether the status message should be displayed.

	sf::Text playerNameLabel; ///< Label for the player name input field.
	sf::Text ipLabel; ///< Label for the IP address input field.
	sf::Text statusLabel; ///< Label displaying the connection status message.
};
