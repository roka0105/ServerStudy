#pragma once
#include "global.h"
#include "ClientInfo.h"
class MenuManager
{
private:
	static MenuManager* instance;
public:
	static MenuManager* Instance();
	static void Create();
	static void Destroy();
	void Init();
	void End();
	void MainProgram(ClientInfo* _client, STATE& _state);
	void EndProgram(ClientInfo* _client, STATE& _state);
private:
	void UnPackPacket(const char* recvbuf,int& menu_number);
};

