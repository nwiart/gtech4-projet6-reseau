#include "MainMenu.h"

MainMenu::MainMenu(sf::Font& font, std::function<void(std::string, std::string)> startGameCallback)
    : playerNameField(200, 100, font)
    , ipField(200, 170, font, true)
    , createButton(100, 250, "Cr�er Partie", font, [this]() { startGame(playerNameField.getInput(), "Serveur"); })
    , joinButton(320, 250, "Rejoindre", font, [this]() {
        std::string playerName = playerNameField.getInput();
        std::string ip = ipField.getInput();
        if (!playerName.empty() && !ip.empty()) {
            int connectionResult = network.connect(ip.c_str());
            if (connectionResult == 0) {  // Success
                std::cout << "Connected to server successfully.\n";
                isConnected = true;
                opponentName = "En attente...";
                startGame(playerName, opponentName);
            }
        }
    })
    , startGame(startGameCallback)
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
}

void MainMenu::handleEvent(sf::Event event, sf::RenderWindow& window) {
    playerNameField.handleEvent(event, window);
    ipField.handleEvent(event, window);
    createButton.handleEvent(event, window);
    joinButton.handleEvent(event, window);
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
    window.draw(playerNameLabel);
    window.draw(ipLabel);

    playerNameField.draw(window);
    ipField.draw(window);
    createButton.draw(window);
    joinButton.draw(window);
}
