template<typename T>
bool network::sendPacketTCP(Socket& s, uint32_t packetID, const T& data)
{
	static_assert(sizeof(T) <= MAX_PACKET_SIZE - 4);

	char buf[MAX_PACKET_SIZE];
	*((uint32_t*)buf) = packetID;
	memcpy(buf + 4, &data, sizeof(T));

	size_t bytesSent = sendPacketTCP(s, buf, sizeof(T) + 4);
	if (bytesSent == 0) {
		std::cerr << "sendPacketTCP() failed! WSA Error: " << WSAGetLastError() << std::endl;
		return false;
	}

	return true;
}

template<typename T>
bool network::sendPacketUDP(Socket& s, const sockaddr* addr, uint32_t packetID, const T& data)
{
	static_assert(sizeof(T) <= MAX_PACKET_SIZE - 4);

	char buf[MAX_PACKET_SIZE];
	*reinterpret_cast<uint32_t*>(buf) = packetID;
	memcpy(buf + 4, &data, sizeof(T));

	return sendto(s.mSocket, buf, sizeof(T) + 4, 0, addr, sizeof(sockaddr_in)) != SOCKET_ERROR;
}

template<typename T>
bool network::receivePacketTCP(Socket& s, T& data) {
	int receivedBytes = recv(s.mSocket, reinterpret_cast<char*>(&data), sizeof(T), 0);

	if (receivedBytes == SOCKET_ERROR) {
		std::cerr << "Error receiving packet TCP! WSA Error: " << WSAGetLastError() << std::endl;
		return false;
	}

	return receivedBytes == sizeof(T);
}


template<typename T>
bool network::receivePacketUDP(Socket& s, sockaddr_in* senderAddr, T& data) {
	char buffer[MAX_PACKET_SIZE];
	int senderAddrSize = sizeof(sockaddr_in);

	int bytesReceived = recvfrom(s.mSocket, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(senderAddr), &senderAddrSize);

	if (bytesReceived == SOCKET_ERROR) {
		return false;
	}

	if (bytesReceived < sizeof(T)) {
		return false;
	}

	memcpy(&data, buffer, sizeof(T));
	return true;
}
