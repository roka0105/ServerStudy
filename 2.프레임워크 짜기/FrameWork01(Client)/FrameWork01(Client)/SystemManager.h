#pragma once
#include "NetWorkProgram.h"
#include "FuntionManager.h"

//template<class T>
//using Func=void(T::*)(char*);

class SystemManager
{
private:
	static SystemManager* instance;
	ClientInfo* Client;
	HANDLE hThread[2];
	HINSTANCE hInst;
	map<PROTOCOL, void(FuntionManager::*)(HINSTANCE, ClientInfo*)>protocolFunction;
public:
	void Init(HINSTANCE hInstance);
	void Start();
	void End();
	void ProtocolMapInit();
	static SystemManager* Instance();
	static void Create();
	static void Destroy();
private:
	void UnPackPacket(const char* buffer, PROTOCOL& protocol);
	SystemManager();
	~SystemManager();
};

