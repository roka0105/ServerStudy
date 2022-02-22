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
	cout << "�������� �޸� ����" << endl;
	//�������� �޸� ����(List�� data=UserInfo*)
	for (int i = 0; i < size; ++i)
	{
		UserInfo* temp;
		temp = UserList[i];
		delete temp;
	}
	//����Ʈ �޸� ����(List�� Node �޸� ����)
	cout << "����Ʈ ��� �޸� ����" << endl;
	UserList.Clear();
}
void LoginManager::LoginProgram(ClientInfo* _client, STATE& _state)
{
	int size = 0;
	if (Is_Logout(_client, _state))
		return;
	if (Is_Loging(_client, _state))
		return;
	//1.�α����Է��϶�� Ŭ�� �������� ����
	_client->Send(PROTOCOL::LOGININFO,nullptr,0);
	//2.�α��� ������ Ŭ�󿡰� �޾ƿ� recv
	// +�޾ƿ� ���ú� ������ back(�ڷΰ���)�̸� state=���θ޴��� ������ Ŭ�� Menuselect �������� ���� �� return;
	PROTOCOL protocol;
	char databuf[MAXBUF];
	ZeroMemory(databuf, MAXBUF);
	if (!_client->Recv(protocol,databuf))
	{
		_state = STATE::END;
		return;
	}
	char ID[MAXBUF];
	ZeroMemory(ID, MAXBUF);
	char PW[MAXBUF];
	ZeroMemory(PW, MAXBUF);
	if (protocol == PROTOCOL::BACKPAGE)
	{
		_state = STATE::MAIN;
		//_client->sendbuf.MemoryZero();
		//size=_client->sendbuf.PackPacket(PROTOCOL::MENU_SELECT);
		//_client->Send(_client->sendbuf.Data_Pop(), size);
		return;
	}
	UnPackPacket(databuf, ID, PW);
	//3.�α��� ������ ȸ�������� ���ؼ� �α��� ��� ����
	RESULT result = LoginCheck(ID, PW);
	ZeroMemory(databuf, MAXBUF);
	switch (result)
	{
	case RESULT::ISLOGINGFAIL:
		size = PackPacket(databuf,RESULT::ISLOGINGFAIL, "�̹� �α������� ���̵� �Դϴ�\n");
		break;
	case RESULT::FAIL:
		size = PackPacket(databuf,RESULT::FAIL, "�α��� ����\n");
		break;
	case RESULT::SUCCESS:
		char temp[MAXBUF];
		ZeroMemory(temp, MAXBUF);
		_client->SetUserInfo(ID, PW, true);
		sprintf(temp, "�α��� ����\nȯ���մϴ� %s��!", ID);
		size = PackPacket(databuf,RESULT::SUCCESS, temp);
		_state = STATE::MAIN;
		break;
	}
	_client->Send(PROTOCOL::LOGINRESULT, databuf, size);
	cout << "Login" << endl;
}
void LoginManager::JoinProgram(ClientInfo* _client, STATE& _state)
{
	int size = 0;
	if (Is_Loging(_client, _state))
		return;
	//1.Ŭ�� ȸ���������� �Է��϶�� JoinInfo Protocol send
	_client->Send(PROTOCOL::JOININFO,nullptr,0);
	//2.Ŭ�󿡼� ȸ���������� �޾ƿ�
	// +�޾ƿ� ���ú� ������ back(�ڷΰ���)�̸� state=���θ޴��� ������ Ŭ�� Menuselect �������� ���� �� return;
	PROTOCOL protocol;
	char databuf[MAXBUF];
	ZeroMemory(databuf, MAXBUF);
	if (!_client->Recv(protocol,databuf))
	{
		_state = STATE::END;
		return;
	}
	if (protocol == PROTOCOL::BACKPAGE)
	{
		_state = STATE::MAIN;
		return;
	}
	//3.ȸ������ �������� üũ �� ������ �Ϸ�Ǹ� filesave(); ȸ������ ������� send.
	char ID[MAXBUF];
	char PW[MAXBUF];
	ZeroMemory(ID, MAXBUF);
	ZeroMemory(PW, MAXBUF);
	UnPackPacket(databuf, ID, PW);
	ZeroMemory(databuf, MAXBUF);
	RESULT result=JoinCheck(ID,PW);
	switch (result)
	{
	case RESULT::SUCCESS:
		char temp[MAXBUF];
		ZeroMemory(temp, MAXBUF);
		sprintf(temp, "ȸ������ ����\nȯ���մϴ� %s��!", ID);
		size=PackPacket(databuf,RESULT::SUCCESS, temp);
		_state = STATE::MAIN;
		break;
	case RESULT::FAIL:
		size = PackPacket(databuf,RESULT::FAIL, "ȸ������ ����\n");
		break;
	}
	_client->Send(PROTOCOL::JOINRESULT, databuf, size);
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
		if (UserList[i]->is_loging)
		{
			if (!allflag)
			{
				if (!strcmp(UserList[i]->ID, _client->GetUserInfo()->ID))
				{
					UserList[i]->is_loging = false;
					cout << UserList[i]->ID << "�� �α׾ƿ�" << endl;
				}
			}
			else
			{
				UserList[i]->is_loging = false;
				cout << "��� ���� �α׾ƿ�" << endl;
			}
		}
	}
	_client->SetUserInfo((char*)"\0", (char*)"\0", false);
	FileSave();
}
bool LoginManager::Is_Loging(ClientInfo* _client, STATE& _state)
{
	UserInfo* userinfo = _client->GetUserInfo();
	//test
	//userinfo->is_loging = true;
	//
	if (userinfo->is_loging)
	{
		char buf[MAXBUF];
		ZeroMemory(buf, MAXBUF);
		int size = this->PackPacket(buf, "�α����߿��� ���� �Ͻ� �� �����ϴ�.\r\n");
		_client->Send(PROTOCOL::MENU_RESULT, buf, size);
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
		int size = this->PackPacket(buf, "�α׾ƿ��ϼ̽��ϴ�.\r\n");
		_client->Send(PROTOCOL::MENU_RESULT, buf, size);
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
			UserInfo* user = UserList[i];
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
			UserInfo* user = UserList[i];
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
		//cout << "���Ͽ������!" << endl;
		return;
	}
	cout << "===============���� ������===============" << endl;
 	for(int i=0;i<UserList.size();++i)
	{
		UserInfo* user_info = UserList[i];
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
		//cout << "���Ͽ������!" << endl;
		return;
	}
	UserInfo* user_info;
	cout << "===============���� �б���===============" << endl;
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
	const char* ptr = recvbuf;
	int strsize = 0;
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(ID, ptr, strsize);
	ptr += strsize;
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(PW, ptr, strsize);
}