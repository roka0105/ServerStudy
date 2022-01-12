#pragma once
#include "Socket.h"
#include "NetworkBuffer.h"
class NetworkSocket:public Socket
{
public:
	NetworkBuffer sendbuf;
	NetworkBuffer recvbuf;
public:
	NetworkSocket();
	virtual ~NetworkSocket();
	void Send(PROTOCOL protocol, char* data, int size);
	bool Recv(PROTOCOL& protocol, char* data);
private:
	int recvn(char* buf, int len, bool flag);
};
