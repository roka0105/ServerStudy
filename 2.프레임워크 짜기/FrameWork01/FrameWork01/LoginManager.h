#pragma once
#include "global.h"
#include "ClientInfo.h"
#include "_List.h"
class LoginManager
{
private:
	static LoginManager* instance;
	_List<UserInfo*> UserList;
	enum class RESULT
	{
		NONE = -1,
		ISLOGINGFAIL,
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
	RESULT LoginCheck(const char* id,const char* pw);
	RESULT JoinCheck(const char* id,const char* pw);
	void FileSave();
	void FileLoad();
	void End();
private:
	int PackPacket(char* sendbuf, const char* data);
	int PackPacket(char* sendbuf,RESULT result,const char* data);
	void UnPackPacket(const char* recvbuf, char* ID, char* PW);
	LoginManager();
	~LoginManager();
};

