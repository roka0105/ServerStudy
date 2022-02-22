#pragma once
#include "global.h"
class NetworkSocket
{
public:
	NetworkSocket();
	virtual ~NetworkSocket();
protected:
	void Send(const char* databuf, int size);
	bool Recv(char* databuf, int& size);
	void err_quit(const char* msg);
	void err_display(const char* msg);
	SOCKADDR_IN GetAddr();
private:
	int recvn(char* buf, int len, bool flag);
protected:
	char buf[MAXBUF];
	int bufsize;
	SOCKET sock;
	SOCKADDR_IN addr;
};
