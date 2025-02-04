#pragma once

#include "ConnectionDenialReason.h"
#include "GameMode.h"


enum class ClientPackets : uint32_t
{
	PlayerConnect,
	CreateLobby,
	JoinLobby,

	PlayerMove,
};

enum class ServerPackets : uint32_t
{
	ConnectResult,
	LobbyCreation,
	AcceptJoin,
	DenyJoin,

	PlayerMove,
	BallInfo,
	Score,
};


struct Client_PlayerConnect
{
	char playerName[32];
};

// Sent by client to create a new lobby.
struct Client_CreateLobby
{
	GameMode gamemode;
};

// Sent by client to provide the info.
struct Client_JoinLobby
{
	// TODO : define.
};

struct Server_ConnectResult
{
	uint32_t playerID;
	bool success;
};

struct Server_LobbyCreation
{
	int playerID;  // Lobby creator's assigned ID (if successful).
	bool success;
};

// Sent by server to a new player if their connection was successful.
struct Server_AcceptJoin
{
	int playerID;
};

// Sent by server if a new player's connection is refused.
struct Server_DenyJoin
{
	ConnectionDenialReason reason;
};


// Sent by client when they move their paddle.
struct Client_PlayerMove
{
	float position;
};

// Sent by server to all players when a player moves.
struct Server_PlayerMove
{
	int playerID;
	float position;
};

// Sent by server to update the ball's position and velocity.
struct Server_BallInfo
{
	float xPos;
	float yPos;
	float xVel;
	float yVel;
};

// Sent by server when a team scores a point.
struct Server_Score
{
	int team;
};
