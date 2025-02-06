#pragma once

#include "Scene.h"
#include "TextField.h"
#include "Button.h"

class CreateLobbyMenu : public Scene
{
public:
    CreateLobbyMenu(sf::Font& font);
    void handleEvent(sf::Event event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

private:
    TextField fieldLobbyName;
    Button buttonCreatePong1v1;
    Button buttonCreatePong2v2;
    Button buttonBack;

    std::function<void(std::string, std::string)> startGame;

    bool showStatus = false;
    bool isConnected = false;
    std::string opponentName;

    sf::Text labelLobbyName;
};
