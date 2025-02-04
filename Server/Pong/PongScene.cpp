#include "PongScene.h"
#include <iostream>

PongScene::PongScene(int sizeX, int sizeY)
    : m_sizeX(sizeX), m_sizeY(sizeY), m_started(false), paddle1Y(sizeY / 2), paddle2Y(sizeY / 2)
{
    m_ball.reset(sizeX / 2, sizeY / 2);
}

void PongScene::update(float dt) {
    m_ball.update(dt, this);
    checkCollisions();
}

void PongScene::checkCollisions() {
    float ballX = m_ball.getPosition().x;
    float ballY = m_ball.getPosition().y;

    if (ballY <= 0 || ballY >= m_sizeY) {
        m_ball.reverseY();
    }

    if ((ballX <= 30 && ballY >= paddle1Y && ballY <= paddle1Y + 100) ||
        (ballX >= m_sizeX - 30 && ballY >= paddle2Y && ballY <= paddle2Y + 100)) {
        m_ball.reverseX();
    }

    if (ballX < 0) {
        score.addPointPlayer2();
        m_ball.reset(m_sizeX / 2, m_sizeY / 2);
    }
    if (ballX > m_sizeX) {
        score.addPointPlayer1();
        m_ball.reset(m_sizeX / 2, m_sizeY / 2);
    }

    if (score.isGameOver()) {
        score.reset();
    }
}


void PongScene::setPaddlePositions(float p1, float p2) {
    paddle1Y = p1;
    paddle2Y = p2;
}
