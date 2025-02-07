#include "PongScene.h"

PongScene::PongScene(int sizeX, int sizeY)
    : sizeX(sizeX), sizeY(sizeY), paddle1Y(360), paddle2Y(360), started(false) {}

void PongScene::startGame() {
    started = true;

    setPaddlePositions(sizeY / 2 - 50, sizeY / 2 - 50);

}

void PongScene::update(float dt) {
    if (!started) return;

    m_ball.update(dt, nullptr);
    checkCollisions();
}

void PongScene::setPaddlePositions(float newPaddle1Y, float newPaddle2Y) {
    if (newPaddle1Y < 0) newPaddle1Y = 0;
    if (newPaddle1Y > sizeY - 100) newPaddle1Y = sizeY - 100;

    if (newPaddle2Y < 0) newPaddle2Y = 0;
    if (newPaddle2Y > sizeY - 100) newPaddle2Y = sizeY - 100;

    paddle1Y = newPaddle1Y;
    paddle2Y = newPaddle2Y;

    float paddle1X = 50;
    float paddle2X = sizeX - 60;

    m_ball.setPosition(sizeX / 2, sizeY / 2);
}

void PongScene::receivePlayerMove(uint32_t playerID, float positionY) {
    if (playerID == 0) {
        if (positionY >= 0 && positionY <= sizeY - 100)
            paddle1Y = positionY;
    }
    else if (playerID == 1) {
        if (positionY >= 0 && positionY <= sizeY - 100)
            paddle2Y = positionY;
    }
}

void PongScene::getGameState(float& ballX, float& ballY, float& ballRadius,
    float& paddle1Y, float& paddle2Y,
    int& score1, int& score2) const {
    ballX = m_ball.getPosition().x;
    ballY = m_ball.getPosition().y;
    ballRadius = m_ball.getRadius();
    paddle1Y = this->paddle1Y;
    paddle2Y = this->paddle2Y;
    score1 = m_score.getScore1();
    score2 = m_score.getScore2();
}

void PongScene::checkCollisions() {
    if (m_ball.getPosition().y - m_ball.getRadius() <= 0 ||
        m_ball.getPosition().y + m_ball.getRadius() >= sizeY) {
        m_ball.reverseY();
    }

    float ballLeft = m_ball.getPosition().x - m_ball.getRadius();
    float ballRight = m_ball.getPosition().x + m_ball.getRadius();
    float ballTop = m_ball.getPosition().y - m_ball.getRadius();
    float ballBottom = m_ball.getPosition().y + m_ball.getRadius();

    float paddle1Left = 50;
    float paddle1Right = 60;
    float paddle1Top = paddle1Y;
    float paddle1Bottom = paddle1Y + 100;

    float paddle2Left = sizeX - 60;
    float paddle2Right = sizeX - 50;
    float paddle2Top = paddle2Y;
    float paddle2Bottom = paddle2Y + 100;

    if (ballLeft <= paddle1Right && ballRight >= paddle1Left &&
        ballBottom >= paddle1Top && ballTop <= paddle1Bottom) {
        m_ball.reverseX();

        float offset = (m_ball.getPosition().y - (paddle1Y + 50)) / 50;
        m_ball.setVelocity(m_ball.getVelocity().x, offset * 200.0f);
    }

    if (ballRight >= paddle2Left && ballLeft <= paddle2Right &&
        ballBottom >= paddle2Top && ballTop <= paddle2Bottom) {
        m_ball.reverseX();

        float offset = (m_ball.getPosition().y - (paddle2Y + 50)) / 50;
        m_ball.setVelocity(m_ball.getVelocity().x, offset * 200.0f);
    }

    if (m_ball.getPosition().x < 0) {
        m_score.addPoint(false);
        m_ball.resetPosition();
    }
    else if (m_ball.getPosition().x > sizeX) {
        m_score.addPoint(true);
        m_ball.resetPosition();
    }
}

void PongScene::resetPaddles() {
    setPaddlePositions(sizeY / 2 - 50, sizeY / 2 - 50);
}