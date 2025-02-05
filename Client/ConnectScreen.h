#pragma once

#include "Scene.h"
#include "TextField.h"
#include "Button.h"
#include "Network.h"

class ConnectScreen : public Scene
{
public:
    ConnectScreen(sf::Font& font);
    void handleEvent(sf::Event event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

private:
    TextField playerNameField;
    TextField ipField;
    Button buttonConnect;

    std::function<void(std::string, std::string)> startGame;

    Network network;
    bool showStatus = false;
    bool isConnected = false;
    std::string opponentName;

    sf::Font font;
    sf::Text playerNameLabel;
    sf::Text ipLabel;
    sf::Text statusLabel;
};
