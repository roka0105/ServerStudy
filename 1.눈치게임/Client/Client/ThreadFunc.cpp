#include "global.h"
DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval = 0;
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)return -1;
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		err_quit((char*)"socket()");
	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(SERVERIP);
	addr.sin_port = htons(SERVERPORT);
	Client = new ClientInfo(sock, addr);
	retval = connect(Client->sock, (SOCKADDR*)&Client->addr, sizeof(ClientInfo));
	if (retval == SOCKET_ERROR)err_quit((char*)"connect()");
	hThread[1] = CreateThread(0, NULL, RecvThread, NULL, 0, NULL);

	bool endflag = false;
	int size = 0;
	while (1)
	{
		WaitForSingleObject(hWriteEvent, INFINITE);
		ZeroMemory(Client->sendbuf, MAXBUF);
		switch (Client->state)
		{
		case STATE::MAINMENU:
			//로그인중일때
			if (Client->user->loging && (MenuSelect == 1 || MenuSelect == 2))
			{
				size = PackPacket(Client->sendbuf, PROTOCOL::LOGINRESULT);
				retval = send(Client->sock, Client->sendbuf, size, 0);
				if (retval == SOCKET_ERROR)
				{
					err_display((char*)"send()");
					Client->state = STATE::EXIT;
				}
				break;
			}
			//로그인 안한 상태에서 게임 입장시도할때
			else if (!Client->user->loging && MenuSelect == 3)
			{
				size = PackPacket(Client->sendbuf, PROTOCOL::ROOMRESULT);
				retval = send(Client->sock, Client->sendbuf, size, 0);
				if (retval == SOCKET_ERROR)
				{
					err_display((char*)"send()");
					Client->state = STATE::EXIT;
				}
				break;
			}
			size = PackPacket(Client->sendbuf, PROTOCOL::MENUNUMBER, MenuSelect);
			retval = send(Client->sock, Client->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display((char*)"send()");
				Client->state = STATE::EXIT;
			}
			switch (MenuSelect)
			{
			case 1:
				Client->state = STATE::LOGIN;
				break;
			case 2:
				Client->state = STATE::JOIN;
				break;
			case 3:
				Client->state = STATE::ROOMLIST;
				break;
			case 4:
				Client->state = STATE::END;
				break;
			}
			break;
		case STATE::LOGOUT:
			size = PackPacket(Client->sendbuf, PROTOCOL::LOGOUT);
			retval = send(Client->sock, Client->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display((char*)"send()");
				Client->state = STATE::EXIT;
			}
			break;
		case STATE::LOGIN:
			size = PackPacket(Client->sendbuf, PROTOCOL::USERINFO, buf, buf2, buf3);
			retval = send(Client->sock, Client->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display((char*)"send()");
				Client->state = STATE::EXIT;
			}
			break;
		case STATE::JOIN:
			size = PackPacket(Client->sendbuf, PROTOCOL::USERINFO, buf, buf2, buf3);
			retval = send(Client->sock, Client->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display((char*)"send()");
				Client->state = STATE::EXIT;
			}
			break;
		case STATE::ROOMLIST:
			switch (Client->protocol)
			{
			case PROTOCOL::ROOMINFO:
				size = PackPacket(Client->sendbuf, PROTOCOL::ROOMINFO);
				retval = send(Client->sock, Client->sendbuf, size, 0);
				if (retval == SOCKET_ERROR)
				{
					err_display((char*)"send()");
					Client->state = STATE::EXIT;
				}
				Client->protocol = PROTOCOL::SELECTROOM;
				break;
			case PROTOCOL::SELECTROOM:
				size = PackPacket(Client->sendbuf, PROTOCOL::SELECTROOM, Roomindex);
				retval = send(Client->sock, Client->sendbuf, size, 0);
				if (retval == SOCKET_ERROR)
				{
					err_display((char*)"send()");
					Client->state = STATE::EXIT;
				}
				Client->protocol = PROTOCOL::CHECKSTARTGAME;
				break;
				/*	case PROTOCOL::STARTGAME:
						size = PackPacket(Client->sendbuf, PROTOCOL::STARTGAME);
						retval = send(Client->sock, Client->sendbuf, size, 0);
						if (retval == SOCKET_ERROR)
						{
							err_display((char*)"send()");
							Client->state = STATE::EXIT;
						}
						Client->state = STATE::ROOM;
						break;*/
			}
			break;
		case STATE::ROOM:
			switch (Client->protocol)
			{
			case PROTOCOL::CHECKSTARTGAME:
				size = PackPacket(Client->sendbuf, PROTOCOL::CHECKSTARTGAME);
				retval = send(Client->sock, Client->sendbuf, size, 0);
				if (retval == SOCKET_ERROR)
				{
					err_display((char*)"send()");
					Client->state == STATE::EXIT;
				}
				break;
			case PROTOCOL::STARTGAME:
				break;
			}
			break;
		case STATE::CREATEROOM:
			size = PackPacket(Client->sendbuf, PROTOCOL::CREATEROOM, buf);
			retval = send(Client->sock, Client->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display((char*)"send()");
				Client->state == STATE::EXIT;
			}
			Client->state = STATE::ROOMLIST;
			Client->protocol = PROTOCOL::ROOMINFO;
			break;
		case STATE::REROOMLISTINFO:
			break;
		case STATE::GAMESTART:
			size = PackPacket(Client->sendbuf, PROTOCOL::STARTGAME, Client->game_number);
			retval = send(Client->sock, Client->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display((char*)"send()");
				Client->state == STATE::EXIT;
			}
			//Client->state = STATE::END;
			break;
		case STATE::BACKPAGE:
			if (Client->protocol == PROTOCOL::ROOMINFO)
			{
				Client->state = STATE::ROOMLIST;
			}
			else 
			{
				Client->state = STATE::MAINMENU;
			}
			size = PackPacket(Client->sendbuf, PROTOCOL::BACKPAGE);
			retval = send(Client->sock, Client->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display((char*)"send()");
				Client->state = STATE::EXIT;
			}
			break;
		case STATE::END2:
			size = PackPacket(Client->sendbuf, PROTOCOL::END);
			retval = send(Client->sock, Client->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display((char*)"send()");
			}
			Client->state = STATE::END;
			break;
		case STATE::EXIT:
			size = PackPacket(Client->sendbuf, PROTOCOL::EXIT);
			retval = send(Client->sock, Client->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display((char*)"send()");
			}
			endflag = true;
			break;
		}
		SetEvent(hReadEvent);
		if (endflag)
			break;
	}
	return 0;
}
DWORD CALLBACK RecvThread(LPVOID arg)
{
	bool endflag = false;
	MSGTYPE type;
	while (1)
	{
		ZeroMemory(Client->recvbuf, MAXBUF);
		if (!RecvPacket(Client->sock, Client->recvbuf))
		{
			Client->state = STATE::EXIT;
			return 0;
		}

		PROTOCOL protocol = GetProtocol(Client->recvbuf);
		switch (protocol)
		{
		case PROTOCOL::LOGINRESULT:
			UnPackPacket(Client->recvbuf, &type, buf4);
			switch (type)
			{
				ZeroMemory(buf4, MAXBUF);
			case MSGTYPE::IS_LOGIN_TRUE_ERR:
				MessageBox(NULL, buf4, "로그인 불가", MB_OK);
				//SendMessage(_hDlg, WM_COMMAND, MAKEWORD(IDCANCEL, 0), NULL);
				break;
			case MSGTYPE::LOGINSUCCESS:
				EnableWindow(hLogOutBtn, TRUE);
				Client->user->loging = true;
				strcpy(Client->user->ID, buf);
				strcpy(Client->user->PW, buf2);
				strcpy(Client->user->NICK, buf3);
				MessageBox(NULL, buf4, "로그인 성공", MB_OK);
				SendMessage(_hDlg, WM_COMMAND, MAKEWORD(IDCANCEL, 0), NULL);
				break;
			case MSGTYPE::LOGIN_NONE_FAIL:
				SetWindowText(hErrStr, buf4);
				break;
			case MSGTYPE::NULL_FAIL:
				SetWindowText(hErrStr, buf4);
				break;
			case MSGTYPE::LOGOUT:
				EnableWindow(hLogOutBtn, FALSE);
				MessageBox(NULL, buf4, "로그아웃 성공", MB_OK);
				break;
			}
			ZeroMemory(buf4, MAXBUF);
			break;
		case PROTOCOL::JOINRESULT:
			ZeroMemory(buf4, MAXBUF);
			UnPackPacket(Client->recvbuf, &type, buf4);
			switch (type)
			{
			case MSGTYPE::JOINSUCCESS:
				strcpy(Client->user->ID, buf);
				strcpy(Client->user->PW, buf2);
				strcpy(Client->user->NICK, buf3);
				MessageBox(NULL, buf4, "로그인 성공", MB_OK);
				SendMessage(_hDlg, WM_COMMAND, MAKEWORD(IDCANCEL, 0), NULL);
				break;
				//여기 왜 멈추는지 모르겠음.
			case MSGTYPE::JOIN_SAMEID_FAIL:
				SetWindowText(hErrStr, buf4);
				break;
			case MSGTYPE::JOIN_SAMENICK_FAIL:
				SetWindowText(hErrStr, buf4);
				break;
			}
			ZeroMemory(buf4, MAXBUF);
			break;
		case PROTOCOL::ROOMRESULT:
			ZeroMemory(buf4, MAXBUF);
			UnPackPacket(Client->recvbuf, &type, buf4);
			switch (type)
			{
			case MSGTYPE::IS_LOGIN_FALSE_ERR:
				MessageBox(NULL, buf4, "입장불가", MB_OK);
				break;
			case MSGTYPE::IS_FULL_ROOM_ERR:
				MessageBox(NULL, buf4, "입장불가", MB_OK);
				break;
			case MSGTYPE::IN_ROOM_SUCCESS:
				Client->state = STATE::ROOM;
				SetEvent(hWaitEvent);
				break;
			case MSGTYPE::STARTGAME:
				//시작한다 메세지 출력
				SendMessage(hEdit1, EM_REPLACESEL, 0, (LPARAM)buf4);
				Client->state = STATE::GAMESTART;
				for (int i = 0; i < LIMITNUM; ++i)
				{
					EnableWindow(hBtn[i], TRUE);
				}
				break;
			case MSGTYPE::WAITROOM:
				//기다려라 출력
				SendMessage(hEdit1, EM_REPLACESEL, 0, (LPARAM)buf4);
				break;
			case MSGTYPE::GAMENUMBER:
				//SetWindowText(hEdit1, (LPCSTR)buf4);
				SendMessage(hEdit1, EM_REPLACESEL, 0, (LPARAM)buf4);
				break;
			case MSGTYPE::WRONGNUMBER:
				SendMessage(hEdit1, EM_REPLACESEL, 0, (LPARAM)buf4);
				Client->state = STATE::GAMESTART;
				for (int i = 0; i < LIMITNUM; ++i)
				{
					EnableWindow(hBtn[i], TRUE);
				}
				break;
			case MSGTYPE::GAMERESULT:
				SendMessage(hEdit1, EM_REPLACESEL, 0, (LPARAM)buf4);
				for(int i=0;i<LIMITNUM;++i)
				EnableWindow(hBtn[i], FALSE);
				Client->state = STATE::END;
				break;
			}
			ZeroMemory(buf4, MAXBUF);
			break;
		case PROTOCOL::ROOMINFO:
			UnPackPacket(Client->recvbuf, Roomcount, LimitAttend, roominfo, Client->user);
			char temp[MAXBUF];
			ZeroMemory(temp, MAXBUF);
			for (int i = 0; i < Roomcount; ++i)
			{
				SendMessage(hList, LB_DELETESTRING, i, (LPARAM)"");
				sprintf(temp, "%d.%s (%d/%d)",i+1,roominfo[i].name, roominfo[i].attend_count, LIMITNUM);
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)temp);
			}
			SetWindowText(hUserinfo_nick, Client->user->NICK);
			break;
		case PROTOCOL::END:
			Client->state = STATE::END2;
			SetEvent(hWriteEvent);
			break;
		case PROTOCOL::EXIT:
			endflag = true;
			break;
		}
		if (endflag)
			break;
	}
	return 0;
}