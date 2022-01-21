#pragma once
#include "Socket.h"
class ListenSocket:public Socket
{
public:
	ListenSocket();
	~ListenSocket();
	void _Socket(int family, int streamtype);
	void Bind(int family, int ipaddr, int port);
	void Listen();
	SOCKET Accept(SOCKADDR_IN& output_addr);

};

