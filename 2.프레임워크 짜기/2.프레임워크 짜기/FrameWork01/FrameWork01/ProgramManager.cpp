#include "ProgramManager.h"
ProgramManager* ProgramManager::instance = nullptr;
ProgramManager* ProgramManager::Instance()
{
	if (instance == NULL)
	{
		instance = new ProgramManager();
	}
	instance->NetWork_Manager = NetWorkProc::Instance();
	instance->Client_Manager = ClientManager::Instance();
	instance->SystemFunc = SystemFuntion::Instance();
	return instance;
}
ProgramManager::ProgramManager()
{
	CALL_Program.insert({ STATE::MAIN, SystemFuntion::MainMenu });
	CALL_Program.insert({ STATE::LOGIN,SystemFuntion::LoginMenu });
	CALL_Program.insert({ STATE::JOIN, SystemFuntion::JoinMenu });
	Program_mem.This = nullptr;
	Program_mem.client = nullptr;
}
ProgramManager::~ProgramManager()
{

}
void ProgramManager::MainThread()
{
	if (!NetWork_Manager->Init())//��Ʈ��ũ �ʱ�ȭ
		return;
	NetWork_Manager->Start();//���α׷� ����.
	while (1)
	{
		SOCKET sock;
		SOCKADDR_IN sockaddr;
		ZeroMemory(&sockaddr, sizeof(SOCKADDR_IN));
		sock = NetWork_Manager->UpDate(sockaddr);
		//Ŭ���̾�Ʈ �Ŵ����� NetWorkManager.GetClientSocket() �ֱ�.//Ŭ�� �Ŵ������� Ŭ��� ���� ����.
		ClientInfo* client = Client_Manager->AddClient(sock, sockaddr);
		//���α׷� �Ŵ������� CreateThread�Լ� �����ؼ� ����� Ŭ���̾�Ʈ �Ű����� �ѱ�� ���� Ŭ���̾�Ʈ �Ŵ������� GetClient�Լ� ����
		Program_mem.This = this;
		Program_mem.client = client;
		ProgramManager::ClientThread();
	}
	NetWork_Manager->End();//�������۾�.
}
void ProgramManager::ClientThread()
{
	HANDLE hThread;
	hThread = CreateThread(0, NULL, Client_Thread, &Program_mem, 0, NULL);
	if (hThread == NULL)
		Client_Manager->RemoveClient(Program_mem.client);
	else CloseHandle(hThread);
}
DWORD CALLBACK ProgramManager::Client_Thread(LPVOID arg)
{
	ProgramMember* m_Program = (ProgramMember*)arg;
	SystemFuntion* menu_funtion = m_Program->This->SystemFunc;

	ClientInfo* client = m_Program->client;
	STATE state = STATE::MAIN;
	while (1)
	{
		(menu_funtion->*CALL_Program[state])(client,state);
		return 0;
	}
}
