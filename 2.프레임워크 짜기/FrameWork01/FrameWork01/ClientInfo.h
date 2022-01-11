#pragma once
#include "global.h"
class ClientInfo
{
private:
	NetworkSocket* sock;
	NetworkBuffer sendbuf;
	NetworkBuffer recvbuf;
	PROTOCOL protocol;
public:
	enum Buff
	{
		send = 1,
		recv = 2
	};
	ClientInfo();
	ClientInfo(NetworkSocket* clientsock);
	ClientInfo(const ClientInfo& ref);
	~ClientInfo();
	SOCKET Get_Sock();
	void SetSendBuff(char* buf);
	void SetRecvBuff(char* buf);
	void Send();
	void Recv();
};

