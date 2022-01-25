#pragma once
#include "NetworkSocket.h"
struct UserInfo
{
	UserInfo()
	{
		ZeroMemory(PW, MAXBUF);
		ZeroMemory(ID, MAXBUF);
		is_loging = false;
	}
	UserInfo(const char* id, const char* pw)
	{
		ZeroMemory(PW, MAXBUF);
		ZeroMemory(ID, MAXBUF);
		strcpy(ID, id);
		strcpy(PW, pw);
		is_loging = false;
	}
	char PW[MAXBUF];
	char ID[MAXBUF];
	bool is_loging;
};
class ClientInfo:public NetworkSocket
{
public:
	ClientInfo();
	ClientInfo(SOCKET clientsock,SOCKADDR_IN clientaddr);
	ClientInfo(ClientInfo& ref);
	~ClientInfo();
	UserInfo* GetUserInfo();
private:
	UserInfo* userInfo;
};

