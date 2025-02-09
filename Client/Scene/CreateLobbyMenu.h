#pragma once

#include "Scene.h"
#include "Widget/TextField.h"
#include "Widget/Button.h"

/// <summary>
/// Represents the menu where players can create a new game lobby,
/// selecting either a 1v1 or 2v2 Pong match.
/// </summary>
class CreateLobbyMenu : public Scene
{
public:
	/// <summary>
	/// Constructs the lobby creation menu, initializing UI elements.
	/// </summary>
	CreateLobbyMenu();

	/// <summary>
	/// Handles user input events, such as text entry and button clicks.
	/// </summary>
	/// <param name="event">SFML event to process.</param>
	/// <param name="window">Reference to the render window.</param>
	void handleEvent(sf::Event event, sf::RenderWindow& window) override;

	/// <summary>
	/// Draws the UI elements of the lobby creation menu on the window.
	/// </summary>
	/// <param name="window">Reference to the render window.</param>
	void draw(sf::RenderWindow& window) override;

private:
	TextField fieldLobbyName; ///< Input field for entering the lobby name.
	Button buttonCreatePong1v1; ///< Button to create a 1v1 Pong lobby.
	Button buttonCreatePong2v2; ///< Button to create a 2v2 Pong lobby.
	Button buttonBack; ///< Button to go back to the previous menu.

	sf::Text labelLobbyName; ///< Label for the lobby name input field.
};
