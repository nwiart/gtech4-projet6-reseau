#pragma once

#include "Scene.h"
#include "TextField.h"
#include "Button.h"

#include <map>

class MainMenu : public Scene {
public:
    MainMenu(sf::Font& font);
    void handleEvent(sf::Event event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

    void refreshLobbyList();
    void listLobby(uint32_t id, const char* name, int numPlayers, int maxPlayers);
    void joinLobby(uint32_t lobbyID, const std::string& lobbyName);

private:
    Button createButton;
    Button buttonRefresh;
    sf::Font font;
    std::map<uint32_t, Button> lobbies;
    std::function<void(std::string, std::string)> startGame;

    bool isConnected = false;
    std::string opponentName;
};
