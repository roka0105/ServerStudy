#include "GameManager.h"
GameManager* GameManager::instance = nullptr;

GameManager* GameManager::Instance()
{
	return instance;
}
void GameManager::Create()
{
	if (instance == nullptr)
	{
		instance = new GameManager();
	}
}
void GameManager::Destroy()
{
	delete instance;
}
void GameManager::Init()
{
	for (int i = 0; i < MINROOM; ++i)
	{
		roomList.Push_back(new Room(i));
		++roomcount;
	}
}
void GameManager::End()
{
	for (int i = 0; i < roomcount; ++i)
	{
		delete roomList.Pop_front();
	}
}
void GameManager::LauncharProgram(ClientInfo* client, STATE& state)
{
	client->Send(PROTOCOL::LAUNCHAR);
	PROTOCOL protocol;
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	//���ӽ��� ��ư ������ ����� �����ϱ�.
	if (!client->Recv(protocol, buf))
	{
		state = STATE::END;
		return;
	}
	//�� �����ֱ�
	AssignRoom(client);
	//�濡 �ο����� ��á���� üũ á���� ���� run �ƴϸ� wait
	WaitCheck(client);
	while (1)
	{
		if (GameRun(client))
			break;
	}
	state = STATE::MAIN;
}
bool GameManager::GameRun(ClientInfo* client)
{
	ClientInfo* client1 = client;
	ClientInfo* client2;
	for (int i = 0; i < MAXPLAYER; ++i)
	{
		if (roomList[currentRoom]->clientlist[i] != client1)
		{
			client2=roomList[currentRoom]->clientlist[i];
		}
	}
	//�÷��̾���� ���� �� Ŭ�󿡰� ����
	//�÷��̾���� �Է����� ���� ���� �� �޾ƿ���(hp,��ǥ��)
	//�� Ŭ���� hp�� 0�� �Ǹ� ���� ���� 
	return true;
}
void GameManager::AssignRoom(ClientInfo* client)
{
	for (int i = 0; i < roomcount; ++i)
	{
		int* count = &roomList[i]->clientcount;
		if (*count != MAXPLAYER)
		{
			roomList[i]->clientlist[*count] = client;
			currentRoom = i;
			++* count;
			return;
		}
	}
	//����� ���ٸ�
	roomList.Push_back(new Room(roomcount));
	int* count2 = &roomList[roomcount]->clientcount;
	roomList[roomcount]->clientlist[*count2] = client;
	currentRoom = roomcount;
	++*count2;
	++roomcount;
}
void GameManager::WaitCheck(ClientInfo* client)
{
	if (roomList[currentRoom]->clientcount == MAXPLAYER)
	{
		client->_SetEvent();
		ClientInfo* client2 = roomList[currentRoom]->clientlist[0];
		client2->_SetEvent();
	}
	WaitForSingleObject(client->_GetEvent(), INFINITY);
}
GameManager::GameManager()
{

}
GameManager::~GameManager()
{

}
