#ifndef NETWORKING_H
#define NETWORKING_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

#include "Socket.h"


#define UDPPort "27015"
#define TCPPort "27014"

//#define DEFAULT_BUFLEN 512

class network {
public:
	network();
	~network();
	std::string serverAddress;
	struct sockaddr_in serverAddr;

	static void initializeWinsock();
	static void cleanupWinsock();

	static int getServerAddressUDP(struct sockaddr* out, PCSTR PORT);
	static int getServerAddressTCP(struct sockaddr* out, const char* ip, uint16_t port);
	void sendSocketUDP(std::string message);
	void sendSocketTCP(std::string message);
};

#endif // NETWORKING_H