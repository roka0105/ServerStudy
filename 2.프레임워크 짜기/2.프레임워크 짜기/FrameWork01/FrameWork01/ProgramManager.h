#pragma once
#include "NetWorkProc.h"
#include "ClientManager.h"
#include "LoginManager.h"
#include "JoinManager.h"
#include "MainManager.h"
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
	static ProgramManager* instance;
	NetWorkProc* NetWork_Manager;
	ClientManager* Client_Manager;
	ProgramMember Program_mem;
	SystemFuntion* SystemFunc;
	typedef void (SystemFuntion::*func)(ClientInfo*,STATE&);
	map<STATE, func> CALL_Program;
protected:
  
public:
	static ProgramManager* Instance();	
	void MainThread();
	void ClientThread();
private:
	ProgramManager();
	~ProgramManager();
	static DWORD CALLBACK Client_Thread(LPVOID arg);
};

