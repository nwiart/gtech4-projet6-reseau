#include "Ball.h"
#include "PongScene.h"

Ball::Ball() {
    reset(400, 300);
}

void Ball::update(float dt, PongScene* scene) {
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
}

void Ball::reset(float startX, float startY) {
    position = sf::Vector2f(startX, startY);
    velocity = sf::Vector2f(-200.f, 150.f);
}

void Ball::reverseX() {
    velocity.x = -velocity.x;
}

void Ball::reverseY() {
    velocity.y = -velocity.y;
}
