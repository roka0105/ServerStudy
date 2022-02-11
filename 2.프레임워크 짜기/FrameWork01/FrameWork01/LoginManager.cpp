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
void LoginManager::Init()
{
	FileLoad();

}
void LoginManager::End()
{
	//FileSave();
	int size = UserList.size();
	LogOut(true);
	cout << "유저정보 메모리 해제" << endl;
	//유저정보 메모리 해제(List의 data=UserInfo*)
	for (int i = 0; i < size; ++i)
	{
		UserInfo* temp;
		temp = UserList[i].data;
		delete temp;
	}
	//리스트 메모리 해제(List의 Node 메모리 해제)
	cout << "리스트 노드 메모리 해제" << endl;
	UserList.Clear();
}
void LoginManager::LoginProgram(ClientInfo* _client, STATE& _state)
{
	int size = 0;
	if (Is_Logout(_client, _state))
		return;
	if (Is_Loging(_client, _state))
		return;
	//1.로그인입력하라고 클라에 프로토콜 전송
	size = _client->sendbuf.PackPacket(PROTOCOL::LOGININFO);
	_client->Send(_client->sendbuf.Data_Pop(), size);
	//2.로그인 정보를 클라에게 받아옴 recv
	// +받아온 리시브 정보가 back(뒤로가기)이면 state=메인메뉴로 돌리고 클라에 Menuselect 프로토콜 전송 후 return;
	if (!_client->Recv())
	{
		_state = STATE::END;
		return;
	}
	char ID[MAXBUF];
	ZeroMemory(ID, MAXBUF);
	char PW[MAXBUF];
	ZeroMemory(PW, MAXBUF);
	PROTOCOL protocol;
	_client->recvbuf.UnPackPacket(protocol);
	if (protocol == PROTOCOL::BACKPAGE)
	{
		_state = STATE::MAIN;
		//_client->sendbuf.MemoryZero();
		//size=_client->sendbuf.PackPacket(PROTOCOL::MENU_SELECT);
		//_client->Send(_client->sendbuf.Data_Pop(), size);
		return;
	}
	UnPackPacket(_client->recvbuf.Data_Pop(), ID, PW);
	//3.로그인 정보를 회원정보와 비교해서 로그인 결과 전송
	RESULT result = LoginCheck(ID, PW);
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	switch (result)
	{
	case RESULT::ISLOGINGFAIL:
		size = PackPacket(buf,RESULT::ISLOGINGFAIL, "이미 로그인중인 아이디 입니다\n");
		break;
	case RESULT::FAIL:
		size = PackPacket(buf,RESULT::FAIL, "로그인 실패\n");
		break;
	case RESULT::SUCCESS:
		char temp[MAXBUF];
		ZeroMemory(temp, MAXBUF);
		_client->SetUserInfo(ID, PW, true);
		sprintf(temp, "로그인 성공\n환영합니다 %s님!", ID);
		size = PackPacket(buf,RESULT::SUCCESS, temp);
		_state = STATE::MAIN;
		break;
	}
	_client->sendbuf.MemoryZero();
	size=_client->sendbuf.PackPacket(PROTOCOL::LOGINRESULT, buf, size);
	_client->Send(_client->sendbuf.Data_Pop(), size);
	cout << "Login" << endl;
}
void LoginManager::JoinProgram(ClientInfo* _client, STATE& _state)
{
	int size = 0;
	if (Is_Loging(_client, _state))
		return;
	//1.클라에 회원가입정보 입력하라고 JoinInfo Protocol send
	_client->sendbuf.MemoryZero();
	size=_client->sendbuf.PackPacket(PROTOCOL::JOININFO);
	_client->Send(_client->sendbuf.Data_Pop(), size);
	//2.클라에서 회원가입정보 받아옴
	// +받아온 리시브 정보가 back(뒤로가기)이면 state=메인메뉴로 돌리고 클라에 Menuselect 프로토콜 전송 후 return;
	_client->recvbuf.MemoryZero();
	if (!_client->Recv())
	{
		_state = STATE::END;
		return;
	}
	PROTOCOL protocol;
	_client->recvbuf.UnPackPacket(protocol);
	if (protocol == PROTOCOL::BACKPAGE)
	{
		_state = STATE::MAIN;
		//_client->sendbuf.MemoryZero();
		//size=_client->sendbuf.PackPacket(PROTOCOL::MENU_SELECT);
		//_client->Send(_client->sendbuf.Data_Pop(), size);
		return;
	}
	//3.회원가입 가능한지 체크 후 가입이 완료되면 filesave(); 회원가입 결과정보 send.
	char ID[MAXBUF];
	char PW[MAXBUF];
	ZeroMemory(ID, MAXBUF);
	ZeroMemory(PW, MAXBUF);
	UnPackPacket(_client->recvbuf.Data_Pop(), ID, PW);
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	_client->sendbuf.MemoryZero();
	RESULT result=JoinCheck(ID,PW);
	switch (result)
	{
	case RESULT::SUCCESS:
		char temp[MAXBUF];
		ZeroMemory(temp, MAXBUF);
		sprintf(temp, "회원가입 성공\n환영합니다 %s님!", ID);
		size=PackPacket(buf,RESULT::SUCCESS, temp);
		_state = STATE::MAIN;
		break;
	case RESULT::FAIL:
		size = PackPacket(buf,RESULT::FAIL, "회원가입 실패\n");
		break;
	}
	_client->sendbuf.MemoryZero();
	size = _client->sendbuf.PackPacket(PROTOCOL::JOINRESULT, buf, size);
	_client->Send(_client->sendbuf.Data_Pop(), size);
	cout << "Join" << endl;
}
LoginManager::LoginManager()
{

}
LoginManager::~LoginManager()
{

}
void LoginManager::LogOut(bool allflag,ClientInfo* _client)
{
	int size = UserList.size();
	for (int i = 0; i < size; ++i)
	{
		if (UserList[i].data->is_loging)
		{
			if (!allflag)
			{
				if (!strcmp(UserList[i].data->ID, _client->GetUserInfo()->ID))
				{
					UserList[i].data->is_loging = false;
					cout << UserList[i].data->ID << "님 로그아웃" << endl;
				}
			}
			else
			{
				UserList[i].data->is_loging = false;
				cout << "모든 유저 로그아웃" << endl;
			}
		}
	}
	_client->SetUserInfo((char*)"\0", (char*)"\0", false);
	FileSave();
}
bool LoginManager::Is_Loging(ClientInfo* _client, STATE& _state)
{
	UserInfo* userinfo = _client->GetUserInfo();
	_client->sendbuf.MemoryZero();
	//test
	//userinfo->is_loging = true;
	//
	if (userinfo->is_loging)
	{
		char buf[MAXBUF];
		ZeroMemory(buf, MAXBUF);
		int size = this->PackPacket(buf, "로그인중에는 선택 하실 수 없습니다.\r\n");
		size = _client->sendbuf.PackPacket(PROTOCOL::MENU_RESULT, buf, size);
		_client->Send(_client->sendbuf.Data_Pop(), size);
		_state = STATE::MAIN;
		return true;
	}
	return false;
}
bool LoginManager::Is_Logout(ClientInfo* _client, STATE& state)
{
	if (_client->LogOutRequest())
	{
		char buf[MAXBUF];
		ZeroMemory(buf, MAXBUF);
		int size = this->PackPacket(buf, "로그아웃하셨습니다.\r\n");
		size = _client->sendbuf.PackPacket(PROTOCOL::MENU_RESULT, buf, size);
		_client->Send(_client->sendbuf.Data_Pop(), size);
		LogOut(false, _client);
		state = STATE::MAIN;
		return true;
	}
	return false;
}
LoginManager::RESULT LoginManager::LoginCheck(const char* id,const char* pw)
{
	if (!UserList.is_empty())
	{
		for (int i = 0; i < UserList.size(); ++i)
		{
			UserInfo* user = UserList[i].data;
			if (!strcmp(id, user->ID) && !strcmp(pw, user->PW))
			{
				if (user->is_loging)
					return RESULT::ISLOGINGFAIL;
				else
				{
					user->is_loging = true;
					FileSave();
					return RESULT::SUCCESS;
				}
			}
		}
	}
	return RESULT::FAIL;
}
LoginManager::RESULT LoginManager::JoinCheck(const char* id,const char* pw)
{
	if (!UserList.is_empty())
	{
		for (int i = 0; i < UserList.size(); ++i)
		{
			UserInfo* user = UserList[i].data;
			if (!strcmp(id, user->ID))
			{
				return RESULT::FAIL;
			}
		}
	}
	UserList.Push_back(new UserInfo(id, pw));
	FileSave();
	return RESULT::SUCCESS;
}
void LoginManager::FileSave()
{
	FILE* fp = fopen("UserInfo.bin", "wb");
	if (fp == NULL)
	{
		//cout << "파일열기실패!" << endl;
		return;
	}
	cout << "===============파일 쓰기중===============" << endl;
 	for(int i=0;i<UserList.size();++i)
	{
		UserInfo* user_info = UserList[i].data;
		fwrite(user_info, sizeof(UserInfo), 1, fp);
		cout << user_info->ID << " " << user_info->PW << " " << user_info->is_loging << endl;
	}
	fclose(fp);
}
void LoginManager::FileLoad()
{
	FILE* fp = fopen("UserInfo.bin", "rb");
	if (fp == NULL)
	{
		//cout << "파일열기실패!" << endl;
		return;
	}
	UserInfo* user_info;
	cout << "===============파일 읽기중===============" << endl;
	int check = 0;
	while (!feof(fp))
	{
		user_info = new UserInfo();
		check=fread(user_info, sizeof(UserInfo), 1, fp);
		if (check == NULL)
			break;
		cout << user_info->ID << " " << user_info->PW << " " << user_info->is_loging << endl;
		UserList.Push_back(user_info);
	}
	fclose(fp);
}
int LoginManager::PackPacket(char* sendbuf,RESULT result, const char* data)
{
	char* ptr = sendbuf;
	int strsize = strlen(data);
	int size = 0;
	memcpy(ptr, &result, sizeof(RESULT));
	size += sizeof(RESULT);
	ptr += sizeof(RESULT);
	memcpy(ptr, &strsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);
	memcpy(ptr, data, strsize);
	size += strsize;
	return size;
}
int LoginManager::PackPacket(char* sendbuf, const char* data)
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
void LoginManager::UnPackPacket(const char* recvbuf, char* ID, char* PW)
{
	const char* ptr = recvbuf + sizeof(PROTOCOL);
	int strsize = 0;
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(ID, ptr, strsize);
	ptr += strsize;
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(PW, ptr, strsize);
}