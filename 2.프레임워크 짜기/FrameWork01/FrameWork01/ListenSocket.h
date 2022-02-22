#pragma once
#include "global.h"
class ListenSocket
{
public:
	ListenSocket();
	~ListenSocket();
//protected:
	void _Socket(int family, int streamtype);
	void Bind(int family, int ipaddr, int port);
	void Listen();
	SOCKET Accept(SOCKADDR_IN& output_addr);
	void err_quit(const char* msg);
	void err_display(const char* msg);
	SOCKADDR_IN GetAddr();
protected:
	SOCKET sock;
	SOCKADDR_IN addr;
};

