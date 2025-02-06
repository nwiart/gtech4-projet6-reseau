#include "Networking.h"
#include "Server.h"

#include <iostream>
#include <sstream>
#include <chrono>


#define MESSAGE_ACCEPT (WM_USER)
#define MESSAGE_UDP (WM_USER+1)
#define MESSAGE_RECV (WM_USER+2)

Server server;
HWND hwnd;


void create_window();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int main(int argc, const char** argv)
{
	network::initializeWinsock();

	create_window();

	server.open();

	WSAAsyncSelect(server.getListenSocket().mSocket, hwnd, MESSAGE_ACCEPT, FD_ACCEPT);

	std::cout << "Server started.\n";

	MSG msg;

	while (1)
	{
		while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
			DispatchMessage(&msg);
			TranslateMessage(&msg);

		}

		float dt = 1.0f / 60.0f;
		server.updateGames(dt);
		std::chrono::milliseconds(10);
	}

	network::cleanupWinsock();

	return 0;
}


void create_window()
{
	static const char className[] = "ServerWnd";

	HINSTANCE inst = GetModuleHandleA(0);

	WNDCLASSEX wcex; ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hInstance = inst;
	wcex.lpszClassName = className;
	wcex.lpfnWndProc = WndProc;

	RegisterClassEx(&wcex);

	hwnd = CreateWindow(className, "", 0, 0, 0, 100, 100, 0, 0, inst, 0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case MESSAGE_ACCEPT:
	{
		Socket newClientSocket;
		server.getListenSocket().acceptTCP(newClientSocket);
		WSAAsyncSelect(newClientSocket.mSocket, hwnd, MESSAGE_RECV, FD_READ);

		if (newClientSocket.mSocket == INVALID_SOCKET) {
			//std::cerr << "New client socket is invalid after accept!" << std::endl;
		}
		else {
			std::stringstream ss;
			sockaddr_in addr;
			int len = sizeof(sockaddr_in);
			getsockname(newClientSocket.mSocket, (sockaddr*) &addr, &len);

			UCHAR* ipb = &addr.sin_addr.S_un.S_un_b.s_b1;
			ss << (int) ipb[0] << '.' << (int)ipb[1] << '.' << (int)ipb[2] << '.' << (int)ipb[3] << ':' << (int)addr.sin_port;
			std::cout << "New connection from " << ss.str() << std::endl;
		}

		server.notifyConnect(newClientSocket);
	}
	break;

	case MESSAGE_RECV:
		{
			SOCKET socket = (SOCKET)wparam;
			server.notifyReceiveTCP(socket);
		}
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}
