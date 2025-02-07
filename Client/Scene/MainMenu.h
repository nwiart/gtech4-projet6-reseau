#pragma once

#include "Scene.h"

#include "Widget/TextField.h"
#include "Widget/Button.h"

#include <map>

class MainMenu : public Scene {
public:
    MainMenu();
    void handleEvent(sf::Event event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

    void refreshLobbyList();
    void listLobby(uint32_t id, const char* name, int numPlayers, int maxPlayers);

private:
    Button createButton;
    Button buttonRefresh;

    std::map<uint32_t, Button> lobbies;
};
