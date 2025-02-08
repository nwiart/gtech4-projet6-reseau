#include "Client.h"
#include "PongPackets.h"

#include "Scene/MainMenu.h"
#include "Scene/LobbyMenu.h"

#include "Pong/GameScene.h"

#include <string.h>
#include <iostream>

#define MESSAGE_RECV (WM_USER + 1)
#define MESSAGE_UDP (WM_USER+2)


static HWND hwnd;

void create_window();
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

Socket Client::m_socketTCP;
Socket Client::m_socketUDP;
sockaddr_in Client::serverUDPAddr;


extern sf::Font font;


void Client::init()
{
    create_window();
}

void Client::pollEvents()
{
    MSG msg;
    while (PeekMessageA(&msg, hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

int Client::connect(const char* ip, const std::string& playerName)
{
    m_playerName = playerName;

    m_socketTCP.createSocketTCP();

    // Attempt to connect via TCP
    if (m_socketTCP.connectTCP(ip, serverBasePort) == SOCKET_ERROR) {
        int error = WSAGetLastError();
        std::cerr << "TCP connection failed. Error: " << error << "\n";
        return -1;
    }

    // Now that we are connected, set up async socket event handling
    if (WSAAsyncSelect(m_socketTCP.mSocket, hwnd, MESSAGE_RECV, FD_READ) == SOCKET_ERROR) {
        std::cerr << "WSAAsyncSelect failed. Error: " << WSAGetLastError() << "\n";
        return -1;
    }

    // UDP Initialization
    m_socketUDP.createSocketUDP();

    /*if (m_socketUDP.bindUDP(0) == SOCKET_ERROR) {
        std::cerr << "UDP socket bind failed.\n";
        return -1;
    }*/

    /*sockaddr_in ua;
    ua.sin_family = AF_INET;
    ua.sin_port = 27014;
    ua.sin_addr.s_addr = inet_addr(ip);
    ::connect(m_socketUDP.mSocket, (sockaddr*) &ua, sizeof(sockaddr));*/

    bool t = true;
    setsockopt(m_socketUDP.mSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&t, 1);

    sockaddr_in ua;
    int len = sizeof(sockaddr);
    getpeername(m_socketUDP.mSocket, (sockaddr*) &ua, &len);

    // Copy player name safely
    Client_PlayerConnect packet;
    len = playerName.length();
    if (len > sizeof(packet.playerName) - 1) {
        len = sizeof(packet.playerName) - 1;
    }
    strncpy(packet.playerName, playerName.c_str(), len);
    packet.playerName[len] = '\0';

    bool success = network::sendPacketTCP(m_socketTCP, static_cast<uint32_t>(ClientPackets::PlayerConnect), packet);
    if (!success) {
        std::cerr << "Failed to send PlayerConnect packet.\n";
        return -1;
    }

    WSAAsyncSelect(m_socketUDP.mSocket, hwnd, MESSAGE_UDP, FD_READ);

    // Setup server UDP address
    memset(&serverUDPAddr, 0, sizeof(serverUDPAddr));
    serverUDPAddr.sin_family = AF_INET;
    serverUDPAddr.sin_port = htons(serverSecondaryPort-1);

    if (inet_pton(AF_INET, ip, &serverUDPAddr.sin_addr) <= 0) {
        std::cerr << "Invalid IP address format.\n";
        return -1;
    }

    return 0;
}

void Client::createLobbyPong1v1(const std::string& name)
{
    this->createLobby(GameMode::PONG_1v1, name);
}

void Client::createLobbyPong2v2(const std::string& name)
{
    this->createLobby(GameMode::PONG_2v2, name);
}

void Client::startGame()
{
    Client_StartGame p;
    network::sendPacketTCP(m_socketTCP, (uint32_t)ClientPackets::StartGame, p);
}

void Client::joinLobby(uint32_t lobbyID)
{
    Client_JoinLobby p;
    p.lobbyID = lobbyID;
    network::sendPacketTCP(m_socketTCP, (uint32_t)ClientPackets::JoinLobby, p);
}

void Client::leaveLobby()
{
    Client_LeaveLobby p;
    network::sendPacketTCP(m_socketTCP, (uint32_t)ClientPackets::LeaveLobby, p);

    m_lobby.leave();
}


void Client::completeAuthentication(uint32_t playerID)
{
    m_playerID = playerID;
}

void Client::createLobby(GameMode gm, const std::string& name)
{
    int namelen = name.size() > 31 ? 31 : name.size();

    Client_CreateLobby p;
    memcpy(p.lobbyName, name.c_str(), namelen); p.lobbyName[namelen] = 0;
    p.gamemode = gm;

    network::sendPacketTCP(m_socketTCP, (uint32_t)ClientPackets::CreateLobby, p);
}

int Client::sendPosition(int posY)
{
    Client_PlayerMove packet{ m_playerID, posY };

    network::sendPacketUDP(m_socketUDP, reinterpret_cast<const sockaddr*>(&serverUDPAddr), static_cast<uint32_t>(ClientPackets::PlayerMove), packet);

    return 0;
}

void Client::handleTCPPacket(uint32_t packetID)
{
    char buf[1020];

    switch ((ServerPackets)packetID)
    {
    case ServerPackets::ConnectResult:
    {
        recv(m_socketTCP.mSocket, buf, sizeof(Server_ConnectResult), 0);
        this->completeAuthentication(((Server_ConnectResult*) buf)->playerID);

        Client_PlayerConnectUDP p;
        p.playerID = m_playerID;
        network::sendPacketUDP(m_socketUDP, (sockaddr*) & serverUDPAddr, (uint32_t)ClientPackets::PlayerConnectUDP, p);
    }
    break;

    case ServerPackets::GetLobbies:
    {
        recv(m_socketTCP.mSocket, buf, sizeof(Server_GetLobbies), 0);
        MainMenu* menu = dynamic_cast<MainMenu*>(Scene::getCurrentScene());
        if (menu) {
            Server_GetLobbies& p = *reinterpret_cast<Server_GetLobbies*>(buf);
            menu->listLobby(p.lobbyID, p.lobbyName, p.numPlayers, p.maxPlayers);
        }
    }
    break;

    // Lobby join / create.
    case ServerPackets::AcceptJoin:
    {
        int received = recv(m_socketTCP.mSocket, buf, sizeof(Server_AcceptJoin), 0);
        if (received == sizeof(Server_AcceptJoin)) {
            int idInLobby = reinterpret_cast<Server_AcceptJoin*>(buf)->inLobbyID;
            int max = reinterpret_cast<Server_AcceptJoin*>(buf)->maxPlayers;

            printf("Accepted! Player ID: %d\n", idInLobby);

            m_lobby.setupGuest(idInLobby, max);

            Scene::setCurrentScene(new LobbyMenu());
        }
    }
    break;

    case ServerPackets::DenyJoin:
    {
        int received = recv(m_socketTCP.mSocket, buf, sizeof(Server_DenyJoin), 0);
        if (received == sizeof(Server_DenyJoin)) {
            printf("Join request denied: %s\n", reinterpret_cast<Server_DenyJoin*>(buf)->reason);
        }
    }
    break;
    case ServerPackets::LobbyCreation:
    {
        recv(m_socketTCP.mSocket, buf, sizeof(Server_LobbyCreation), 0);
        Server_LobbyCreation* p = (Server_LobbyCreation*) buf;
        if (p->success) {
            m_lobby.setupHost(p->playerID, p->maxPlayers);

            Scene::setCurrentScene(new LobbyMenu());
        }
    }
    break;

    case ServerPackets::PlayerJoinedLobby:
    {
        recv(m_socketTCP.mSocket, buf, sizeof(Server_PlayerJoinedLobby), 0);

        // We're not in lobby, ignore it.
        if (!m_lobby.isValid()) break;

        Server_PlayerJoinedLobby* p = (Server_PlayerJoinedLobby*) buf;
        m_lobby.listPlayer(p->playerID, p->idInLobby, p->playerName);
    }
    break;

    case ServerPackets::GameStart:
    {
        recv(m_socketTCP.mSocket, buf, sizeof(Server_GameStart), 0);
        Scene::setCurrentScene(new GameScene());
    }
    break;
    }
}

void Client::handleUDPPacket()
{
    char buf[MAX_PACKET_SIZE];
    int rec = recvfrom(m_socketUDP.mSocket, buf, MAX_PACKET_SIZE, 0, 0, 0);

    uint32_t packetID = *((uint32_t*) buf);

    switch ((ServerPackets) packetID)
    {
    case ServerPackets::PlayerMove: {
        Server_PlayerMove* packet = (Server_PlayerMove*) (buf + 4);
        GameScene* scene = dynamic_cast<GameScene*>(Scene::getCurrentScene());
        scene->setPlayerPos(packet->playerID, packet->position);
    }
    break;

    case ServerPackets::BallInfo: {
        Server_BallInfo* packet = (Server_BallInfo*)(buf + 4);
        GameScene* scene = dynamic_cast<GameScene*>(Scene::getCurrentScene());
        if (scene) {
            //scene->setBallInfo(sf::Vector2f(packet->xPos, packet->yPos), sf::Vector2f(packet->xVel, packet->yVel));
        }
    }
    break;
    }
}


void create_window()
{
    static const char className[] = "ClientWnd";

    HINSTANCE inst = GetModuleHandleA(0);

    WNDCLASSEXA wcex; ZeroMemory(&wcex, sizeof(WNDCLASSEXA));
    wcex.cbSize = sizeof(WNDCLASSEXA);
    wcex.hInstance = inst;
    wcex.lpszClassName = className;
    wcex.lpfnWndProc = WndProc;

    RegisterClassExA(&wcex);

    hwnd = CreateWindowA(className, "Title", 0, 0, 0, 100, 100, 0, 0, inst, 0);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case MESSAGE_RECV:
        {
            SOCKET socket = (SOCKET)wparam;
            uint32_t packetID;
            int received = recv(socket, reinterpret_cast<char*>(&packetID), sizeof(packetID), 0);

            if (received == sizeof(packetID)) {
                Client::getInstance().handleTCPPacket(packetID);
            }
            else {
                std::cerr << "Failed to receive valid packet ID.\n";
            }
        }
        break;
    case MESSAGE_UDP:
        Client::getInstance().handleUDPPacket();
        break;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}
