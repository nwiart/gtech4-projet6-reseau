#include "Ball.h"

#include <iostream>

Ball::Ball() {
    shape.setFillColor(sf::Color::White);
    shape.setRadius(20.0F);
    //shape.setOrigin(10.f, 10.f);
}

void Ball::setPosition(float x, float y)
{
    shape.setPosition(x, y);
}

void Ball::updateFromServer(float x, float y, float r) {
    shape.setRadius(r);
    shape.setOrigin(r, r);
    shape.setPosition(x, y);
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
