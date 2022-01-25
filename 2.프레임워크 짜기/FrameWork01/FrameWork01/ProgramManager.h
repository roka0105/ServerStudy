#pragma once
#include "NetWorkProc.h"
#include "ClientManager.h"
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
	typedef void (SystemFuntion::*func)(ClientInfo*,STATE&);
	map<STATE, func> CALL_Program;
protected:
  
public:
	static ProgramManager* Instance();	
	void MainThread();
	void ClientThread(ProgramMember* member);
	void Init();
	void End();
private:
	ProgramManager();
	~ProgramManager();
	static DWORD CALLBACK Client_Thread(LPVOID arg);
};

