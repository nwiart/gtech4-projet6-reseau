#ifndef SCORE_H
#define SCORE_H

#include <SFML/Graphics.hpp>

/// <summary>
/// Manages the display and updating of the game score in a Pong match.
/// </summary>
class Score {
public:
	/// <summary>
	/// Constructs the Score object, initializing text properties and position.
	/// </summary>
	/// <param name="font">Reference to the font used for displaying the score.</param>
	/// <param name="windowSize">Size of the game window to position the score correctly.</param>
	Score(sf::Font& font, sf::Vector2u windowSize);

	/// <summary>
	/// Updates the displayed score and repositions it based on the window size.
	/// </summary>
	/// <param name="score1">Current score of player 1.</param>
	/// <param name="score2">Current score of player 2.</param>
	/// <param name="windowSize">Current size of the game window.</param>
	void update(int score1, int score2, sf::Vector2u windowSize);

	/// <summary>
	/// Draws the score text on the window.
	/// </summary>
	/// <param name="window">Reference to the render window.</param>
	void draw(sf::RenderWindow& window);

	/// <summary>
	/// Sets the current score values for both players.
	/// </summary>
	/// <param name="score1">New score for player 1.</param>
	/// <param name="score2">New score for player 2.</param>
	void setScore(int score1, int score2);

private:
	sf::Text scoreText; ///< SFML text object used to display the score.
	int scorePlayer1; ///< Player 1's current score.
	int scorePlayer2; ///< Player 2's current score.
	sf::Vector2u windowSize; ///< Current size of the game window.
};

#endif // SCORE_H
