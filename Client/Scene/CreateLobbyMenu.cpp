#include "CreateLobbyMenu.h"

#include "MainMenu.h"

#include "Client.h"


CreateLobbyMenu::CreateLobbyMenu()
    : labelLobbyName("Nom de la salle", getGlobalFont())
    , fieldLobbyName(600, 260, getGlobalFont())
    , buttonCreatePong1v1(400, 500, "Create Pong (1v1)", getGlobalFont(), [this]() {
        Client::getInstance().createLobbyPong1v1(fieldLobbyName.getInput());
    })
    , buttonCreatePong2v2(700, 500, "Create Pong (2v2)", getGlobalFont(), [this]() {
        Client::getInstance().createLobbyPong2v2(fieldLobbyName.getInput());
    })
    , buttonBack(80, 80, "Retour", getGlobalFont(), [this]() {
        Scene::setCurrentScene(new MainMenu());
    })
{
    labelLobbyName.setPosition(600, 200);
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
