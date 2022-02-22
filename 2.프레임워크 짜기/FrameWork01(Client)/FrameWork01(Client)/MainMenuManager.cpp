#include "MainMenuManager.h"
MainMenuManager* MainMenuManager::instance = nullptr;
MainMenuManager* MainMenuManager::Instance()
{
	return instance;
}
void MainMenuManager::Create()
{
	instance = new MainMenuManager();
}
void MainMenuManager::Destroy()
{
	delete instance;
}
void  MainMenuManager::MenuSelect(HINSTANCE ins, ClientInfo* client)
{
	//MainDialogMem* dialog_param = new MainDialogMem(this, client);
	int menu_number = 0;
	int size = 0;
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	//int retval = DialogBox(ins, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc1);
	int retval = DialogBoxParam(ins, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc1, (LPARAM)client);
	if (retval == IDCANCEL || retval == WM_CLOSE)
	{
		menu_number =(int)MENU::END;
	}
	else
	{
		menu_number = retval - 11;
		if (menu_number == (int)MENU::LOGOUT)
		{
			client->SetUserInfo((char*)"\0",(char*)"\0", false);
		}
	}
	size = PackPacket(buf,PROTOCOL::MENU_RESULT,menu_number);
	NetWorkProgram::Instance()->S_Packet_Push(buf,size);
}
bool MainMenuManager::EndProgram()
{
	return true;
}
void  MainMenuManager::ShowResult(HINSTANCE ins, ClientInfo* client)
{
	int size = 0;
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	char temp[MAXBUF];
	ZeroMemory(temp, MAXBUF);
	client->GetData(buf, size);
	UnPackPacket(buf, size,temp);
	MessageBox(NULL, temp, "입장불가", MB_OK);
}
void MainMenuManager::UnPackPacket(const char* buffer, int size,char* data)
{
	const char* ptr = buffer;
	int _size = 0;
	memcpy(&_size, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(data, ptr, _size);
}
int MainMenuManager::PackPacket(char* buffer,PROTOCOL protocol,int& menunumber)
{
	char* ptr = buffer+sizeof(int);
	int size = 0;
	memcpy(ptr, &protocol, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	ptr += sizeof(PROTOCOL);
	memcpy(ptr, &menunumber, sizeof(int));
	size += sizeof(int);
	ptr = buffer;
	memcpy(ptr, &size, sizeof(int));
	size += sizeof(int);
	return size;
}
INT_PTR CALLBACK MainMenuManager::DlgProc1(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ClientInfo* client = (ClientInfo*)lParam;
	int menu_number = 0;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetWindowText(GetDlgItem(hDlg, IDC_IDTXT), client->GetUserInfo()->ID);
		if (client->GetUserInfo()->is_loging)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_RADIO1),false);
			EnableWindow(GetDlgItem(hDlg, IDC_RADIO2), false);
			EnableWindow(GetDlgItem(hDlg, IDC_LOGOUT), true);
			EnableWindow(GetDlgItem(hDlg, IDC_LAUNCHERSTART), true);

			ShowWindow(GetDlgItem(hDlg, IDC_RADIO1), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, IDC_RADIO2), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, IDC_LOGOUT), SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDC_LAUNCHERSTART), SW_SHOW);
		}
		else
		{
			EnableWindow(GetDlgItem(hDlg, IDC_RADIO1), true);
			EnableWindow(GetDlgItem(hDlg, IDC_RADIO2), true);
			EnableWindow(GetDlgItem(hDlg, IDC_LOGOUT), false);
			EnableWindow(GetDlgItem(hDlg, IDC_LAUNCHERSTART), false);

			ShowWindow(GetDlgItem(hDlg, IDC_RADIO1), SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDC_RADIO2), SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDC_LOGOUT), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, IDC_LAUNCHERSTART), SW_HIDE);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			if (IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED)
			{
				menu_number = (int)MENU::LOGIN+11;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
			{
				menu_number = (int)MENU::JOIN + 11;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_LOGOUT) == BST_CHECKED)
			{
				menu_number = (int)MENU::LOGOUT + 11;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_LAUNCHERSTART) == BST_CHECKED)
			{
				menu_number = (int)MENU::LAUNCHERSTART + 11;
			}
			EndDialog(hDlg, menu_number);
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
MainMenuManager::MainMenuManager()
{

}
MainMenuManager::~MainMenuManager()
{

}