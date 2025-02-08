#pragma once

#include "Scene.h"

#include "Widget/TextField.h"
#include "Widget/Button.h"

class ConnectScreen : public Scene
{
public:
    ConnectScreen();
    void handleEvent(sf::Event event, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

    void setStatus(const std::string& msg);

private:

    TextField playerNameField;
    TextField ipField;
    Button buttonConnect;

    bool showStatus;

    sf::Text playerNameLabel;
    sf::Text ipLabel;
    sf::Text statusLabel;
};
