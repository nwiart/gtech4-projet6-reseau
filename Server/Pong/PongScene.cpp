#include "PongScene.h"

PongScene::PongScene(int sizeX, int sizeY)
    : sizeX(sizeX), sizeY(sizeY), paddle1Y(360), paddle2Y(360), started(false) {
}

void PongScene::startGame() {
    started = true;
    reset();
}

void PongScene::update(float dt) {
    if (!started) return;
    m_ball.update(dt, nullptr);
    checkCollisions();
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

void PongScene::getScoreInfo(int& score1, int& score2) const {
    score1 = m_score.getScore1();
    score2 = m_score.getScore2();
}

void PongScene::getBallInfo(float& xDir, float& yDir, float& speed) const {
    xDir = m_ball.getVelocity().x != 0 ? m_ball.getVelocity().x / abs(m_ball.getVelocity().x) : 0;
    yDir = m_ball.getVelocity().y != 0 ? m_ball.getVelocity().y / abs(m_ball.getVelocity().y) : 0;
    speed = sqrt(m_ball.getVelocity().x * m_ball.getVelocity().x + m_ball.getVelocity().y * m_ball.getVelocity().y);
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

void PongScene::reset() {
    paddle1Y = sizeY / 2 - 50;
    paddle2Y = sizeY / 2 - 50;
    m_ball.setPosition(sizeX / 2, sizeY / 2);
}