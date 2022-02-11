#pragma once
#include "MultiThread.h"
class MainManager:public MultiThread
{
private:
	static MainManager* instance;
	typedef void (SystemFuntion::*func)(ClientInfo*,STATE&);
	map<STATE, func> CALL_Program;
protected:
  
public:
	static MainManager* Instance();
	static void Create();
	static void Destroy();
	virtual void Init();
	virtual void* AcceptClient();
	virtual void Run(void*);
	virtual void Err(HANDLE,void*);
	virtual void End();
private:
	MainManager();
	~MainManager();
};

