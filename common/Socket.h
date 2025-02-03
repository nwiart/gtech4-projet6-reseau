#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>
#include <iostream>

typedef uint64_t SOCKET;

class Socket {
public:
	SOCKET mSocket;

	Socket();
	~Socket();

	void createSocketUDP();
	void createSocketTCP();

	int connectTCP(const char* ip, uint16_t port);
	int listenTCP(uint16_t port);
	int acceptTCP();

	int bindUDP(uint16_t port);
	int receiveUDP(std::string& outMessage);

};

#endif // SOCKET_H