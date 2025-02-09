#include "Server.h"
#include "Games/LobbyPong.h"
#include "PongPackets.h"
#include "Networking.h"

#include <WinSock2.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>

Server *Server::m_instance = 0;

Server::Server()
    : m_lobbyUID(0), m_clientUID(0)
{
    m_instance = this;
}

void Server::open()
{
    m_socketListener.createSocketTCP();
    m_socketListener.listenTCP(serverBasePort);

    m_socketUDP.createSocketUDP();
    m_socketUDP.bindUDP(serverSecondaryPort - 1);
}

bool Server::notifyConnect(Socket &clientSocketTCP)
{
    if (m_clients.find(clientSocketTCP.mSocket) != m_clients.end())
    {
        return false;
    }

    auto it = m_clients.emplace(clientSocketTCP.mSocket, ClientConnection());
    ClientConnection &conn = it.first->second;

    conn.m_id = -1;
    conn.m_name = "PLAYER_CONNECTING";
    conn.m_socket = std::move(clientSocketTCP);

    return true;
}

void Server::notifyDisconnect(Socket &clientSocketTCP)
{
    auto it = m_clients.find(clientSocketTCP.mSocket);

    if (it == m_clients.end())
    {
        std::cerr << "Error: Attempt to disconnect an unknown client!" << std::endl;
        return;
    }

    ClientConnection &conn = it->second;

    if (conn.m_lobby)
    {
        conn.m_lobby->disconnectPlayer(&conn);
        std::cout << "Player " << conn.m_id << " removed from lobby " << conn.m_lobby->getLobbyID() << std::endl;
    }

    shutdown(clientSocketTCP.mSocket, SD_BOTH);
    closesocket(clientSocketTCP.mSocket);

    std::cout << "Client " << conn.m_id << " (" << conn.m_name << ") disconnected.\n";

    conn.m_socket.closeSocket();
    m_clients.erase(it);
}

uint32_t Server::confirmClient(Socket &clientSocketTCP, const std::string &playerName)
{
    sockaddr_in addr;
    int addrSize = sizeof(addr);
    if (getpeername(clientSocketTCP.mSocket, reinterpret_cast<sockaddr *>(&addr), &addrSize) == SOCKET_ERROR)
    {
        std::cerr << "getpeername failed with error: " << WSAGetLastError() << std::endl;
    }

    uint32_t clientIP = addr.sin_addr.s_addr;

    ClientConnection &conn = m_clients.at(clientSocketTCP.mSocket);

    conn.m_id = m_clientUID;
    conn.m_name = playerName;
    conn.setIP(addr);

    m_clientUID++;

    return conn.m_id;
}

void Server::createLobby(Socket &initiator, const std::string &name, GameMode gm)
{
    Lobby *lobby = 0;
    ClientConnection& conn = m_clients[initiator.mSocket];

    auto it = m_clients.find(initiator.mSocket);
    if (it == m_clients.end() || it->second.getLobby() != 0)
    {
        Server_LobbyCreation p;
        p.success = false;
        network::sendPacketTCP(initiator, (uint32_t)ServerPackets::LobbyCreation, p);

        return;
    }

    uint32_t id = m_lobbyUID;
    switch (gm)
    {
    case GameMode::PONG_1v1:
    {
        LobbyPong *pong = new LobbyPong(id, false);
        pong->init(name);
        lobby = pong;
    }
    break;
    case GameMode::PONG_2v2:
    {
        LobbyPong *pong = new LobbyPong(id, true);
        pong->init(name);
        lobby = pong;
    }
    break;
    }

    m_lobbyUID++;

    if (lobby)
    {
        m_games.push_back(lobby);
        conn.m_lobby = lobby;

        it->second.m_lobby = lobby;
        uint32_t pid = lobby->addPlayer(&conn);

        std::cout << "Player \"" << it->second.getName() << "\" created a new lobby \"" << lobby->getName() << "\".\n";

        Server_LobbyCreation p;
        p.success = true;
        p.playerID = pid;
        network::sendPacketTCP(initiator, (uint32_t)ServerPackets::LobbyCreation, p);
    }
    else
    {
        Server_LobbyCreation p;
        p.success = false;
        network::sendPacketTCP(initiator, (uint32_t)ServerPackets::LobbyCreation, p);
    }
}

void Server::joinLobby(Socket& player, Lobby* l)
{
    ClientConnection& conn = m_clients[player.mSocket];

    if (!l) {
        std::cerr << "Erreur: tentative de rejoindre un lobby inexistant !" << std::endl;
        return;
    }

    if (conn.getLobby() != nullptr) {
        std::cerr << "Erreur: le joueur est déjà dans un lobby !" << std::endl;
        return;
    }

    if (l->hasGameStarted()) {
        Server_DenyJoin p;
        p.reason = ConnectionDenialReason::GAME_STARTED;
        network::sendPacketTCP(player, (uint32_t)ServerPackets::DenyJoin, p);
        return;
    }

    uint32_t inLobbyID = l->addPlayer(&conn);
    conn.m_lobby = l;

    Server_AcceptJoin p;
    p.inLobbyID = inLobbyID;
    network::sendPacketTCP(player, (uint32_t)ServerPackets::AcceptJoin, p);

    std::cout << "Le joueur " << conn.getName() << " a rejoint le lobby " << l->getLobbyID() << "." << std::endl;

    // **Envoyer à tous les joueurs les pseudos mis à jour**
    for (auto& p : l->getPlayers()) {
        Server_PlayerJoinedLobby packet;
        packet.idInLobby = p.second.m_inLobbyID;
        packet.playerID = p.second.m_client->m_id;

        std::string playerName = p.second.m_client->getName();
        strncpy(packet.playerName, playerName.c_str(), sizeof(packet.playerName) - 1);
        packet.playerName[sizeof(packet.playerName) - 1] = '\0';

        // Envoyer les informations au nouveau joueur
        network::sendPacketTCP(player, (uint32_t)ServerPackets::PlayerJoinedLobby, packet);

        // Envoyer les informations du nouveau joueur à tous les autres joueurs
        if (p.first != player.mSocket) {
            Server_PlayerJoinedLobby newPlayerPacket;
            newPlayerPacket.idInLobby = inLobbyID;
            newPlayerPacket.playerID = conn.m_id;

            std::string newPlayerName = conn.getName();
            strncpy(newPlayerPacket.playerName, newPlayerName.c_str(), sizeof(newPlayerPacket.playerName) - 1);
            newPlayerPacket.playerName[sizeof(newPlayerPacket.playerName) - 1] = '\0';

            network::sendPacketTCP(p.second.m_client->getSocket(), (uint32_t)ServerPackets::PlayerJoinedLobby, newPlayerPacket);
        }
    }

    // **Nouvelle étape : envoyer les noms de tous les joueurs au nouveau joueur**
    for (auto& p : l->getPlayers()) {
        Server_PlayerJoinedLobby packet;
        packet.idInLobby = p.second.m_inLobbyID;
        packet.playerID = p.second.m_client->m_id;

        std::string playerName = p.second.m_client->getName();
        strncpy(packet.playerName, playerName.c_str(), sizeof(packet.playerName) - 1);
        packet.playerName[sizeof(packet.playerName) - 1] = '\0';

        network::sendPacketTCP(player, (uint32_t)ServerPackets::PlayerJoinedLobby, packet);
    }

    if (l->getNumPlayers() == 2) {
        std::cout << "Lobby complet ! Lancement de la partie..." << std::endl;
        l->start();
    }
}


void Server::leaveLobby(ClientConnection* conn)
{
    if (!conn->getLobby()) return;

    Lobby* lobby = conn->getLobby();
    uint32_t inLobbyID = lobby->getPlayerID(conn->getSocket().mSocket);
    lobby->disconnectPlayer(conn);

    // Delete lobby if empty.
    if (lobby->getNumPlayers() == 0) {
        m_games.erase(std::remove(m_games.begin(), m_games.end(), lobby));
        delete lobby;
    }

    conn->m_lobby = 0;
}

void Server::createBotLobby()
{
    uint32_t botID = 9999; // ID unique du bot
    std::string botName = "Bot_Host";

    std::cout << "Création d'un lobby avec un bot en hôte..." << std::endl;

    // Ajouter le bot en tant que faux client
    ClientConnection& botClient = m_clients[botID];
    botClient.m_id = botID;
    botClient.m_name = botName;
    botClient.m_lobby = nullptr;

    Socket fakeSocket;
    createLobby(fakeSocket, "Lobby_Bot", GameMode::PONG_1v1);

    // Récupérer le lobby nouvellement créé et ajouter le bot
    Lobby* newLobby = getLobbyByID(m_lobbyUID - 1);
    if (newLobby) {
        newLobby->addPlayer(&botClient);
        std::cout << "Lobby créé par le bot (" << botName << ") avec succès ! En attente d'un joueur..." << std::endl;
    }
}

void Server::notifyReceiveTCP(SOCKET clientSocketTCP)
{
    if (m_clients.find(clientSocketTCP) == m_clients.end())
    {
        std::cerr << "Client socket not found in server!" << std::endl;
        return;
    }

    ClientConnection &conn = m_clients.at(clientSocketTCP);
    uint32_t packetID;
    char buf[1020];

    if (clientSocketTCP == INVALID_SOCKET)
    {
        std::cerr << "Error: Attempted to receive on an invalid socket!\n";
        return;
    }

    int receivedBytes = recv(clientSocketTCP, reinterpret_cast<char *>(&packetID), sizeof(packetID), 0);

    if (receivedBytes == 0)
    {
        std::cerr << "Client disconnected gracefully.\n";
        notifyDisconnect(conn.getSocket());
        return;
    }
    else if (receivedBytes == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        std::cerr << "Error receiving packet ID. WSA Error: " << error << std::endl;

        if (error == WSAECONNRESET)
        {
            std::cerr << "Client forcibly closed the connection.\n";
        }
        else if (error == WSAETIMEDOUT)
        {
            std::cerr << "Connection timed out.\n";
        }
        else if (error == WSAENOTSOCK)
        {
            std::cerr << "Invalid socket error (10038). Check if the socket is closed or uninitialized.\n";
        }

        notifyDisconnect(conn.getSocket());
        return;
    }

    int e = WSAGetLastError();

    switch ((ClientPackets)packetID)
    {
    case ClientPackets::PlayerConnect:
    {
        recv(clientSocketTCP, buf, sizeof(Client_PlayerConnect), 0);
        uint32_t playerID = confirmClient(conn.getSocket(), reinterpret_cast<Client_PlayerConnect*>(buf)->playerName);

        Server_ConnectResult p;
        p.success = true;
        p.playerID = playerID;
        network::sendPacketTCP(conn.getSocket(), (uint32_t)ServerPackets::ConnectResult, p);

        std::cout << "Successfully connected Player " << playerID << " with name \"" << reinterpret_cast<Client_PlayerConnect*>(buf)->playerName << "\"." << std::endl;

        // Vérifier s'il y a déjà un lobby avec un bot
        if (m_games.empty()) {
            std::cout << "Aucun lobby trouvé. Création d'un lobby automatique avec un bot..." << std::endl;
            createBotLobby();
        }

        // Trouver le lobby avec un bot et y ajouter le joueur
        for (Lobby* lobby : m_games) {
            if (lobby->getNumPlayers() < lobby->getMaxPlayers()) {
                std::cout << "Ajout du joueur " << playerID << " au lobby du bot..." << std::endl;
                joinLobby(conn.getSocket(), lobby);
                return;
            }
        }
    }
    break;


    case ClientPackets::GetLobbies:
    {
        for (Lobby *lobby : m_games)
        {
            Server_GetLobbies p;
            strcpy(p.lobbyName, lobby->getName().c_str());
            p.numPlayers = lobby->getNumPlayers();
            p.maxPlayers = lobby->getMaxPlayers();
            p.lobbyID = lobby->getLobbyID();
            network::sendPacketTCP(conn.getSocket(), (uint32_t)ServerPackets::GetLobbies, p);
        }
    }
    break;

    case ClientPackets::CreateLobby:
    {
        recv(clientSocketTCP, buf, sizeof(Client_CreateLobby), 0);
        Socket &playerSocket = conn.getSocket();
        createLobby(playerSocket,
                    reinterpret_cast<Client_CreateLobby *>(buf)->lobbyName,
                    reinterpret_cast<Client_CreateLobby *>(buf)->gamemode);
    }
    break;

    case ClientPackets::JoinLobby:
    {
        recv(clientSocketTCP, buf, sizeof(Client_JoinLobby), 0);
        Socket &playerSocket = conn.getSocket();
        joinLobby(playerSocket, getLobbyByID(reinterpret_cast<Client_JoinLobby *>(buf)->lobbyID));
    }
    break;

    case ClientPackets::LeaveLobby:
    {
        leaveLobby(&m_clients[clientSocketTCP]);
    }
    break;

    case ClientPackets::StartGame:
    {
        // Not in lobby or not owner.
        if (conn.getLobby() == 0 || conn.getLobby()->getPlayerID(clientSocketTCP) != 0)
        {
            Server_GameStart p;
            p.started = false;
            network::sendPacketTCP(conn.getSocket(), (uint32_t)ServerPackets::GameStart, p);
            return;
        }

        conn.getLobby()->start();
    }
    break;
    }
}

void Server::notifyReceiveUDP()
{
    char buf[MAX_PACKET_SIZE];
    sockaddr_in clientAddr;
    int addrlen = sizeof(sockaddr);

    int rec = recvfrom(m_socketUDP.mSocket, buf, MAX_PACKET_SIZE, 0, (sockaddr *)&clientAddr, &addrlen);
    int error = WSAGetLastError();

    uint32_t packetID = *((uint32_t*)buf);
    ClientConnection* conn = getClientByAddress(*(sockaddr*)&clientAddr);

    // Handle UDP authentication here.
    if ((ClientPackets) packetID == ClientPackets::PlayerConnectUDP && !conn) {
        uint32_t id = ((Client_PlayerConnectUDP*)(buf+4))->playerID;
        getClientByID(id)->m_udpAddr = clientAddr;
        return;
    }

    if (!conn) {
        std::cout << "Received UDP data, but the client could not be resolved..." << std::endl;
        return;
    }

    handleUDPPacket(packetID, buf + 4, (sockaddr *)&clientAddr);
}

void Server::handleUDPPacket(uint32_t packetID, char *buf, sockaddr *addr)
{
    switch (static_cast<ClientPackets>(packetID))
    {
    case ClientPackets::PlayerConnectUDP:
    {
        uint32_t id = ((Client_PlayerConnectUDP *)buf)->playerID;
        getClientByID(id)->m_udpAddr = *(sockaddr_in *)addr;
    }
    break;
    case ClientPackets::PlayerMove:
    {
        auto pingStartTime = std::chrono::high_resolution_clock::now();

        auto pingEndTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> latency = pingEndTime - pingStartTime;
        int roundedLatency = static_cast<int>(latency.count() + 0.5);
        if (roundedLatency < 1) roundedLatency = 0;

        // Création du paquet de ping
        Server_PingInfo pingPacket;
        pingPacket.ping = roundedLatency;

        // Envoi du ping au client
        network::sendPacketUDP(m_socketUDP, addr, (uint32_t)ServerPackets::PingInfo, pingPacket);

        // Gestion du mouvement du joueur
        Client_PlayerMove* packet = reinterpret_cast<Client_PlayerMove*>(buf);
        ClientConnection* conn = getClientByID(packet->playerID);

        LobbyPong* pong = dynamic_cast<LobbyPong*>(conn->getLobby());
        pong->receivePlayerMove(pong->getPlayerID(conn->getSocket().mSocket), packet->position);
    }
    break;


    default:
        std::cerr << "Unknown UDP packet received: " << packetID << std::endl;
        break;
    }
}

void Server::updateGames(float dt)
{
    for (auto it = m_games.begin(); it != m_games.end();)
    {
        (*it)->update(dt);

        if ((*it)->getNumPlayers() == 0)
        {
            std::cout << "Lobby " << (*it)->getLobbyID() << " deleted (no active players)" << std::endl;
            delete *it;
            it = m_games.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

Lobby *Server::getLobbyByID(uint32_t id) const
{
    for (Lobby *l : m_games)
    {
        if (l->getLobbyID() == id)
        {
            return l;
        }
    }
    return 0;
}

ClientConnection *Server::getClientByID(uint32_t id)
{
    for (auto &p : m_clients)
    {
        if (p.second.m_id == id)
        {
            return &p.second;
        }
    }
    return 0;
}

ClientConnection *Server::getClientBySocket(SOCKET s)
{
    auto it = m_clients.find(s);
    if (it == m_clients.end())
        return 0;
    return &it->second;
}

ClientConnection *Server::getClientByAddress(const sockaddr &addr)
{
    const sockaddr_in &inetAddr = reinterpret_cast<const sockaddr_in &>(addr);
    for (auto &p : m_clients)
    {
        if (p.second.getUDPAddr()->sin_addr.s_addr == inetAddr.sin_addr.s_addr &&
            p.second.getUDPAddr()->sin_port == inetAddr.sin_port)
        {
            return &p.second;
        }
    }
    return 0;
}
