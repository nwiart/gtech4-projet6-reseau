#include "GameScene.h"
#include "PongPackets.h"

GameScene::GameScene(sf::Font& font, const std::string& player1Name, const std::string& player2Name, Socket serverSocket)
    : player1(0.02f, 0.5f, sf::Keyboard::W, sf::Keyboard::S, true),
    player2(0.96f, 0.5f, sf::Keyboard::Up, sf::Keyboard::Down, false),
    ball(640, 360),
    score(font, sf::Vector2u(1280, 720)),
    m_serverSocket(serverSocket)
{
    player1Text.setFont(font);
    player1Text.setCharacterSize(30);
    player1Text.setFillColor(sf::Color::White);
    player1Text.setString(player1Name);
    player1Text.setPosition(50, 20);

    player2Text.setFont(font);
    player2Text.setCharacterSize(30);
    player2Text.setFillColor(sf::Color::White);
    player2Text.setString(player2Name);
    player2Text.setPosition(1100, 20);
}


void GameScene::handleEvent(sf::Event event, sf::RenderWindow& window) {
    if (event.type == sf::Event::Closed)
        window.close();
}

void GameScene::update(sf::RenderWindow& window) {
    player1.update(window.getSize().y, window);
    player2.update(window.getSize().y, window);

    ball.draw(window);

    sendPlayerMove(window);
    receiveGameStateUDP();
}

void GameScene::draw(sf::RenderWindow& window) {
    window.draw(player1Text);
    window.draw(player2Text);

    score.draw(window);
    player1.draw(window);
    player2.draw(window);
}

void GameScene::sendPlayerMove(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    float paddleY = static_cast<float>(mousePos.y);

    Client_PlayerMove packet;
    packet.position = paddleY;

    network::sendPacketTCP(m_serverSocket, (uint32_t)ClientPackets::PlayerMove, packet);
}

void GameScene::receiveGameStateUDP() {
    sockaddr_in senderAddr;
    GameState state;

    if (network::receivePacketUDP(m_serverSocket, &senderAddr, state)) {
        ball.setPosition(sf::Vector2f(state.ballX, state.ballY));
        score.update(state.scoreP1, state.scoreP2, sf::Vector2u(1280, 720));
        player1.setPosition(state.paddle1Y);
        player2.setPosition(state.paddle2Y);
    }
}
