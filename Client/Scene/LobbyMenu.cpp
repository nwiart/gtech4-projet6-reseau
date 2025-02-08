#include "LobbyMenu.h"

#include "MainMenu.h"

#include "Client.h"


LobbyMenu::LobbyMenu()
    : labelLobbyName("Salle Pong", getGlobalFont())
    , buttonStart(600, 500, "Démarrer la partie", getGlobalFont(), []() {
        Client::getInstance().startGame();
    })
    , buttonLeave(80, 80, "Quitter", getGlobalFont(), []() {
        Client::getInstance().leaveLobby();
        Scene::setCurrentScene(new MainMenu());
    })
{
    labelLobbyName.setPosition(600, 200);
    labelLobbyName.setString("1/2 players");
}

void LobbyMenu::handleEvent(sf::Event event, sf::RenderWindow& window) {
    if (Client::getInstance().getLobby().isHost()) buttonStart.handleEvent(event, window);
    buttonLeave.handleEvent(event, window);
}

void LobbyMenu::update(sf::RenderWindow& window) {

}

void LobbyMenu::draw(sf::RenderWindow& window) {
    if (Client::getInstance().getLobby().isHost()) buttonStart.draw(window);
    buttonLeave.draw(window);
    window.draw(labelLobbyName);

    float yPos = 100.0F;
    for (auto& p : Client::getInstance().getLobby().getPlayers()) {
        auto it = m_playersInLobby.find(p.first);
        if (it == m_playersInLobby.end()) {
            sf::Text text;
            text.setFont(getGlobalFont());
            text.setString(p.second.m_name);

            m_playersInLobby.emplace(p.first, text);
            it = m_playersInLobby.find(p.first);
        }

        it->second.setPosition(1000.0F, yPos);
        window.draw(it->second);

        yPos += 40.0F;
    }
}
