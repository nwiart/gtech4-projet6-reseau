#include "Networking.h"
#include "Server.h"

#include <iostream>


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
	while (GetMessage(&msg, hwnd, 0, 0)) {
		DispatchMessage(&msg);
		TranslateMessage(&msg);
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
			Socket newClientSocket = server.getListenSocket().acceptTCP();
			if (newClientSocket.mSocket != INVALID_SOCKET) {
				server.notifyConnect(newClientSocket);
				WSAAsyncSelect(newClientSocket.mSocket, hwnd, MESSAGE_RECV, 0);
			}
		}
		return 0;

	case MESSAGE_RECV:
		{
			SOCKET socket = (SOCKET)wparam;
			server.notifyReceiveTCP(socket);
		}
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}
