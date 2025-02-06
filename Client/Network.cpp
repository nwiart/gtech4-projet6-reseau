#include "Network.h"
#include "PongPackets.h"

#include "MainMenu.h"

#include <string.h>
#include <iostream>

#define MESSAGE_RECV (WM_USER + 1)  // Ensure a unique user-defined message

static HWND hwnd;

void create_window();
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

Socket Network::m_socketTCP;
Socket Network::m_socketUDP;
sockaddr_in Network::serverUDPAddr;

void Network::init()
{
    create_window();
}

void Network::pollEvents()
{
    MSG msg;
    while (PeekMessageA(&msg, hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    //receiveUDPPackets();
}

int Network::connect(const char* ip, const char* playerName)
{
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

    // Copy player name safely
    Client_PlayerConnect packet;
    int len = strlen(playerName);
    if (len > sizeof(packet.playerName) - 1) {
        len = sizeof(packet.playerName) - 1;
    }
    strncpy(packet.playerName, playerName, len);
    packet.playerName[len] = '\0';

    // Send the PlayerConnect packet
    bool success = network::sendPacketTCP(m_socketTCP, static_cast<uint32_t>(ClientPackets::PlayerConnect), packet);
    if (!success) {
        std::cerr << "Failed to send PlayerConnect packet.\n";
        return -1;
    }

    // UDP Initialization
    m_socketUDP.createSocketUDP();

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

void Network::createLobbyPong1v1(const std::string& name)
{
    Network::createLobby(GameMode::PONG_1v1, name);
}

void Network::createLobbyPong2v2(const std::string& name)
{
    Network::createLobby(GameMode::PONG_2v2, name);
}

void Network::createLobby(GameMode gm, const std::string& name)
{
    int namelen = name.size() > 31 ? 31 : name.size();

    Client_CreateLobby p;
    memcpy(p.lobbyName, name.c_str(), namelen); p.lobbyName[namelen] = 0;
    p.gamemode = gm;

    network::sendPacketTCP(m_socketTCP, (uint32_t)ClientPackets::CreateLobby, p);
}

int Network::sendPosition(int posY)
{
    Client_PlayerMove packet{ posY };

    bool success = network::sendPacketUDP(m_socketUDP, reinterpret_cast<const sockaddr*>(&serverUDPAddr), static_cast<uint32_t>(ClientPackets::PlayerMove), packet);

    if (!success) {
        std::cerr << "Failed to send PlayerMove UDP packet!" << std::endl;
        return -1;
    }

    return 0;
}

void Network::handleTCPPacket(uint32_t packetID)
{
    char buf[1020];

    switch ((ServerPackets)packetID)
    {
    case ServerPackets::GetLobbies:
    {
        recv(m_socketTCP.mSocket, buf, sizeof(Server_GetLobbies), 0);
        MainMenu* menu = dynamic_cast<MainMenu*>(Scene::getCurrentScene());
        if (menu) {
            Server_GetLobbies& p = *reinterpret_cast<Server_GetLobbies*>(buf);
            menu->listLobby(p.lobbyName, p.numPlayers, p.maxPlayers);
        }
    }
    break;

    case ServerPackets::AcceptJoin:
    {
        int received = recv(m_socketTCP.mSocket, buf, sizeof(Server_AcceptJoin), 0);
        if (received == sizeof(Server_AcceptJoin)) {
            int playerID = reinterpret_cast<Server_AcceptJoin*>(buf)->playerID;
            printf("Accepted! Player ID: %d\n", playerID);
        }
        else {
            std::cerr << "Failed to receive AcceptJoin packet.\n";
        }
    }
    break;

    case ServerPackets::DenyJoin:
    {
        int received = recv(m_socketTCP.mSocket, buf, sizeof(Server_DenyJoin), 0);
        if (received == sizeof(Server_DenyJoin)) {
            printf("Join request denied: %s\n", reinterpret_cast<Server_DenyJoin*>(buf)->reason);
        }
        else {
            std::cerr << "Failed to receive DenyJoin packet.\n";
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
                Network::handleTCPPacket(packetID);
            }
            else {
                std::cerr << "Failed to receive valid packet ID.\n";
            }
        }
        break;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Network::receiveUDPPackets() {
    sockaddr_in senderAddr;
    uint32_t packetID;

    while (network::receivePacketUDP(m_socketUDP, &senderAddr, packetID)) {
        handleUDPPacket(packetID);
    }
}


void Network::handleUDPPacket(uint32_t packetID) {
    switch (static_cast<ServerPackets>(packetID)) {
    case ServerPackets::PlayerMove: {
        Server_PlayerMove packet;
        if (network::receivePacketUDP(m_socketUDP, nullptr, packet)) {
            std::cout << "Player " << packet.playerID << " move to " << packet.position << std::endl;
        }
    } break;

    case ServerPackets::BallInfo: {
        Server_BallInfo packet;
        if (network::receivePacketUDP(m_socketUDP, nullptr, packet)) {
            std::cout << "Ball position: (" << packet.xPos << ", " << packet.yPos << ")" << std::endl;
        }
    } break;

    case ServerPackets::Score: {
        Server_Score packet;
        if (network::receivePacketUDP(m_socketUDP, nullptr, packet)) {
            std::cout << "Team " << packet.team << " scored!" << std::endl;
        }
    } break;

    default:
        std::cerr << "Unknown UDP packet received: " << packetID << std::endl;
        break;
    }
    /////////ADD THEM ALL!!!!!///////// eventually
}