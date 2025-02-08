#ifndef PONGSCENE_H
#define PONGSCENE_H

#include "Ball.h"
#include "Score.h"

class PongScene {
public:
    PongScene(int sizeX, int sizeY);
    void startGame();
    void update(float dt);
    float receivePlayerMove(uint32_t playerID, float positionY);

    void getScoreInfo(int& score1, int& score2) const;
    void getBallInfo(float& x, float& y, float& xDir, float& yDir, float& speed) const;

    int getWidth() const { return sizeX; }
    int getHeight() const { return sizeY; }
    float getPaddle1Y() const { return paddle1Y; }
    float getPaddle2Y() const { return paddle2Y; }

    void addPointToPlayer(bool player1);
    void reset();

private:
    void checkCollisions();

    static constexpr int PADDLE_MARGIN = 50;
    static constexpr int PADDLE_WIDTH = 10;

    int sizeX, sizeY;
    float paddle1Y, paddle2Y;
    bool started;
    Ball m_ball;
    Score m_score;
};

#endif // PONGSCENE_H