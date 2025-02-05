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

	template<typename T>
	static bool receivePacketTCP(Socket& s, T& data);

	template<typename T>
	static bool receivePacketUDP(Socket& s, sockaddr_in* senderAddr, T& data);
private:
};


template<typename T>
bool network::sendPacketTCP(Socket& s, uint32_t packetID, const T& data)
{
	static_assert(sizeof(T) <= MAX_PACKET_SIZE - 4);

	char buf[MAX_PACKET_SIZE];
	*((uint32_t*)buf) = packetID;
	memcpy(buf + 4, &data, sizeof(T));

	std::cout << "Sending Packet TCP: ID=" << packetID
		<< ", Size=" << (sizeof(T) + 4)
		<< ", Socket=" << s.mSocket << std::endl;

	size_t bytesSent = sendPacketTCP(s, buf, sizeof(T) + 4);
	if (bytesSent == 0) {
		std::cerr << "sendPacketTCP() failed! WSA Error: " << WSAGetLastError() << std::endl;
		return false;
	}

	std::cout << "Sent " << bytesSent << " bytes successfully." << std::endl;
	return true;
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

template<typename T>
bool network::receivePacketTCP(Socket& s, T& data) {
	int receivedBytes = recv(s.mSocket, reinterpret_cast<char*>(&data), sizeof(T), 0);

	if (receivedBytes == SOCKET_ERROR) {
		std::cerr << "Error receiving packet TCP! WSA Error: " << WSAGetLastError() << std::endl;
		return false;
	}

	return receivedBytes == sizeof(T);
}


template<typename T>
bool network::receivePacketUDP(Socket& s, sockaddr_in* senderAddr, T& data) {
	char buffer[MAX_PACKET_SIZE];
	int senderAddrSize = sizeof(sockaddr_in);

	int bytesReceived = recvfrom(s.mSocket, buffer, sizeof(buffer), 0,
		reinterpret_cast<sockaddr*>(senderAddr), &senderAddrSize);

	if (bytesReceived == SOCKET_ERROR) {
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK) {
			std::cerr << "UDP receive failed! WSA Error: " << error << std::endl;
		}
		return false;
	}

	if (bytesReceived < sizeof(T)) {
		std::cerr << "Incomplete UDP packet received! Expected: " << sizeof(T)
			<< ", Received: " << bytesReceived << std::endl;
		return false;
	}

	memcpy(&data, buffer, sizeof(T));
	return true;
}

#endif // NETWORKING_H