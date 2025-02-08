#include "Lobby.h"

#include "Server.h"

#include <iostream>


void Lobby::init(const std::string& name)
{
	m_name = name;
}

uint32_t Lobby::addPlayer(ClientConnection* conn)
{
    if (m_players.size() >= getMaxPlayers())
    {
        std::cout << "Lobby complet !" << std::endl;
        return -1;
    }

    uint32_t playerID = m_players.size();
    m_players.emplace(conn->getSocket().mSocket, Player{ playerID, conn });

    std::cout << "Joueur " << playerID << " ajout� !" << std::endl;

    return playerID;
}
