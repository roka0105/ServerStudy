#include "SystemManager.h"

void SystemManager::Init(HINSTANCE hInstance)
{
	hInst = hInstance;
	Network = NetWorkProgram::Instance();
	Network->Init();
	Client=Network->Connect();
	//���� �̺�Ʈ �ʱ�ȭ
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
			//�Լ�������
			(Funcmanager->*protocolFunction[protocol])
				(hInst,Client);
		}
	}
}
void SystemManager::End()
{
	//�ڵ�� ��ȯ

	Network->End();
	return;
}
