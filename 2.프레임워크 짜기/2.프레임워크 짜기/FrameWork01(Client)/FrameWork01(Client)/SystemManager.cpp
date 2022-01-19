#include "SystemManager.h"

void SystemManager::Init(HINSTANCE hInstance)
{
	hInst = hInstance;
	Network = NetWorkProgram::Instance();
	Network->Init();
	Client=Network->Connect();
	//각종 이벤트 초기화
}
void SystemManager::ProtocolMapInit()
{
	Funcmanager = FuntionManager::Instance();
	protocolFunction[MENU_SELECT] = &FuntionManager::MenuSelect;
	protocolFunction[MENU_RESULT] = &FuntionManager::MenuResult;
}
void SystemManager::Start()
{
	hThread[0] = CreateThread(NULL, 0, NetWorkProgram::SendThread, Client, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, NetWorkProgram::RecvThread, Client, 0, NULL);
	PROTOCOL protocol;
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	while (1)
	{
		if (!Client->recvbuf.is_empty())
		{
			Client->recvbuf.UnPackPacket(protocol);
			//함수포인터
			(Funcmanager->*protocolFunction[protocol])
				(hInst,Client);
		}
	}
}
void SystemManager::End()
{
	//핸들들 반환

	Network->End();
	return;
}
