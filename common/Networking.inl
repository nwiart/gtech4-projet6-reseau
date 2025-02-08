template<typename T>
bool network::sendPacketTCP(Socket& s, uint32_t packetID, const T& data)
{
	static_assert(sizeof(T) <= MAX_PACKET_SIZE - 4);

	char buf[MAX_PACKET_SIZE];
	uint32_t packetIDLE = htonl(packetID);
	memcpy(buf, &packetIDLE, sizeof(packetIDLE));
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
	uint32_t packetIDLE = htonl(packetID);
	memcpy(buf, &packetIDLE, sizeof(packetIDLE));
	memcpy(buf + 4, &data, sizeof(T));

	return sendto(s.mSocket, buf, sizeof(T) + 4, 0, addr, sizeof(sockaddr_in)) != SOCKET_ERROR;
}

template<typename T>
bool network::receivePacketTCP(Socket& s, uint32_t& packetID, T& data) {
	char buffer[MAX_PACKET_SIZE];
	int receivedBytes = recv(s.mSocket, buffer, sizeof(buffer), 0);

	if (receivedBytes == SOCKET_ERROR) {
		std::cerr << "Error receiving packet TCP! WSA Error: " << WSAGetLastError() << std::endl;
		return false;
	}

	if (receivedBytes < sizeof(uint32_t)) {
		std::cerr << "Invalid packet received (too small)\n";
		return false;
	}

	memcpy(&packetID, buffer, sizeof(uint32_t));
	packetID = ntohl(packetID); // Convert back to host byte order

	if (receivedBytes < sizeof(uint32_t) + sizeof(T)) {
		std::cerr << "Incomplete packet data received\n";
		return false;
	}

	memcpy(&data, buffer + sizeof(uint32_t), sizeof(T));

	return true;
}


template<typename T>
bool network::receivePacketUDP(Socket& s, sockaddr_in* senderAddr, T& data) {
	char buffer[MAX_PACKET_SIZE];
	int senderAddrSize = sizeof(sockaddr_in);

	int bytesReceived = recvfrom(s.mSocket, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(senderAddr), &senderAddrSize);

	if (bytesReceived < sizeof(uint32_t)) {
		std::cerr << "Invalid UDP packet received (too small)\n";
		return false;
	}

	uint32_t packetID;
	memcpy(&packetID, buffer, sizeof(uint32_t));
	packetID = ntohl(packetID);

	if (bytesReceived < sizeof(uint32_t) + sizeof(T)) {
		std::cerr << "Incomplete UDP packet data received\n";
		return false;
	}

	memcpy(&data, buffer + sizeof(uint32_t), sizeof(T));
	return true;
}
