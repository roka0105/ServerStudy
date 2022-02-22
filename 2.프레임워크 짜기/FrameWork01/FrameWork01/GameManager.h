#pragma once
#include "ClientInfo.h"
#include "_List.h"
#define MAXPLAYER 2
#define MINROOM 4
struct Room
{
	int number;
	int clientcount;
	ClientInfo* clientlist[MAXPLAYER];
	Room(int num)
	{
		number = num;
		for (int i = 0; i < MAXPLAYER; i++)
			clientlist[i] = nullptr;
	}
};
class GameManager
{
private:
	static GameManager* instance;
	_List<Room*> roomList;
	int roomcount;
	int currentRoom;
public:
	static GameManager* Instance();
	static void Create();
	static void Destroy();
	void Init();
	void End();
	void LauncharProgram(ClientInfo* client, STATE& state);
	bool GameRun(ClientInfo* client);
	void AssignRoom(ClientInfo* client);
	void WaitCheck(ClientInfo* client);
private: 
	GameManager();
	~GameManager();
};

