#include "Ball.h"
#include <iostream>
#include <cmath>
#include <corecrt_math_defines.h>

Ball::Ball()
{
    resetPosition();
    radius = 10;
}

void Ball::update(float dt, PongScene* scene) {
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;

    std::cout << "[DEBUG] Ball update - Position X: " << position.x << ", Y: " << position.y << std::endl;
}


void Ball::resetPosition() {
    std::cout << "[DEBUG] Réinitialisation de la balle au centre." << std::endl;

    position.x = 640;
    position.y = 360;

    float angle = ((rand() % 120) - 60) * (M_PI / 180.0f);
    float speed = 300.0f;

    velocity.x = cos(angle) * speed * ((rand() % 2 == 0) ? 1 : -1);
    velocity.y = sin(angle) * speed;

    std::cout << "[DEBUG] Nouvelle direction balle : X=" << velocity.x << " Y=" << velocity.y << std::endl;
}

void Ball::reverseX() {
    velocity.x = -velocity.x;
    std::cout << "[DEBUG] Rebond horizontal, nouvelle vitesse X=" << velocity.x << std::endl;
}

void Ball::reverseY() {
    velocity.y = -velocity.y;
    std::cout << "[DEBUG] Rebond vertical, nouvelle vitesse Y=" << velocity.y << std::endl;
}

void Ball::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

void Ball::setVelocity(float vx, float vy) {
    velocity.x = vx;
    velocity.y = vy;
}
