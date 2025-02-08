#pragma once

#include <stdint.h>
#include <string>
#include <map>

class ClientConnection;


class Lobby
{
public:

	struct Player
	{
		uint32_t m_inLobbyID;
		ClientConnection* m_client;
	};

public:

	Lobby(uint32_t id) : m_lobbyID(id) { }

	void init(const std::string& name);
	virtual void update(float dt) = 0;

	virtual int getNumPlayers() const = 0;
	virtual int getMaxPlayers() const = 0;

	virtual void start() = 0;

	virtual bool hasGameStarted() const = 0;

	virtual uint32_t addPlayer(ClientConnection* conn);
	virtual void disconnectPlayer(uint32_t playerID) { }

	const std::map<uint64_t, Player>& getPlayers() const { return m_players; }
	virtual uint32_t getPlayerID(uint64_t socket) const { return -1; }
	uint32_t getLobbyID() const { return m_lobbyID; }

	inline const std::string& getName() const { return m_name; }


protected:

	uint32_t m_lobbyID;
	std::string m_name;

	std::map<uint64_t, Player> m_players;
};
