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
	cout << "�������� �޸� ����" << endl;
	//�������� �޸� ����(List�� data=UserInfo*)
	for (int i = 0; i < size; ++i)
	{
		UserInfo* temp;
		temp = UserList[i].data;
		delete temp;
	}
	//����Ʈ �޸� ����(List�� Node �޸� ����)
	cout << "����Ʈ ��� �޸� ����" << endl;
	UserList.Clear();
}
void LoginManager::LoginProgram(ClientInfo* _client, STATE& _state)
{
	int size = 0;
	if (Is_Loging(_client, _state))
		return;
	//1.�α����Է��϶�� Ŭ�� �������� ����
	size = _client->sendbuf.PackPacket(PROTOCOL::LOGININFO);
	_client->Send(_client->sendbuf.Data_Pop(), size);
	//2.�α��� ������ Ŭ�󿡰� �޾ƿ� recv
	// +�޾ƿ� ���ú� ������ back(�ڷΰ���)�̸� state=���θ޴��� ������ Ŭ�� Menuselect �������� ���� �� return;
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
	//3.�α��� ������ ȸ�������� ���ؼ� �α��� ��� ����
	LOGINRESULT result = LoginCheck(ID, PW);
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	int size = 0;
	switch (result)
	{
	case LOGINRESULT::ISLOGINGFAIL:
		size = PackPacket(buf, "�̹� �α������� ���̵� �Դϴ�\r\n");
		break;
	case LOGINRESULT::FAIL:
		size = PackPacket(buf, "�α��� ����\r\n");
		break;
	case LOGINRESULT::SUCCESS:
		char temp[MAXBUF];
		ZeroMemory(temp, MAXBUF);
		_client->GetUserInfo()->is_loging = true;
		sprintf(temp, "�α��� ����\nȯ���մϴ� %s��!", ID);
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
	//1.Ŭ�� ȸ���������� �Է��϶�� JoinInfo Protocol send
	_client->sendbuf.MemoryZero();
	size=_client->sendbuf.PackPacket(PROTOCOL::JOININFO);
	_client->Send(_client->sendbuf.Data_Pop(), size);
	//2.Ŭ�󿡼� ȸ���������� �޾ƿ�
	// +�޾ƿ� ���ú� ������ back(�ڷΰ���)�̸� state=���θ޴��� ������ Ŭ�� Menuselect �������� ���� �� return;
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
	//3.ȸ������ �������� üũ �� ������ �Ϸ�Ǹ� filesave(); ȸ������ ������� send.
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
		sprintf(temp, "ȸ������ ����\nȯ���մϴ� %s��!", ID);
		size=PackPacket(buf, temp);
		break;
	case JOINRESULT::FAIL:
		size = PackPacket(buf, "ȸ������ ����\r\n");
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
		int size = this->PackPacket(buf, "�α����߿��� ���� �Ͻ� �� �����ϴ�.\r\n");
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
		//cout << "���Ͽ������!" << endl;
		return;
	}
	cout << "===============���� ������===============" << endl;
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
		//cout << "���Ͽ������!" << endl;
		return;
	}
	UserInfo* user_info;
	cout << "===============���� �б���===============" << endl;
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