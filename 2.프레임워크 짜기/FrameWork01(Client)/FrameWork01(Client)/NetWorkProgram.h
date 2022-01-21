#pragma once
#include "ClientInfo.h"
#include "FuntionManager.h"
class NetWorkProgram
{
public:
	static NetWorkProgram* Instance();
	static void Create();
	static void Destroy();
	void Init();
	ClientInfo* Connect();
	void End();
	//static DWORD WINAPI SendThread(LPVOID arg);
	static DWORD WINAPI RecvThread(LPVOID arg);
private:
	static NetWorkProgram* instance;
	NetWorkProgram();
	~NetWorkProgram();
};

