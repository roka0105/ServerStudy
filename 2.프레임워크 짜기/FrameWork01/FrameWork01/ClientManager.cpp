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
void ClientManager::AddClient(NetworkSocket* client_sock)
{
	ClientInfo* _client = new ClientInfo(client_sock);
	Client[ClientCount] = _client;
	++ClientCount;
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
ClientInfo* ClientManager::GetClient()
{
	return Client[ClientCount - 1];
}