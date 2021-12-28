#include "global.h"
void AddClient(ClientInfo* c)
{
	EnterCriticalSection(&cs);
	printf("[TCP��������] IP:%s PORT:%d\n", inet_ntoa(c->addr.sin_addr), ntohs(c->addr.sin_port));
	c->state = STATE::MAINMENU;
	c->user = new UserInfo();
	Client[ClientCount] = c;
	++ClientCount;
	LeaveCriticalSection(&cs);
}
void RemoveClient(ClientInfo* c)
{
	EnterCriticalSection(&cs);
	printf("[TCP��������] IP:%s PORT:%d\n", inet_ntoa(c->addr.sin_addr), ntohs(c->addr.sin_port));
	for (int i = 0; i < ClientCount; ++i)
	{
		if (Client[i] == c)
		{
			closesocket(c->sock);
			delete c;

			for (int j = i; j < ClientCount - 1; ++j)
			{
				Client[j] = Client[j + 1];
			}
			--ClientCount;
		}
	}
	LeaveCriticalSection(&cs);
}
void AddUser(UserInfo* u)
{
	EnterCriticalSection(&cs);
	User[UserCount++] = u;
	LeaveCriticalSection(&cs);
}
void FileSave()
{
	FILE* fp;
	fp = fopen("UserInfo.dat", "wb");
	if (fp == NULL) err_quit((char*)"�������� ����");
	for (int i = 0; i < UserCount; ++i)
	{
		int retval = fwrite(User[i], 1, sizeof(UserInfo), fp);
		if (retval != sizeof(UserInfo))
		{
			err_quit((char*)"�������� ����");
		}
	}
}
void FileLoad()
{
	FILE* fp = nullptr;
	fp = fopen("UserInfo.dat", "rb");
	if (fp == NULL)return;
	while (1)
	{
		UserInfo info;
		ZeroMemory(&info, sizeof(UserInfo));
		int retval = fread(&info, 1, sizeof(UserInfo), fp);
		if (retval != sizeof(UserInfo))
		{
			break;
		}
		UserInfo* ptr = new UserInfo();
		memcpy(ptr, &info, sizeof(UserInfo));
		User[UserCount++] = ptr;
	}
}
char* RandomRoom_Setname()
{
	int index = rand() % LIMITNUM;
	return Roomname[index];
}
void InitRoom()
{
	RoomCount = LIMITNUM;
	for (int i = 0; i < LIMITNUM; ++i)
	{
		Roomname[i] = new char[MAXBUF]();
	}
	strcpy(Roomname[0], "�ų��� ��ġ����~");
	strcpy(Roomname[1], "�������� ����!");
	strcpy(Roomname[2], "3���Ŀ� ����");
	for (int i = 0; i < LIMITNUM; ++i)
	{
		Room[i] = new RoomInfo();
		Room[i]->attend_count = 0;
		sprintf(Room[i]->name, "%d.%s", i + 1, RandomRoom_Setname());
		Room[i]->number = i + 1;
	}
}
bool LoginCheck(ClientInfo* c, UserInfo* userinfo)
{
	int retval = 0;
	int size = 0;
	//������ �Է����� �ʾ����� ���� ������.
	if (!strcmp(userinfo->ID, "\0") || !strcmp(userinfo->PW, "\0"))
	{
		size = PackPacket(c->sendbuf, PROTOCOL::LOGINRESULT, MSGTYPE::NULL_FAIL, NULL_MSG);
		retval = send(c->sock, c->sendbuf, size, 0);
		if (retval == SOCKET_ERROR)err_display((char*)"login success send()");
		return false;
	}
	//������ �Է��� ���̵�,��й�ȣ�� ��ġ�ϴ� ���� ã��.
	for (int i = 0; i < UserCount; ++i)
	{
		if (!strcmp(User[i]->ID, userinfo->ID) && !strcmp(User[i]->PW, userinfo->PW))
		{
			c->user->loging = true;
			size = PackPacket(c->sendbuf, PROTOCOL::LOGINRESULT, MSGTYPE::LOGINSUCCESS, LOGIN_SUCCESS_MSG);
			retval = send(c->sock, c->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)err_display((char*)"login success send()");
			return true;
		}
	}
	//�����߿� ������ �Է��� ������ ���� ���.
	size = PackPacket(c->sendbuf, PROTOCOL::LOGINRESULT, MSGTYPE::LOGIN_NONE_FAIL, LOGIN_NONE_MSG);
	retval = send(c->sock, c->sendbuf, size, 0);
	if (retval == SOCKET_ERROR)err_display((char*)"login none_err send()");
	return false;
}
bool JoinCheck(ClientInfo* c, UserInfo* userinfo)
{
	int retval = 0;
	int size = 0;
	//������ �Է����� �ʾ����� ���� ������.
	if (!strcmp(userinfo->ID, "\0") || !strcmp(userinfo->PW, "\0") || !strcmp(userinfo->NICK, "\0"))
	{
		size = PackPacket(c->sendbuf, PROTOCOL::JOINRESULT, MSGTYPE::NULL_FAIL, NULL_MSG);
		retval = send(c->sock, c->sendbuf, size, 0);
		if (retval == SOCKET_ERROR)err_display((char*)"join nullfail send()");
		return false;
	}
	//������ ������ �ߺ��ϴ� ������ �ִ��� üũ
	for (int i = 0; i < UserCount; ++i)
	{
		if (!strcmp(User[i]->ID, userinfo->ID))
		{
			size = PackPacket(c->sendbuf, PROTOCOL::JOINRESULT, MSGTYPE::JOIN_SAMEID_FAIL, JOIN_SAMEID_MSG);
			retval = send(c->sock, c->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)err_display((char*)"join samefail send()");
			return false;
		}
		if (!strcmp(User[i]->NICK, userinfo->NICK))
		{
			size = PackPacket(c->sendbuf, PROTOCOL::JOINRESULT, MSGTYPE::JOIN_SAMENICK_FAIL, JOIN_SAMENICK_MSG);
			retval = send(c->sock, c->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)err_display((char*)"join samefail send()");
			return false;
		}
	}
	size = PackPacket(c->sendbuf, PROTOCOL::JOINRESULT, MSGTYPE::JOINSUCCESS, JOIN_SUCCESS_MSG);
	retval = send(c->sock, c->sendbuf, size, 0);
	if (retval == SOCKET_ERROR)err_display((char*)"join success send()");
	return true;
}
bool CheckInRoom(ClientInfo*, int roomindex)
{
	if (Room[roomindex]->attend_count >= LIMITNUM)
	{
		return true;
	}
	else return false;
}
void MenuProcess(ClientInfo* c)
{
	int menunumber = 0;
	int size = 0;
	int retval = 0;
	if (!RecvPacket(c->sock, c->recvbuf))
	{
		c->state = STATE::EXIT;
		return;
	}
	PROTOCOL protocol = GetProtocol(c->recvbuf);
	switch (protocol)
	{
	case PROTOCOL::LOGOUT:
		c->user->loging = false;
		ZeroMemory(c->user, sizeof(UserInfo));
		size = PackPacket(c->sendbuf, PROTOCOL::LOGINRESULT, MSGTYPE::LOGOUT, LOGOUT_MSG);
		retval = send(c->sock, c->sendbuf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			if (retval == SOCKET_ERROR)err_display((char*)"is login true err send()");
		}
		return;
		break;
	case PROTOCOL::LOGINRESULT:
		size = PackPacket(c->sendbuf, PROTOCOL::LOGINRESULT, MSGTYPE::IS_LOGIN_TRUE_ERR, IS_LOGINTRUE_ERR_MSG);
		retval = send(c->sock, c->sendbuf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display((char*)"is login true err send()");
		}
		return;
	case PROTOCOL::ROOMRESULT:
		size = PackPacket(c->sendbuf, PROTOCOL::ROOMRESULT, MSGTYPE::IS_LOGIN_FALSE_ERR, IS_LOGINFALSE_ERR_MSG);
		retval = send(c->sock, c->sendbuf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display((char*)"is loging false err send()");
		}
		return;
		break;
	case PROTOCOL::EXIT:
		c->state = STATE::EXIT;
		return;
		break;
	}
	UnPackPacket(c->recvbuf, menunumber);
	switch ((MENUNUMBER)menunumber)
	{
	case MENUNUMBER::LOGIN:
		c->state = STATE::LOGIN;
		break;
	case MENUNUMBER::JOIN:
		c->state = STATE::JOIN;
		break;
	case MENUNUMBER::LIST:
		c->state = STATE::ROOMLIST;
		break;
	case MENUNUMBER::END:
		c->state = STATE::END;
		break;
	}
}
void LoginProcess(ClientInfo* c)
{
	if (!RecvPacket(c->sock, c->recvbuf))
	{
		c->state = STATE::EXIT;
		return;
	}
	UserInfo* userinfo = new UserInfo();
	PROTOCOL protocol = GetProtocol(c->recvbuf);
	int size;
	int retval;
	switch (protocol)
	{
	case PROTOCOL::USERINFO:
		UnPackPacket(c->recvbuf, userinfo->ID, userinfo->PW, userinfo->NICK);
		//�α��� ���� -> ���θ޴��� ���ư���
		if (LoginCheck(c, userinfo))
		{
			for (int i = 0; i < UserCount; ++i)
			{
				if (!strcmp(userinfo->ID, User[i]->ID))
				{
					memcpy(c->user, User[i], sizeof(UserInfo));
				}
			}
			c->state = STATE::MAINMENU;
			return;
		}
		else
		{
			delete userinfo;
			return;
		}
		break;
	case  PROTOCOL::BACKPAGE:
		c->state = STATE::MAINMENU;
		break;
	case PROTOCOL::EXIT:
		c->state = STATE::EXIT;
		break;
	}
	return;
}
void JoinProcess(ClientInfo* c)
{
	if (!RecvPacket(c->sock, c->recvbuf))
	{
		c->state = STATE::EXIT;
		return;
	}
	UserInfo* userinfo = new UserInfo();
	PROTOCOL protocol = GetProtocol(c->recvbuf);
	switch (protocol)
	{
	case PROTOCOL::USERINFO:
		UnPackPacket(c->recvbuf, userinfo->ID, userinfo->PW, userinfo->NICK);
		//�α��� ���� -> ���θ޴��� ���ư���
		if (JoinCheck(c, userinfo))
		{
			//ȸ������ �߰��ϱ�
			AddUser(userinfo);
			//�����ڵ�����
			FileSave();
			c->state = STATE::MAINMENU;
		}
		else
		{
			return;
		}
		break;
	}
}
void RoomListProcess(ClientInfo* c)
{
	int size = 0;
	int retval = 0;
	int index = 0;
	if (!RecvPacket(c->sock, c->recvbuf))
	{
		c->state = STATE::EXIT;
		return;
	}
	PROTOCOL protocol = GetProtocol(c->recvbuf);
	switch (protocol)
	{
	case PROTOCOL::ROOMINFO:
		//���� �� ����,���̸�����Ʈ,�� ���� ���� �ο���,���� ���� �ο��� ��� �ѹ��� ������
		size = PackPacket(c->sendbuf, PROTOCOL::ROOMINFO, RoomCount, LIMITNUM, Room, c->user);
		retval = send(c->sock, c->sendbuf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display((char*)"room info send()");
		}
		break;
	case PROTOCOL::SELECTROOM:
		UnPackPacket(c->recvbuf, index);
		//�濡 �� �� �ִ��� üũ
		if (CheckInRoom(c, index))
		{
			size = PackPacket(c->sendbuf, PROTOCOL::ROOMRESULT, MSGTYPE::IS_FULL_ROOM_ERR, IS_FULL_ROOM_ERR_MSG);
			retval = send(c->sock, c->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display((char*)"room full err send()");
			}
		}//�� �� ������ �ش� �濡 ����. �濡 ���� �������� Ŭ�� ������.
		else
		{
			EnterCriticalSection(&cs);
			RoomInfo* room = Room[index];
			room->client[room->attend_count] = c;
			++room->attend_count;
			c->room = Room[index];
			LeaveCriticalSection(&cs);
			size = PackPacket(c->sendbuf, PROTOCOL::ROOMRESULT, MSGTYPE::IN_ROOM_SUCCESS, NULL);
			retval = send(c->sock, c->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display((char*)"select room info send()");
			}
			c->state = STATE::ROOM;
			return;
		}
		break;
	case PROTOCOL::BACKPAGE:
		c->state = STATE::MAINMENU;
		return;
	}
}
void RoomProcess(ClientInfo* c)
{
	int size = 0;
	int retval = 0;
	int index = 0;
	if (!RecvPacket(c->sock, c->recvbuf))
	{
		c->state = STATE::EXIT;
		return;
	}
	PROTOCOL protocol = GetProtocol(c->recvbuf);
	switch (protocol)
	{
		//case PROTOCOL::STARTGAME:
		//	c->state = STATE::GAMESTART;
		//	return;
		//	break;
	case PROTOCOL::CHECKSTARTGAME:
		if (c->room->attend_count == LIMITNUM)
		{
			for (int i = 0; i < LIMITNUM; ++i)
			{
				ClientInfo* client = c->room->client[i];
				size = PackPacket(client->sendbuf, PROTOCOL::ROOMRESULT, MSGTYPE::STARTGAME, STARTGAME_MSG);
				retval = send(client->sock, client->sendbuf, size, 0);

				if (retval == SOCKET_ERROR)
				{
					err_display((char*)"startgame send()");
				}
				//client->state = STATE::GAMESTART;
			}
			c->state = STATE::GAMESTART;
			return;
		}
		else
		{
			if (game == NULL)
				game = new GameInfo();

			size = PackPacket(c->sendbuf, PROTOCOL::ROOMRESULT, MSGTYPE::WAITROOM, WAITGAME_MSG);
			retval = send(c->sock, c->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display((char*)"wait room send()");
			}
			c->state = STATE::GAMESTART;
			return;
		}
		break;
	case PROTOCOL::BACKPAGE:
		//�� ������.	
		c->room->attend_count--;
		EnterCriticalSection(&cs);
		for (int i = 0; i < c->room->attend_count + 1; ++i)
		{
			if (c->room->client[i] == c)
			{
				c->room->client[i] = NULL;
				c->room = NULL;
				break;
			}
		}

		LeaveCriticalSection(&cs);
		c->state = STATE::ROOMLIST;

		return;
	}
}
void GameStartProcess(ClientInfo* c)
{
	int size = 0;
	int retval = 0;
	char temp[MAXBUF];
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	ZeroMemory(temp, MAXBUF);
	if (game->befor_client!=NULL&&game->befor_client->game_number == LIMITNUM - 1)
	{
		strcpy(game->lose_name[game->lose_count], c->user->NICK);
		++game->lose_count;
		for (int i = 0; i < c->room->attend_count; ++i)
		{
			SetEvent(c->room->client[i]->hWaitEvent);
		}
		c->state = STATE::END;
		return;
	}
	if (!RecvPacket(c->sock, c->recvbuf))
	{
		c->state = STATE::EXIT;
		return;
	}
	PROTOCOL protocol = GetProtocol(c->recvbuf);
	switch (protocol)
	{
	case PROTOCOL::STARTGAME:
		//Ŭ�󿡼� �Է��� ��ư������ �Ǵ�
		UnPackPacket(c->recvbuf, c->game_number);
		if (game->start_time != 0)
		{
			game->end_time = time(NULL);
		}
		//���� Ŭ�� �Է��� ���ڰ� ���� Ŭ�� �Է��� ���ڿ� ���ٸ�
		if (game->befor_client != NULL)
		{
			if (c->game_number <= game->befor_client->game_number)
			{
				//���� �ð� �ȿ� ���� �Է��� ���ƴٸ�(�Ѵ� Ż������ üũ)
				if ((game->end_time - game->start_time) <= LIMITTIME)
				{
					strcpy(game->lose_name[game->lose_count], c->user->NICK);
					++game->lose_count;
					for (int i = 0; i < c->room->attend_count; ++i)
					{
						SetEvent(c->room->client[i]->hWaitEvent);
					}
					c->state = STATE::END;
					return;
				}

				strcpy(game->lose_name[0], c->user->NICK);
				for (int i = 0; i < c->room->attend_count; ++i)
				{
					SetEvent(c->room->client[i]->hWaitEvent);
				}
				c->state = STATE::END;
				return;
			}
		}
		//Ż���� �ƴ� ��
		game->befor_client = c;
		game->game_number = c->game_number;
		game->start_time = time(NULL);
		strcpy(temp, GAMENUMBER_MSG);
		sprintf(buf, temp, c->user->NICK, game->game_number);
		for (int i = 0; i < c->room->attend_count; ++i)
		{
			size = PackPacket(c->room->client[i]->sendbuf, PROTOCOL::ROOMRESULT, MSGTYPE::GAMENUMBER, buf);
 			retval = send(c->room->client[i]->sock, c->room->client[i]->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				c->state = STATE::EXIT;
				return;
			}
		}
		WaitForSingleObject(c->hWaitEvent, INFINITE);
		printf("������õ\n");
		c->state = STATE::END;
		return;
		break;
	case PROTOCOL::BACKPAGE:

		break;
	}
}
void EndProcess(ClientInfo* c)
{
	int size = 0;
	int retval = 0;
	char temp[MAXBUF];
	ZeroMemory(temp, MAXBUF);

	for (int i = 0; i < game->lose_count; ++i)
	{
		if (!strcmp(c->user->NICK, game->lose_name[i]))
		{
			strcpy(temp, FAIMENUMBER_MSG);
		}
		else strcpy(temp, SUCCESSNUMBER_MSG);
		sprintf(temp, GAMERESULT_MSG, game->game_number);

		strcat(temp, game->lose_name[i]);
		if (i != game->lose_count - 1)
			strcat(temp, ",");
	}
	strcat(temp, "\r\n");
	//Ŭ�� ����޼��� ������
	size = PackPacket(c->sendbuf, PROTOCOL::ROOMRESULT, MSGTYPE::GAMERESULT, temp);
	retval = send(c->sock, c->sendbuf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		c->state = STATE::EXIT;
		return;
	}
	ZeroMemory(game, sizeof(GameInfo));
	delete(game);
	c->state = STATE::MAINMENU;
	return;
}
void ExitProcess(ClientInfo* c)
{
	int size = 0;
	int retval = 0;
	size = PackPacket(c->sendbuf, PROTOCOL::EXIT);
	retval = send(c->sock, c->sendbuf, size, 0);
	if (retval == SOCKET_ERROR)
		err_display((char*)"send()");
	return;
}