#include "ClientManager.h"
ClientManager::ClientManager()
{
	ClientCount = 0;
}
ClientManager::~ClientManager()
{
	for (ClientInfo* client : Client)
	{
		delete client;
		client = NULL;
	}
}
ClientInfo* ClientManager::AddClient(SOCKET client_sock,SOCKADDR_IN client_addr)
{
	ClientInfo* _client = new ClientInfo(client_sock,client_addr);
	Client[ClientCount] = _client;
	++ClientCount;
	return Client[ClientCount - 1];
}
void ClientManager::RemoveClient(ClientInfo* client)
{
	for (int i=0;i<ClientCount;++i)
	{
		if (Client[i] == client)
		{
			delete client;
			Client[i] = NULL;
			for (int j = i; j < ClientCount - 1; ++j)
			{
				Client[j] = Client[j + 1];
			}
			--ClientCount;
			break;
		}
	}
}
