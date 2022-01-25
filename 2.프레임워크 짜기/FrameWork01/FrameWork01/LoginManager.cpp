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
	FileSave();
	int size = UserList.size();
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
		_client->sendbuf.MemoryZero();
		size=_client->sendbuf.PackPacket(PROTOCOL::MENU_SELECT);
		_client->Send(_client->sendbuf.Data_Pop(), size);
		return;
	}
	UnPackPacket(_client->recvbuf.Data_Pop(), ID, PW);
	//3.로그인 정보를 회원정보와 비교해서 로그인 결과 전송
	LOGINRESULT result = LoginCheck(ID, PW);
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	int size = 0;
	switch (result)
	{
	case LOGINRESULT::ISLOGINGFAIL:
		size = PackPacket(buf, "이미 로그인중인 아이디 입니다\r\n");
		break;
	case LOGINRESULT::FAIL:
		size = PackPacket(buf, "로그인 실패\r\n");
		break;
	case LOGINRESULT::SUCCESS:
		char temp[MAXBUF];
		ZeroMemory(temp, MAXBUF);
		_client->GetUserInfo()->is_loging = true;
		sprintf(temp, "로그인 성공\n환영합니다 %s님!", ID);
		size = PackPacket(buf, temp);
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
		_client->sendbuf.MemoryZero();
		size=_client->sendbuf.PackPacket(PROTOCOL::MENU_SELECT);
		_client->Send(_client->sendbuf.Data_Pop(), size);
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
	JOINRESULT result=JoinCheck(ID,PW);
	switch (result)
	{
	case JOINRESULT::SUCCESS:
		char temp[MAXBUF];
		ZeroMemory(temp, MAXBUF);
		sprintf(temp, "회원가입 성공\n환영합니다 %s님!", ID);
		size=PackPacket(buf, temp);
		break;
	case JOINRESULT::FAIL:
		size = PackPacket(buf, "회원가입 실패\r\n");
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
bool LoginManager::Is_Loging(ClientInfo* _client, STATE& _state)
{
	UserInfo* userinfo = _client->GetUserInfo();
	_client->sendbuf.MemoryZero();
	//test
	//userinfo->is_loging = true;
	//
	if (userinfo->is_loging)
	{
		NetworkBuffer temp;
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
LoginManager::LOGINRESULT LoginManager::LoginCheck(const char* id,const char* pw)
{
	if (!UserList.is_empty())
	{
		for (int i = 0; i < UserList.size(); ++i)
		{
			UserInfo* user = UserList[i].data;
			if (!strcmp(id, user->ID) && !strcmp(pw, user->PW))
			{
				if (user->is_loging)
					return LOGINRESULT::ISLOGINGFAIL;
				else
				{
					user->is_loging = true;
					return LOGINRESULT::SUCCESS;
				}
			}
		}
	}
	return LOGINRESULT::FAIL;
}
LoginManager::JOINRESULT LoginManager::JoinCheck(const char* id,const char* pw)
{
	if (!UserList.is_empty())
	{
		for (int i = 0; i < UserList.size(); ++i)
		{
			UserInfo* user = UserList[i].data;
			if (!strcmp(id, user->ID))
			{
				return JOINRESULT::FAIL;
			}
		}
	}
	UserList.Push_back(new UserInfo(id, pw));
	FileSave();
	return JOINRESULT::SUCCESS;
}
void LoginManager::FileSave()
{
	FILE* fp = fopen("UserInfo.dat", "wb");
	if (fp == NULL)
	{
		//cout << "파일열기실패!" << endl;
		return;
	}
	cout << "===============파일 쓰기중===============" << endl;
	while (!UserList.is_empty())
	{
		UserInfo* user_info = UserList.Pop_front();
		fwrite(user_info, sizeof(UserInfo), 1, fp);
		cout << user_info->ID << " " << user_info->PW << " " << user_info->is_loging << endl;
	}
	fclose(fp);
}
void LoginManager::FileLoad()
{
	FILE* fp = fopen("UserInfo.dat", "rb");
	if (fp == NULL)
	{
		//cout << "파일열기실패!" << endl;
		return;
	}
	UserInfo* user_info;
	cout << "===============파일 읽기중===============" << endl;
	while (!feof(fp))
	{
		user_info = new UserInfo();
		fread(user_info, sizeof(UserInfo), 1, fp);
		cout << user_info->ID << " " << user_info->PW << " " << user_info->is_loging << endl;
		UserList.Push_back(user_info);
	}
	fclose(fp);
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