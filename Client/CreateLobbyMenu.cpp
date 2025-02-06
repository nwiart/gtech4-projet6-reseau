#include "CreateLobbyMenu.h"

#include "MainMenu.h"

#include "Network.h"

CreateLobbyMenu::CreateLobbyMenu(sf::Font& font)
    : font(font)
    , labelLobbyName("Nom de la salle", this->font)
    , fieldLobbyName(600, 160, this->font, true)
    , buttonCreatePong1v1(500, 600, "Create Pong (1v1)", this->font, [this]() {
        
    })
    , buttonCreatePong2v2(700, 600, "Create Pong (2v2)", this->font, [this]() {

    })
    , buttonBack(80, 80, "Retour", this->font, [this]() {
        Scene::setCurrentScene(new MainMenu(this->font));
    })
{
    labelLobbyName.setPosition(600, 100);
}

void CreateLobbyMenu::handleEvent(sf::Event event, sf::RenderWindow& window) {
    fieldLobbyName.handleEvent(event, window);
    buttonCreatePong1v1.handleEvent(event, window);
    buttonCreatePong2v2.handleEvent(event, window);
    buttonBack.handleEvent(event, window);
}

void CreateLobbyMenu::update(sf::RenderWindow& window) {

}

void CreateLobbyMenu::draw(sf::RenderWindow& window) {
    fieldLobbyName.draw(window);
    buttonCreatePong1v1.draw(window);
    buttonCreatePong2v2.draw(window);
    buttonBack.draw(window);
    window.draw(labelLobbyName);
}
