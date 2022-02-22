#pragma once
#include "MenuManager.h"
#include "LoginManager.h"
#include "GameManager.h"
class SystemFuntion
{
private:
	static SystemFuntion* instance;
public:
	static SystemFuntion* Instance();
	static void Create();
	static void Destroy();
	void Init();
	void End();
	void MainMenu(ClientInfo* client,STATE& state);
	void LoginMenu(ClientInfo* client, STATE& state);
	void JoinMenu(ClientInfo* client, STATE& state);
	void LauncharMenu(ClientInfo* client, STATE& state);
	void EndMenu(ClientInfo* client, STATE& state);
private:
	SystemFuntion();
	~SystemFuntion();
};

