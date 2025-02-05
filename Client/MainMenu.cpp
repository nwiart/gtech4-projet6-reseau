#include "MainMenu.h"

#include "Networking.h"   // Go figure
#include "Network.h"      //

#include "PongPackets.h"

#include <sstream>

MainMenu::MainMenu(sf::Font& font)
    : createButton(100, 250, "Créer Partie", font, [this]() { 
        
    })
    , buttonRefresh(100, 250, "Actualiser", font, [this]() {
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

    Button b(0, 0, label.str(), font, []() {

    });
    lobbies.push_back(b);
}
