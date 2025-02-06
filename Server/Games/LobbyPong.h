#pragma once

#include "Lobby.h"
#include "Pong/PongScene.h"
#include <unordered_map>
#include "Socket.h"
#include "Pong/Ball.h"

class LobbyPong : public Lobby {
public:
    LobbyPong(bool twoPlayersTeam);

    virtual int getNumPlayers() const override { return m_players.size(); }
    virtual int getMaxPlayers() const override { return m_twoPlayerTeams ? 4 : 2; }

    virtual void start() override;

    virtual uint32_t addPlayer(uint64_t id) override;

    virtual uint32_t getPlayerID(uint64_t socket) const override;

    void update(float dt) override;
    void receivePlayerMove(uint32_t playerID, float positionY);

private:
    PongScene m_pong;
    Ball m_ball;
    std::unordered_map<uint32_t, Socket> m_players;
    bool m_twoPlayerTeams;
    bool gameStarted = false;
};
