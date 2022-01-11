#pragma once
#include "Socket.h"
class NetworkSocket:public Socket
{
protected:
	NetworkBuffer sendbuf;
	NetworkBuffer recvbuf;
public:
	NetworkSocket();
	~NetworkSocket();
	void Send(PROTOCOL protocol, char* data, int size);
	bool Recv(PROTOCOL& protocol, char& data);
private:
	int recvn(char* buf, int len, bool flag);
};

