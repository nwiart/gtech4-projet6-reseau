#pragma once

#include "Scene/Scene.h"

#include "Widget/Score.h"

#include "Ball.h"
#include "Player.h"


class GameScene : public Scene
{
public:
    GameScene();
    void handleEvent(sf::Event event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

    void updatePlayerMovement();

    void setPlayerPos(int p);
    void setBallInfo(const sf::Vector2f& pos, const sf::Vector2f& vel);

private:
    Player player1;
    Player player2;
    Ball ball;
    Score score;
    sf::Text player1Text;
    sf::Text player2Text;

    int score1 = 0;
    int score2 = 0;
};
