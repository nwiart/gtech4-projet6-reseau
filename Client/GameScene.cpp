#include "GameScene.h"
#include "PongPackets.h"
#include "Network.h"

extern sf::Font font;

GameScene::GameScene()
    : player1(0.02f),
    player2(0.96f),
    ball(),
    score(font, sf::Vector2u(1280, 720))
{
    player1Text.setFont(font);
    player1Text.setCharacterSize(30);
    player1Text.setFillColor(sf::Color::White);
    player1Text.setString("player1Name");
    player1Text.setPosition(50, 20);

    player2Text.setFont(font);
    player2Text.setCharacterSize(30);
    player2Text.setFillColor(sf::Color::White);
    player2Text.setString("player2Name");
    player2Text.setPosition(1100, 20);
}


void GameScene::handleEvent(sf::Event event, sf::RenderWindow& window) {
    if (event.type == sf::Event::Closed)
        window.close();
}

void GameScene::update(sf::RenderWindow& window) {
    sendPlayerMove(window);
    receiveGameStateUDP();
}

void GameScene::draw(sf::RenderWindow& window) {
    window.draw(player1Text);
    window.draw(player2Text);

    ball.draw(window);
    score.draw(window);
    player1.draw(window);
    player2.draw(window);
}

void GameScene::sendPlayerMove(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    int paddleY = mousePos.y;

    Network::sendPosition(paddleY);
}

void GameScene::receiveGameStateUDP() {
    sockaddr_in senderAddr;
    Server_GameState state;

    if (network::receivePacketUDP(m_serverSocket, &senderAddr, state)) {
        ball.updateFromServer(state.ballX, state.ballY, state.ballRadius);
        score.update(state.scoreP1, state.scoreP2, sf::Vector2u(1280, 720));
        player1.updateFromServer(state.paddle1Y);
        player2.updateFromServer(state.paddle2Y);
    }
}
