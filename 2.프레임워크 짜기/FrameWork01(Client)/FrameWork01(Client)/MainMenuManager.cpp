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
	client->recvbuf.MemoryZero();
	int retval = DialogBox(ins, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc1);
	if (retval == IDCANCEL || retval == WM_CLOSE)
	{
		menu_number =(int)MENU::END;
	}
	else
	{
		menu_number = retval - 11;
	}
	this->PackPacket(menu_number);
	client->sendbuf.PackPacket(PROTOCOL::MENU_RESULT, this->tempbuf.Data_Pop(), this->tempbuf.Size_Pop());
	NetWorkProgram::Instance()->S_Packet_Push(client->sendbuf.Data_Pop(),client->sendbuf.Size_Pop());
	client->sendbuf.MemoryZero();
}
bool MainMenuManager::EndProgram()
{
	return true;
}
void  MainMenuManager::ShowResult(HINSTANCE ins, ClientInfo* client)
{
	tempbuf.MemoryZero();
	NetWorkProgram::Instance()->R_Packet_Pop(client->recvbuf.Data_Pop(),client->recvbuf.Size_Pop());
	UnPackPacket(client->recvbuf.Data_Pop(), client->recvbuf.Size_Pop());
	MessageBox(NULL, tempbuf.Data_Pop(), "입장불가", MB_OK);
	client->recvbuf.MemoryZero();
}
void MainMenuManager::UnPackPacket(const char* buffer, int size)
{
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	const char* ptr = buffer + sizeof(PROTOCOL);
	int _size = 0;
	memcpy(&_size, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(buf, ptr, _size);
	tempbuf.Data_Push(buf,_size);
}
void MainMenuManager::PackPacket(int& menunumber)
{
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	char* ptr = buf;
	memcpy(ptr, &menunumber, sizeof(int));
	ptr = buf;
	tempbuf.Data_Push(ptr,sizeof(int));
}
INT_PTR CALLBACK MainMenuManager::DlgProc1(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int menu_number = 0;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			if (IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED)
			{
				menu_number = 12;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
			{
				menu_number = 13;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)
			{
				menu_number = 14;
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