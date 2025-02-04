#include "Ball.h"

#include "Pong/PongScene.h"

Ball::Ball()
{
    shape.setRadius(10);
    shape.setFillColor(sf::Color::White);
}

void Ball::update(float dt, PongScene* scene, sf::RectangleShape& paddle1, sf::RectangleShape& paddle2)
{
    shape.move(velocity * dt);

    if (shape.getPosition().y <= 0 || shape.getPosition().y + shape.getRadius() * 2 >= scene->getSizeY()) {
        velocity.y *= -1;
    }

    if (shape.getGlobalBounds().intersects(paddle1.getGlobalBounds()) ||
        shape.getGlobalBounds().intersects(paddle2.getGlobalBounds())) {
        velocity.x *= -1;
    }
}

void Ball::reset(float startX, float startY)
{
    shape.setPosition(startX, startY);
    velocity = sf::Vector2f(-30, 30);
}

const sf::Vector2f& Ball::getPosition() const
{
    return shape.getPosition();
}
