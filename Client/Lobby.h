#pragma once

#include <stdint.h>
#include <string>
#include <map>


class Lobby
{
public:

	class Player
	{
	public:
		std::string m_name;
		uint32_t m_playerID;
		uint32_t m_idInLobby;
	};

public:

	Lobby();

	void setupHost(uint32_t myID, uint32_t maxPlayers);
	void setupGuest(uint32_t myID, uint32_t maxPlayers);
	void leave();

	void listPlayer(uint32_t playerID, uint32_t inLobbyID, const std::string& playerName);
	void removePlayer(uint32_t playerID);

	inline bool isValid() const { return !m_players.empty(); }
	inline bool isHost() const { return m_isHost; }

	inline int getNumPlayers() const { return m_players.size(); }
	inline int getMaxPlayers() const { return m_maxPlayers; }
	inline uint32_t getLocalPlayerID() const { return m_myID; }

	inline const std::map<uint32_t, Player>& getPlayers() const { return m_players; }


private:

	std::map<uint32_t, Player> m_players;

	uint32_t m_maxPlayers;

	uint32_t m_myID;
	bool m_isHost;
};
