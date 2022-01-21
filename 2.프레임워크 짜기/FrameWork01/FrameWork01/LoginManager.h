#pragma once
#include "global.h"
#include "ClientInfo.h"
class LoginManager
{
private:
	static LoginManager* instance;
public:
	static LoginManager* Instance();
	static void Create();
	static void Destroy();
	void LoginProgram(ClientInfo* _client, STATE& _state);
	void JoinProgram(ClientInfo* _client, STATE& _state);
	bool Is_Loging(ClientInfo* _client,STATE& _state);
	bool LoginCheck();
	bool JoinCheck();
private:
	int PackPacket(char* sendbuf,const char* data);
	LoginManager();
	~LoginManager();
};

