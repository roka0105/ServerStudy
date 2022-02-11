#include "MainManager.h"
MainManager* MainManager::instance = nullptr;
MainManager* MainManager::Instance()
{
	return instance;
}
void MainManager::Create()
{
	if (instance == NULL)
	{
		instance = new MainManager();
	}
	NetWorkProc::Create();
	ClientManager::Create();
	SystemFuntion::Create();
}
void MainManager::Destroy()
{
	NetWorkProc::Destroy();
	ClientManager::Destroy();
	SystemFuntion::Destroy();
	delete instance;
}
MainManager::MainManager()
{
	CALL_Program.insert({ STATE::MAIN, &SystemFuntion::MainMenu });
	CALL_Program.insert({ STATE::LOGIN,&SystemFuntion::LoginMenu });
	CALL_Program.insert({ STATE::JOIN, &SystemFuntion::JoinMenu });
	CALL_Program.insert({ STATE::END,&SystemFuntion::EndMenu });
}
MainManager::~MainManager()
{

}
void MainManager::Init()
{
	if (!NetWorkProc::Instance()->Init())
		return;
	SystemFuntion::Instance()->Init();
}
void* MainManager::AcceptClient()
{
	SOCKET sock;
	SOCKADDR_IN sockaddr;
	ZeroMemory(&sockaddr, sizeof(SOCKADDR_IN));
	sock = NetWorkProc::Instance()->UpDate(sockaddr);
	ClientInfo* client = ClientManager::Instance()->AddClient(sock, sockaddr);
	return client;
}
void MainManager::Run(void* main_member)
{
	MainMember* mainptr = (MainMember*)main_member;
	ClientInfo* client = (ClientInfo*)mainptr->client;
	MainManager* Program_manager = (MainManager*)mainptr->This;
	
	SystemFuntion* menu_funtion = SystemFuntion::Instance();
	delete mainptr;

	STATE state = STATE::MAIN;
	while (1)
	{
		//�����ڵ� ������ �� ����������� �ٽ� �����ϱ�. �ϴ� �ӽ÷� ���.
#pragma region End_Thread
		if (state == STATE::END)
		{
			LoginManager::Instance()->LogOut(false, client);
			(menu_funtion->*(Program_manager->CALL_Program[state]))(client, state);
			//cout << "���α׷� ����" << endl;
			ClientManager::Instance()->RemoveClient(client);
			return;
		}
#pragma endregion 
		(menu_funtion->*(Program_manager->CALL_Program[state]))(client, state);
	}
}
void MainManager::Err(HANDLE handle,void*main_member)
{
	MainMember* main_ptr = (MainMember*)main_member;
	ClientInfo* client = (ClientInfo*)main_ptr->client;
	if (handle == NULL)
		ClientManager::Instance()->RemoveClient(client);
	else CloseHandle(handle);
}
void MainManager::End()
{
	LoginManager::Instance()->FileSave();
	NetWorkProc::Instance()->End();//�������۾�.
	ClientManager::Instance()->End();
	SystemFuntion::Instance()->End();
}

