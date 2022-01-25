#include "FuntionManager.h"
FuntionManager* FuntionManager::instance = nullptr;
FuntionManager* FuntionManager::Instance()
{
	return instance;
}
void FuntionManager::Create()
{
	instance = new FuntionManager();
	MainMenuManager::Create();
	LoginManager::Create();
}
void FuntionManager::Destroy()
{
	MainMenuManager::Destroy();
	LoginManager::Destroy();
	delete instance;
}
void FuntionManager::MenuSelect(HINSTANCE ins,ClientInfo* client)
{
	MainMenuManager::Instance()->MenuSelect(ins,client);
}
void FuntionManager::MenuResult(HINSTANCE ins,ClientInfo*client)
{
	MainMenuManager::Instance()->ShowResult(ins,client);
}
void FuntionManager::MenuEnd(HINSTANCE ins, ClientInfo* client)
{
	is_EndProgram = MainMenuManager::Instance()->EndProgram();
}
void FuntionManager::LoginInfo(HINSTANCE ins, ClientInfo* client)
{
	LoginManager::Instance()->LoginInfo(ins, client);
}
void FuntionManager::LoginResult(HINSTANCE ins, ClientInfo* client)
{
	LoginManager::Instance()->LoginResult(ins, client);
}
void FuntionManager::JoinInfo(HINSTANCE ins, ClientInfo* client)
{
	LoginManager::Instance()->JoinInfo(ins, client);
}
void FuntionManager::JoinResult(HINSTANCE ins, ClientInfo* client)
{
	LoginManager::Instance()->JoinResult(ins, client);
}
void FuntionManager::Set_IsEndProgram(bool flag)
{
	is_EndProgram = flag;
}
bool FuntionManager::Is_EndProgram()
{
	return is_EndProgram;
}
FuntionManager::FuntionManager()
{
	is_EndProgram = false;
}
FuntionManager::~FuntionManager()
{

}