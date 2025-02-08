#pragma once

#include "Lobby.h"
#include "Pong/PongScene.h"
#include "Socket.h"

#include <map>
#include <stdint.h>

class ClientConnection;

class LobbyPong : public Lobby {

public:
    LobbyPong(uint32_t id, bool twoPlayersTeam);

    virtual int getNumPlayers() const override { return m_players.size(); }
    virtual int getMaxPlayers() const override { return m_twoPlayerTeams ? 4 : 2; }

    virtual void start() override;
    virtual uint32_t addPlayer(ClientConnection* conn) override;
    virtual uint32_t getPlayerID(uint64_t socket) const override;

    virtual bool hasGameStarted() const override;

    void update(float dt) override;
    void receivePlayerMove(uint32_t playerID, float positionY);
    void sendGameState();
    void getGameState(float& ballX, float& ballY, float& ballRadius,
        float& paddle1Y, float& paddle2Y,
        int& score1, int& score2) const;

private:
    PongScene m_pong;

    bool m_twoPlayerTeams;
    bool gameStarted = false;
};
