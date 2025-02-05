#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "Ball.h"
#include "Player.h"
#include "Score.h"
#include "Networking.h"

class GameScene : public Scene {
public:
    GameScene(sf::Font& font, const std::string& player1Name, const std::string& player2Name);
    void handleEvent(sf::Event event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

    void receiveGameStateUDP();
    void sendPlayerMove(sf::RenderWindow& window);
private:
    Player player1;
    Player player2;
    Ball ball;
    Score score;
    sf::Text player1Text;
    sf::Text player2Text;

    int score1 = 0;
    int score2 = 0;
    Socket m_serverSocket;
};

#endif
