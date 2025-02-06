#pragma once

#include "Scene.h"
#include "TextField.h"
#include "Button.h"

class LobbyMenu : public Scene
{
public:
    LobbyMenu(sf::Font& font);
    void handleEvent(sf::Event event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

private:
    Button buttonStart;
    Button buttonBack;

    std::vector<sf::Text> playerNames;

    int maxPlayers;

    sf::Font font;
    sf::Text labelLobbyName;
    sf::Text labelNumPlayers;
};
