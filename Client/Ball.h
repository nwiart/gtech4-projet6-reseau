#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

class Ball {
public:
    Ball(float startX, float startY);
    void update(sf::RenderWindow& window, sf::RectangleShape& paddle1, sf::RectangleShape& paddle2);
    void draw(sf::RenderWindow& window);
    void reset(float startX, float startY);
    sf::Vector2f getPosition() const;

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

#endif
