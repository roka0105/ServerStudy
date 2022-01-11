#include "ProgramManager.h"
void ProgramManager::MainThread()
{
	if (!NetWork_Manager.Init())//네트워크 초기화
		return;
	NetWork_Manager.Start();//프로그램 시작.
	while (1)
	{
		NetWork_Manager.UpDate();
		//클라이언트 매니저에 NetWorkManager.GetClientSocket() 넣기.//클라 매니저에서 클라들 정보 관리.
		Client_Manager.AddClient(NetWork_Manager.GetClientSock());
		//프로그램 매니저에서 CreateThread함수 구현해서 만들기 클라이언트 매개변수 넘기는 곳에 클라이언트 매니저에서 GetClient함수 쓰기
		ProgramManager::ClientThread(Client_Manager.GetClient());
	}
	NetWork_Manager.End();//마무리작업.
}
void ProgramManager::ClientThread(ClientInfo* _client)
{
	HANDLE hThread;
	hThread = CreateThread(0, NULL,Client_Thread, _client, 0, NULL);
	if (hThread == NULL)
	Client_Manager.RemoveClient(_client);
	else CloseHandle(hThread);
}
DWORD CALLBACK Client_Thread(LPVOID arg)
{

}