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
class ClientInfo :public NetworkSocket
{
private:
	UserInfo userInfo;
public:
	SOCKADDR_IN GetAddr();
	void Send(const char* databuf, int size);
	bool Recv(char* databuf,int&size);
	void err_display(const char* msg);
	void err_quit(const char* msg);
	UserInfo* GetUserInfo();
	void SetUserInfo(char* id, char* pw, bool login);
	void SetData(const char* data,int size);
	void GetData(char* data, int& size);
	void ClearData();
	ClientInfo();
	ClientInfo(SOCKET clientsock, SOCKADDR_IN clientaddr);
	ClientInfo(ClientInfo& ref);
	~ClientInfo();
private:
};

