#pragma once
#include "global.h"
class Socket
{
//protected:
public:
	Socket();
	Socket(SOCKET _sock, SOCKADDR_IN _addr);
	Socket(const Socket& ref);
	~Socket();
	void err_quit(const char* msg);
	void err_display(const char* msg);
	SOCKET GetSock();
	void SetSock(SOCKET s);
	SOCKADDR_IN GetAddr();
	void SetAddr(SOCKADDR_IN a);
private:
	SOCKET sock;
	SOCKADDR_IN addr;
};

