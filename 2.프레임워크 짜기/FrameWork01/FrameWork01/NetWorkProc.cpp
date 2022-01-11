#include "NetWorkProc.h"
bool NetWorkProc::Init()
{   
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
		return false;
	Listen._Socket(AF_INET, SOCK_STREAM);
	Listen.Bind(AF_INET, INADDR_ANY, SERVERPORT);
	Listen.Listen();
	return true;
}
void NetWorkProc::Start()
{
	InitializeCriticalSection(&cs);
}
void NetWorkProc::UpDate()//여기서 메세지 들어오는지 체크해서 메세지 처리까지하기.
{
	ClientSocket = Listen.Accept();
}
void NetWorkProc::End()
{
	DeleteCriticalSection(&cs);
	closesocket(Listen.Sock()->GetSock());
	WSACleanup();
}
NetworkSocket* NetWorkProc::GetClientSock()
{
	if (ClientSocket != nullptr)
		return ClientSocket;
}