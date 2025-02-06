#include "MainMenu.h"

#include "CreateLobbyMenu.h"

#include "Networking.h"
#include "Network.h"

#include "PongPackets.h"

#include <sstream>

MainMenu::MainMenu(sf::Font& font)
    : createButton(100, 250, "Creer une Partie", this->font, [this]() { 
        Scene::setCurrentScene(new CreateLobbyMenu(this->font));
    })
    , buttonRefresh(100, 350, "Actualiser", this->font, [this]() {
        refreshLobbyList();
    })
    , font(font)
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
    if (isConnected) {
        // std::string receivedName = network.receiveDataUDP();
        // if (!receivedName.empty()) {
        //     opponentName = receivedName;
        //     startGame(playerNameField.getInput(), opponentName);
        // }
    }
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

    network::sendPacketTCP(Network::getServerTCP(), (uint32_t)ClientPackets::GetLobbies, Client_GetLobbies());
}

void MainMenu::listLobby(uint32_t id, const char* name, int numPlayers, int maxPlayers)
{
    std::stringstream label;
    label << name << " (" << numPlayers << '/' << maxPlayers << ")";

    int yPos = lobbies.size() * 60 + 40;
    Button b(1000, yPos, label.str(), font, [this, name]() {
        joinLobby(0, name);
        });

    lobbies.insert(std::pair<uint32_t, Button>(id, b));
}

void MainMenu::joinLobby(uint32_t lobbyID, const std::string& playerName)
{
    Client_JoinLobby packet;
    packet.lobbyID = lobbyID;

    network::sendPacketTCP(Network::getServerTCP(), (uint32_t)ClientPackets::JoinLobby, packet);
}
