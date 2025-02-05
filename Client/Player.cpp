#include "Player.h"

Player::Player(float xRatio, float yRatio, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey, bool useMouse)
    : upKey(upKey), downKey(downKey), useMouse(useMouse) {
    paddle.setFillColor(sf::Color::White);
    resize(sf::Vector2u(1280, 720));
    paddle.setPosition(1280 * xRatio, 720 * yRatio);
}

void Player::update(float windowHeight, sf::RenderWindow& window) {
    if (useMouse) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        float newY = static_cast<float>(mousePos.y) - (paddle.getSize().y / 2);

        if (newY < 0) newY = 0;
        if (newY + paddle.getSize().y > windowHeight) newY = windowHeight - paddle.getSize().y;

        paddle.setPosition(paddle.getPosition().x, newY);
    }
    else {
        if (sf::Keyboard::isKeyPressed(upKey) && paddle.getPosition().y > 0)
            paddle.move(0, -7);

        if (sf::Keyboard::isKeyPressed(downKey) && paddle.getPosition().y + paddle.getSize().y < windowHeight)
            paddle.move(0, 7);
    }
}

void Player::resize(sf::Vector2u windowSize) {
    paddle.setSize(sf::Vector2f(windowSize.x * RECT_WIDTH_RATIO, windowSize.y * RECT_HEIGHT_RATIO));
    paddle.setPosition(paddle.getPosition().x, windowSize.y / 2.f - paddle.getSize().y / 2.f);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(paddle);
}

void Player::setPosition(float y) {
    paddle.setPosition(paddle.getPosition().x, y);
}

float Player::getPositionY() const {
    return paddle.getPosition().y;
}
