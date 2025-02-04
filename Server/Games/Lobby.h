#pragma once

#include <stdint.h>


class Lobby
{
public:

	Lobby() { }

	virtual void disconnectPlayer(uint32_t playerID) { }
};
