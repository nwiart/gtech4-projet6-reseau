#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>
#pragma comment(lib, "ws2_32.lib")

typedef uint64_t SOCKET;

class Socket {
public:
	Socket();
	~Socket();

	SOCKET mSocket;

	void createSocketUDP();
	void createSocketTCP();
	void bindSocketUDP(const char* port);
private:
};

#endif // SOCKET_H