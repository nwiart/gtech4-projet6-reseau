#include "Score.h"

Score::Score(sf::Font& font, sf::Vector2u windowSize) {
    scoreText.setFont(font);
    scoreText.setCharacterSize(150);
    scoreText.setFillColor(sf::Color(255, 255, 255, 100));
    scoreText.setStyle(sf::Text::Bold);

    scorePlayer1 = 0;
    scorePlayer2 = 0;

    update(scorePlayer1, scorePlayer2, windowSize);
}

void Score::update(int score1, int score2, sf::Vector2u windowSize) {
    scorePlayer1 = score1;
    scorePlayer2 = score2;

    scoreText.setString(std::to_string(scorePlayer1) + " - " + std::to_string(scorePlayer2));

    sf::FloatRect textRect = scoreText.getLocalBounds();
    scoreText.setOrigin(textRect.width / 2.f, textRect.height / 2.f);

}

void Score::draw(sf::RenderWindow& window) {
    scoreText.setPosition(window.getSize().x / 2.f, window.getSize().y / 4.f);

    window.draw(scoreText);
}

void Score::setScore(int score1, int score2)
{
    scorePlayer1 = score1;
    scorePlayer2 = score2;
    update(scorePlayer1, scorePlayer2, windowSize);
}
