#include "MainMenuManager.h"
MainMenuManager* MainMenuManager::instance = nullptr;
MainMenuManager* MainMenuManager::Instance()
{
	if (instance == NULL)
	{
		instance = new MainMenuManager();
	}
	return instance;
}
void  MainMenuManager::MenuSelect(HINSTANCE ins, ClientInfo* client)
{
	dialog_param = new MainDialogMem(this,client);
	//DialogBox(ins, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc1);
	DialogBoxParam(ins, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc1,(LPARAM)dialog_param);
}
void  MainMenuManager::ShowResult(HINSTANCE ins,ClientInfo* client)
{
	tempbuf->MemoryZero();
	UnPackPacket(client->recvbuf.Data_Pop(), client->recvbuf.Size_Pop());
	MessageBox(NULL, L"입장불가", (LPCWSTR)tempbuf->Data_Pop(), MB_OK);
}
void MainMenuManager::UnPackPacket(const char* buffer, int size)
{
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	const char* ptr = buffer + sizeof(PROTOCOL);
	int _size = size - sizeof(PROTOCOL);
	memcpy(buf, ptr, _size);
	tempbuf->Data_Push(buf);
}
void MainMenuManager::PackPacket(int& menunumber)
{
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	char* ptr = buf;
	memcpy(ptr, &menunumber, sizeof(int));
	ptr = buf;

	tempbuf->Data_Push(ptr);

}
INT_PTR CALLBACK MainMenuManager::DlgProc1(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int menu_number = 0;
	MainDialogMem* dialogmem;
	ClientInfo* client;
	MainMenuManager* main_manager;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		dialogmem = (MainDialogMem*)lParam;
		client = dialogmem->Client;
		main_manager = dialogmem->This;
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			if (IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED)
			{
				menu_number = 1;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
			{
				menu_number = 2;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)
			{
				menu_number = 3;
			}
			main_manager->PackPacket(menu_number);
			client->sendbuf.PackPacket
			(MENU_RESULT, main_manager->tempbuf->Data_Pop(), main_manager->tempbuf->Size_Pop());
			return TRUE;
		case IDCANCEL:
			return TRUE;
		}
		return FALSE;
	case WM_CLOSE:
		return TRUE;
	}
	return FALSE;
}