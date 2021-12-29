#include "global.h"
INT_PTR CALLBACK DlgProc1(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//메인메뉴
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hLogOutBtn = GetDlgItem(hDlg, IDC_LOGOUT);
		EnableWindow(hLogOutBtn, FALSE);
		CheckDlgButton(hDlg, IDC_LOGIN, BST_UNCHECKED);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:
			WaitForSingleObject(hReadEvent, INFINITE);
			Client->state = STATE::MAINMENU;
			if (IsDlgButtonChecked(hDlg, IDC_LOGIN) == BST_CHECKED)
			{
				MenuSelect = 1;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_JOIN) == BST_CHECKED)
			{
				MenuSelect = 2;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_LIST) == BST_CHECKED)
			{
				MenuSelect = 3;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_END) == BST_CHECKED)
			{
				MenuSelect = 4;
			}
			SetEvent(hWriteEvent);
			switch (MenuSelect)
			{
			case 1:
				if (Client->user->loging)
					return TRUE;
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), NULL, DlgProc2);
				break;
			case 2:
				if (Client->user->loging)
					return TRUE;
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), NULL, DlgProc3);
				break;
			case 3:
				if (Client->user->loging)
					DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG4), NULL, DlgProc4);
				break;
			case 4:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG5), NULL, DlgProc5);
				break;
			}
			return TRUE;
		case IDC_LOGOUT:
			Client->user->loging = false;
			Client->state = STATE::LOGOUT;
			SetEvent(hWriteEvent);
			return TRUE;
		}
		return FALSE;
	case WM_CLOSE:
		Client->state = STATE::EXIT;
		SetEvent(hWriteEvent);
		EndDialog(hDlg, WM_CLOSE);
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK DlgProc2(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//로그인
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hErrStr = GetDlgItem(hDlg, IDC_ERR);
		_hDlg = hDlg;
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:
			GetDlgItemText(hDlg, IDC_ID, buf, MAXBUF);
			GetDlgItemText(hDlg, IDC_PW, buf2, MAXBUF);
			GetDlgItemText(hDlg, IDC_NICKNAME, buf3, MAXBUF);
			SetEvent(hWriteEvent);
			SetDlgItemText(hDlg, IDC_ERR, buf4);
			SetDlgItemText(hDlg, IDC_ID, "");
			SetDlgItemText(hDlg, IDC_PW, "");
			return TRUE;
		case IDCANCEL:
			//Client->state = STATE::BACKPAGE;
			//SetEvent(hWriteEvent);
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		return FALSE;
	case WM_CLOSE:
		Client->state = STATE::EXIT;
		SetEvent(hWriteEvent);
		EndDialog(hDlg, WM_CLOSE);
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK DlgProc3(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//회원가입
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hErrStr = GetDlgItem(hDlg, IDC_ERR);
		_hDlg = hDlg;
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			GetDlgItemText(hDlg, IDC_ID, buf, MAXBUF);
			GetDlgItemText(hDlg, IDC_PW, buf2, MAXBUF);
			GetDlgItemText(hDlg, IDC_NICKNAME, buf3, MAXBUF);
			SetEvent(hWriteEvent);
			SetDlgItemText(hDlg, IDC_ERR, buf4);
			SetDlgItemText(hDlg, IDC_ID, "");
			SetDlgItemText(hDlg, IDC_PW, "");
			SetDlgItemText(hDlg, IDC_NICKNAME, "");
			return TRUE;
		case IDCANCEL:
			//Client->state = STATE::BACKPAGE;
			//SetEvent(hWriteEvent);
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		return FALSE;
	case WM_CLOSE:
		Client->state = STATE::EXIT;
		SetEvent(hWriteEvent);
		EndDialog(hDlg, WM_CLOSE);
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK DlgProc4(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//방 리스트
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hList = GetDlgItem(hDlg, IDC_ROOMLIST);
		hUserinfo_nick = GetDlgItem(hDlg, IDC_NICKNAME);
		_hDlg = hDlg;
		Client->protocol = PROTOCOL::ROOMINFO;
		SetEvent(hWriteEvent);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			WaitForSingleObject(hReadEvent, INFINITE);
			Roomindex = SendMessage(hList, LB_GETCURSEL, 0, 0);
			SetEvent(hWriteEvent);
			WaitForSingleObject(hWaitEvent, INFINITE);
			if (Client->state == STATE::ROOM)
			{
				EndDialog(hDlg, IDCANCEL);
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG5), NULL, DlgProc5);
			}
			return TRUE;
		case IDCANCEL:
			Client->state = STATE::BACKPAGE;
			Client->protocol = PROTOCOL::ROOMINFO;
			SetEvent(hWriteEvent);
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		return FALSE;
	case WM_CLOSE:
		Client->state = STATE::EXIT;
		SetEvent(hWriteEvent);
		EndDialog(hDlg, WM_CLOSE);
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK DlgProc5(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//방 입장
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		_hDlg = hDlg;
		SetWindowText(hDlg, roominfo->name);
		hEdit1 = GetDlgItem(hDlg, IDC_EDIT2);
		/*방에 인원이 다 찼는지 state room 에서 checkstartgame 하게 하는 setevent
		  이렇게 하는 이유는 wait msg 또는 gamestart msg를 띄우기 위해 hedit 핸들이 필요한데,
		  다이어로그를 먼저 열고나서 그 메세지를 받아오도록 흐름이 흘러가야 하기 때문이다.
		  방 선택시 흐름
		  1.send thread: state:roomlist->selectroom send & client protocol=checkstartgame.
		  2.recv thread: 방입장 성공시 -> protocol roomresult->inroomsuccess => client state=room & setevent(wait)4번다이어로그
		  3.4번 다이어로그에서 5번 다이어로그를 열며 4번 닫음
		  4.5번 다이어로그가 열리면서 writeevent 켜짐으로써 state=ROOM에서 protocol=checkstartgame으로 가서 send
		  5.결과값 wait or gamestart msg를 받아와서 hedit에 출력.*/
		SetEvent(hWriteEvent);
		char temp[MAXBUF];
		ZeroMemory(temp, MAXBUF);
		for (int i = 0; i < LIMITNUM; ++i)
		{
			sprintf(temp, "%d", i + 1);
			hBtn[i] = CreateWindow((LPCSTR)"BUTTON", (LPCSTR)temp, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				50 + (i * 60), 150, 50, 50, hDlg, (HMENU)(IDC_BTN1 + i), (HINSTANCE)GetWindowLongPtr(hDlg, GWLP_HINSTANCE),
				NULL);
			EnableWindow(hBtn[i], FALSE);
		}
		Client->game_number = -1;
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTN1:
			ZeroMemory(buf, MAXBUF);
			GetWindowText(hBtn[0], (LPSTR)buf, MAXBUF);
			Client->game_number = atoi(buf);
			for (int i = 0; i < LIMITNUM; ++i)
			{
				EnableWindow(hBtn[i], FALSE);
			}
			SetEvent(hWriteEvent);
			return TRUE;
		case IDC_BTN2:
			ZeroMemory(buf, MAXBUF);
			GetWindowText(hBtn[1], (LPSTR)buf, MAXBUF);
			Client->game_number = atoi(buf);
			for (int i = 0; i < LIMITNUM; ++i)
			{
				EnableWindow(hBtn[i], FALSE);
			}
			SetEvent(hWriteEvent);
			return TRUE;
		case IDC_BTN3:
			ZeroMemory(buf, MAXBUF);
			GetWindowText(hBtn[2], (LPSTR)buf, MAXBUF);
			Client->game_number = atoi(buf);
			for (int i = 0; i < LIMITNUM; ++i)
			{
				EnableWindow(hBtn[i], FALSE);
			}
			SetEvent(hWriteEvent);
			return TRUE;
		case IDCANCEL:
			if (Client->state == STATE::END)
			{
				Client->state = STATE::MAINMENU;
				SetEvent(hReadEvent);
				EndDialog(hDlg, IDCANCEL);
				return TRUE;
			}
			Client->state = STATE::BACKPAGE;
			Client->protocol = PROTOCOL::ROOMINFO;
			SetEvent(hWriteEvent);
			EndDialog(hDlg, IDCANCEL);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG4), NULL, DlgProc4);
			return TRUE;
		}
		return FALSE;
	case WM_CLOSE:
		if (Client->state == STATE::END)
		{
			Client->state = STATE::MAINMENU;	
			SetEvent(hReadEvent);
			EndDialog(hDlg, WM_CLOSE);
			return TRUE;
		}
		Client->state = STATE::BACKPAGE;
		Client->protocol = PROTOCOL::ROOMINFO;
		SetEvent(hWriteEvent);
		EndDialog(hDlg, WM_CLOSE);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG4), NULL, DlgProc4);
		return TRUE;
	}
	return FALSE;
}