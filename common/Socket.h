#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>
#include <iostream>
#include <winsock2.h>

class Socket {
public:
	SOCKET mSocket;

	Socket();
	Socket(SOCKET s);

	~Socket();

	void createSocketUDP();
	void createSocketTCP();

	int connectTCP(const char* ip, uint16_t port);
	int listenTCP(uint16_t port);
	void acceptTCP(Socket& outSocket);

	int bindUDP(uint16_t port);
	//std::string receiveUDP();

	bool isValid() const;
	Socket& operator=(const Socket& other) {
		if (this != &other) {
			this->mSocket = other.mSocket;
		}
		return *this;
	}

};

#endif // SOCKET_H