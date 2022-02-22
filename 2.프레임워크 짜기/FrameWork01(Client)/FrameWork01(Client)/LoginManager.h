#pragma once
#include "global.h"
#include "ClientInfo.h"
#include "resource.h"
#include "NetWorkProgram.h"
class LoginManager
{
private:
	enum class RESULT
	{
		NONE = -1,
		ISLOGINGFAIL,
		FAIL,
		SUCCESS,
		MAX
	};
	static LoginManager* instance;
	char ID[MAXBUF], PW[MAXBUF];
	//HWND hDialog;
	//HANDLE hIDEdit, hPWEdit, hFAILTYPEEdit;
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
	int PackPacket(char* buf,PROTOCOL protocol, const char* id, const char* pw);
	int PackPacket(char* buf, PROTOCOL protocol);
	void UnPackPacket(const char* recvbuf, RESULT& result,char* msg);
	LoginManager();
	~LoginManager();
};

