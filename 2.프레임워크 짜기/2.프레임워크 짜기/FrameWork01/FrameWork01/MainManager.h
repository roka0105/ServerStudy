#pragma once
#include "global.h"
#include "ClientInfo.h"
class MainManager
{
private:
	static MainManager* instance;
public:
	static MainManager* Instance();
	static void Create();
	static void Destroy();
	void MainProgram(ClientInfo* _client, STATE& _state);
private:
	void UnPackPacket(const char* recvbuf,int& menu_number);
	void PackPacket(char* sendbuf, char* data);
};

