#pragma once

#include "ConnectionDenialReason.h"
#include "GameMode.h"

/// <summary>
/// Enum defining packets sent by the client to the server.
/// </summary>
enum class ClientPackets : uint32_t
{
    // TCP packets sent by the client.
    PlayerConnect,      ///< Sent when a player connects to the server.
    PlayerConnectUDP,   ///< Sent when a player establishes a UDP connection.
    GetLobbies,         ///< Request to retrieve the list of available lobbies.
    CreateLobby,        ///< Request to create a new lobby.
    JoinLobby,          ///< Request to join a specific lobby.
    LeaveLobby,         ///< Request to leave the current lobby.
    StartGame,          ///< Request to start the game (must be lobby master).

    // UDP packets sent by the client.
    PlayerMove,         ///< Sent when a player moves their paddle.
};

/// <summary>
/// Enum defining packets sent by the server to the client.
/// </summary>
enum class ServerPackets : uint32_t
{
    // TCP packets sent by the server.
    ConnectResult,      ///< Sent as a response to PlayerConnect, providing connection status.
    GetLobbies,         ///< Response containing a list of available lobbies.
    LobbyCreation,      ///< Sent after a player creates a lobby, confirming success or failure.
    AcceptJoin,        ///< Sent to a player confirming they successfully joined a lobby.
    DenyJoin,          ///< Sent to a player if they were denied entry into a lobby.
    GameStart,         ///< Sent when the game officially starts.
    PlayerJoinedLobby, ///< Sent to all players in a lobby when a new player joins.

    // UDP packets sent by the server.
    PlayerMove,        ///< Sent when a player's movement is updated for synchronization.
    BallInfo,         ///< Sent to update the ball's position and direction.
    ScoreInfo,        ///< Sent to update the current game score.
    PingInfo,         ///< Sent to provide latency (ping) information.
};

/// <summary>
/// Packet sent by the client when connecting to the server.
/// </summary>
struct Client_PlayerConnect
{
    char playerName[32]; ///< Player's chosen username.
};

/// <summary>
/// Packet sent by the client to establish a UDP connection.
/// </summary>
struct Client_PlayerConnectUDP
{
    int playerID; ///< Unique identifier assigned to the player.
};

/// <summary>
/// Packet sent by the client to request a list of available lobbies.
/// </summary>
struct Client_GetLobbies
{
};

/// <summary>
/// Packet sent by the client to create a new lobby.
/// </summary>
struct Client_CreateLobby
{
    char lobbyName[32]; ///< Name of the lobby.
    GameMode gamemode;  ///< Selected game mode.
};

/// <summary>
/// Packet sent by the client to join a specific lobby.
/// The server responds with Server_AcceptJoin or Server_DenyJoin.
/// </summary>
struct Client_JoinLobby
{
    int lobbyID; ///< The unique identifier of the lobby to join.
};

/// <summary>
/// Packet sent by the client to leave the current lobby.
/// </summary>
struct Client_LeaveLobby
{
};

/// <summary>
/// Packet sent by the client to start the game (must be the lobby master).
/// </summary>
struct Client_StartGame
{
};

/// <summary>
/// Packet sent by the server in response to a client's connection attempt.
/// </summary>
struct Server_ConnectResult
{
    uint32_t playerID; ///< Assigned unique player ID.
    bool success;      ///< True if the connection was successful, false otherwise.
};

/// <summary>
/// Packet sent by the server containing information about available lobbies.
/// </summary>
struct Server_GetLobbies
{
    char lobbyName[32]; ///< Name of the lobby.
    short numPlayers;   ///< Current number of players in the lobby.
    short maxPlayers;   ///< Maximum number of players allowed.
    int lobbyID;        ///< Unique identifier of the lobby.
    bool more;          ///< True if there are more lobbies to be listed.
};

/// <summary>
/// Packet sent by the server after a lobby creation request.
/// </summary>
struct Server_LobbyCreation
{
    int playerID;  ///< Assigned player ID if the lobby was created successfully.
    int maxPlayers; ///< Maximum number of players allowed in the lobby.
    bool success;  ///< True if the lobby was created successfully.
};

/// <summary>
/// Packet sent by the server to confirm a player has successfully joined a lobby.
/// </summary>
struct Server_AcceptJoin
{
    int inLobbyID;  ///< Unique ID assigned within the lobby (used for teams/roles).
    int maxPlayers; ///< Maximum number of players in the lobby.
};

/// <summary>
/// Packet sent by the server if a player's request to join a lobby is denied.
/// </summary>
struct Server_DenyJoin
{
    ConnectionDenialReason reason; ///< Reason why the player was denied.
};

/// <summary>
/// Packet sent by the server to signal that the game has started.
/// </summary>
struct Server_GameStart
{
    bool started; ///< True if the game has officially started.
};

/// <summary>
/// Packet sent by the server to inform all players in a lobby that a new player has joined.
/// </summary>
struct Server_PlayerJoinedLobby
{
    char playerName[32]; ///< Name of the newly joined player.
    int playerID;        ///< Unique ID of the new player.
    int idInLobby;       ///< The player's assigned ID within the lobby.
};

/// <summary>
/// Packet sent by the client when moving their paddle.
/// </summary>
struct Client_PlayerMove
{
    int playerID;  ///< ID of the player moving.
    float position; ///< New Y position of the paddle.
};

/// <summary>
/// Packet sent by the server to update all players about a player's movement.
/// </summary>
struct Server_PlayerMove
{
    int playerID;  ///< ID of the player who moved.
    float position; ///< New Y position of the paddle.
};

/// <summary>
/// Packet sent by the server to update all players on the ball's position and movement.
/// </summary>
struct Server_BallInfo
{
    float xPos, yPos; ///< Ball's current X and Y position.
    float xDir, yDir; ///< Ball's movement direction (-1 or 1).
    float speed;      ///< Ball's current speed.
};

/// <summary>
/// Packet sent by the server to update the game score.
/// </summary>
struct Server_ScoreInfo
{
    int score1, score2; ///< Scores of both players/teams.
};

/// <summary>
/// Packet sent by the server to provide latency (ping) information.
/// </summary>
struct Server_PingInfo
{
    uint32_t ping; ///< Measured network latency in milliseconds.
};
