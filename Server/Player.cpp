#include "Player.h"

Player::Player(float x, float y) {
    position.x = x;
    position.y = y;
}

void Player::update(float dt, bool upPressed, bool downPressed, float windowHeight) {
    if (upPressed && position.y > 0) {
        position.y -= speed * dt;
    }
    if (downPressed && position.y + paddleHeight < windowHeight) {
        position.y += speed * dt;
    }
}

void Player::setPosition(float y) {
    position.y = y;
}

float Player::getPositionY() const {
    return position.y;
}

void Player::getNetworkData(float& y) const {
    y = position.y;
}
