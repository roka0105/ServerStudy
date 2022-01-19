#include "FuntionManager.h"
FuntionManager* FuntionManager::instance = nullptr;
FuntionManager* FuntionManager::Instance()
{
	if (instance == NULL)
	{
		instance = new FuntionManager();
	}
	return instance;
}
FuntionManager::FuntionManager()
{
	InitManager();
}
void FuntionManager::InitManager()
{
	menu_manager = MainMenuManager::Instance();
}
void FuntionManager::MenuSelect(HINSTANCE ins,ClientInfo* client)
{
	menu_manager->MenuSelect(ins,client);
}
void FuntionManager::MenuResult(HINSTANCE ins,ClientInfo*client)
{
	menu_manager->ShowResult(ins,client);
}