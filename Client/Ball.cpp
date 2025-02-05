#include "Ball.h"

Ball::Ball(float startX, float startY) {
    shape.setRadius(10.f);
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(10.f, 10.f);
    shape.setPosition(startX, startY);
}

void Ball::setPosition(const sf::Vector2f& pos) {
    shape.setPosition(pos);
}

sf::Vector2f Ball::getPosition() const {
    return shape.getPosition();
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
