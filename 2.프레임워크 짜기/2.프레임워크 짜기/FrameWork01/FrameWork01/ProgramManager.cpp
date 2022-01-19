#include "ProgramManager.h"
ProgramManager* ProgramManager::instance = nullptr;
ProgramManager* ProgramManager::Instance()
{
	if (instance == NULL)
	{
		instance = new ProgramManager();
	}
	NetWorkProc::Create();
	ClientManager::Create();
	SystemFuntion::Create();
	return instance;
}
ProgramManager::ProgramManager()
{
	CALL_Program.insert({ STATE::MAIN, &SystemFuntion::MainMenu });
	CALL_Program.insert({ STATE::LOGIN,&SystemFuntion::LoginMenu });
	CALL_Program.insert({ STATE::JOIN, &SystemFuntion::JoinMenu });
	
}
ProgramManager::~ProgramManager()
{

}
void ProgramManager::MainThread()
{
	if (!NetWorkProc::Instance()->Init())//��Ʈ��ũ �ʱ�ȭ
		return;
	NetWorkProc::Instance()->Start();//���α׷� ����.
	while (1)
	{
		SOCKET sock;
		SOCKADDR_IN sockaddr;
		ZeroMemory(&sockaddr, sizeof(SOCKADDR_IN));
		sock = NetWorkProc::Instance()->UpDate(sockaddr);
		//Ŭ���̾�Ʈ �Ŵ����� NetWorkManager.GetClientSocket() �ֱ�.//Ŭ�� �Ŵ������� Ŭ��� ���� ����.
		ClientInfo* client = ClientManager::Instance()->AddClient(sock, sockaddr);
		//���α׷� �Ŵ������� CreateThread�Լ� �����ؼ� ����� Ŭ���̾�Ʈ �Ű����� �ѱ�� ���� Ŭ���̾�Ʈ �Ŵ������� GetClient�Լ� ����
		ProgramMember* Program_mem = new ProgramMember();
		Program_mem->This = this;
		Program_mem->client = client;
		ProgramManager::ClientThread(Program_mem);
	}
	NetWorkProc::Instance()->End();//�������۾�.
}
void ProgramManager::ClientThread(ProgramMember* member)
{
	HANDLE hThread;
	hThread = CreateThread(0, NULL, Client_Thread, member, 0, NULL);
	if (hThread == NULL)
		ClientManager::Instance()->RemoveClient(member->client);
	else CloseHandle(hThread);
}
DWORD CALLBACK ProgramManager::Client_Thread(LPVOID arg)
{
	ProgramMember* m_Program = (ProgramMember*)arg;
	ProgramManager* Program_manager = m_Program->This;
	ClientInfo* client = m_Program->client;
	SystemFuntion* menu_funtion = SystemFuntion::Instance();
	delete m_Program;
	
	STATE state = STATE::MAIN;
	while (1)
	{
		(menu_funtion->*(Program_manager->CALL_Program[state]))(client,state);
		return 0;
	}
}
