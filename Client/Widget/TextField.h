#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "SFMLInclude.h"
#include <string>

/// <summary>
/// Represents an interactive text input field in the game, allowing players to enter text.
/// Can be used for general input or restricted to IP address format.
/// </summary>
class TextField {
public:
	/// <summary>
	/// Constructs a text field with a specified position, font, and optional IP format restriction.
	/// </summary>
	/// <param name="x">X coordinate of the text field.</param>
	/// <param name="y">Y coordinate of the text field.</param>
	/// <param name="font">Reference to the font used for text display.</param>
	/// <param name="isIpField">If true, restricts input to an IP address format.</param>
	TextField(float x, float y, sf::Font& font, bool isIpField = false);

	/// <summary>
	/// Handles user input events such as keyboard typing and focus changes.
	/// </summary>
	/// <param name="event">SFML event to process.</param>
	/// <param name="window">Reference to the render window.</param>
	void handleEvent(sf::Event event, sf::RenderWindow& window);

	/// <summary>
	/// Draws the text field on the window.
	/// </summary>
	/// <param name="window">Reference to the render window.</param>
	void draw(sf::RenderWindow& window);

	/// <summary>
	/// Retrieves the current user input as a string.
	/// </summary>
	/// <returns>Reference to the input string.</returns>
	const std::string& getInput() const { return input; }

	/// <summary>
	/// Clears the text field input.
	/// </summary>
	void clear() { input.clear(); }

private:
	sf::RectangleShape box; ///< Graphical representation of the text field's background.
	sf::Text text; ///< Displayed text inside the text field.
	std::string input; ///< Stores the user's input text.
	bool isActive; ///< Indicates whether the text field is active for user input.
	bool isIpField; ///< If true, limits input to characters valid for an IP address.
};

#endif // TEXTFIELD_H
