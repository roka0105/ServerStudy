#include "SystemFuntion.h"
SystemFuntion* SystemFuntion::instance=nullptr;
SystemFuntion* SystemFuntion::Instance()
{
	if (instance == NULL)
	{
		instance = new SystemFuntion();
	}
	return instance;
}
void SystemFuntion::ManagerInit()
{
	Main = MainManager::Instance();
}
void SystemFuntion::MainMenu(ClientInfo* client,STATE& state)
{
	Main->MainProgram(client, state);
}