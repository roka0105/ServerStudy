#include "LoginManager.h"
LoginManager* LoginManager::instance = nullptr;
LoginManager* LoginManager::Instance()
{
	if (instance == NULL)
	{
		instance = new LoginManager();
	}
	return instance;
}
void LoginManager::LoginProgram(ClientInfo* _client,STATE& _state)
{
	cout << "Login" << endl;
}
LoginManager::LoginManager()
{

}
LoginManager::~LoginManager()
{

}
bool LoginManager::LoginCheck()
{
	return false;
}