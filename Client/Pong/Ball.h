#ifndef BALL_H
#define BALL_H

#include "SFMLInclude.h"

class Ball {
public:
    Ball();

    void setPosition(float x, float y);
    void setVelocity(const sf::Vector2f& velocity);
    void update(float dt);

    void updateFromServer(float x, float y, float xDir, float yDir, float speed);
    void draw(sf::RenderWindow& window);

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

#endif
