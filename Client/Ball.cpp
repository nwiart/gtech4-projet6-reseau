#include "Ball.h"

Ball::Ball(float startX, float startY) {
    shape.setRadius(10);
    shape.setFillColor(sf::Color::White);
    shape.setPosition(startX, startY);
    velocity = sf::Vector2f(-5, 5);
}

void Ball::update(sf::RenderWindow& window, sf::RectangleShape& paddle1, sf::RectangleShape& paddle2) {
    shape.move(velocity);

    if (shape.getPosition().y <= 0 || shape.getPosition().y + shape.getRadius() * 2 >= window.getSize().y) {
        velocity.y *= -1;
    }

    if (shape.getGlobalBounds().intersects(paddle1.getGlobalBounds()) ||
        shape.getGlobalBounds().intersects(paddle2.getGlobalBounds())) {
        velocity.x *= -1;
    }
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Ball::reset(float startX, float startY) {
    shape.setPosition(startX, startY);
    velocity = sf::Vector2f(-5, 5);
}

sf::Vector2f Ball::getPosition() const {
    return shape.getPosition();
}
