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
	for (int i = 0; i < UserCount; ++i)
	{
		if (!strcmp(c->user->ID, User[i]->ID))
		{
			User[i]->loging = false;
		}
	}
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
void RemoveRoom(RoomInfo* room)
{
	char temp[MAXBUF];
	ZeroMemory(temp, MAXBUF);
	EnterCriticalSection(&cs);
	for (int i = 0; i < RoomCount; ++i)
	{
		if (Room[i] == room)
		{
			room->game->befor_client = NULL;
			delete room->game->befor_client;
			room->game->Last_client = NULL;
			delete room->game->Last_client;
			room->game = NULL;
			delete room->game;
			delete room;
			for (int j = i; j < RoomCount - 1; ++j)
			{
				Room[j] = Room[j + 1];
				/*char* name = strtok(Room[j]->name, ".");
				name = strtok(NULL, name);
				sprintf(temp, "%d.", j + 1);
				strcat(temp, name);
				strcpy(Room[j]->name, temp);*/
			}
			--RoomCount;
			break;
		}
	}
	LeaveCriticalSection(&cs);
}
void CreateRoom(char* roomname)
{
	EnterCriticalSection(&cs);
	RoomInfo* room = new RoomInfo();
	ZeroMemory(room->name, MAXBUF);
	strcpy(room->name, roomname);
	Room[RoomCount] = room;
	++RoomCount;
	LeaveCriticalSection(&cs);
}
void UserLogOut(bool all, ClientInfo* c)
{
	for (int i = 0; i < UserCount; ++i)
	{
		if (all)
		{
			User[i]->loging = false;
		}
		else
		{
			if (c != NULL && !strcmp(c->user->ID, User[i]->ID))
			{
				User[i]->loging = false;
			}
		}
	}
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
	int index = rand() % 3;
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
		sprintf(Room[i]->name, "%s", RandomRoom_Setname());
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
		{   //�̹� �α��� ���� ���
			if (User[i]->loging)
			{
				size = PackPacket(c->sendbuf, PROTOCOL::LOGINRESULT, MSGTYPE::NULL_FAIL, LOGING_TRUE_MSG);
				retval = send(c->sock, c->sendbuf, size, 0);
				if (retval == SOCKET_ERROR)err_display((char*)"login success send()");
				return false;
			}//�α��� ������
			else
			{
				size = PackPacket(c->sendbuf, PROTOCOL::LOGINRESULT, MSGTYPE::LOGINSUCCESS, LOGIN_SUCCESS_MSG);
				retval = send(c->sock, c->sendbuf, size, 0);
				if (retval == SOCKET_ERROR)err_display((char*)"login success send()");
				return true;
			}
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
void SendGameNumber(ClientInfo* c)
{
	int size = 0;
	int retval = 0;
	char temp[MAXBUF];
	char buf[MAXBUF];
	ZeroMemory(temp, MAXBUF);
	ZeroMemory(buf, MAXBUF);
	strcpy(temp, GAMENUMBER_MSG);
	sprintf(buf, temp, c->user->NICK, c->game_number);
	size = PackPacket(c->sendbuf, PROTOCOL::ROOMRESULT, MSGTYPE::GAMENUMBER, buf);
	for (int i = 0; i < c->room->attend_count; ++i)
	{
		retval = send(c->room->client[i]->sock, c->sendbuf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			c->state = STATE::EXIT;
			return;
		}
	}
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
		for (int i = 0; i < UserCount; ++i)
		{
			if (!strcmp(c->user->ID, User[i]->ID))
			{
				User[i]->loging = false;
				break;
			}
		}
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
					User[i]->loging = true;
					memcpy(c->user, User[i], sizeof(UserInfo));
					break;
				}
			}
			//c->state = STATE::MAINMENU;
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
			//c->state = STATE::MAINMENU;
		}
		else
		{
			return;
		}
		break;
	case PROTOCOL::BACKPAGE:
		c->state = STATE::MAINMENU;
		break;
	}
}
void RoomListProcess(ClientInfo* c)
{
	int size = 0;
	int retval = 0;
	int index = 0;
	char temp[MAXBUF];
	ZeroMemory(temp, MAXBUF);
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
	case PROTOCOL::CREATEROOM:
		UnPackPacket(c->recvbuf, temp);
		CreateRoom(temp);
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
		if (c->room->game != NULL)
		{
			ZeroMemory(c->room->game, sizeof(GameInfo));
			delete c->room->game;
			c->room->game = NULL;
		}
		if (c->room->attend_count == LIMITNUM)
		{
			if (c->room->game == NULL)
			{
				hThread2 = CreateThread(0, NULL, TimerThread, c, 0, NULL);
				CloseHandle(hThread2);
				c->room->game = new GameInfo();
				c->room->game->Timer = LIMITTIME;
				for (int i = 0; i < LIMITNUM; ++i)
				{
					c->room->game->hTimerEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
				}
				c->room->game->hTimerStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			}
			for (int i = 0; i < LIMITNUM; ++i)
			{
				ClientInfo* client = c->room->client[i];
				size = PackPacket(client->sendbuf, PROTOCOL::ROOMRESULT, MSGTYPE::STARTGAME, STARTGAME_MSG);
				retval = send(client->sock, client->sendbuf, size, 0);

				if (retval == SOCKET_ERROR)
				{
					err_display((char*)"startgame send()");
				}
				SetEvent(client->hWaitEvent);
				//client->state = STATE::GAMESTART;
			}
			c->state = STATE::GAMESTART;
			return;
		}
		else
		{
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
		EnterCriticalSection(&cs);
		c->room->attend_count--;
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
//�ش� ���μ������� �й�,�¸� �Ǵ� ��� �� ������ ���� ������ ���� �ּ�
/*���� �й�, �¸� �ý��� ����

�й�����(Server)
1.�����ð����� �� Ŭ�� ���� �Է��� ���� �� �Ѵ� Ż��
2.�ð��� ������ ���� Ŭ���� �Է°��� ���� �Է��� ���� �� �ش� Ŭ�� Ż��
3.������ �Է��� �ƴҽ� �ش� Ŭ�� Ż��(���� ���ڰ� 1�ε� 3�� �Է��ߴٴ��� 2�ε� 1�� �Է��ߴٴ��� �ϴ� ���)
4.���� Ŭ�� LIMITNUM(�ο����Ѽ���) - 1 �� ���ڸ� Ż�� ���ǿ� �ɸ��� �ʰ� ������ ��
���� Ŭ��� ��ġ���ӿ��� �й�.(������ ���ڸ� ��ġ�� �Ǳ� ������ �ڵ����� Ż�� ó��)
============================================================================================================

*���� ������ ���� ���� ����
1.ª�� �ð� ���� �ΰ��� Ŭ���� �Է��� �������� �����尡 �����ڿ��� �����Ҷ�
ù��° Ŭ���� �Է°� ó�� �� game������ beforclient�� starttime������ ������ �Ŀ� �ι�° Ŭ�� ó���� �̷�� ���� �ϴµ�
cpu�� �����带 � �����带 ó������ �𸣱� ������ 1��° Ŭ���� ������ game������ ����Ǳ⵵ ����
2��° Ŭ���� ������ ó���ϴ� ������ �߻�.
= > �����ڿ� ���� �� EnterCriticalsection / LeaveCriticalsection �������ν� �ذ�.(���� �������� ���谡 �ݳ��Ǿ�� ���� �����尡 ���� ����)

2.�� ����� state::gamestartprocess�� recvpacket�κп��� ��� Ŭ���̾�Ʈ�� wait�� �ɷ��ְ� �� wait�� ����ڰ� ���ڸ� Ŭ��������
�Է°��� �޾ƿ��� Ǯ����.
������ �̹� ������ �ºΰ� ���� �� ��� Ŭ���� �Է��� ������ �ʾ������� ������ �����ؾ��Ѵ�.
�׷��� �ϱ� ���ؼ��� �Է��� ���� ���� Ŭ�� ���� recvpacket wait �������� ó���� �ʿ��ߴ�.

= > ������ ����� ���(1.Ż���ڰ� ���� ��� 2.���� Ŭ�� ��ģ ���ڰ� ������ ��ȸ���� ���)
(Server)
1.���� ��������� Ŭ�󿡼� ���� �Է°��� ����(client->gamenumber == 0)
Ŭ�������� �����Ͽ� �Է����� ���� Ŭ�� protocol::end�� send�Ѵ�.
(Client)
2.Ŭ���̾�Ʈ recv thread��  protocol::end���� Client->state == STATE::END2�� ������ hWriteEvent  ���ش�.
3.Ŭ���̾�Ʈ send thread�� WaitForSingleObject(hWriteEvent)�� wait���¿��� �����Ǹ� END2�� ����� ����
END2���� server�� protocol end�� send�Ѵ�.
(Server)
4.���� �Է°��� ���� recv���� wait�̴� Ŭ�󿡰� ��Ŷ�� ������ wait���� ����, �� ��Ŷ�� ���������� end �̹Ƿ�
c->state == STATE::END�� �ٷ� ���¸� �ٲ� endprocess �Լ��� ������.(cs.675)=>�ּ����� 711�ٷ� �����.
============================================================================================================
*/
void GameStartProcess(ClientInfo* c)
{
	int size = 0;
	int retval = 0;
	char temp[MAXBUF];
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	ZeroMemory(temp, MAXBUF);
	if (!RecvPacket(c->sock, c->recvbuf))
	{
		c->state = STATE::EXIT;
		return;
	}
	PROTOCOL protocol = GetProtocol(c->recvbuf);
	switch (protocol)
	{
	case PROTOCOL::STARTGAME:
		WaitForSingleObject(c->hWaitEvent, INFINITE);
		//periodFrequency = 0;
		//QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency);
		//printf("%d", periodFrequency);
		//PER_SEC = periodFrequency/100;
		//Ŭ�󿡼� �Է��� ��ư������ �Ǵ�
		UnPackPacket(c->recvbuf, c->game_number);
		c->endtimer = clock();
		EnterCriticalSection(&cs);
		//������ �Է��� �ƴҽ� (�����غ��ϱ� �̰͵� �й�ó���ϴ°� ������.)
		if (c->room->game->befor_client != NULL)
		{
			//���� Ŭ�� �Է��� ���ڰ� ���� Ŭ�� �Է��� ���ڿ� ���ų� �۰ų� ������ �Է��� �ƴҰ��(���� ū���)
			if (c->game_number <= c->room->game->befor_client->game_number || c->game_number > c->room->game->befor_client->game_number + 1)
			{
				//���� �ð� �ȿ� ���� �Է��� ���ƴٸ�(�Ѵ� Ż������ üũ)
				if (!(c->room->game->Next) && c->game_number == c->room->game->befor_client->game_number)
				{
					c->room->game->game_number = c->game_number;
					c->protocol = PROTOCOL::SAMENUMBER;
					if (!c->room->game->sametime_check)
					{
						strcpy(c->room->game->lose_name[0], c->room->game->befor_client->user->NICK);
						c->room->game->lose_count = 1;
						c->room->game->sametime_check = true;
						SetEvent(c->room->game->hTimerEvent[c->room->game->timer_event_index]);
						++c->room->game->timer_event_index;
					}
					//�ߺ� Ŭ�� �������� ��� ������ Ŭ�� ���� ������ �����Ѵ�.
					c->room->game->Last_client = c;
					strcpy(c->room->game->lose_name[c->room->game->lose_count], c->user->NICK);
					++c->room->game->lose_count;
					SendGameNumber(c);
					c->room->game->loseresult = true;
					c->state = STATE::END;
					//endprocess���� Multipleobject ����� ���� �迭. �ش� �迭�� �ߺ��� ������ ��ŭ�� �̺�Ʈ�� ���ش�.
					SetEvent(c->room->game->hTimerEvent[c->room->game->timer_event_index]);
					++c->room->game->timer_event_index;
					LeaveCriticalSection(&cs);
					return;
				}
				//�ð� ������ ���� �Է°����� �۰ų� ���� ���� �Է� �� 
				else if (c->room->game->befor_client->game_number >= c->game_number && !c->room->game->loseresult)
				{
					c->room->game->loseresult = true;
					c->room->game->game_number = c->game_number;
					++c->room->game->lose_count;
					c->room->game->game_number = c->game_number;
					strcpy(c->room->game->lose_name[0], c->user->NICK);
					SendGameNumber(c);
					c->state = STATE::END;
					//�Է¸��� Ŭ�� ���� ���
					for (int i = 0; i < c->room->attend_count; ++i)
					{
						if (c->room->client[i]->game_number == 0)
						{
							size = PackPacket(c->room->client[i]->sendbuf, PROTOCOL::END);
							retval = send(c->room->client[i]->sock, c->room->client[i]->sendbuf, size, 0);
							if (retval == SOCKET_ERROR)
							{
								c->state = STATE::EXIT;
								LeaveCriticalSection(&cs);
								return;
							}
						}
					}
					for (int i = 0; i < c->room->attend_count; ++i)
					{
						SetEvent(c->room->client[i]->hWaitEvent);
					}
					LeaveCriticalSection(&cs);
					return;
				}
				else if (c->game_number != c->room->game->befor_client->game_number + 1 && !c->room->game->loseresult)
				{   //������ �Է��� �ƴ� ū�� �Է½�
					c->room->game->loseresult = true;
					c->room->game->game_number = c->game_number;
					++c->room->game->lose_count;
					strcpy(c->room->game->lose_name[0], c->user->NICK);
					SendGameNumber(c);
					c->state = STATE::END;
					//�Է¸��� Ŭ�� ���� ���
					for (int i = 0; i < c->room->attend_count; ++i)
					{
						if (c->room->client[i]->game_number == 0)
						{
							size = PackPacket(c->room->client[i]->sendbuf, PROTOCOL::END);
							retval = send(c->room->client[i]->sock, c->room->client[i]->sendbuf, size, 0);
							if (retval == SOCKET_ERROR)
							{
								c->state = STATE::EXIT;
								LeaveCriticalSection(&cs);
								return;
							}
						}
					}
					for (int i = 0; i < c->room->attend_count; ++i)
					{
						SetEvent(c->room->client[i]->hWaitEvent);
					}
					LeaveCriticalSection(&cs);
					return;
				}
			}
			//�������ڰ� ������ ���� ���ڿ��ٸ� ���� Ŭ��� �ڵ� Ż��
		
			if (c->game_number == LIMITNUM - 1 && !c->room->game->loseresult)
			{	c->room->game->game_number = c->game_number;
				c->room->game->loseresult = true;
				for (int i = 0; i < c->room->attend_count; ++i)
				{
					if (c->room->client[i]->game_number == 0)
					{
						++c->room->game->lose_count;
						c->room->game->game_number = LIMITNUM;
						strcpy(c->room->game->lose_name[0], c->room->client[i]->user->NICK);
						SendGameNumber(c);
						size = PackPacket(c->room->client[i]->sendbuf, PROTOCOL::END);
						retval = send(c->room->client[i]->sock, c->room->client[i]->sendbuf, size, 0);
						if (retval == SOCKET_ERROR)
						{
							c->state = STATE::EXIT;
							LeaveCriticalSection(&cs);
							return;
						}
						break;
					}
				}
				c->state = STATE::END;
				for (int i = 0; i < c->room->attend_count; ++i)
				{
					SetEvent(c->room->client[i]->hWaitEvent);
				}
				LeaveCriticalSection(&cs);
				return;
			}
		}//ó�� ���� ���� 1�� �ƴѰ��
		else
		{
			if (c->game_number != 1)
			{   
				c->room->game->loseresult = true;
				c->room->game->start_time = clock();
				c->room->game->game_number = c->game_number;
				++c->room->game->lose_count;
				strcpy(c->room->game->lose_name[0], c->user->NICK);
				if (c->room->game->timer_event_index == 0)
					SetEvent(c->room->game->hTimerStartEvent);
				SendGameNumber(c);
				c->state = STATE::END;
				//�Է¸��� Ŭ�� ���� ���
				for (int i = 0; i < c->room->attend_count; ++i)
				{
					if (c->room->client[i]->game_number == 0)
					{
						size = PackPacket(c->room->client[i]->sendbuf, PROTOCOL::END);
						retval = send(c->room->client[i]->sock, c->room->client[i]->sendbuf, size, 0);
						if (retval == SOCKET_ERROR)
						{
							c->state = STATE::EXIT;
							LeaveCriticalSection(&cs);
							return;
						}
					}
					for (int i = 0; i < c->room->attend_count; ++i)
					{
						SetEvent(c->room->client[i]->hWaitEvent);
					}
				}
				LeaveCriticalSection(&cs);
				return;
			}
		}
		if (!c->room->game->loseresult)
		{
			//Ż���� �ƴ� ��
			c->room->game->befor_client = c;
			c->room->game->game_number = c->game_number;
			c->room->game->start_time = clock();
			//���ʿ� �ѹ��� ���ֱ�.
			if (c->room->game->timer_event_index == 0)
				SetEvent(c->room->game->hTimerStartEvent);
			c->room->game->Next = false;
			c->room->game->Timer = LIMITTIME;
			SendGameNumber(c);
		}
		c->state = STATE::END;
		LeaveCriticalSection(&cs);
		return;
		break;
	case PROTOCOL::END:
		c->state = STATE::END;
		/*	for (int i = 0; i < c->room->attend_count; ++i)
			{
				SetEvent(c->room->client[i]->hWaitEvent);
			}*/
		break;
	case PROTOCOL::BACKPAGE:
		EnterCriticalSection(&cs);
		c->room->attend_count--;
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
		break;
	}
}
void EndProcess(ClientInfo* c)
{
	int size = 0;
	int retval = 0;
	char temp[MAXBUF];
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	ZeroMemory(temp, MAXBUF);
	//������ �ð��� ������ ���Ͻð� �ߺ�üũ�� �׸� �д�.(timerthread���� �����ð��� ������ �ش� �̺�Ʈ�� ����)
	WaitForSingleObject(c->hTimercheck, INFINITE);
	/*�й��� ������ samenumber�� ��� �׸��� ������ �й� ����ڰ� ���� Ŭ��� ���� ���
	  �Է¸��� Ŭ�� ������� �ش� Ŭ�� ���� wait���� ���� �����ش�.
	  (�Է¾��� Ŭ�� gamestart process���� end protocol�� �����Ҷ� ��� Ŭ���̾�Ʈ�� wait event�� ����.)
	  ������ ��� �Է��� �Ϸ�� ���¶�� ���⼭ ��� Ŭ�� ���� wait event�� ���ش�.*/
	if (c->protocol == PROTOCOL::SAMENUMBER && c->room->game->Last_client == c)
	{
		printf("%d\n", c->room->game->timer_event_index);
		WaitForMultipleObjects(c->room->game->timer_event_index, c->room->game->hTimerEvent, TRUE, INFINITE);
		bool find = false;
		EnterCriticalSection(&cs);
		if (c->room->game->loseresult)
		{
			c->room->game->loseresult = false;
			//�Է¸��� Ŭ�� ���� ���
			for (int i = 0; i < c->room->attend_count; ++i)
			{
				if (c->room->client[i]->game_number != c->game_number)
				{
					size = PackPacket(c->room->client[i]->sendbuf, PROTOCOL::END);
					retval = send(c->room->client[i]->sock, c->room->client[i]->sendbuf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						c->state = STATE::EXIT;
						LeaveCriticalSection(&cs);
						return;
					}
				}
			}
			
			for (int j = 0; j < c->room->attend_count; ++j)
				SetEvent(c->room->client[j]->hWaitEvent);
			
		}
		LeaveCriticalSection(&cs);
	}
	//���� ���ῡ ���� ����� ����� �� �����ٸ� waitevent�� �����Ƿ� �� ������ �Ѿ��.
	WaitForSingleObject(c->hWaitEvent, INFINITE);
	EnterCriticalSection(&cs);
	//���� ��� ����
	for (int i = 0; i < c->room->game->lose_count; ++i)
	{
		if (!strcmp(c->user->NICK, c->room->game->lose_name[i]))
		{
			strcpy(temp, FAIMENUMBER_MSG);
			break;
		}
		else strcpy(temp, SUCCESSNUMBER_MSG);

	}
	sprintf(buf, GAMERESULT_MSG, c->room->game->game_number);
	strcat(temp, buf);
	for (int i = 0; i < c->room->game->lose_count; ++i)
	{
		strcat(temp, c->room->game->lose_name[i]);
		if (i != c->room->game->lose_count - 1)
			strcat(temp, ",");
	}
	LeaveCriticalSection(&cs);
	strcat(temp, "\r\n");
	//Ŭ�� ���Ӱ�� �޼��� ������
	size = PackPacket(c->sendbuf, PROTOCOL::ROOMRESULT, MSGTYPE::GAMERESULT, temp);
	retval = send(c->sock, c->sendbuf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		c->state = STATE::EXIT;
		return;
	}
	c->state = STATE::MAINMENU;
	++waitcount;
	/*������ Ŭ�� �����۾��� �Ѵ�
	  1.hTimerStartEvent�� �������ν� timerthread�� �����Ų��.
	  2.��� Ŭ���̾�Ʈ�� endevent�� ���ش�.
	  3.endevent�� ������ room�� ���� ������ �����Ѵ�.
	  4.��� �۾��� ������ ����ȭ������ ���ư���.*/
	if (waitcount == LIMITNUM)
	{
		//timer ������ ����
		SetEvent(c->room->game->hTimerStartEvent);

		for (int i = 0; i < LIMITNUM; ++i)
		{
			SetEvent(c->room->client[i]->hEndEvent);
		}
	}
	WaitForSingleObject(c->hEndEvent, INFINITE);
	waitcount = 0;
	if (c->room != NULL && c == c->room->client[LIMITNUM - 1])
	{
		for (int i = 0; i < c->room->attend_count; ++i)
		{
			c->room->client[i]->protocol = PROTOCOL::NONE;
			c->room->client[i]->game_number = 0;
		}
		RemoveRoom(c->room);
	}

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