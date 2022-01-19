#include "MainManager.h"
MainManager* MainManager::instance = nullptr;
MainManager* MainManager::Instance()
{
	return instance;
}
void MainManager::Create()
{
		instance = new MainManager();
}
void MainManager::Destroy()
{

}
void MainManager::MainProgram(ClientInfo* _client, STATE& _state)
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
	//��� üũ �� send
	//state�� �ش��ϴ°����� �ٲٱ�.
	/*
	switch ((MENUNUMBER)menunumber)
	{
	case MENUNUMBER::LOGIN:
		c->state = STATE::LOGIN;
		break;
	case MENUNUMBER::JOIN:
		c->state = STATE::JOIN;
		break;
	case MENUNUMBER::LIST:
		c->state = STATE::ROOMLIST;
		break;
	case MENUNUMBER::END:
		c->state = STATE::END;
		break;
	}*/
}
void MainManager::UnPackPacket(const char* recvbuf,int& menu_number)
{

}
void MainManager::PackPacket(char* sendbuf, char* data)
{
	
}