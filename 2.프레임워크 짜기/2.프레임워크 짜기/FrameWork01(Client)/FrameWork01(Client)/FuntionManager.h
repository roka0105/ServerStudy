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
	MainMenuManager* menu_manager;
public:
	void InitManager();
	void MenuSelect(HINSTANCE ins,ClientInfo* client);
	void MenuResult(HINSTANCE ins,ClientInfo* client);
	static FuntionManager* Instance();
private:
	FuntionManager();
	~FuntionManager();
};

