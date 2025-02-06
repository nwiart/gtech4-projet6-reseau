#include "LobbyMenu.h"

#include "MainMenu.h"

#include "Network.h"


extern int playerID;


LobbyMenu::LobbyMenu(sf::Font& font)
    : font(font)
    , labelLobbyName("Salle Pong", this->font)
    , buttonStart(600, 500, "Démarrer la partie", this->font, [this]() {
        Network::startGame();
    })
    , buttonBack(80, 80, "Retour", this->font, [this]() {
        Scene::setCurrentScene(new MainMenu(this->font));
    })
{
    labelLobbyName.setPosition(600, 200);
    labelLobbyName.setString("1 player");
}

void LobbyMenu::handleEvent(sf::Event event, sf::RenderWindow& window) {
    if (playerID == 0) buttonStart.handleEvent(event, window);
    buttonBack.handleEvent(event, window);
}

void LobbyMenu::update(sf::RenderWindow& window) {

}

void LobbyMenu::draw(sf::RenderWindow& window) {
    if (playerID == 0) buttonStart.draw(window);
    buttonBack.draw(window);
    window.draw(labelLobbyName);
}
