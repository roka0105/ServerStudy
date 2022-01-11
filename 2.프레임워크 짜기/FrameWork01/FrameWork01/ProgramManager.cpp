#include "ProgramManager.h"
void ProgramManager::MainThread()
{

	if (!NetWork_Manager.Init())//네트워크 초기화
		return;
	NetWork_Manager.Start();//프로그램 시작.
	while (1)
	{
		SOCKET sock;
		SOCKADDR_IN sockaddr;
		ZeroMemory(&sockaddr, sizeof(SOCKADDR_IN));
		sock = NetWork_Manager.UpDate(sockaddr);
		//클라이언트 매니저에 NetWorkManager.GetClientSocket() 넣기.//클라 매니저에서 클라들 정보 관리.
		ClientInfo* client = Client_Manager.AddClient(sock, sockaddr);
		//프로그램 매니저에서 CreateThread함수 구현해서 만들기 클라이언트 매개변수 넘기는 곳에 클라이언트 매니저에서 GetClient함수 쓰기
		Program_mem.This = this;
		Program_mem.client = client;
		ProgramManager::ClientThread();
	}
	NetWork_Manager.End();//마무리작업.
}
void ProgramManager::ClientThread()
{
	HANDLE hThread;
	hThread = CreateThread(0, NULL, Client_Thread, &Program_mem, 0, NULL);
	if (hThread == NULL)
		Client_Manager.RemoveClient(Program_mem.client);
	else CloseHandle(hThread);
}
DWORD CALLBACK ProgramManager::Client_Thread(LPVOID arg)
{
	ProgramMember* m_Program = (ProgramMember*)arg;
}