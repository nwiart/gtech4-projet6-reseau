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

	auto start = std::chrono::high_resolution_clock().now();
	auto last = start;
	std::chrono::high_resolution_clock::duration elapsed(std::chrono::nanoseconds(0));
	while (1)
	{
		MSG msg;
		while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		auto now = std::chrono::high_resolution_clock().now();
		elapsed += (now - last);
		last = now;

		const double dt = 1.0 / 60.0;
		while (elapsed.count() / 1000000000.0 >= dt) {
			elapsed -= std::chrono::nanoseconds(1000000000 / 60);
			server.updateGames(dt);
		}

		Sleep(1);
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
		if (!server.getListenSocket().acceptTCP(newClientSocket) || !newClientSocket.isValid()) {
			std::cerr << "Failed to accept new client connection. Invalid socket.\n";
			break;
		}

		std::cout << "New client connected! Socket: " << newClientSocket.mSocket << std::endl;

		if (WSAAsyncSelect(newClientSocket.mSocket, hwnd, MESSAGE_RECV, FD_READ | FD_CLOSE) == SOCKET_ERROR) {
			std::cerr << "WSAAsyncSelect failed. Error: " << WSAGetLastError() << "\n";
			break;
		}

		server.notifyConnect(newClientSocket);
	}
	break;
	case MESSAGE_RECV:
	{
		SOCKET socket = (SOCKET)wparam;

		if (!server.isClientSocketValid(socket)) {
			std::cerr << "MESSAGE_RECV: Invalid or unknown client socket: " << socket << "\n";
			break;
		}

		if (socket == INVALID_SOCKET) {
			std::cerr << "ERROR: SOCKET IS ALREADY INVALID! (Socket: " << socket << ")\n";
			break;
		}

		std::cout << "Receiving data on socket: " << socket << "\n";

		uint32_t packetID;
		int received = recv(socket, reinterpret_cast<char*>(&packetID), sizeof(packetID), 0);

		if (received == sizeof(packetID)) {
			std::cout << "Received packet ID: " << packetID << " from socket: " << socket << "\n";
			server.notifyReceiveTCP(socket, packetID);
		}
		else if (received == 0) {
			std::cerr << "Client disconnected gracefully.\n";
			server.notifyDisconnect(socket);
		}
		else if (received == SOCKET_ERROR) {
			int error = WSAGetLastError();
			std::cerr << "recv() failed. Error: " << error << " (Socket: " << socket << ")\n";

			if (error == WSAENOTSOCK) {
				std::cerr << "WSAENOTSOCK: The socket is invalid! Check if it was closed elsewhere.\n";
			}

			server.notifyDisconnect(socket);
		}
		else {
			std::cerr << "Failed to receive valid packet ID.\n";
		}
	}
	break;
	case FD_CLOSE:
	{
		std::cerr << "Client closed the connection.\n";
		//Client::getInstance().disconnect();

		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);

	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}