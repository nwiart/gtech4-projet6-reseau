#include "PongScene.h"
#include <iostream>
#include <cmath>

PongScene::PongScene(int sizeX, int sizeY)
    : m_sizeX(sizeX), m_sizeY(sizeY), m_started(false), paddle1Y(sizeY / 2), paddle2Y(sizeY / 2)
{
    m_ball.resetPosition();
}

void PongScene::startGame() {
    m_started = true;
}


void PongScene::update(float dt) {
    if (!m_started) return;

    m_ball.update(dt, this);
    checkCollisions();
}

void PongScene::checkCollisions() {
    float ballX = m_ball.getPosition().x;
    float ballY = m_ball.getPosition().y;

    float ballRadius = m_ball.getRadius();
    float paddleWidth = 10;
    float paddleHeight = 100;

    if (ballY - ballRadius <= 0 || ballY + ballRadius >= m_sizeY) {
        m_ball.reverseY();
    }

    if ((ballX - ballRadius <= paddleWidth && ballY >= paddle1Y && ballY <= paddle1Y + paddleHeight) ||
        (ballX + ballRadius >= m_sizeX - paddleWidth && ballY >= paddle2Y && ballY <= paddle2Y + paddleHeight)) {
        m_ball.reverseX();
    }

    if (ballX < 0) { // Joueur 2 marque
        score.addPointPlayer2();
        resetBall();
    }
    else if (ballX > m_sizeX) { // Joueur 1 marque
        score.addPointPlayer1();
        resetBall();
    }

    if (score.isGameOver()) {
        score.reset();
    }
}

void PongScene::resetBall() {
    m_ball.resetPosition();
}

void PongScene::setPaddlePositions(float p1, float p2) {
    paddle1Y = p1;
    paddle2Y = p2;
}
