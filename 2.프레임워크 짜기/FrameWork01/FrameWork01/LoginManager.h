#pragma once
#include "global.h"
#include "ClientInfo.h"
#include "_List.h"
class LoginManager
{
private:
	static LoginManager* instance;
	_List<UserInfo*> UserList;
	enum class LOGINRESULT
	{
		NONE = -1,
		ISLOGINGFAIL,
		FAIL,
		SUCCESS,
		MAX
	};
	enum class JOINRESULT
	{
		NONE = -1,
		FAIL,
		SUCCESS,
		MAX
	};
public:
	static LoginManager* Instance();
	static void Create();
	static void Destroy();
	void Init();
	void LoginProgram(ClientInfo* _client, STATE& _state);
	void JoinProgram(ClientInfo* _client, STATE& _state);
	bool Is_Loging(ClientInfo* _client,STATE& _state);
	LOGINRESULT LoginCheck(const char* id,const char* pw);
	JOINRESULT JoinCheck(const char* id,const char* pw);
	void FileSave();
	void FileLoad();
	void End();
private:
	int PackPacket(char* sendbuf,const char* data);
	void UnPackPacket(const char* recvbuf, char* ID, char* PW);
	LoginManager();
	~LoginManager();
};

