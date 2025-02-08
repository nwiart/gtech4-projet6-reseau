#include "MainMenu.h"

#include "CreateLobbyMenu.h"

#include "Networking.h"
#include "Client.h"

#include "PongPackets.h"

#include <sstream>

MainMenu::MainMenu()
    : createButton(100, 250, "Creer une Partie", getGlobalFont(), [this]() { 
        Scene::setCurrentScene(new CreateLobbyMenu());
    })
    , buttonRefresh(100, 350, "Actualiser", getGlobalFont(), [this]() {
        refreshLobbyList();
    })
{
    refreshLobbyList();
}

void MainMenu::handleEvent(sf::Event event, sf::RenderWindow& window) {
    createButton.handleEvent(event, window);
    buttonRefresh.handleEvent(event, window);

    for (auto& p : lobbies) {
        p.second.handleEvent(event, window);
    }
}

void MainMenu::update(sf::RenderWindow& window) {

}

void MainMenu::draw(sf::RenderWindow& window) {
    createButton.draw(window);
    buttonRefresh.draw(window);

    for (auto& p : lobbies) {
        p.second.draw(window);
    }
}

void MainMenu::refreshLobbyList()
{
    lobbies.clear();

    network::sendPacketTCP(Client::getInstance().getSocketTCP(), (uint32_t)ClientPackets::GetLobbies, Client_GetLobbies());
}

void MainMenu::listLobby(uint32_t id, const char* name, int numPlayers, int maxPlayers)
{
    std::stringstream label;
    label << name << " (" << numPlayers << '/' << maxPlayers << ")";

    int yPos = lobbies.size() * 60 + 40;
    Button b(1000, yPos, label.str(), getGlobalFont(), [id]() {
        //Send packet for joing lobby
    });

    lobbies.insert(std::pair<uint32_t, Button>(id, b));
}
