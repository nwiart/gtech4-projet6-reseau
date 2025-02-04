#ifndef NETWORKING_H
#define NETWORKING_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <stdint.h>
#pragma comment(lib, "Ws2_32.lib")

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
private:
};


template<typename T>
bool network::sendPacketTCP(Socket& s, uint32_t packetID, const T& data)
{
	static_assert(sizeof(T) <= MAX_PACKET_SIZE - 4);

	char buf[MAX_PACKET_SIZE];
	*((uint32_t*) buf) = packetID;
	memcpy(buf + 4, &data, sizeof(T));

	return sendPacketTCP(s, buf, sizeof(T) + 4) != 0;
}

template<typename T>
bool network::sendPacketUDP(Socket& s, const sockaddr* addr, uint32_t packetID, const T& data)
{
	static_assert(sizeof(T) <= MAX_PACKET_SIZE - 4);

	char buf[MAX_PACKET_SIZE];
	*((uint32_t*)buf) = packetID;
	memcpy(buf + 4, &data, sizeof(T));

	return sendto(s.mSocket, buf, sizeof(T) + 4, 0, addr, sizeof(sockaddr)) != SOCKET_ERROR;
}


#endif // NETWORKING_H