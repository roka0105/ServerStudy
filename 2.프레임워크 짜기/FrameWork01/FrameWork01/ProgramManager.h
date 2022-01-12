#pragma once
#include "NetWorkProc.h"
#include "ClientManager.h"
#include "LoginManager.h"
#include "SystemFuntion.h"
#include "PhoenixSingleton.h"
class ProgramManager;
struct ProgramMember
{
	ProgramManager* This;
	ClientInfo* client;
};
class ProgramManager//:public PhoenixSingleton<ProgramManager>
{
private:
	NetWorkProc NetWork_Manager;
	ClientManager Client_Manager;
	ProgramMember Program_mem;
	LoginManager Login_Manager;
	SystemFuntion SystemFunc;
	typedef void (*func)(ClientInfo*,int*);
	map<STATE, func> CALL_Program;
public:
	void MainThread();
	void ClientThread();
	void NetworkThread();
	ProgramManager();
	~ProgramManager();
private:
	static DWORD CALLBACK Client_Thread(LPVOID arg);
	static DWORD CALLBACK NetWork_Thread(LPVOID arg);
	
};

