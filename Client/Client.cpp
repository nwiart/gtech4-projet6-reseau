#include "Client.h"
#include "PongPackets.h"

#include "Scene/MainMenu.h"
#include "Scene/LobbyMenu.h"

#include "Pong/GameScene.h"

#include <string.h>
#include <iostream>
#include <thread>
#include <mutex>

#define MESSAGE_RECV (WM_USER + 1)
#define MESSAGE_UDP (WM_USER+2)


HWND hwnd = 0;

void create_window();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


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

    // Copy player name safely
    Client_PlayerConnect packet;
    int len = playerName.length();
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


    // UDP Initialization
    m_socketUDP.createSocketUDP();
    bool t = true;
    setsockopt(m_socketUDP.mSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&t, sizeof(t));

    if (m_socketUDP.bindUDP(0) == SOCKET_ERROR) {
        std::cerr << "UDP socket bind failed.\n";
        return -1;
    }

    // Setup server UDP address
    memset(&serverUDPAddr, 0, sizeof(serverUDPAddr));
    serverUDPAddr.sin_family = AF_INET;
    serverUDPAddr.sin_port = htons(serverSecondaryPort);

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

void Client::createLobby(GameMode gm, const std::string& name)
{
    int namelen = name.size() > 31 ? 31 : name.size();

    Client_CreateLobby p;
    memcpy(p.lobbyName, name.c_str(), namelen); p.lobbyName[namelen] = 0;
    p.gamemode = gm;

    network::sendPacketTCP(m_socketTCP, (uint32_t)ClientPackets::CreateLobby, p);
}

void Client::handleTCPPacket(uint32_t packetID)
{
    switch ((ServerPackets)packetID)
    {
    case ServerPackets::ConnectResult:
    {
        Server_ConnectResult result;
         network::receivePacketTCP(m_socketTCP, packetID, result);

        if (result.success) {
            m_playerID = result.playerID;

            Client_PlayerConnectUDP p;
            p.playerID = m_playerID;
            network::sendPacketUDP(m_socketUDP, (sockaddr*)&serverUDPAddr, (uint32_t)ClientPackets::PlayerConnectUDP, p);
        }
    }
    break;
    case ServerPackets::GetLobbies:
    {
        Server_GetLobbies lobby;
        network::receivePacketTCP(m_socketTCP, packetID, lobby);
        MainMenu* menu = dynamic_cast<MainMenu*>(Scene::getCurrentScene());
        if (menu) {
            menu->listLobby(lobby.lobbyID, lobby.lobbyName, lobby.numPlayers, lobby.maxPlayers);
        }
    }
    break;
    case ServerPackets::AcceptJoin:
    {
        Server_AcceptJoin result;
        network::receivePacketTCP(m_socketTCP, packetID, result);
        int idInLobby = result.playerID;
        int max = result.maxPlayers;

        printf("Accepted! Player ID: %d\n", idInLobby);

        m_lobby.setupGuest(idInLobby, max);

        Scene::setCurrentScene(new LobbyMenu());
    }
    break;

    case ServerPackets::DenyJoin:
    {
        Server_DenyJoin result;
        network::receivePacketTCP(m_socketTCP, packetID, result);
        printf("Join request denied: %s\n", result.reason);
    }
    break;
    case ServerPackets::LobbyCreation:
    {
        Server_LobbyCreation lobby;
        network::receivePacketTCP(m_socketTCP, packetID, lobby);
        m_lobby.setupHost(lobby.playerID, lobby.maxPlayers);
        Scene::setCurrentScene(new LobbyMenu());
    }
    break;

    case ServerPackets::GameStart:
    {
        Server_GameStart gamestart;
        network::receivePacketTCP(m_socketTCP, packetID, gamestart);
        Scene::setCurrentScene(new GameScene());
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
        if (received == SOCKET_ERROR) {
            std::cerr << "recv() failed. Error: " << WSAGetLastError() << "\n";
            break;
        }
        else {
            std::cerr << "Failed to receive valid packet ID.\n";
        }
        break;
    }
    case FD_CLOSE:
    {
        std::cerr << "Server closed the connection.\n";
        //Client::getInstance().disconnect();

        break;
    }
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
 
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}