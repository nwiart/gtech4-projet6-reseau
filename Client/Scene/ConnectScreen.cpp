#include "ConnectScreen.h"

#include "MainMenu.h"

#include "Client.h"

ConnectScreen::ConnectScreen()
    : playerNameField(200, 100, getGlobalFont())
    , ipField(200, 170, getGlobalFont(), true)
    , buttonConnect(100, 250, "Connexion", getGlobalFont(), [this]() {
        std::string playerName = playerNameField.getInput();
        std::string ip = ipField.getInput();
        if (!playerName.empty() && !ip.empty()) {
            showStatus = true;
            statusLabel.setString("Connexion en cours...");

            int connectionResult = Client::getInstance().connect(ip.c_str(), playerName.c_str());
            if (connectionResult == 0) {
                Scene::setCurrentScene(new MainMenu());
            }
        }
     })
{
    playerNameLabel.setFont(getGlobalFont());
    playerNameLabel.setCharacterSize(24);
    playerNameLabel.setFillColor(sf::Color::White);
    playerNameLabel.setString("Nom du joueur:");
    playerNameLabel.setPosition(5, 110);

    ipLabel.setFont(getGlobalFont());
    ipLabel.setCharacterSize(24);
    ipLabel.setFillColor(sf::Color::White);
    ipLabel.setString("Adresse IP:");
    ipLabel.setPosition(5, 180);

    statusLabel.setFont(getGlobalFont());
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
