#include "LoginManager.h"
LoginManager* LoginManager::instance = nullptr;
LoginManager* LoginManager::Instance()
{
	return instance;
}
void LoginManager::Create()
{
	if (instance == NULL)
	{
		instance = new LoginManager();
	}
}
void LoginManager::Destroy()
{
	delete instance;
}
void LoginManager::Init()
{
	
}
void LoginManager::End()
{

}
void LoginManager::LoginInfo(HINSTANCE ins, ClientInfo* client)
{
	int size = 0;
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	ZeroMemory(ID, MAXBUF);
	ZeroMemory(PW, MAXBUF);
	int retval=DialogBox(ins, MAKEINTRESOURCE(IDD_DIALOG2), NULL, DlgProc2);
	if (retval == WM_CLOSE || retval == IDCANCEL)
	{
		size=PackPacket(buf,PROTOCOL::BACKPAGE);
		NetWorkProgram::Instance()->S_Packet_Push(buf, size);
		return;
	}
	size = PackPacket(buf,PROTOCOL::LOGININFO, ID, PW);
	NetWorkProgram::Instance()->S_Packet_Push(buf, size);
}
void LoginManager::LoginResult(HINSTANCE ins, ClientInfo* client)
{
	RESULT result;
	int size = 0;
	char buf[MAXBUF];
	char temp[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	ZeroMemory(temp, MAXBUF);
	client->GetData(buf, size);
	UnPackPacket(buf, result, temp);
	MessageBox(NULL, temp, "로그인결과", MB_OK);
	if (result == RESULT::SUCCESS)
	{
		client->SetUserInfo(ID, PW, true);
	}
}
void LoginManager::JoinInfo(HINSTANCE ins, ClientInfo* client)
{
	int size = 0;
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	ZeroMemory(ID, MAXBUF);
	ZeroMemory(PW, MAXBUF);
	int retval = DialogBox(ins, MAKEINTRESOURCE(IDD_DIALOG2), NULL, DlgProc3);
	if (retval == WM_CLOSE || retval == IDCANCEL)
	{
		size=PackPacket(buf,PROTOCOL::BACKPAGE);
		NetWorkProgram::Instance()->S_Packet_Push(buf, size);
		return;
	}
	size=PackPacket(buf,PROTOCOL::JOININFO, ID, PW);
	NetWorkProgram::Instance()->S_Packet_Push(buf, size);
}
void LoginManager::JoinResult(HINSTANCE ins, ClientInfo* client)
{
	RESULT result;
	char buf[MAXBUF];
	char temp[MAXBUF];
	int size = 0;
	ZeroMemory(buf, MAXBUF);
	ZeroMemory(temp, MAXBUF);
	client->GetData(buf, size);
	UnPackPacket(buf, result, temp);
	MessageBox(NULL, temp, "회원가입결과", MB_OK);
}
INT_PTR CALLBACK LoginManager::DlgProc2(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LoginManager* M_login = LoginManager::Instance();
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetWindowText(hDlg, "로그인");
		/*M_login->hDialog = hDlg;
		M_login->hIDEdit = GetDlgItem(hDlg, IDC_ID);
		M_login->hPWEdit = GetDlgItem(hDlg, IDC_Pw);*/
		//M_login->hFAILTYPEEdit = GetDlgItem(hDlg, IDC_FAILTYPE);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			GetDlgItemText(hDlg, IDC_ID, M_login->ID, MAXBUF);
			GetDlgItemText(hDlg, IDC_Pw, M_login->PW, MAXBUF);
			EndDialog(hDlg, IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		return FALSE;
	case WM_CLOSE:
		EndDialog(hDlg, WM_CLOSE);
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK LoginManager::DlgProc3(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LoginManager* M_login = LoginManager::Instance();
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetWindowText(hDlg, "회원가입");
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			GetDlgItemText(hDlg, IDC_ID, M_login->ID, MAXBUF);
			GetDlgItemText(hDlg, IDC_Pw, M_login->PW, MAXBUF);
			EndDialog(hDlg, IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		return FALSE;
	case WM_CLOSE:
		EndDialog(hDlg, WM_CLOSE);
		return TRUE;
	}
	return FALSE;
}
LoginManager::LoginManager()
{

}
LoginManager::~LoginManager()
{

}
int LoginManager::PackPacket(char* buf,PROTOCOL protocol, const char* id, const char* pw)
{
	char* ptr = buf+sizeof(int);
	int size = 0;
	int strsize = strlen(id);
	memcpy(ptr, &protocol, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	ptr += sizeof(PROTOCOL);
	memcpy(ptr, &strsize, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	memcpy(ptr, id, strsize);
	ptr += strsize;
	size += strsize;
	strsize = strlen(pw);
	memcpy(ptr, &strsize, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	memcpy(ptr, pw, strsize);
	size += strsize;
	ptr = buf;
	memcpy(ptr, &size, sizeof(int));
	size += sizeof(int);
	return size;
}
int LoginManager::PackPacket(char* buf, PROTOCOL protocol)
{
	char* ptr = buf + sizeof(int);
	int size = 0;
	memcpy(ptr, &protocol, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	ptr = buf;
	memcpy(ptr, &size, sizeof(int));
	size += sizeof(int);
	return size;
}
void LoginManager::UnPackPacket(const char* recvbuf, RESULT& result,char* msg)
{
	const char* ptr = recvbuf;
	int strsize = 0;
	memcpy(&result, ptr, sizeof(RESULT));
	ptr += sizeof(RESULT);
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(msg, ptr, strsize);
}