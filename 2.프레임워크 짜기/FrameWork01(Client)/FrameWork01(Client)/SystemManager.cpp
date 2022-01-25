#include "SystemManager.h"
SystemManager* SystemManager::instance = nullptr;
SystemManager* SystemManager::Instance()
{
	return instance;
}
void SystemManager::Create()
{
	instance = new SystemManager();
	NetWorkProgram::Create();
	FuntionManager::Create();
}
void SystemManager::Destroy()
{
	FuntionManager::Destroy();
	NetWorkProgram::Destroy();
	delete instance;
}
void SystemManager::Init(HINSTANCE hInstance)
{
	hInst = hInstance;
	NetWorkProgram::Instance()->Init();
	Client= NetWorkProgram::Instance()->Connect();
	
	ProtocolMapInit();
	//각종 이벤트 초기화
}
void SystemManager::ProtocolMapInit()
{
	protocolFunction[PROTOCOL::MENU_SELECT] = &FuntionManager::MenuSelect;
	protocolFunction[PROTOCOL::MENU_RESULT] = &FuntionManager::MenuResult;
	protocolFunction[PROTOCOL::ENDPROGRAM] = &FuntionManager::MenuEnd;
	protocolFunction[PROTOCOL::LOGININFO] = &FuntionManager::LoginInfo;
	protocolFunction[PROTOCOL::LOGINRESULT] = &FuntionManager::LoginResult;
	protocolFunction[PROTOCOL::JOININFO] = &FuntionManager::JoinInfo;
	protocolFunction[PROTOCOL::JOINRESULT] = &FuntionManager::JoinResult;
}
void SystemManager::Start()
{
	//hThread[0] = CreateThread(NULL, 0, NetWorkProgram::SendThread, Client, 0, NULL);
	hThread[0] = CreateThread(NULL, 0, NetWorkProgram::NetworkThread, Client, 0, NULL);
	PROTOCOL protocol;
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	while (1)
	{
		if (FuntionManager::Instance()->Is_EndProgram())
		{
			MessageBox(NULL, "클라이언트 스레드 정상 종료", "종료", MB_OK);
			return;
		}
		if (!Client->recvbuf.is_empty())
		{
			Client->recvbuf.UnPackPacket(protocol);
			//함수포인터
			(FuntionManager::Instance()->*protocolFunction[protocol])
				(hInst,Client);
		}
	}
}
void SystemManager::End()
{
	//핸들들 반환

	NetWorkProgram::Instance()->End();
	return;
}
SystemManager::SystemManager()
{

}
SystemManager::~SystemManager()
{

}