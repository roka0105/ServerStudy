#include "NetWorkProgram.h"
NetWorkProgram* NetWorkProgram::instance = nullptr;
NetWorkProgram* NetWorkProgram::Instance()
{
	return instance;
}
void NetWorkProgram::Create()
{
	if (instance == NULL)
	{
		instance = new NetWorkProgram();
	}
}
void NetWorkProgram::Destroy()
{
	delete instance;
}
void NetWorkProgram::Init()
{
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
		return;

}
ClientInfo* NetWorkProgram::Connect()
{
	ClientInfo* Client;
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVERPORT);
	addr.sin_addr.s_addr = inet_addr(SERVERIP);

	int retval = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (retval == SOCKET_ERROR)
		Client->err_quit("connect()");

	Client = new ClientInfo(sock, addr);
	return Client;
}
void NetWorkProgram::End()
{
	WSACleanup();
}
//DWORD WINAPI NetWorkProgram::SendThread(LPVOID arg)
//{
//	ClientInfo* client = (ClientInfo*)arg;
//	bool endflag = false;
//	while (1)
//	{
//		if (FuntionManager::Instance()->Is_EndProgram())
//		{
//			return 0;
//		}
//		if (!client->sendbuf.is_empty())
//		{
//			client->Send();
//		}
//	}
//	return 0;
//}
DWORD WINAPI NetWorkProgram::RecvThread(LPVOID arg)
{
	ClientInfo* client = (ClientInfo*)arg;
	bool endflag = false;
	while (1)
	{
		if (FuntionManager::Instance()->Is_EndProgram())
		{
			return 0;
		}
		if (client->recvbuf.is_empty())
		{
			if (!client->Recv())
			{
				FuntionManager::Instance()->Set_IsEndProgram(true);
			}
		}

	}
	return 0;
}
NetWorkProgram::NetWorkProgram()
{

}
NetWorkProgram::~NetWorkProgram()
{

}