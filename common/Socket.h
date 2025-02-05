#pragma once

#include <stdint.h>
#include <iostream>

typedef uint64_t SOCKET;

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
