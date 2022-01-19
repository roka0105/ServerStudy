#pragma once
#include "NetworkSocket.h"
class ClientInfo:public NetworkSocket
{
public:
	ClientInfo();
	ClientInfo(SOCKET clientsock,SOCKADDR_IN clientaddr);
	ClientInfo(ClientInfo& ref);
	~ClientInfo();
};

