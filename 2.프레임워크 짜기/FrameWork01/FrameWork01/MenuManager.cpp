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
void MenuManager::MainProgram(ClientInfo* _client, STATE& _state)
{
	int menunumber = 0;
	int size = 0;
	int retval = 0;
	PROTOCOL protocol;
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);

	_client->sendbuf.MemoryZero();
	size=_client->sendbuf.PackPacket(PROTOCOL::MENU_SELECT);
	_client->Send(_client->sendbuf.Data_Pop(),size);

	_client->recvbuf.MemoryZero();
	if (!_client->Recv())
	{
		_state = STATE::EXIT;
		return;
	}
	_client->recvbuf.UnPackPacket(protocol);
	UnPackPacket(_client->recvbuf.Data_Pop(), menunumber);
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
	case MENU::END:
		_state = STATE::END;
		break;
	}
}
void MenuManager::UnPackPacket(const char* recvbuf,int& menu_number)
{
	const char* ptr = recvbuf + sizeof(PROTOCOL);
	memcpy(&menu_number, ptr, sizeof(int));
}
