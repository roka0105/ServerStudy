#include "global.h"
void AddClient(ClientInfo* c)
{
	EnterCriticalSection(&cs);
	printf("[TCP서버접속] IP:%s PORT:%d\n", inet_ntoa(c->addr.sin_addr), ntohs(c->addr.sin_port));
	c->state = STATE::MAINMENU;
	c->user = new UserInfo();
	Client[ClientCount] = c;
	++ClientCount;
	LeaveCriticalSection(&cs);
}
void RemoveClient(ClientInfo* c)
{
	EnterCriticalSection(&cs);
	printf("[TCP서버종료] IP:%s PORT:%d\n", inet_ntoa(c->addr.sin_addr), ntohs(c->addr.sin_port));
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
			delete room;
			for (int j = i; j < RoomCount - 1; ++j)
			{
				Room[j] = Room[j + 1];
				char* name = strtok(Room[j]->name, ".");
				name = strtok(NULL, name);
				sprintf(temp, "%d.", j + 1);
				strcat(temp, name);
				strcpy(Room[j]->name, temp);
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
	strcpy(room->name, roomname);
	Room[RoomCount] = room;
	++RoomCount;
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
	if (fp == NULL) err_quit((char*)"파일저장 실패");
	for (int i = 0; i < UserCount; ++i)
	{
		int retval = fwrite(User[i], 1, sizeof(UserInfo), fp);
		if (retval != sizeof(UserInfo))
		{
			err_quit((char*)"파일저장 실패");
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
	strcpy(Roomname[0], "신나는 눈치게임~");
	strcpy(Roomname[1], "빨리빨리 들어와!");
	strcpy(Roomname[2], "3초후에 시작");
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
	//정보를 입력하지 않았을때 에러 보내기.
	if (!strcmp(userinfo->ID, "\0") || !strcmp(userinfo->PW, "\0"))
	{
		size = PackPacket(c->sendbuf, PROTOCOL::LOGINRESULT, MSGTYPE::NULL_FAIL, NULL_MSG);
		retval = send(c->sock, c->sendbuf, size, 0);
		if (retval == SOCKET_ERROR)err_display((char*)"login success send()");
		return false;
	}
	//유저가 입력한 아이디,비밀번호와 일치하는 정보 찾기.
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
	//정보중에 유저가 입력한 정보가 없는 경우.
	size = PackPacket(c->sendbuf, PROTOCOL::LOGINRESULT, MSGTYPE::LOGIN_NONE_FAIL, LOGIN_NONE_MSG);
	retval = send(c->sock, c->sendbuf, size, 0);
	if (retval == SOCKET_ERROR)err_display((char*)"login none_err send()");
	return false;
}
bool JoinCheck(ClientInfo* c, UserInfo* userinfo)
{
	int retval = 0;
	int size = 0;
	//정보를 입력하지 않았을때 에러 보내기.
	if (!strcmp(userinfo->ID, "\0") || !strcmp(userinfo->PW, "\0") || !strcmp(userinfo->NICK, "\0"))
	{
		size = PackPacket(c->sendbuf, PROTOCOL::JOINRESULT, MSGTYPE::NULL_FAIL, NULL_MSG);
		retval = send(c->sock, c->sendbuf, size, 0);
		if (retval == SOCKET_ERROR)err_display((char*)"join nullfail send()");
		return false;
	}
	//가입할 정보와 중복하는 정보가 있는지 체크
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
		//로그인 성공 -> 메인메뉴로 돌아가기
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
		//로그인 성공 -> 메인메뉴로 돌아가기
		if (JoinCheck(c, userinfo))
		{
			//회원정보 추가하기
			AddUser(userinfo);
			//파일자동저장
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
		//현재 방 갯수,방이름리스트,각 방의 현재 인원수,현재 제한 인원수 묶어서 한번에 보내기
		size = PackPacket(c->sendbuf, PROTOCOL::ROOMINFO, RoomCount, LIMITNUM, Room, c->user);
		retval = send(c->sock, c->sendbuf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display((char*)"room info send()");
		}
		break;
	case PROTOCOL::SELECTROOM:
		UnPackPacket(c->recvbuf, index);
		//방에 들어갈 수 있는지 체크
		if (CheckInRoom(c, index))
		{
			size = PackPacket(c->sendbuf, PROTOCOL::ROOMRESULT, MSGTYPE::IS_FULL_ROOM_ERR, IS_FULL_ROOM_ERR_MSG);
			retval = send(c->sock, c->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display((char*)"room full err send()");
			}
		}//들어갈 수 있으면 해당 방에 입장. 방에 대한 정보값을 클라에 보내기.
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
		if (game != NULL)
		{
			ZeroMemory(game, sizeof(GameInfo));
			delete(game);
		}
		if (c->room->attend_count == LIMITNUM)
		{
			if (game == NULL)
				game = new GameInfo();

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
		//case PROTOCOL::BACKPAGE:
		//	//방 나가기.	
		//	EnterCriticalSection(&cs);
		//	c->room->attend_count--;
		//	for (int i = 0; i < c->room->attend_count + 1; ++i)
		//	{
		//		if (c->room->client[i] == c)
		//		{
		//			c->room->client[i] = NULL;
		//			c->room = NULL;
		//			break;
		//		}
		//	}
		//	LeaveCriticalSection(&cs);
		//	c->state = STATE::ROOMLIST;
		//	return;
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
	if (!RecvPacket(c->sock, c->recvbuf))
	{
		c->state = STATE::EXIT;
		return;
	}
	PROTOCOL protocol = GetProtocol(c->recvbuf);
	switch (protocol)
	{
	case PROTOCOL::STARTGAME:
		//클라에서 입력한 버튼값으로 판단
		UnPackPacket(c->recvbuf, c->game_number);
		//순차적 입력이 아닐시 (생각해보니까 이것도 패배처리하는게 맞을듯.)
		/*if (c->game_number != game->game_number + 1)
		{
			size = PackPacket(c->sendbuf, PROTOCOL::ROOMRESULT, MSGTYPE::WRONGNUMBER, WRONG_MSG);
			retval = send(c->sock, c->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display((char*)"wrong msg send err()");
			}
			return;
		}*/
		if (game->start_time != 0)
		{
			game->end_time = clock();
		}
		EnterCriticalSection(&cs);
		//현재 클라가 입력한 숫자가 이전 클라가 입력한 숫자와 같거나 작거나 순차적 입력이 아닐경우(값이 큰경우)
		if (game->befor_client != NULL)
		{
			if (c->game_number <= game->befor_client->game_number || c->game_number > game->befor_client->game_number + 1)
			{
				game->game_number = c->game_number;
				float time = (game->end_time - game->start_time) / CLOCKS_PER_SEC;
				//일정 시간 안에 둘의 입력이 겹쳤다면(둘다 탈락으로 체크)
				if (time <= LIMITTIME&&c->game_number==game->befor_client->game_number)
				{
					if (!game->sametime_check)
					{
						strcpy(game->lose_name[0], game->befor_client->user->NICK);
						game->lose_count = 1;
						game->sametime_check = true;
					}
					strcpy(game->lose_name[game->lose_count], c->user->NICK);
					++game->lose_count;
					SendGameNumber(c);
					//아직 입력하지 않은 클라에 대한 게임 종료 처리
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
					c->state = STATE::END;
					LeaveCriticalSection(&cs);
					return;
				}
				//시간 지나고 이전 입력값보다 작거나 같은 숫자 입력 시 
				else if (game->game_number <= c->game_number)
				{
					++game->lose_count;
					game->game_number = c->game_number;
					strcpy(game->lose_name[0], c->user->NICK);
					SendGameNumber(c);
					//아직 입력하지 않은 클라에 대한 게임 종료 처리
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
					c->state = STATE::END;
					LeaveCriticalSection(&cs);
					return;
				}
				//순차적 입력이 아닌 큰값 입력시
				++game->lose_count;
				strcpy(game->lose_name[0], c->user->NICK);
				//아직 입력하지 않은 클라에 대한 게임 종료 처리
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
				c->state = STATE::END;
				LeaveCriticalSection(&cs);
				return;
			}
			//이전숫자가 마지막 생존 숫자였다면 남은 클라는 자동 탈락
			else if (c->game_number == LIMITNUM - 1)
			{
				for (int i = 0; i < c->room->attend_count; ++i)
				{
					if (c->room->client[i]->game_number == 0)
					{
						++game->lose_count;
						game->game_number = LIMITNUM;
						strcpy(game->lose_name[0], c->room->client[i]->user->NICK);
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
				for (int i = 0; i < c->room->attend_count; ++i)
				{
					SetEvent(c->room->client[i]->hWaitEvent);
				}
				c->state = STATE::END;
				LeaveCriticalSection(&cs);
				return;
			}
			
		}

		//탈락이 아닐 시
		game->befor_client = c;
		game->game_number = c->game_number;
		game->start_time = clock();
		LeaveCriticalSection(&cs);
		SendGameNumber(c);
		/*for (int i = 0; i < c->room->attend_count; ++i)
		{
			size = PackPacket(c->room->client[i]->sendbuf, PROTOCOL::ROOMRESULT, MSGTYPE::GAMENUMBER, buf);
			retval = send(c->room->client[i]->sock, c->room->client[i]->sendbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				c->state = STATE::EXIT;
				return;
			}
		}*/
	
		WaitForSingleObject(c->hWaitEvent, INFINITE);
		//printf("웨이트 풀림\n");
		c->state = STATE::END;
		return;
		break;
	case PROTOCOL::END:
		c->state = STATE::END;
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
	EnterCriticalSection(&cs);
	for (int i = 0; i < game->lose_count; ++i)
	{
		if (!strcmp(c->user->NICK, game->lose_name[i]))
		{
			strcpy(temp, FAIMENUMBER_MSG);
			break;
		}
		else strcpy(temp, SUCCESSNUMBER_MSG);

	}
	sprintf(buf, GAMERESULT_MSG, game->game_number);
	strcat(temp, buf);
	for (int i = 0; i < game->lose_count; ++i)
	{
		strcat(temp, game->lose_name[i]);
		if (i != game->lose_count - 1)
			strcat(temp, ",");
	}
	LeaveCriticalSection(&cs);
	strcat(temp, "\r\n");
	//클라에 종료메세지 보내기
	size = PackPacket(c->sendbuf, PROTOCOL::ROOMRESULT, MSGTYPE::GAMERESULT, temp);
	retval = send(c->sock, c->sendbuf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		c->state = STATE::EXIT;
		return;
	}
	++waitcount;
	if (waitcount == LIMITNUM)
	{
		for (int i = 0; i < LIMITNUM; ++i)
		{
			SetEvent(c->room->client[i]->hEndEvent);
		}
	}
	WaitForSingleObject(c->hEndEvent, INFINITE);
	if (c->room != NULL && c == c->room->client[LIMITNUM - 1])
	{
		RemoveRoom(c->room);
	}
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