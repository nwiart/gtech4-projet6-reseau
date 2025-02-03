#ifndef NETWORKING_H
#define NETWORKING_H

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

	void initializeWinsock();
	void cleanupWinsock();
	int getServerAddressUDP();
	int getServerAddressTCP();
	void sendSocketUDP(std::string message);
	void sendSocketTCP(std::string message);
private:
	Socket udpSocket;
	Socket tcpSocket;
};

#endif // NETWORKING_H