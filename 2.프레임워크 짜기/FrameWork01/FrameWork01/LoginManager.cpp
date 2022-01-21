#include "LoginManager.h"
LoginManager* LoginManager::instance = nullptr;
LoginManager* LoginManager::Instance()
{
	return instance;
}
void LoginManager::Create()
{
	if (instance == NULL)
	{
		instance = new LoginManager();
	}
}
void LoginManager::Destroy()
{
	delete instance;
}
void LoginManager::LoginProgram(ClientInfo* _client,STATE& _state)
{
	if (Is_Loging(_client,_state))
		return;

	cout << "Login" << endl;
}
void LoginManager::JoinProgram(ClientInfo* _client, STATE& _state)
{
	cout << "Join" << endl;
}
LoginManager::LoginManager()
{

}
LoginManager::~LoginManager()
{

}
bool LoginManager::Is_Loging(ClientInfo* _client,STATE& _state)
{ 
	UserInfo* userinfo = _client->GetUserInfo();
	_client->sendbuf.MemoryZero();
	//test
	userinfo->is_loging = true;
	//
	if (userinfo->is_loging)
	{
		NetworkBuffer temp;
		char buf[MAXBUF];
		ZeroMemory(buf, MAXBUF);
		int size=this->PackPacket(buf, "로그인중에는 선택 하실 수 없습니다.\r\n");
		size=_client->sendbuf.PackPacket(PROTOCOL::MENU_RESULT, buf, size);
		_client->Send(_client->sendbuf.Data_Pop(), size);
		_state = STATE::MAIN;
		return true;
	}
	return false;
}
bool LoginManager::LoginCheck()
{
	return false;
}
bool LoginManager::JoinCheck()
{
	return false;
}
int LoginManager::PackPacket(char* sendbuf,const char* data)
{
	char* ptr = sendbuf;
	int strsize = strlen(data);
	int size = 0;
	memcpy(ptr, &strsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);
	memcpy(ptr, data, strsize);
	size += strsize;
	return size;
}