#pragma once
#include "Socket.h"
class NetworkSocket
{
public:
	NetworkSocket();
	NetworkSocket(const NetworkSocket& ref);
	~NetworkSocket();
	Socket* Sock();
	void Send(NetworkBuffer sendbuf);
	void Recv(NetworkBuffer recvbuf);
	/*SOCKET GetSocket();
	void SetSocket();
	SOCKADDR_IN GetAddr();
	void SetAddr();*/

private:
	Socket* sock;
};

