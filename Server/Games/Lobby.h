#pragma once

#include <stdint.h>

#include <string>


class Lobby
{
public:

	Lobby(uint32_t id) : m_lobbyID(id) { }

	void init(const std::string& name);
	virtual void update(float dt) = 0;

	virtual int getNumPlayers() const = 0;
	virtual int getMaxPlayers() const = 0;

	virtual void start() = 0;

	virtual uint32_t addPlayer(uint64_t playerID) { return -1; }
	virtual void disconnectPlayer(uint32_t playerID) { }

	virtual uint32_t getPlayerID(uint64_t socket) const { return -1; }
	uint32_t getLobbyID() const { return m_lobbyID; }

	inline const std::string& getName() const { return m_name; }


private:

	uint32_t m_lobbyID;
	std::string m_name;
};
