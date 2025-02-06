#include "ConnectScreen.h"

#include "MainMenu.h"

#include "Network.h"

ConnectScreen::ConnectScreen(sf::Font& font)
    : playerNameField(200, 100, font)
    , font(font)
    , ipField(200, 170, font, true)
    , buttonConnect(100, 250, "Connexion", font, [this]() {
        std::string playerName = playerNameField.getInput();
        std::string ip = ipField.getInput();
        if (!playerName.empty() && !ip.empty()) {
            showStatus = true;
            statusLabel.setString("Connexion en cours...");

            int connectionResult = Network::connect(ip.c_str(), playerName.c_str());
            if (connectionResult == 0) {  // Success.
                Scene::setCurrentScene(new MainMenu(this->font));
            }
        }
     })
{
    playerNameLabel.setFont(font);
    playerNameLabel.setCharacterSize(24);
    playerNameLabel.setFillColor(sf::Color::White);
    playerNameLabel.setString("Nom du joueur:");
    playerNameLabel.setPosition(5, 110);

    ipLabel.setFont(font);
    ipLabel.setCharacterSize(24);
    ipLabel.setFillColor(sf::Color::White);
    ipLabel.setString("Adresse IP:");
    ipLabel.setPosition(5, 180);

    statusLabel.setFont(font);
    statusLabel.setCharacterSize(24);
    statusLabel.setFillColor(sf::Color::White);
    statusLabel.setPosition(30, 680);
}

void ConnectScreen::handleEvent(sf::Event event, sf::RenderWindow& window) {
    playerNameField.handleEvent(event, window);
    ipField.handleEvent(event, window);
    buttonConnect.handleEvent(event, window);
}

void ConnectScreen::update(sf::RenderWindow& window) {
    if (isConnected) {
        // std::string receivedName = network.receiveDataUDP();
        // if (!receivedName.empty()) {
        //     opponentName = receivedName;
        //     startGame(playerNameField.getInput(), opponentName);
        // }
    }
}

void ConnectScreen::draw(sf::RenderWindow& window) {
    window.draw(playerNameLabel);
    window.draw(ipLabel);
    if (showStatus)
        window.draw(statusLabel);

    playerNameField.draw(window);
    ipField.draw(window);
    buttonConnect.draw(window);
}
