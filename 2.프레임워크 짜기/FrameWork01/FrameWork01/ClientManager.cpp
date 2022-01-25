#include "ClientManager.h"
ClientManager* ClientManager::instance = nullptr;
ClientManager*ClientManager::Instance()
{
	
	return instance;
}
void ClientManager::Create()
{
	if (instance == NULL)
	{
		instance = new ClientManager();
	}
}
void ClientManager::Destroy()
{
	delete instance;
}
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
void ClientManager::Init()
{

}
void ClientManager::End()
{

}
ClientInfo* ClientManager::AddClient(SOCKET client_sock,SOCKADDR_IN client_addr)
{
	cout << "[TCP서버 접속] IP:" << inet_ntoa(client_addr.sin_addr) << " " << "PORT:" << ntohs(client_addr.sin_port)<< endl;
	ClientInfo* _client = new ClientInfo(client_sock,client_addr);
	Client[ClientCount] = _client;
	++ClientCount;
	return Client[ClientCount - 1];
}
void ClientManager::RemoveClient(ClientInfo* client)
{
	cout << "[TCP서버 종료] IP:" << inet_ntoa(client->GetAddr().sin_addr) << " " << "PORT:" << ntohs(client->GetAddr().sin_port)<<endl;
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
