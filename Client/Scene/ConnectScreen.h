#pragma once

#include "Scene.h"

#include "Widget/TextField.h"
#include "Widget/Button.h"

class ConnectScreen : public Scene
{
public:
    ConnectScreen();
    void handleEvent(sf::Event event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

private:

    TextField playerNameField;
    TextField ipField;
    Button buttonConnect;

    bool showStatus;

    sf::Text playerNameLabel;
    sf::Text ipLabel;
    sf::Text statusLabel;
};
