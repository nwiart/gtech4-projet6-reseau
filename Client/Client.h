#pragma once

#include "Networking.h"
#include "Socket.h"
#include "GameMode.h"
#include "Lobby.h"

#include <string>

/// <summary>
/// Represents the client-side networking system, handling connections,
/// event polling, lobby management, and game communication.
/// </summary>
class Client
{
public:
    /// <summary>
    /// Retrieves the singleton instance of the client.
    /// </summary>
    /// <returns>Reference to the client instance.</returns>
    static inline Client& getInstance();

public:
    /// <summary>
    /// Default constructor initializing the client with an undefined player ID.
    /// </summary>
    Client() : m_playerID(-1) {}

    /// <summary>
    /// Initializes the client networking components.
    /// </summary>
    void init();

    /// <summary>
    /// Connects to the server using the provided IP address and player name.
    /// </summary>
    /// <param name="serverIP">IP address of the server.</param>
    /// <param name="playerName">The player's chosen name.</param>
    /// <returns>0 on success, negative value on failure.</returns>
    int connect(const char* serverIP, const std::string& playerName);

    /// <summary>
    /// Polls for incoming network events (TCP & UDP).
    /// </summary>
    void pollEvents();

    /// <summary>
    /// Handles an incoming TCP packet based on its ID.
    /// </summary>
    /// <param name="packetID">ID of the received packet.</param>
    void handleTCPPacket(uint32_t packetID);

    /// <summary>
    /// Handles an incoming UDP packet.
    /// </summary>
    void handleUDPPacket();

    /// <summary>
    /// Retrieves the client's TCP socket connected to the server.
    /// </summary>
    /// <returns>Reference to the server TCP socket.</returns>
    inline Socket& getServerTCP() { return m_socketTCP; }

public:
    /// <summary>
    /// Signals that the connection to the server has been lost.
    /// </summary>
    /// <param name="err">Error code of the disconnection.</param>
    void signalConnectionLost(int err);

    /// <summary>
    /// Signals the start of the game.
    /// </summary>
    void signalGameStart();

    /// <summary>
    /// Creates a new Pong 1v1 lobby with the specified name.
    /// </summary>
    /// <param name="name">The name of the lobby.</param>
    void createLobbyPong1v1(const std::string& name);

    /// <summary>
    /// Creates a new Pong 2v2 lobby with the specified name.
    /// </summary>
    /// <param name="name">The name of the lobby.</param>
    void createLobbyPong2v2(const std::string& name);

    /// <summary>
    /// Requests to start the game in the current lobby.
    /// </summary>
    void startGame();

    /// <summary>
    /// Requests to join a lobby with the given ID.
    /// </summary>
    /// <param name="lobbyID">The unique ID of the lobby to join.</param>
    void joinLobby(uint32_t lobbyID);

    /// <summary>
    /// Requests to leave the current lobby.
    /// </summary>
    void leaveLobby();

    /// <summary>
    /// Sends the player's paddle position to the server.
    /// </summary>
    /// <param name="posY">The Y position of the paddle.</param>
    /// <returns>0 on success, negative value on failure.</returns>
    int sendPosition(int posY);

    /// <summary>
    /// Retrieves the player's name.
    /// </summary>
    /// <returns>Reference to the player's name string.</returns>
    inline const std::string& getPlayerName() const { return m_playerName; }

    /// <summary>
    /// Retrieves the player's unique ID.
    /// </summary>
    /// <returns>The player's ID.</returns>
    inline uint32_t getPlayerID() const { return m_playerID; }

    /// <summary>
    /// Retrieves the current lobby the player is in.
    /// </summary>
    /// <returns>Reference to the player's lobby.</returns>
    inline Lobby& getLobby() { return m_lobby; }

private:
    /// <summary>
    /// Completes the client authentication process by assigning a player ID.
    /// </summary>
    /// <param name="playerID">The assigned player ID.</param>
    void completeAuthentication(uint32_t playerID);

    /// <summary>
    /// Creates a new game lobby with the specified game mode and name.
    /// </summary>
    /// <param name="gm">The selected game mode.</param>
    /// <param name="name">The name of the lobby.</param>
    void createLobby(GameMode gm, const std::string& name);

private:
    static const uint16_t serverBasePort = 27014; ///< Base port for the server.
    static const uint16_t serverSecondaryPort = serverBasePort + 1; ///< Secondary UDP port.

    Lobby m_lobby; ///< The current lobby the player is in.

    static Socket m_socketTCP; ///< Client's TCP socket connected to the server.
    static Socket m_socketUDP; ///< Client's UDP socket for game communication.
    static sockaddr_in serverUDPAddr; ///< Server UDP address.

    uint32_t m_playerID; ///< Unique player ID assigned by the server.
    std::string m_playerName; ///< The player's username.
};

#include "Client.inl"
