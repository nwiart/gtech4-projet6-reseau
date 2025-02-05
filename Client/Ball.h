#ifndef BALL_H
#define BALL_H

#include "SFMLInclude.h"

class Ball {
public:
    Ball(float startX, float startY);

    void draw(sf::RenderWindow& window);
    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;

private:
    sf::CircleShape shape;
};

#endif
