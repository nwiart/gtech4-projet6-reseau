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
	Score,
};

#pragma pack(push, 1)

//sent by client to request a TCP connection to the server
struct Client_PlayerConnect
{
	char playerName[32];
};

// Sent by the client to request a udp connection to server
struct Client_PlayerConnectUDP
{
	int playerID;
};

// sent by the client to request to be sent the list of lobbies on the server
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

// Sent by the client to request to leave the lobby
struct Client_LeaveLobby
{

};

// Sent by Client to request to start the game they are in
struct Client_StartGame
{

};

// Sent by server to confirm the client is connected and send their Corresponding id
struct Server_ConnectResult
{
	uint32_t playerID;
	bool success;
};

// Sent by the server for the client to see available lobbies
struct Server_GetLobbies
{
	char lobbyName[32];
	short numPlayers;
	short maxPlayers;
	int lobbyID;
	bool more;
};

// Sent by the server to confirm they have successfully created a lobby
struct Server_LobbyCreation
{
	int playerID;  // Lobby creator's assigned ID (if successful).
	int maxPlayers;
	bool success;
};

// Sent by server to a new player if their connection was successful.
struct Server_AcceptJoin
{
	int playerID;
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

// Sent by server to a new player if their connection was successful.
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

struct Server_Score
{
	int player1Score;
	int player2Score;
};

#pragma pack(pop)