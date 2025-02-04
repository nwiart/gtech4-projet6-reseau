#include "GameScene.h"

GameScene::GameScene(sf::Font& font, const std::string& player1Name, const std::string& player2Name)
    : player1(0.02f, 0.5f, sf::Keyboard::W, sf::Keyboard::S, true),
    player2(0.96f, 0.5f, sf::Keyboard::Up, sf::Keyboard::Down, false),
    ball(640, 360),
    score(font, sf::Vector2u(1280, 720))
{
    player1Text.setFont(font);
    player1Text.setCharacterSize(30);
    player1Text.setFillColor(sf::Color::White);
    player1Text.setString(player1Name);
    player1Text.setPosition(50, 20);

    player2Text.setFont(font);
    player2Text.setCharacterSize(30);
    player2Text.setFillColor(sf::Color::White);
    player2Text.setString(player2Name);
    player2Text.setPosition(1100, 20);
}

void GameScene::handleEvent(sf::Event event, sf::RenderWindow& window) {
    if (event.type == sf::Event::Closed)
        window.close();
}

void GameScene::update(sf::RenderWindow& window) {
    player1.update(window.getSize().y, window);
    player2.update(window.getSize().y, window);

    bool scoredForPlayer1 = false, scoredForPlayer2 = false;
    if (ball.isOutOfBounds(window.getSize(), scoredForPlayer1, scoredForPlayer2)) {
        if (scoredForPlayer1) {
            score1++;
        }
        if (scoredForPlayer2) {
            score2++;
        }

        ball.reset(window.getSize().x / 2.f, window.getSize().y / 2.f);
        score.update(score1, score2, window.getSize());
    }

    ball.update(window, player1.getPaddle(), player2.getPaddle());
}

void GameScene::draw(sf::RenderWindow& window) {
    window.draw(player1Text);
    window.draw(player2Text);

    score.draw(window);
    player1.draw(window);
    player2.draw(window);
    ball.draw(window);
}
