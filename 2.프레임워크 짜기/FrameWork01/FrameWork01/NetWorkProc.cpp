#include "NetWorkProc.h"
bool NetWorkProc::Init()
{   
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
		return false;
	Listen._Socket(AF_INET, SOCK_STREAM);
	Listen.Bind(AF_INET, INADDR_ANY, SERVERPORT);
	Listen.Listen();
}
void NetWorkProc::Start()
{
	InitializeCriticalSection(&cs);
	while (1)
	{
		UpDate();
	}
	DeleteCriticalSection(&cs);
}
void NetWorkProc::UpDate()//���⼭ �޼��� �������� üũ�ؼ� �޼��� ó�������ϱ�.
{
	ClientSocket = Listen.Accept();
	
	//Ŭ���̾�Ʈ ������ ����
}
void NetWorkProc::End()
{

}