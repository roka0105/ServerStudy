#pragma once
#include "global.h"
class ProgramManager
{
private:
	NetWorkProc NetWork_Manager;
	ClientManager Client_Manager;
public:
	ProgramManager();
	~ProgramManager(); 
	void MainThread();
	void ClientThread(ClientInfo* _client);
	//DWORD CALLBACK Client_Thread(LPVOID arg);
private:
};

