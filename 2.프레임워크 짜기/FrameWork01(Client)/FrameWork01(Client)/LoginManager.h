#pragma once
#include "global.h"
#include "ClientInfo.h"
class LoginManager
{
private:
	static LoginManager* instance;
public:
	static LoginManager* Instance();
	static void Create();
	static void Destroy();
	void Init();
	void End();
	void LoginInfo(HINSTANCE ins, ClientInfo* client);
	void LoginResult(HINSTANCE ins, ClientInfo* client);
	void JoinInfo(HINSTANCE ins, ClientInfo* client);
	void JoinResult(HINSTANCE ins, ClientInfo* client);
private:
	static INT_PTR CALLBACK DlgProc2(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK DlgProc3(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LoginManager();
	~LoginManager();
};

