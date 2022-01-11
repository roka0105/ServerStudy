#pragma once
#include "global.h"
struct ProgramMember
{
	ProgramManager* This;
	ClientInfo* client;
};
class ProgramManager
{
private:
	NetWorkProc NetWork_Manager;
	ClientManager Client_Manager;
	ProgramMember Program_mem;
public:
	ProgramManager();
	~ProgramManager(); 
	void MainThread();
	void ClientThread();
	static DWORD CALLBACK Client_Thread(LPVOID arg);
private:
};

