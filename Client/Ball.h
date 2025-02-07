#ifndef BALL_H
#define BALL_H

#include "SFMLInclude.h"

class Ball {
public:
    Ball();

    void updateFromServer(float x, float y, float r);
    void draw(sf::RenderWindow& window);

private:
    sf::CircleShape shape;
};

#endif
