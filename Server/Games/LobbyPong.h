#pragma once

#include "Lobby.h"
#include "Pong/PongScene.h"
#include <unordered_map>
#include "Networking.h"

class LobbyPong : public Lobby {
public:
    LobbyPong();

    virtual int getNumPlayers() const override { return m_players.size(); }
    virtual int getMaxPlayers() const override { return 2; }

    void addPlayer(uint32_t playerID, Socket playerSocket);
    void startGame();
    void update(float dt);
    void receivePlayerMove(uint32_t playerID, float positionY);

private:
    PongScene m_pong;
    std::unordered_map<uint32_t, Socket> m_players;
    bool gameStarted = false;
};
