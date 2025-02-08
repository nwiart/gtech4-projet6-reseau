#pragma once

#include "ConnectionDenialReason.h"
#include "GameMode.h"


enum class ClientPackets : uint32_t
{
	// TCP packets sent by client.
	PlayerConnect,
	PlayerConnectUDP,
	GetLobbies,
	CreateLobby,
	JoinLobby,
	LeaveLobby,
	StartGame,

	// UDP packets sent by client.
	PlayerMove,
};

enum class ServerPackets : uint32_t
{
	// TCP packets sent by server.
	ConnectResult,
	GetLobbies,
	LobbyCreation,
	AcceptJoin,
	DenyJoin,
	GameStart,
	PlayerJoinedLobby,

	// UDP packets sent by server.
	PlayerMove,
	BallInfo,
};


struct Client_PlayerConnect
{
	char playerName[32];
};

struct Client_PlayerConnectUDP
{
	int playerID;
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

// Sent by client to join a specific lobby.
// The client should expect a Server_AcceptJoin or Server_DenyJoin as response.
struct Client_JoinLobby
{
	int lobbyID;
};

struct Client_LeaveLobby
{

};

// Sent by client to start a game. This requires the client to be the master of the lobby.
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
	int maxPlayers;
	bool success;
};

// Sent by server to a new player if their connection was successful.
// The "inLobbyID" is the ID assigned to the player WITHIN the lobby, not to be confused with the unique player ID.
// The inLobbyID also determines teams and positions in games.
struct Server_AcceptJoin
{
	int inLobbyID;
	int maxPlayers;
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

// Sent by server to all players in a lobby to signal that someone just joined.
struct Server_PlayerJoinedLobby
{
	char playerName[32];
	int playerID;
	int idInLobby;
};


// Sent by client when they move their paddle.
struct Client_PlayerMove
{
	int playerID;
	float position;
};

// Sent by server to all players when a player moves.
struct Server_PlayerMove
{
	int playerID;
	float position;
};

struct Server_BallInfo
{
	float xPos, yPos;
	float xVel, yVel;
};
