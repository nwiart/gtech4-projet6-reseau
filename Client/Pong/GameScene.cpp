#include "GameScene.h"
#include "PongPackets.h"

#include "Client.h"


GameScene::GameScene()
    : score(getGlobalFont(), sf::Vector2u(1280, 720)), twoTeams(false)
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
}

void GameScene::handleEvent(sf::Event event, sf::RenderWindow &window)
{
    if (event.type == sf::Event::Closed)
        window.close();
}

void GameScene::update(sf::RenderWindow &window)
{
    updateLocalPlayerMovement();
}

void GameScene::setPlayerPos(uint32_t id, int p)
{
    players[id].setPosition(p);
}

void GameScene::setBallInfo(const sf::Vector2f& pos, const sf::Vector2f& vel)
{
    ball.setPosition(pos.x, pos.y);
}


void GameScene::draw(sf::RenderWindow &window)
{
    window.draw(player1Text);
    window.draw(player2Text);

    score.draw(window);

    for (int i = 0; i < (twoTeams ? 4 : 2); i++) {
        players[i].draw(window);
    }

    ball.draw(window);
}

void GameScene::updateLocalPlayerMovement()
{
    int localID = Client::getInstance().getLobby().getLocalPlayerID();
    Player& localPlayer = players[localID];

    int paddleY = localPlayer.getPositionY();

    if (sf::Keyboard::isKeyPressed(localPlayer.getUpKey()))
    {
        paddleY -= 7;
    }
    else if (sf::Keyboard::isKeyPressed(localPlayer.getDownKey()))
    {
        paddleY += 7;
    }
    else
    {
        return;
    }
}
