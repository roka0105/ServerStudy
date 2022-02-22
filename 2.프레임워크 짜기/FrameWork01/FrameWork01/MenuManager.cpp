#include "MenuManager.h"
MenuManager* MenuManager::instance = nullptr;
MenuManager* MenuManager::Instance()
{
	return instance;
}
void MenuManager::Create()
{
	instance = new MenuManager();
}
void MenuManager::Destroy()
{

}
void MenuManager::Init()
{

}
void MenuManager::End()
{

}
void MenuManager::MainProgram(ClientInfo* _client, STATE& _state)
{
	int menunumber = 0;
	int size = 0;
	int retval = 0;
	PROTOCOL protocol;
	char databuf[MAXBUF];
	ZeroMemory(databuf, MAXBUF);

	_client->Send(PROTOCOL::MENU_SELECT,nullptr, 0);

	if (!_client->Recv(protocol,databuf))
	{
		_state = STATE::END;
		return;
	}

	UnPackPacket(databuf, menunumber);
	//결과 체크 후 send
	//state는 해당하는것으로 바꾸기.

	switch ((MENU)menunumber)
	{
	
	case MENU::LOGIN:
		_state = STATE::LOGIN;
		break;
	case MENU::JOIN:
		_state = STATE::JOIN;
		break;
	case MENU::LOGOUT:
		_client->LogOut();
		_state = STATE::LOGIN;
		break;
	case MENU::LAUNCHERSTART:
		_state = STATE::LAUNCHERSTART;
		break;
	case MENU::END:
		_state = STATE::END;
		break;
	}
}
void MenuManager::EndProgram(ClientInfo* _client, STATE& _state)
{
	_client->Send(PROTOCOL::ENDPROGRAM,nullptr, 0);
}
void MenuManager::UnPackPacket(const char* recvbuf, int& menu_number)
{
	const char* ptr = recvbuf;
	memcpy(&menu_number, ptr, sizeof(int));
}
