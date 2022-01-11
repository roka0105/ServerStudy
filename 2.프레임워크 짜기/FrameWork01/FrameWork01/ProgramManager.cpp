#include "ProgramManager.h"
void ProgramManager::MainThread()
{

	if (!NetWork_Manager.Init())//��Ʈ��ũ �ʱ�ȭ
		return;
	NetWork_Manager.Start();//���α׷� ����.
	while (1)
	{
		SOCKET sock;
		SOCKADDR_IN sockaddr;
		ZeroMemory(&sockaddr, sizeof(SOCKADDR_IN));
		sock = NetWork_Manager.UpDate(sockaddr);
		//Ŭ���̾�Ʈ �Ŵ����� NetWorkManager.GetClientSocket() �ֱ�.//Ŭ�� �Ŵ������� Ŭ��� ���� ����.
		ClientInfo* client = Client_Manager.AddClient(sock, sockaddr);
		//���α׷� �Ŵ������� CreateThread�Լ� �����ؼ� ����� Ŭ���̾�Ʈ �Ű����� �ѱ�� ���� Ŭ���̾�Ʈ �Ŵ������� GetClient�Լ� ����
		Program_mem.This = this;
		Program_mem.client = client;
		ProgramManager::ClientThread();
	}
	NetWork_Manager.End();//�������۾�.
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