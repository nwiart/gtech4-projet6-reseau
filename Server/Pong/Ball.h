#ifndef BALL_H
#define BALL_H

#include <SFML/System/Vector2.hpp>

class PongScene;

class Ball {
public:
    Ball();

    void update(float dt, PongScene* scene);
    void reset(float startX, float startY);

    const sf::Vector2f& getPosition() const { return position; }
    const sf::Vector2f& getVelocity() const { return velocity; }

    void reverseX();
    void reverseY();

private:
    sf::Vector2f position;
    sf::Vector2f velocity;
};

#endif
