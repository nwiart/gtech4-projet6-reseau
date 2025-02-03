#ifndef GAMEMENU_H
#define GAMEMENU_H

#include "TextField.h"
#include "Button.h"

class GameMenu {
public:
    GameMenu(sf::Font& font);
    void handleEvent(sf::Event event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    TextField usernameField;
    TextField ipField;
    Button createButton;
    Button joinButton;
};

#endif
