#pragma once
#include "Socket.h"
class ListenSocket
{
public:
	ListenSocket();
	~ListenSocket();
	void _Socket(int family, int streamtype);
	void Bind(int family, int ipaddr, int port);
	void Listen();
	Socket* Sock();
	NetworkSocket* Accept();
private:
	Socket* sock;
};

