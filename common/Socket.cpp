#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Socket.h"
#include <WS2tcpip.h>

Socket::Socket() : mSocket(INVALID_SOCKET) {}

Socket::Socket(SOCKET s) : mSocket(s) {}

Socket::~Socket()
{
    closeSocket();
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

int Socket::connectTCP(const char* ip, uint16_t port) {
    if (!isValid()) {
        createSocketTCP();  // Ensure we have a valid socket
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid IP address: " << ip << std::endl;
        return SOCKET_ERROR;
    }

    if (connect(mSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connect failed to " << ip << ":" << port << ", error: " << WSAGetLastError() << std::endl;
        return SOCKET_ERROR;
    }

    return 0;
}

int Socket::listenTCP(uint16_t port) {
    if (!isValid()) {
        std::cerr << "Socket is not valid!" << std::endl;
        return SOCKET_ERROR;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(mSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Erreur: Bind du socket échoué !" << std::endl;
        return SOCKET_ERROR;
    }

    return listen(mSocket, SOMAXCONN);
}

bool Socket::acceptTCP(Socket& outSocket) {
    sockaddr_in clientAddr;
    socklen_t addrSize = sizeof(clientAddr);

    SOCKET clientSocket = accept(mSocket, (sockaddr*)&clientAddr, &addrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Erreur: échec de l'acceptation de la connexion" << std::endl;
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

int Socket::sendUDP(const void *data, int dataSize, sockaddr_in &destAddr)
{
    return sendto(mSocket, reinterpret_cast<const char *>(data), dataSize, 0,
                  (sockaddr *)&destAddr, sizeof(destAddr));
}

int Socket::receiveUDP(void *buffer, int bufferSize, sockaddr_in &senderAddr)
{
    int senderAddrSize = sizeof(senderAddr);
    return recvfrom(mSocket, reinterpret_cast<char *>(buffer), bufferSize, 0,
                    (sockaddr *)&senderAddr, &senderAddrSize);
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
