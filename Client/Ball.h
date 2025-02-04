#ifndef BALL_H
#define BALL_H

#include "SFMLInclude.h"

class Ball {
public:
    Ball(float startX, float startY);
    void update(sf::RenderWindow& window, sf::RectangleShape& paddle1, sf::RectangleShape& paddle2);
    void draw(sf::RenderWindow& window);
    void reset(float startX, float startY);
    sf::Vector2f getPosition() const;
    bool isOutOfBounds(sf::Vector2u windowSize, bool& scoredForPlayer1, bool& scoredForPlayer2);

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

#endif
