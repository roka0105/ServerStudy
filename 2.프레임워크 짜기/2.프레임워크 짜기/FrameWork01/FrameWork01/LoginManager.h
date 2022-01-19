#pragma once
#include "global.h"
#include "ClientInfo.h"
class LoginManager
{
private:
	static LoginManager* instance;
public:
	static LoginManager* Instance();
	static void LoginProgram(ClientInfo* _client, STATE& _state);
	bool LoginCheck();
private:
	LoginManager();
	~LoginManager();
};

