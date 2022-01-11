#pragma once
#include "global.h"
class ClientInfo:public NetworkSocket
{
public:
	ClientInfo();
	ClientInfo(SOCKET clientsock,SOCKADDR_IN clientaddr);
	ClientInfo(const ClientInfo& ref);
	~ClientInfo();
};

