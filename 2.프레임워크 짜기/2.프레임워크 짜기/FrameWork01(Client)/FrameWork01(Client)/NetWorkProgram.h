#pragma once
#include "ClientInfo.h"
class NetWorkProgram
{
public:
	static NetWorkProgram* Instance();
	void Init();
	ClientInfo* Connect();
	void End();
	static DWORD WINAPI SendThread(LPVOID arg);
	static DWORD WINAPI RecvThread(LPVOID arg);
private:
	ClientInfo* Client;
	static NetWorkProgram* instance;
};

