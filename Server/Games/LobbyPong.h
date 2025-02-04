#pragma once

#include "Lobby.h"
#include "Pong/PongScene.h"


class LobbyPong : public Lobby
{
public:

	LobbyPong();

	void addPlayer();

	void startGame();

private:

	PongScene m_pong;
};
