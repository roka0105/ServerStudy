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
struct CharacterInfo
{
	int HP;
	int Atk;
	POINT pos;
};
class ClientInfo:public NetworkSocket
{
public:
	ClientInfo();
	ClientInfo(SOCKET clientsock,SOCKADDR_IN clientaddr);
	ClientInfo(ClientInfo& ref);
	~ClientInfo();
	SOCKADDR_IN GetAddr();
	void Send(PROTOCOL protocol, char* databuf=nullptr, int size=0);
	bool Recv(PROTOCOL& protocol, char* databuf);
	void err_display(const char* msg);
	void err_quit(const char* msg);
	UserInfo* GetUserInfo();
	void SetUserInfo(char* id, char* pw, bool login);
	void LogOut();
	bool LogOutRequest();
	void _SetEvent();
	HANDLE _GetEvent();
	int GetRoomNumber() { return roomnumber; }
private:
	int roomnumber;
	CharacterInfo GameCharacter;
	UserInfo* userInfo;
	bool is_logout_request;
	HANDLE hStartEvent;
};



