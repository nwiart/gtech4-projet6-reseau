#pragma once

#include "Scene.h"
#include "Widget/TextField.h"
#include "Widget/Button.h"

#include <map>

/// <summary>
/// Represents the main menu of the game, allowing players to create a lobby,
/// refresh the lobby list, and join available lobbies.
/// </summary>
class MainMenu : public Scene {
public:
	/// <summary>
	/// Constructs the main menu, initializing UI elements.
	/// </summary>
	MainMenu();

	/// <summary>
	/// Handles user input events such as button clicks.
	/// </summary>
	/// <param name="event">SFML event to process.</param>
	/// <param name="window">Reference to the render window.</param>
	void handleEvent(sf::Event event, sf::RenderWindow& window) override;

	/// <summary>
	/// Draws the UI elements of the main menu on the screen.
	/// </summary>
	/// <param name="window">Reference to the render window.</param>
	void draw(sf::RenderWindow& window) override;

	/// <summary>
	/// Sends a request to the server to refresh the list of available lobbies.
	/// </summary>
	void refreshLobbyList();

	/// <summary>
	/// Adds a lobby to the list of available lobbies.
	/// </summary>
	/// <param name="id">Unique identifier of the lobby.</param>
	/// <param name="name">Lobby name.</param>
	/// <param name="numPlayers">Current number of players in the lobby.</param>
	/// <param name="maxPlayers">Maximum number of players allowed in the lobby.</param>
	void listLobby(uint32_t id, const char* name, int numPlayers, int maxPlayers);

private:
	Button createButton; ///< Button to create a new lobby.
	Button buttonRefresh; ///< Button to refresh the list of available lobbies.

	std::map<uint32_t, Button> lobbies; ///< Stores buttons representing available lobbies.
};
