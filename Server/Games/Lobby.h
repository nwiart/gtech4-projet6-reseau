#pragma once

#include <stdint.h>

#include <string>


class Lobby
{
public:

	Lobby() { }

	void init(const std::string& name);

	virtual int getNumPlayers() const = 0;
	virtual int getMaxPlayers() const = 0;

	virtual uint32_t addPlayer(uint64_t playerID) { return -1; }
	virtual void disconnectPlayer(uint32_t playerID) { }

	inline const std::string& getName() const { return m_name; }


private:

	std::string m_name;
};
