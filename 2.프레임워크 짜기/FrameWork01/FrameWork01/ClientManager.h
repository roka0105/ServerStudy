#pragma once
#include "ClientInfo.h"
class ClientManager
{
private:
	static ClientManager* instance;
	
	ClientInfo* Client[MAXPERSONNEL];
	int ClientCount;
protected:
	ClientManager();
	~ClientManager();
	
public:
	static ClientManager* Instance();
	static void Create();
	static void Destroy();
	void Init();
	void End();
	ClientInfo* AddClient(SOCKET client_sock,SOCKADDR_IN client_addr);
	void RemoveClient(ClientInfo* client);
};

