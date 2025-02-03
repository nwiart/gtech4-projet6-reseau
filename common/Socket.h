#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>

typedef uint64_t SOCKET;

class Socket {
public:
	SOCKET mSocket;

	Socket();

	void createSocketUDP();
	void createSocketTCP();

	int connectTCP(const char* ip, uint16_t port);
	int listenTCP(uint16_t port);
	int acceptTCP();
};

#endif // SOCKET_H