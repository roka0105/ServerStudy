#pragma once
#include "ClientInfo.h"
#include "resource.h"
class MainMenuManager
{
private:
	struct MainDialogMem
	{
		MainDialogMem(MainMenuManager* obj1, ClientInfo* obj2);
		MainMenuManager* This;
		ClientInfo* Client;
	};
	MainDialogMem* dialog_param;
	static MainMenuManager* instance;
	NetworkBuffer tempbuf[MAXBUF];
public:
	static MainMenuManager* Instance();
	void MenuSelect(HINSTANCE ins, ClientInfo* client);
	void ShowResult(HINSTANCE ins, ClientInfo* client);
private:
	void UnPackPacket(const char* buffer,int size);
	void PackPacket(int& menunumber);
	MainMenuManager();
	~MainMenuManager();
	static INT_PTR CALLBACK DlgProc1(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

