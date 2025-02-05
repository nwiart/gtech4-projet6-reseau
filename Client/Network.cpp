#include "Network.h"
#include "PongPackets.h"

#include <string.h>

#define MESSAGE_RECV (WM_USER)



static HWND hwnd;

void create_window();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


Socket Network::m_socketTCP;


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
}

int Network::connect(const char* ip,const char* playerName)
{
    m_socketTCP.createSocketTCP();

    WSAAsyncSelect(m_socketTCP.mSocket, hwnd, MESSAGE_RECV, FD_READ);

    if (m_socketTCP.connectTCP(ip, serverBasePort) == SOCKET_ERROR) {
        std::cerr << "TCP connection failed.\n";
        return -1;
    }

    Client_PlayerConnect packet;
    strcpy(packet.playerName, playerName);
    bool success = network::sendPacketTCP(m_socketTCP, (uint32_t)ClientPackets::PlayerConnect, packet);
    if (!success) {
        return -1;
    }

    /*Server_ConnectResult connectResult;
    //recv(m_socketTCP.mSocket, (char*)&connectResult, sizeof(connectResult), 0);

    if (!connectResult.success)
    {
        std::cerr << "Connection refused by server!" << std::endl;
        return -1;
    }

    m_playerID = connectResult.playerID;*/


    m_socketUDP.createSocketUDP();
    if (m_socketUDP.bindUDP(0) == SOCKET_ERROR) {
        std::cerr << "UDP socket bind failed.\n";
        return -1;
    }

    memset(&serverUDPAddr, 0, sizeof(serverUDPAddr));
    serverUDPAddr.sin_family = AF_INET;
    serverUDPAddr.sin_port = htons(serverSecondaryPort);
    inet_pton(AF_INET, ip, &serverUDPAddr.sin_addr);


    return 0;
}

int Network::sendPosition(float position)
{
    Client_PlayerMove packet{ position };

    bool success = network::sendPacketUDP(m_socketUDP, reinterpret_cast<sockaddr*>(&serverUDPAddr), static_cast<uint32_t>(ClientPackets::PlayerMove), packet);

    if (!success)
    {
        std::cerr << "Failed to send position packet.\n";
        return -1;
    }

    return 0;
}



void Network::handlePacket(uint32_t packetID)
{
    char buf[1020];
    
    switch ((ServerPackets) packetID)
    {
    // Join results.
    case ServerPackets::AcceptJoin:
        {
            recv(m_socketTCP.mSocket, buf, sizeof(Server_AcceptJoin), 0);
            reinterpret_cast<Server_AcceptJoin*>(buf)->playerID;
            printf("Accepted!!!!!! %d\n", reinterpret_cast<Server_AcceptJoin*>(buf)->playerID);
        }
        break;
    case ServerPackets::DenyJoin:
        {
            recv(m_socketTCP.mSocket, buf, sizeof(Server_DenyJoin), 0);
            reinterpret_cast<Server_DenyJoin*>(buf)->reason;
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

	hwnd = CreateWindowA(className, "", 0, 0, 0, 100, 100, 0, 0, inst, 0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case MESSAGE_RECV:
	    {
		    SOCKET socket = (SOCKET)wparam;
            uint32_t packetID;

            recv(socket, (char*) &packetID, 4, 0);
            Network::handlePacket(packetID);
	    }
	    return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}
