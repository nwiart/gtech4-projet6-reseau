#pragma once

#include "ConnectionDenialReason.h"
#include "GameMode.h"


enum class ClientPackets : uint32_t
{
	PlayerConnect,
	GetLobbies,
	CreateLobby,
	JoinLobby,
	StartGame,

	PlayerMove,
};

enum class ServerPackets : uint32_t
{
	ConnectResult,
	GetLobbies,
	LobbyCreation,
	AcceptJoin,
	DenyJoin,
	GameStart,
	PlayerJoined,
	GameState,

	PlayerMove
};


struct Client_PlayerConnect
{
	char playerName[32];
};

struct Client_GetLobbies
{

};

// Sent by client to create a new lobby.
struct Client_CreateLobby
{
	char lobbyName[32];
	GameMode gamemode;
};

// Sent by client to provide the info.
struct Client_JoinLobby
{
	int lobbyID;
};

struct Client_StartGame
{

};

struct Server_ConnectResult
{
	uint32_t playerID;
	bool success;
};

struct Server_GetLobbies
{
	char lobbyName[32];
	short numPlayers;
	short maxPlayers;
	int lobbyID;
	bool more;
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

// Sent by server to a new player if their connection was successful.
struct Server_PlayerJoined
{
	int playerID;
};

// Sent by server if a new player's connection is refused.
struct Server_DenyJoin
{
	ConnectionDenialReason reason;
};

struct Server_GameStart
{
	bool started;
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

struct Server_GameState {
	float ballX, ballY, ballRadius;
	int scoreP1, scoreP2;
	float paddle1Y, paddle2Y;
};
