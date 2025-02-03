#ifndef NETWORKING_H
#define NETWORKING_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#include "Socket.h"

//#define DEFAULT_BUFLEN 512

class network {
public:
	std::string& serverAddress;
	struct sockaddr_in* server;

	static void initializeWinsock();
	static void cleanupWinsock();
	static int getServerAddressUDP(struct sockaddr* out, PCSTR PORT);
	static int getServerAddressTCP(struct sockaddr* out, const char* ip, uint16_t port);
	void sendSocketUDP();
	void sendSocketTCP();
};

#endif // NETWORKING_H