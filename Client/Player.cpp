#include "Player.h"

Player::Player(float xRatio) {
    paddle.setFillColor(sf::Color::White);
    paddle.setSize(sf::Vector2f(10.f, 100.f));
    paddle.setOrigin(5.f, 50.f);
    paddle.setPosition(1280 * xRatio, 360);
}

void Player::updateFromServer(float y) {
    paddle.setPosition(paddle.getPosition().x, y);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(paddle);
}
