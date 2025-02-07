#include "Ball.h"

Ball::Ball() {
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(10.f, 10.f);
}

void Ball::updateFromServer(float x, float y, float r) {
    shape.setRadius(r);
    shape.setOrigin(r, r);
    shape.setPosition(x, y);
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
