#include "GameScene.h"
#include "PongPackets.h"
#include "Client.h"
#include <iostream>

GameScene::GameScene()
    : score(getGlobalFont(), sf::Vector2u(1280, 720)), twoTeams(false)
    , localTimer(0.0)
{
    player1Text.setFont(getGlobalFont());
    player1Text.setCharacterSize(30);
    player1Text.setFillColor(sf::Color::White);
    player1Text.setString("player1Name");
    player1Text.setPosition(50, 20);

    player2Text.setFont(getGlobalFont());
    player2Text.setCharacterSize(30);
    player2Text.setFillColor(sf::Color::White);
    player2Text.setString("player2Name");
    player2Text.setPosition(1100, 20);

    player1PingText.setFont(getGlobalFont());
    player1PingText.setCharacterSize(20);
    player1PingText.setFillColor(sf::Color::White);
    player1PingText.setString("Ping: 15 ms");
    player1PingText.setPosition(50, 70);

    player2PingText.setFont(getGlobalFont());
    player2PingText.setCharacterSize(20);
    player2PingText.setFillColor(sf::Color::White);
    player2PingText.setString("Ping: 15 ms");
    player2PingText.setPosition(1100, 70);

    players[0].setX(50);
    players[1].setX(1280 - 60);
}

void GameScene::handleEvent(sf::Event event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::Closed)
        window.close();
}

void GameScene::update(sf::RenderWindow& window, double dt)
{
    localTimer += dt;

    updateLocalPlayerMovement(dt);
    ball.update(dt); // Update the ball's position
}

void GameScene::updateLocalPlayerMovement(double dt)
{
    int localID = Client::getInstance().getLobby().getLocalPlayerID();
    Player& localPlayer = players[localID];

    float paddleY = localPlayer.getPositionY();
    if (sf::Keyboard::isKeyPressed(localPlayer.getUpKey()))
    {
        paddleY -= 400 * dt;
    }
    else if (sf::Keyboard::isKeyPressed(localPlayer.getDownKey()))
    {
        paddleY += 400 * dt;
    }
    localPlayer.setPosition(paddleY);

    if (localTimer >= 1.0 / 20.0) {
        localTimer = 0;
        Client::getInstance().sendPosition(paddleY);
    }
}

void GameScene::updatePlayerPing(uint32_t playerID, int ping)
{
    playerPings[playerID] = ping;

    // Met à jour les textes selon l'ID du joueur
    if (playerID == 0) {
        player1PingText.setString("Ping: " + std::to_string(ping) + " ms");
    }
    else if (playerID == 1) {
        player2PingText.setString("Ping: " + std::to_string(ping) + " ms");
    }
}

void GameScene::updatePlayerName(uint32_t playerID, const std::string& name)
{
    if (playerID == 0) {
        player1Text.setString(name);
    }
    else if (playerID == 1) {
        player2Text.setString(name);
    }
}


void GameScene::setPlayerPos(uint32_t id, int p)
{
    players[id].setPosition(p);
}

void GameScene::setBallInfo(float x, float y, float xDir, float yDir, float speed)
{
    ball.updateFromServer(x, y, xDir, yDir, speed);
}

void GameScene::setScore(int score1, int score2)
{
    score.setScore(score1, score2);
}

void GameScene::draw(sf::RenderWindow& window)
{
    window.draw(player1Text);
    window.draw(player2Text);

    window.draw(player1PingText);
    window.draw(player2PingText);

    score.draw(window);

    for (int i = 0; i < (twoTeams ? 4 : 2); i++) {
        players[i].draw(window);
    }

    ball.draw(window); // Draw the ball
}

