#pragma once
#include "ClientInfo.h"
class SystemFuntion
{
private:

public:
	SystemFuntion();
	~SystemFuntion();
	static void MainProgram(ClientInfo* _client,int* _state);
	static void LoginProgram(ClientInfo* _client,int* _state);
	static void JoinProgram(ClientInfo* _client,int* _state);
};

