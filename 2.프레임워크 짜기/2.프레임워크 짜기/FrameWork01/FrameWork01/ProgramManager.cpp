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
	if (!NetWorkProc::Instance()->Init())//네트워크 초기화
		return;
	NetWorkProc::Instance()->Start();//프로그램 시작.
	while (1)
	{
		SOCKET sock;
		SOCKADDR_IN sockaddr;
		ZeroMemory(&sockaddr, sizeof(SOCKADDR_IN));
		sock = NetWorkProc::Instance()->UpDate(sockaddr);
		//클라이언트 매니저에 NetWorkManager.GetClientSocket() 넣기.//클라 매니저에서 클라들 정보 관리.
		ClientInfo* client = ClientManager::Instance()->AddClient(sock, sockaddr);
		//프로그램 매니저에서 CreateThread함수 구현해서 만들기 클라이언트 매개변수 넘기는 곳에 클라이언트 매니저에서 GetClient함수 쓰기
		ProgramMember* Program_mem = new ProgramMember();
		Program_mem->This = this;
		Program_mem->client = client;
		ProgramManager::ClientThread(Program_mem);
	}
	NetWorkProc::Instance()->End();//마무리작업.
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
