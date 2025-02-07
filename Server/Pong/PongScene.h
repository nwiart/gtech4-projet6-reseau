#pragma once

#include "Ball.h"
#include "Score.h"

class PongScene {
public:
    PongScene(int sizeX, int sizeY);
    void startGame();
    void update(float dt);

    void setPaddlePositions(float newPaddle1Y, float newPaddle2Y);
    void receivePlayerMove(uint32_t playerID, float positionY);

    void getGameState(float& ballX, float& ballY, float& ballRadius,
        float& paddle1Y, float& paddle2Y,
        int& score1, int& score2) const;
    void resetPaddles();

    inline Ball& getBall() { return m_ball; }
    inline float getPaddle1Y() { return paddle1Y; }
    inline float getPaddle2Y() { return paddle2Y; }
    inline Score& getScore() { return m_score; }

private:
    Ball m_ball;
    Score m_score;
    float paddle1Y, paddle2Y;
    int sizeX, sizeY;
    bool started;

    void checkCollisions();
};
