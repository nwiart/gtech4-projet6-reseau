#pragma once

#include "Ball.h"
#include "Score.h"

class PongScene {
public:
    PongScene(int sizeX, int sizeY);
    void startGame();
    void update(float dt);

    void receivePlayerMove(uint32_t playerID, float positionY);
    void getScoreInfo(int& score1, int& score2) const;
    void getBallInfo(float& xDir, float& yDir, float& speed) const;

    void reset();

    inline Ball& getBall() { return m_ball; }

private:
    Ball m_ball;
    Score m_score;
    float paddle1Y, paddle2Y;
    int sizeX, sizeY;
    bool started;

    void checkCollisions();
};
