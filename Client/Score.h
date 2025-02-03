#ifndef SCORE_H
#define SCORE_H

#include "SFMLInclude.h"

class Score {
public:
    Score(sf::Font& font, sf::Vector2u windowSize);
    void update(int score1, int score2, sf::Vector2u windowSize);
    void draw(sf::RenderWindow& window);

private:
    sf::Text scoreText;
    int scorePlayer1;
    int scorePlayer2;
};

#endif
