#ifndef MAINMENU_H
#define MAINMENU_H

#include "Scene.h"
#include "TextField.h"
#include "Button.h"
#include "Network.h"

class MainMenu : public Scene {
public:
    MainMenu(sf::Font& font, std::function<void(std::string, std::string)> startGameCallback);
    void handleEvent(sf::Event event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

private:
    TextField playerNameField;
    TextField ipField;
    Button createButton;
    Button joinButton;
    std::function<void(std::string, std::string)> startGame;

    Network network;
    bool isConnected = false;
    std::string opponentName;

    sf::Text playerNameLabel;
    sf::Text ipLabel;
};

#endif
