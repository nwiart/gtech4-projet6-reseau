#include "Ball.h"
#include <iostream>
#include <cmath>
#include <corecrt_math_defines.h>

Ball::Ball() {
    resetPosition();
    radius = 10.0f;
}

void Ball::update(float dt, PongScene* scene) {
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
}

void Ball::resetPosition() {
    position.x = 640;
    position.y = 360;

    float angle = ((rand() % 120) - 60) * (M_PI / 180.0f);
    float speed = 100.0f;

    velocity.x = cos(angle) * speed * ((rand() % 2 == 0) ? 1 : -1);
    velocity.y = sin(angle) * speed;
}

void Ball::reverseX() {
    velocity.x = -velocity.x;
}

void Ball::reverseY() {
    velocity.y = -velocity.y;
}

void Ball::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

void Ball::setVelocity(float vx, float vy) {
    velocity.x = vx;
    velocity.y = vy;
}

void Ball::setRadius(float r) {
    radius = r;
}

float Ball::getRadius() const {
    return radius;
}

void Ball::getNetworkData(float& x, float& y, float& r) const {
    x = position.x;
    y = position.y;
    r = radius;
}
