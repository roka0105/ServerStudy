#pragma once
#include "global.h"
#include "NetWorkProc.h"
#include "ClientManager.h"
#include "SystemFuntion.h"
#include "PhoenixSingleton.h"
#include "MultiThread.h"

class MultiThread
{
public:
	virtual void Init() = 0;
	virtual void* AcceptClient() = 0;
	void Loop();
	virtual void Run(void*) = 0;
	virtual void Err(HANDLE,void*) = 0;
	virtual void End() = 0;
	static DWORD __stdcall ThreadFunc(LPVOID ptr);
protected:
	MultiThread(){}
	virtual ~MultiThread(){}
};
struct MainMember
{
	MultiThread* This;
    void* client;
};
