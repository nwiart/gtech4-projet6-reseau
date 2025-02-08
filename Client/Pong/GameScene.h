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
    void update(sf::RenderWindow& window, double dt) override;
    void draw(sf::RenderWindow& window) override;

    void updateLocalPlayerMovement(double dt);

    void setPlayerPos(uint32_t id, int p);
    void setBallInfo(float x, float y, float xDir, float yDir, float speed);
    void setScore(int score1, int score2);

private:
    static constexpr int MAX_PLAYERS = 4;

    Player players[MAX_PLAYERS];
    Ball ball;
    Score score;
    sf::Text player1Text;
    sf::Text player2Text;

    bool twoTeams;
    int scoreTeam1 = 0;
    int scoreTeam2 = 0;

    double localTimer;
};
