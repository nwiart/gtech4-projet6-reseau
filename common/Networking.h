#ifndef NETWORKING_H
#define NETWORKING_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#include "Socket.h"

//#define DEFAULT_BUFLEN 512

class network {
public:
	static PCSTR serverAddress;
	static struct sockaddr_in serverAddr;

	static void initializeWinsock();
	static void cleanupWinsock();
	static int getServerAddressUDP(PCSTR PORT);
	static int getServerAddressTCP(PCSTR PORT);
	void sendSocketUDP();
	void sendSocketTCP();
};

#endif // NETWORKING_H