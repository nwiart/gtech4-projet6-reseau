#include "Player.h"

Player::Player()
    : upKey(sf::Keyboard::Z), downKey(sf::Keyboard::S) {
    paddle.setFillColor(sf::Color::White);
    paddle.setSize(sf::Vector2f(10.f, 100.f));
    paddle.setOrigin(5.f, 50.f);
    paddle.setPosition(0, 360);
}

Player::Player(float xRatio, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey)
    : upKey(upKey), downKey(downKey) {
    paddle.setFillColor(sf::Color::White);
    paddle.setSize(sf::Vector2f(10.f, 100.f));
    paddle.setOrigin(5.f, 50.f);
    paddle.setPosition(1280 * xRatio, 360);
}

sf::Keyboard::Key Player::getUpKey() const {
    return upKey;
}

sf::Keyboard::Key Player::getDownKey() const {
    return downKey;
}

void Player::updateFromServer(float y) {
    paddle.setPosition(paddle.getPosition().x, y);
}

float Player::getPositionY() const {
    return paddle.getPosition().y;
}

void Player::setPosition(float y) {
    paddle.setPosition(paddle.getPosition().x, y);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(paddle);
}
