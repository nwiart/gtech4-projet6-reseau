#pragma once

#include "Scene.h"

#include "Widget/TextField.h"
#include "Widget/Button.h"

class CreateLobbyMenu : public Scene
{
public:
    CreateLobbyMenu();
    void handleEvent(sf::Event event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

private:
    TextField fieldLobbyName;
    Button buttonCreatePong1v1;
    Button buttonCreatePong2v2;
    Button buttonBack;

    sf::Text labelLobbyName;
};
