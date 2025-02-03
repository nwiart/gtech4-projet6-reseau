#include "GameMenu.h"
#include <iostream>

GameMenu::GameMenu(sf::Font& font) :
    usernameField(150, 50, font),
    ipField(150, 150, font, true),
    createButton(100, 250, "Créer Partie", font, []() { std::cout << "Serveur créé!\n"; }),
    joinButton(320, 250, "Rejoindre", font, []() { std::cout << "Tentative de connexion...\n"; })
{
}

void GameMenu::handleEvent(sf::Event event, sf::RenderWindow& window) {
    usernameField.handleEvent(event, window);
    ipField.handleEvent(event, window);
    createButton.handleEvent(event, window);
    joinButton.handleEvent(event, window);
}

void GameMenu::draw(sf::RenderWindow& window) {
    usernameField.draw(window);
    ipField.draw(window);
    createButton.draw(window);
    joinButton.draw(window);
}
