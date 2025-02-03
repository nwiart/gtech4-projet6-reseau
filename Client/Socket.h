#ifndef SOCKET_H
#define COCKET_H

class Socket {
public:
	SOCKET mSocket;

	void createSocketUDP();
	void createSocketTCP();
private:
	void initClient();
};

#endif // SOCKET_H