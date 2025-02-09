#ifndef BUTTON_H
#define BUTTON_H

#include "SFMLInclude.h"
#include <functional>
#include <string>

/// <summary>
/// Represents a clickable button in the game, handling rendering and user interaction.
/// </summary>
class Button {
public:
	/// <summary>
	/// Constructs a button with a specified position, label, font, and callback function.
	/// </summary>
	/// <param name="x">X coordinate of the button.</param>
	/// <param name="y">Y coordinate of the button.</param>
	/// <param name="label">Text displayed on the button.</param>
	/// <param name="font">Font used for the button label.</param>
	/// <param name="callback">Function to execute when the button is clicked.</param>
	Button(float x, float y, std::string label, sf::Font& font, std::function<void()> callback);

	/// <summary>
	/// Handles user input events, checking if the button is clicked.
	/// </summary>
	/// <param name="event">SFML event to process.</param>
	/// <param name="window">Reference to the render window for mouse position detection.</param>
	void handleEvent(sf::Event event, sf::RenderWindow& window);

	/// <summary>
	/// Draws the button on the screen.
	/// </summary>
	/// <param name="window">Reference to the render window.</param>
	void draw(sf::RenderWindow& window);

private:
	sf::RectangleShape box; ///< Rectangle shape representing the button's appearance.
	sf::Text text; ///< Text displayed on the button.
	std::function<void()> onClick; ///< Callback function executed when the button is clicked.
};

#endif // BUTTON_H
