#pragma once
#include "Socket.h"
class NetworkSocket
{
public:
	NetworkSocket();
	NetworkSocket(const NetworkSocket& ref);
	~NetworkSocket();
	Socket* Sock();
	void Send(NetworkBuffer _sendbuf);
	bool Recv(NetworkBuffer _recvbuf);

	//PROTOCOL GetProtocol();
	//void PackPacket(PROTOCOL protocol, const char* result);
	//void UnPackPacket(PROTOCOL protocol, char* id, char* pw);
	
private:
	int recvn(char* buf, int len, bool flag);
	Socket* sock;
};

