#include "SystemFuntion.h"
SystemFuntion* SystemFuntion::instance=nullptr;
SystemFuntion* SystemFuntion::Instance()
{
	return instance;
}
void SystemFuntion::Create()
{
	instance = new SystemFuntion();
	MenuManager::Create();
	LoginManager::Create();
}
void SystemFuntion::Destroy()
{
	LoginManager::Destroy();
	MenuManager::Destroy();
	delete instance;
}
void SystemFuntion::Init()
{
	LoginManager::Instance()->Init();
	MenuManager::Instance()->Init();
}
void SystemFuntion::End()
{
	LoginManager::Instance()->End();
	MenuManager::Instance()->End();
}
void SystemFuntion::MainMenu(ClientInfo* client,STATE& state)
{
	MenuManager::Instance()->MainProgram(client, state);
}
void SystemFuntion::LoginMenu(ClientInfo* client, STATE& state)
{
	LoginManager::Instance()->LoginProgram(client, state);
}
void SystemFuntion::JoinMenu(ClientInfo* client, STATE& state)
{
	LoginManager::Instance()->JoinProgram(client, state);
}
void SystemFuntion::EndMenu(ClientInfo* client, STATE& state)
{
	MenuManager::Instance()->EndProgram(client, state);
}
SystemFuntion::SystemFuntion()
{

}
SystemFuntion::~SystemFuntion()
{

}