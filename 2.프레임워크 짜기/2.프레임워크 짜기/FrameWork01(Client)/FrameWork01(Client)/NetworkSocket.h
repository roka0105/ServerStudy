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
	void Send();
	bool Recv();
private:
	int recvn(char* buf, int len, bool flag);
};
