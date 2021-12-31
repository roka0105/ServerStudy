#include "global.h"
INT_PTR CALLBACK DlgProc1(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//���θ޴�
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hLogOutBtn = GetDlgItem(hDlg, IDC_LOGOUT);
		hOkBtn = GetDlgItem(hDlg, IDC_OK);
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
INT_PTR CALLBACK DlgProc2(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//�α���
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		//���θ޴� ok��ư false,�α׾ƿ� ��ư false
		EnableWindow(hLogOutBtn, FALSE);
		EnableWindow(hOkBtn, FALSE);
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
			Client->state = STATE::BACKPAGE;
			SetEvent(hWriteEvent);
			if(Client->user->loging)
				EnableWindow(hLogOutBtn, TRUE);
			EnableWindow(hOkBtn, TRUE);
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		return FALSE;
	case WM_CLOSE:
		Client->state = STATE::BACKPAGE;
		SetEvent(hWriteEvent);
		if (Client->user->loging)
			EnableWindow(hLogOutBtn, TRUE);
		EnableWindow(hOkBtn, TRUE);
		EndDialog(hDlg, WM_CLOSE);
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK DlgProc3(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//ȸ������
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		EnableWindow(hLogOutBtn, FALSE);
		EnableWindow(hOkBtn, FALSE);
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
			Client->state = STATE::BACKPAGE;
			SetEvent(hWriteEvent);
			if (Client->user->loging)
				EnableWindow(hLogOutBtn, TRUE);
			EnableWindow(hOkBtn, TRUE);
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		return FALSE;
	case WM_CLOSE:
		Client->state = STATE::BACKPAGE;
		SetEvent(hWriteEvent);
		if (Client->user->loging)
			EnableWindow(hLogOutBtn, TRUE);
		EnableWindow(hOkBtn, TRUE);
		EndDialog(hDlg, WM_CLOSE);
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK DlgProc4(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//�� ����Ʈ
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		EnableWindow(hLogOutBtn, FALSE);
		EnableWindow(hOkBtn, FALSE);
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
			if (Client->user->loging)
				EnableWindow(hLogOutBtn, TRUE);
			EnableWindow(hOkBtn, TRUE);
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		case IDC_CREATEROOM:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG6), NULL, DlgProc6);
			break;
		case IDC_REROOMLIST:
			EndDialog(hDlg, IDC_REROOMLIST);
			Client->state = STATE::ROOMLIST;
			Client->protocol = PROTOCOL::ROOMINFO;
			SetEvent(hWriteEvent);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG4), NULL, DlgProc4);
			break;
		}
		return FALSE;
	case WM_CLOSE:
		Client->state = STATE::BACKPAGE;
		Client->protocol = PROTOCOL::ROOMINFO;
		SetEvent(hWriteEvent);
		if (Client->user->loging)
			EnableWindow(hLogOutBtn, TRUE);
		EnableWindow(hOkBtn, TRUE);
		EndDialog(hDlg, WM_CLOSE);
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK DlgProc5(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//�� ����
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		_hDlg = hDlg;
		SetWindowText(hDlg, roominfo->name);
		hEdit1 = GetDlgItem(hDlg, IDC_EDIT2);
		/*�濡 �ο��� �� á���� state room ���� checkstartgame �ϰ� �ϴ� setevent
		  �̷��� �ϴ� ������ wait msg �Ǵ� gamestart msg�� ���� ���� hedit �ڵ��� �ʿ��ѵ�,
		  ���̾�α׸� ���� ������ �� �޼����� �޾ƿ����� �帧�� �귯���� �ϱ� �����̴�.
		  �� ���ý� �帧
		  1.send thread: state:roomlist->selectroom send & client protocol=checkstartgame.
		  2.recv thread: ������ ������ -> protocol roomresult->inroomsuccess => client state=room & setevent(wait)4�����̾�α�
		  3.4�� ���̾�α׿��� 5�� ���̾�α׸� ���� 4�� ����
		  4.5�� ���̾�αװ� �����鼭 writeevent �������ν� state=ROOM���� protocol=checkstartgame���� ���� send
		  5.����� wait or gamestart msg�� �޾ƿͼ� hedit�� ���.*/
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
		case IDC_BTN4:
			ZeroMemory(buf, MAXBUF);
			GetWindowText(hBtn[3], (LPSTR)buf, MAXBUF);
			Client->game_number = atoi(buf);
			for (int i = 0; i < LIMITNUM; ++i)
			{
				EnableWindow(hBtn[i], FALSE);
			}
			SetEvent(hWriteEvent);
			return TRUE;
		case IDC_BTN5:
			ZeroMemory(buf, MAXBUF);
			GetWindowText(hBtn[4], (LPSTR)buf, MAXBUF);
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
				if (Client->user->loging)
					EnableWindow(hLogOutBtn, TRUE);
				EnableWindow(hOkBtn, TRUE);
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
			if (Client->user->loging)
				EnableWindow(hLogOutBtn, TRUE);
			EnableWindow(hOkBtn, TRUE);
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

INT_PTR CALLBACK DlgProc6(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//�����
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			Client->state = STATE::CREATEROOM;
		    hRoomname = GetDlgItem(hDlg, IDC_ROOMNAME);
			GetWindowText(hRoomname,buf, MAXBUF);
			SetEvent(hWriteEvent);
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