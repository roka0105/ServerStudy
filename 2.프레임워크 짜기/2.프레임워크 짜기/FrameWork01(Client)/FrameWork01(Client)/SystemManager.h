#pragma once
#include "NetWorkProgram.h"
#include "FuntionManager.h"

//template<class T>
//using Func=void(T::*)(char*);

class SystemManager
{
public:
	void Init(HINSTANCE hInstance);
	void Start();
	void End();
	void ProtocolMapInit();
private:
	NetWorkProgram* Network;
	FuntionManager* Funcmanager;
	ClientInfo* Client;
	HANDLE hThread[2];
	HINSTANCE hInst;
	
	map<PROTOCOL,void(FuntionManager::*)(HINSTANCE ,ClientInfo*)>protocolFunction;
protected:
};

