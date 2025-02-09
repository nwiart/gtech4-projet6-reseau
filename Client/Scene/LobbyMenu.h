#pragma once

#include "Scene.h"
#include "Widget/TextField.h"
#include "Widget/Button.h"

/// <summary>
/// Represents the lobby menu where players can see other participants,
/// start the game, or leave the lobby.
/// </summary>
class LobbyMenu : public Scene
{
public:
	/// <summary>
	/// Constructs the lobby menu, initializing UI elements.
	/// </summary>
	LobbyMenu();

	/// <summary>
	/// Handles user input events such as button clicks.
	/// </summary>
	/// <param name="event">SFML event to process.</param>
	/// <param name="window">Reference to the render window.</param>
	void handleEvent(sf::Event event, sf::RenderWindow& window) override;

	/// <summary>
	/// Draws the UI elements of the lobby menu on the screen.
	/// </summary>
	/// <param name="window">Reference to the render window.</param>
	void draw(sf::RenderWindow& window) override;

private:
	Button buttonStart; ///< Button to start the game (only available to the host).
	Button buttonLeave; ///< Button to leave the lobby.

	std::map<uint32_t, sf::Text> m_playersInLobby; ///< List of players currently in the lobby.

	sf::Text labelLobbyName; ///< Displays the name of the lobby.
	sf::Text labelNumPlayers; ///< Displays the current number of players in the lobby.
};
