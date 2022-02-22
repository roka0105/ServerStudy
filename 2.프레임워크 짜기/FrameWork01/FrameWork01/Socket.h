#pragma once
#include "global.h"
class Socket
{
//protected:
public:
	Socket();
	Socket(SOCKET _sock, SOCKADDR_IN _addr);
	Socket(const Socket& ref);
	virtual ~Socket();
	void err_quit(const char* msg);
	void err_display(const char* msg);
	SOCKADDR_IN GetAddr();

};

