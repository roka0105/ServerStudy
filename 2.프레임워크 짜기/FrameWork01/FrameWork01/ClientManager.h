#pragma once
#include "ClientInfo.h"
class ClientManager
{
private:
	ClientInfo* Client[MAXPERSONNEL];
	int ClientCount;
public:
	ClientManager();
	~ClientManager();
	ClientInfo* AddClient(SOCKET client_sock,SOCKADDR_IN client_addr);
	void RemoveClient(ClientInfo* client);
};

