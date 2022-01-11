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
	void AddClient(NetworkSocket* client_sock);
	void RemoveClient(ClientInfo* client);
	ClientInfo* GetClient();
};

