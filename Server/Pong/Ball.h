#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

class PongScene;

class Ball {
public:
    Ball();
    void update(float dt, PongScene* scene, sf::RectangleShape& paddle1, sf::RectangleShape& paddle2);
    void reset(float startX, float startY);
    const sf::Vector2f& getPosition() const;

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

#endif
