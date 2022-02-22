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
	//게임시작 버튼 누르면 여기로 오게하기.
	if (!client->Recv(protocol, buf))
	{
		state = STATE::END;
		return;
	}
	//방 정해주기
	AssignRoom(client);
	//방에 인원수가 꽉찼는지 체크 찼으면 게임 run 아니면 wait
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
	//플레이어들의 정보 두 클라에게 전송
	//플레이어들의 입력으로 인해 변한 값 받아오기(hp,좌표값)
	//한 클라의 hp가 0이 되면 게임 종료 
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
	//빈방이 없다면
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
