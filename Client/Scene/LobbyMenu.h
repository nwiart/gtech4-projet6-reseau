#pragma once

#include "Scene.h"

#include "Widget/TextField.h"
#include "Widget/Button.h"

class LobbyMenu : public Scene
{
public:
    LobbyMenu();
    void handleEvent(sf::Event event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

private:
    Button buttonStart;
    Button buttonLeave;

    std::map<uint32_t, sf::Text> m_playersInLobby;

    sf::Text labelLobbyName;
    sf::Text labelNumPlayers;
};
