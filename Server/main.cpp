#include "Networking.h"
#include "Server.h"

#include <iostream>


#define MESSAGE_ACCEPT (WM_USER)
#define MESSAGE_UDP (WM_USER + 1)

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
	WSAAsyncSelect(server.getUDPSocket().mSocket, hwnd, MESSAGE_UDP, FD_READ);

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
		std::cout << "dsdfosignofgn\n";
		return 0;
	case MESSAGE_UDP:
		if (WSAGETSELECTEVENT(lparam) == FD_READ) {
			char buffer[512];
			sockaddr_in udpSenderAddr;
			int addrLen = sizeof(udpSenderAddr);

			int bytesReceived = recvfrom(wparam, buffer, sizeof(buffer), 0, (struct sockaddr*)&udpSenderAddr, &addrLen);
			if (bytesReceived == SOCKET_ERROR) {
				std::cerr << "Error receiving UDP message. Error: " << WSAGetLastError() << std::endl;
				return 0;
			}

			std::cout << "Received UDP message: " << std::string(buffer, bytesReceived) << std::endl;
		}
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}
