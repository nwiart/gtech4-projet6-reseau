#pragma once

#include "Ball.h"
#include "Score.h"

class PongScene {
public:
    PongScene(int sizeX, int sizeY);

    void update(float dt);

    const Ball& getBall() const { return m_ball; }

    int getSizeX() const { return m_sizeX; }
    int getSizeY() const { return m_sizeY; }

    int getScoreP1() const { return scoreP1; }
    int getScoreP2() const { return scoreP2; }

    void setPaddlePositions(float paddle1Y, float paddle2Y);

private:
    Ball m_ball;
    int m_sizeX;
    int m_sizeY;
    bool m_started;

    int scoreP1 = 0;
    int scoreP2 = 0;

    float paddle1Y;
    float paddle2Y;

    Score score;

    void checkCollisions();
};
