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
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	ZeroMemory(ID, MAXBUF);
	ZeroMemory(PW, MAXBUF);
	int retval=DialogBox(ins, MAKEINTRESOURCE(IDD_DIALOG2), NULL, DlgProc2);
	if (retval == WM_CLOSE || retval == IDCANCEL)
	{
		client->sendbuf.PackPacket(PROTOCOL::BACKPAGE);
		NetWorkProgram::Instance()->S_Packet_Push(client->sendbuf.Data_Pop(), client->sendbuf.Size_Pop());
		client->sendbuf.MemoryZero();
		return;
	}
	int size = PackPacket(buf, ID, PW);
	client->sendbuf.PackPacket(PROTOCOL::LOGININFO, buf, size);
	NetWorkProgram::Instance()->S_Packet_Push(client->sendbuf.Data_Pop(), client->sendbuf.Size_Pop());
}
void LoginManager::LoginResult(HINSTANCE ins, ClientInfo* client)
{
	RESULT result;
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	UnPackPacket(client->recvbuf.Data_Pop(), result, buf);
	MessageBox(NULL, buf, "로그인결과", MB_OK);
	if (result == RESULT::SUCCESS)
	{
		client->SetUserInfo(ID, PW, true);
	}
	client->recvbuf.MemoryZero();
}
void LoginManager::JoinInfo(HINSTANCE ins, ClientInfo* client)
{
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	ZeroMemory(ID, MAXBUF);
	ZeroMemory(PW, MAXBUF);
	int retval = DialogBox(ins, MAKEINTRESOURCE(IDD_DIALOG2), NULL, DlgProc3);
	if (retval == WM_CLOSE || retval == IDCANCEL)
	{
		client->sendbuf.PackPacket(PROTOCOL::BACKPAGE);
		NetWorkProgram::Instance()->S_Packet_Push(client->sendbuf.Data_Pop(), client->sendbuf.Size_Pop());
		client->sendbuf.MemoryZero();
		return;
	}
	int size=PackPacket(buf, ID, PW);
	client->sendbuf.PackPacket(PROTOCOL::JOININFO, buf, size);
	NetWorkProgram::Instance()->S_Packet_Push(client->sendbuf.Data_Pop(), client->sendbuf.Size_Pop());
}
void LoginManager::JoinResult(HINSTANCE ins, ClientInfo* client)
{
	RESULT result;
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	UnPackPacket(client->recvbuf.Data_Pop(), result, buf);
	MessageBox(NULL, buf, "회원가입결과", MB_OK);
	client->recvbuf.MemoryZero();
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
int LoginManager::PackPacket(char* buf, const char* id, const char* pw)
{
	char* ptr = buf;
	int size = 0;
	int strsize = strlen(id);
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
	return size;
}
void LoginManager::UnPackPacket(const char* recvbuf, RESULT& result,char* msg)
{
	const char* ptr = recvbuf+sizeof(PROTOCOL);
	int strsize = 0;
	memcpy(&result, ptr, sizeof(RESULT));
	ptr += sizeof(RESULT);
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(msg, ptr, strsize);
}