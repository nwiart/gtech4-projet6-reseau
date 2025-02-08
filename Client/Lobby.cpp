#include "Lobby.h"

#include "Client.h"


Lobby::Lobby()
{

}

void Lobby::setupHost(uint32_t myID, uint32_t maxPlayers)
{
	m_isHost = true;
	m_myID = myID;
	m_maxPlayers = maxPlayers;

	m_players.emplace(myID, Player{ Client::getInstance().getPlayerName(), Client::getInstance().getPlayerID(), myID });
}

void Lobby::setupGuest(uint32_t myID, uint32_t maxPlayers)
{
	m_isHost = false;
	m_myID = myID;
	m_maxPlayers = maxPlayers;
}

void Lobby::leave()
{
	m_players.clear();
	m_myID = -1;
}

void Lobby::listPlayer(uint32_t playerID, uint32_t inLobbyID, const std::string& playerName)
{
	
}

void Lobby::removePlayer(uint32_t playerID)
{
	m_players.erase(playerID);
}
