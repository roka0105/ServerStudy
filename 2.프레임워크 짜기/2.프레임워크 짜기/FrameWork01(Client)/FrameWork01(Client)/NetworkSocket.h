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
	void Send(const char* sendbuf,int size);
	bool Recv();
private:
	int recvn(char* buf, int len, bool flag);
};
