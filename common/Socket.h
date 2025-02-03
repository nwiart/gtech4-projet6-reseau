#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>

typedef uint64_t SOCKET;

class Socket {
public:
	SOCKET mSocket;

	void createSocketUDP();
	void createSocketTCP();
private:
};

#endif // SOCKET_H