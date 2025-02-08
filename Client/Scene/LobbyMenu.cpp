#include "LobbyMenu.h"

#include "MainMenu.h"

#include "Client.h"


LobbyMenu::LobbyMenu()
    : labelLobbyName("Salle Pong", getGlobalFont())
    , buttonStart(600, 500, "Démarrer la partie", getGlobalFont(), []() {
        //send packet for starting game
    })
    , buttonBack(80, 80, "Retour", getGlobalFont(), []() {
        Scene::setCurrentScene(new MainMenu());
    })
{
    labelLobbyName.setPosition(600, 200);
    labelLobbyName.setString("1/2 players");
}

void LobbyMenu::handleEvent(sf::Event event, sf::RenderWindow& window) {
    if (Client::getInstance().getLobby().isHost()) buttonStart.handleEvent(event, window);
    buttonBack.handleEvent(event, window);
}

void LobbyMenu::update(sf::RenderWindow& window) {

}

void LobbyMenu::draw(sf::RenderWindow& window) {
    if (Client::getInstance().getLobby().isHost()) buttonStart.draw(window);
    buttonBack.draw(window);
    window.draw(labelLobbyName);

    // TODO : display names.
    for (auto& p : Client::getInstance().getLobby().getPlayers()) {
        p.second.m_name;
    }
}
