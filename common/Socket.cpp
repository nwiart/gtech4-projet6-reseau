#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Socket.h"
#include <WS2tcpip.h>
#include <iostream>

Socket::Socket() : mSocket(INVALID_SOCKET) {}

Socket::Socket(uint64_t s) : mSocket(s) {}

Socket::~Socket()
{

}

void Socket::createSocketTCP()
{
    mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mSocket == INVALID_SOCKET)
    {
        std::cerr << "Erreur: Impossible de cr�er un socket TCP" << std::endl;
    }
}

void Socket::createSocketUDP()
{
    mSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (mSocket == INVALID_SOCKET)
    {
        std::cerr << "Erreur: Impossible de cr�er un socket UDP" << std::endl;
    }
}

int Socket::connectTCP(const char *ip, uint16_t port)
{
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serverAddr.sin_addr) <= 0)
    {
        std::cerr << "Erreur: Adresse IP invalide " << ip << std::endl;
        return SOCKET_ERROR;
    }

    return connect(mSocket, (sockaddr *)&serverAddr, sizeof(serverAddr));
}

int Socket::listenTCP(uint16_t port)
{
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(mSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Erreur: Bind du socket �chou� !" << std::endl;
        return SOCKET_ERROR;
    }

    return listen(mSocket, SOMAXCONN);
}

bool Socket::acceptTCP(Socket &outSocket)
{
    sockaddr_in clientAddr;
    int addrSize = sizeof(clientAddr);

    SOCKET clientSocket = accept(mSocket, (sockaddr *)&clientAddr, &addrSize);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Erreur: �chec de l'acceptation de la connexion" << std::endl;
        return false;
    }

    outSocket.mSocket = clientSocket;
    return true;
}

int Socket::bindUDP(uint16_t port)
{
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    return bind(mSocket, (sockaddr *)&serverAddr, sizeof(serverAddr));
}


bool Socket::isValid() const
{
    return mSocket != INVALID_SOCKET;
}

void Socket::closeSocket()
{
    if (isValid())
    {
        closesocket(mSocket);
        mSocket = INVALID_SOCKET;
    }
}
