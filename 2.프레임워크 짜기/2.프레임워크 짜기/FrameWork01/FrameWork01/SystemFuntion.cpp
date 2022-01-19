#include "SystemFuntion.h"
SystemFuntion* SystemFuntion::instance=nullptr;
SystemFuntion* SystemFuntion::Instance()
{
	return instance;
}
void SystemFuntion::Create()
{
	instance = new SystemFuntion();
	MainManager::Create();
}
void SystemFuntion::Destroy()
{

}
void SystemFuntion::MainMenu(ClientInfo* client,STATE& state)
{
	MainManager::Instance()->MainProgram(client, state);
}
void SystemFuntion::LoginMenu(ClientInfo* client, STATE& state)
{

}
void SystemFuntion::JoinMenu(ClientInfo* client, STATE& state)
{

}
SystemFuntion::SystemFuntion()
{

}
SystemFuntion::~SystemFuntion()
{

}