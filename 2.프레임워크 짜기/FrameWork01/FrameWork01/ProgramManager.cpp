#include "ProgramManager.h"
void ProgramManager::MainThread()
{
	if (!NetWork_Manager.Init())//��Ʈ��ũ �ʱ�ȭ
		return;
	NetWork_Manager.Start();//���α׷� ����.
	while (1)
	{
		NetWork_Manager.UpDate();
		//Ŭ���̾�Ʈ �Ŵ����� NetWorkManager.GetClientSocket() �ֱ�.//Ŭ�� �Ŵ������� Ŭ��� ���� ����.
		Client_Manager.AddClient(NetWork_Manager.GetClientSock());
		//���α׷� �Ŵ������� CreateThread�Լ� �����ؼ� ����� Ŭ���̾�Ʈ �Ű����� �ѱ�� ���� Ŭ���̾�Ʈ �Ŵ������� GetClient�Լ� ����
		ProgramManager::ClientThread(Client_Manager.GetClient());
	}
	NetWork_Manager.End();//�������۾�.
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