#pragma once
#include "ClientInfo.h"
#include "NetWorkProgram.h"
#include "resource.h"

class MainMenuManager
{
private:
	struct MainDialogMem
	{
		MainDialogMem(MainMenuManager* obj1, ClientInfo* obj2)
		{
			This = obj1;
			Client = obj2;
		}
		MainMenuManager* This;
		ClientInfo* Client;
	};
	static MainMenuManager* instance;
public:
	static MainMenuManager* Instance();
	static void Create();
	static void Destroy();
	void MenuSelect(HINSTANCE ins, ClientInfo* client);
	void ShowResult(HINSTANCE ins, ClientInfo* client);
	bool EndProgram();
private:
	void UnPackPacket(const char* buffer, int size,char* data);
	int PackPacket(char* buffer,PROTOCOL protocol,int& menunumber);
	MainMenuManager();
	~MainMenuManager();
	static INT_PTR CALLBACK DlgProc1(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

