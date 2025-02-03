#ifndef NETWORKING_H
#define NETWORKING_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

#include "Socket.h"


#define UDPPort 27015
#define TCPPort 27014

//#define DEFAULT_BUFLEN 512

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
	void sendPacketUDP(std::string message);
	void sendPacketTCP(std::string message);

	Socket mSocketUDP;
	Socket mSocketTCP;
private:


};

#endif // NETWORKING_H