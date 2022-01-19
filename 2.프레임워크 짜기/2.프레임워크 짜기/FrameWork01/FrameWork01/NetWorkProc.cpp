#include "NetWorkProc.h"
NetWorkProc* NetWorkProc::instance = nullptr;
NetWorkProc* NetWorkProc::Instance()
{
	if (instance == NULL)
	{
		instance = new NetWorkProc();
	}
	return instance;
}
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
SOCKET NetWorkProc::UpDate(SOCKADDR_IN& output)//여기서 메세지 들어오는지 체크해서 메세지 처리까지하기.
{
	SOCKET _sock= Listen.Accept(output);
	return _sock;
}
void NetWorkProc::End()
{
	DeleteCriticalSection(&cs);
	WSACleanup();
}
