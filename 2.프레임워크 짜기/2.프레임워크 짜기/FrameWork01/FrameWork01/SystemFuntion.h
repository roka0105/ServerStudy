#pragma once
#include "MainManager.h"
class SystemFuntion
{
private:
	static SystemFuntion* instance;
	MainManager* Main;
public:
	static SystemFuntion* Instance();
	static void Create();
	static void Destroy();
	void MainMenu(ClientInfo* client,STATE& state);
	void LoginMenu(ClientInfo* client, STATE& state);
	void JoinMenu(ClientInfo* client, STATE& state);
private:
	SystemFuntion();
	~SystemFuntion();
};

