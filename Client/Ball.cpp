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
    velocity = sf::Vector2f((rand() % 2 == 0 ? -5 : 5), (rand() % 2 == 0 ? -5 : 5)); // Direction aléatoire
}

sf::Vector2f Ball::getPosition() const {
    return shape.getPosition();
}

bool Ball::isOutOfBounds(sf::Vector2u windowSize, bool& scoredForPlayer1, bool& scoredForPlayer2) {
    float ballRight = shape.getPosition().x + shape.getRadius() * 2;
    float ballLeft = shape.getPosition().x;

    if (ballRight < 0) {
        scoredForPlayer2 = true;
        return true;
    }
    else if (ballLeft > windowSize.x) {
        scoredForPlayer1 = true;
        return true;
    }

    return false;
}
