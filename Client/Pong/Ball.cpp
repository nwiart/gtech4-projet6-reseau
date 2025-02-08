#include "Ball.h"
#include <iostream>

Ball::Ball() 
{
    shape.setRadius(10);
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(10, 10);
    velocity = { 0, 0 };
}

void Ball::setPosition(float x, float y) {
    shape.setPosition(x, y);
}

void Ball::setVelocity(const sf::Vector2f& velocity) {
    this->velocity = velocity;
}

void Ball::update(float dt) {
    shape.move(velocity * dt);
}

void Ball::updateFromServer(float x, float y, float xDir, float yDir, float speed) {
    setPosition(x, y);
    velocity.x = xDir * speed;
    velocity.y = yDir * speed;
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(shape);
}