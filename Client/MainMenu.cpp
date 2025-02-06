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
    , buttonRefresh(100, 250, "Actualiser", this->font, [this]() {
        refreshLobbyList();
    })
    , font(font)
{
    refreshLobbyList();
}

void MainMenu::handleEvent(sf::Event event, sf::RenderWindow& window) {
    createButton.handleEvent(event, window);

    for (Button& b : lobbies) {
        b.handleEvent(event, window);
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

    for (Button& b : lobbies) {
        b.draw(window);
    }
}

void MainMenu::refreshLobbyList()
{
    lobbies.clear();

    network::sendPacketTCP(Network::getServerTCP(), (uint32_t)ClientPackets::GetLobbies, Client_GetLobbies());
}

void MainMenu::listLobby(const char* name, int numPlayers, int maxPlayers)
{
    std::stringstream label;
    label << name << " (" << numPlayers << '/' << maxPlayers << ")";

    int yPos = lobbies.size() * 60;
    Button b(1000, yPos, label.str(), font, [this, name]() {
        joinLobby(name);
        });

    lobbies.push_back(b);
}

void MainMenu::joinLobby(const std::string& playerName)
{
    std::cout << "[INFO] Demande de rejoindre un lobby avec le pseudo : " << playerName << std::endl;

    Client_JoinLobby packet;
    std::memset(packet.playerName, 0, sizeof(packet.playerName));
    std::strncpy(packet.playerName, playerName.c_str(), sizeof(packet.playerName) - 1);

    network::sendPacketTCP(Network::getServerTCP(), (uint32_t)ClientPackets::JoinLobby, packet);
}

