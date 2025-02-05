#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <stdint.h>

#include "Socket.h"

#define MAX_PACKET_SIZE 1024


#define UDPPort 27015
#define TCPPort 27014

class network {
public:
	network();
	~network();
	std::string serverAddress;
	struct sockaddr_in serverAddr;

	static int initializeWinsock();
	static void cleanupWinsock();

	static int getServerAddressUDP(struct sockaddr* out, const char* ip, uint16_t port);
	static int getServerAddressTCP(struct sockaddr* out, const char* ip, uint16_t port);
	void sendPacketUDP(Socket& s, std::string message);
	void sendPacketTCP(Socket& s, std::string message);

	static size_t sendPacketTCP(Socket& s, const void* buf, size_t size);
	static size_t sendPacketUDP(Socket& s, const sockaddr* addr, const void* buf, size_t size);

		/// Advanced interface for packet-oriented communication.
	template<typename T>
	static bool sendPacketTCP(Socket& s, uint32_t packetID, const T& data);

	template<typename T>
	static bool sendPacketUDP(Socket& s, const sockaddr* addr, uint32_t packetID, const T& data);

	template<typename T>
	static bool receivePacketTCP(Socket& s, T& data);

	template<typename T>
	static bool receivePacketUDP(Socket& s, sockaddr_in* senderAddr, T& data);
private:
};


#include "Networking.inl"
