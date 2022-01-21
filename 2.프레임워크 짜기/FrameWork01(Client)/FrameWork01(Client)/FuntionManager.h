#pragma once
#include "MainMenuManager.h"
/*
#define PROTOCOL_FUNTION_DECLARE(protocol_name)\
void protocolFuntion_##protocol_name(int data_size,char*buffer);
#define PROTOCOL_FUNTION(protocol_name)\
void protocolFuntion_##protocol_name
*/
class FuntionManager
{
private:
	static FuntionManager* instance;
	bool is_EndProgram;
public:
	void MenuSelect(HINSTANCE ins,ClientInfo* client);
	void MenuResult(HINSTANCE ins,ClientInfo* client);
	void MenuEnd(HINSTANCE ins, ClientInfo* client);
	bool Is_EndProgram();
	void Set_IsEndProgram(bool flag);
	static FuntionManager* Instance();
	static void Create();
	static void Destroy();
private:
	FuntionManager();
	~FuntionManager();
};

