#ifndef SCORE_H
#define SCORE_H

#include <SFML/Graphics.hpp>

class Score {
public:
    Score(sf::Font& font, sf::Vector2u windowSize);

    void update(int score1, int score2, sf::Vector2u windowSize);
    void draw(sf::RenderWindow& window);

    void setScore(int score1, int score2);

private:
    sf::Text scoreText;
    int scorePlayer1;
    int scorePlayer2;
    sf::Vector2u windowSize;
};

#endif
