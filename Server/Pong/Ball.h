#ifndef BALL_H
#define BALL_H

#include <SFML/System/Vector2.hpp>

class PongScene;

class Ball {
public:
    Ball();

    void resetPosition();
    void update(float dt, PongScene* scene);

    const sf::Vector2f& getPosition() const { return position; }
    const sf::Vector2f& getVelocity() const { return velocity; }
    const float getRadius() const { return radius;  }

    void setPosition(float x, float y);
    void setVelocity(float x, float y);

    void reverseX();
    void reverseY();

private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float radius;
};

#endif
