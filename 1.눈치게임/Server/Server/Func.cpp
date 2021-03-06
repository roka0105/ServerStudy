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
			for (int j = 0; j < LIMITNUM; ++j)
			{
				CloseHandle(room->game->hTimerEvent[i]);
				CloseHandle(room->game->hTimerStartEvent[i]);
			}
			room->game->first_check = false;
			ZeroMemory(room->game, sizeof(GameInfo));
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
	strcpy(Roomname[0], "신나는 눈치게임~");
	strcpy(Roomname[1], "빨리빨리 들어와!");
	strcpy(Roomname[2], "3초후에 시작");
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
		{   //이미 로그인 중인 경우
			if (User[i]->loging)
			{
				size = PackPacket(c->sendbuf, PROTOCOL::LOGINRESULT, MSGTYPE::NULL_FAIL, LOGING_TRUE_MSG);
				retval = send(c->sock, c->sendbuf, size, 0);
				if (retval == SOCKET_ERROR)err_display((char*)"login success send()");
				return false;
			}//로그인 성공시
			else
			{
				size = PackPacket(c->sendbuf, PROTOCOL::LOGINRESULT, MSGTYPE::LOGINSUCCESS, LOGIN_SUCCESS_MSG);
				retval = send(c->sock, c->sendbuf, size, 0);
				if (retval == SOCKET_ERROR)err_display((char*)"login success send()");
				return true;
			}
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
		//로그인 성공 -> 메인메뉴로 돌아가기
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
		//로그인 성공 -> 메인메뉴로 돌아가기
		if (JoinCheck(c, userinfo))
		{
			//회원정보 추가하기
			AddUser(userinfo);
			//파일자동저장
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
		if (c->room->attend_count == LIMITNUM)
		{
			c->index = c->room->game->index_count;
			c->room->game->index_count++;
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
			if (c->room->game == NULL)
			{
				c->room->game = new GameInfo();
				c->room->game->first_check = false;
				c->room->game->NowTimer = LIMITTIME;
				for (int i = 0; i < LIMITNUM; ++i)
				{
					c->room->game->hTimerEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
					c->room->game->hTimerStartEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
				}
				hThread2 = CreateThread(0, NULL, TimerThread, c, 0, NULL);
				CloseHandle(hThread2);
			}
			c->index = c->room->game->index_count;
			c->room->game->index_count++;
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
		//방 나가기.	
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
//해당 프로세스에서 패배,승리 판단 기능 및 오류에 대한 수정에 대한 주석
/*게임 패배, 승리 시스템 구현

패배조건(Server)
1.일정시간내에 두 클라가 같은 입력을 했을 시 둘다 탈락
2.시간이 지나고 이전 클라의 입력값과 같은 입력을 했을 시 해당 클라만 탈락
3.순차적 입력이 아닐시 해당 클라 탈락(이전 숫자가 1인데 3을 입력했다던가 2인데 1을 입력했다던가 하는 경우)
4.이전 클라가 LIMITNUM(인원제한설정) - 1 의 숫자를 탈락 조건에 걸리지 않고 외쳤을 시
남은 클라는 눈치게임에서 패배.(마지막 숫자를 외치게 되기 때문에 자동으로 탈락 처리)
============================================================================================================

*구현 과정시 생긴 오류 수정
1.짧은 시간 내에 두개의 클라의 입력이 들어왔을때 스레드가 공용자원을 접근할때
첫번째 클라의 입력값 처리 후 game정보에 beforclient와 starttime정보를 저장한 후에 두번째 클라 처리가 이루어 져야 하는데
cpu가 스레드를 어떤 스레드를 처리할지 모르기 때문에 1번째 클라의 정보가 game정보에 저장되기도 전에
2번째 클라의 정보를 처리하는 문제가 발생.
= > 공용자원 접근 시 EnterCriticalsection / LeaveCriticalsection 해줌으로써 해결.(앞의 스레드의 열쇠가 반납되어야 다음 스레드가 접근 가능)

2.방 입장시 state::gamestartprocess의 recvpacket부분에서 모든 클라이언트가 wait이 걸려있고 이 wait은 사용자가 숫자를 클릭했을때
입력값을 받아오며 풀린다.
하지만 이미 게임의 승부가 판정 된 경우 클라의 입력이 들어오지 않았음에도 게임을 종료해야한다.
그렇게 하기 위해서는 입력을 하지 않은 클라에 대한 recvpacket wait 상태해제 처리가 필요했다.

= > 게임이 종료된 경우(1.탈락자가 나온 경우 2.이전 클라가 외친 숫자가 마지막 기회였을 경우)
(Server)
1.게임 종료시점의 클라에서 아직 입력값이 없는(client->gamenumber == 0)
클라정보에 접근하여 입력하지 않은 클라에 protocol::end를 send한다.
(Client)
2.클라이언트 recv thread의  protocol::end에서 Client->state == STATE::END2로 설정후 hWriteEvent  켜준다.
3.클라이언트 send thread의 WaitForSingleObject(hWriteEvent)가 wait상태에서 해제되며 END2의 명령을 수행
END2에서 server에 protocol end를 send한다.
(Server)
4.아직 입력값이 없이 recv에서 wait이던 클라에게 패킷이 들어오며 wait상태 해제, 이 패킷의 프로토콜은 end 이므로
c->state == STATE::END로 바로 상태를 바꿔 endprocess 함수로 보낸다.(cs.675)=>주석땜에 711줄로 변경됨.
============================================================================================================
*/
void GameStartProcess(ClientInfo* c)
{
	int size = 0;
	int retval = 0;
	char temp[MAXBUF];
	char buf[MAXBUF];
	bool flag = false;
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
		//클라에서 입력한 버튼값으로 판단
		UnPackPacket(c->recvbuf, c->game_number);
		//c->endtimer = clock();
		EnterCriticalSection(&cs);
		//순차적 입력이 아닐시 (생각해보니까 이것도 패배처리하는게 맞을듯.)
		if (c->room->game->befor_client != NULL)
		{
			//일정 시간 안에 둘의 입력이 겹쳤다면
			if (!(c->room->game->Next))
			{   
				if (!c->room->game->sametime_check)
				{
					c->protocol = PROTOCOL::SAMETIME;
					strcpy(c->room->game->lose_name[0], c->room->game->befor_client->user->NICK);
					c->room->game->lose_count = 1;
					c->room->game->sametime_check = true;
					SetEvent(c->room->game->hTimerEvent[c->room->game->timer_event_index]);
					++c->room->game->timer_event_index;

				}
				c->protocol = PROTOCOL::SAMETIME;
				c->room->game->game_number = c->game_number;
				//입력이 들어온 클라들의 입력값이 다른데 동시에 입력된 경우
				//중복 클라가 여러개일 경우 마지막 클라에 대한 정보를 저장한다.
				c->room->game->Last_client = c;
				strcpy(c->room->game->lose_name[c->room->game->lose_count], c->user->NICK);
				++c->room->game->lose_count;
				SendGameNumber(c);
				c->room->game->loseresult = true;
				c->state = STATE::END;
				//endprocess에서 Multipleobject 사용을 위한 배열. 해당 배열에 중복된 유저수 만큼의 이벤트를 켜준다.
				SetEvent(c->room->game->hTimerEvent[c->room->game->timer_event_index]);
				++c->room->game->timer_event_index;
				LeaveCriticalSection(&cs);
				return;
			}
			//시간 지나고 이전 입력값보다 작거나 같은 숫자 입력 시 
			else if (c->room->game->befor_client->game_number >= c->game_number && !c->room->game->loseresult)
			{
				c->room->game->loseresult = true;
				c->room->game->game_number = c->game_number;
				++c->room->game->lose_count;
				c->room->game->game_number = c->game_number;
				strcpy(c->room->game->lose_name[0], c->user->NICK);
				SendGameNumber(c);
				c->state = STATE::END;
				//입력못한 클라가 있을 경우
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
			//현재 클라가 입력한 숫자가 이전 클라가 입력한 숫자보다  작거나 크며 순차적 입력이 아닐경우(값이 큰경우)
			else if (c->game_number < c->room->game->befor_client->game_number || c->game_number > c->room->game->befor_client->game_number + 1)
			{   //순차적 입력이 아닌 작은값 또는 큰값 입력시
				if (c->game_number != c->room->game->befor_client->game_number + 1 && !c->room->game->loseresult)
				{
					c->room->game->loseresult = true;
					c->room->game->game_number = c->game_number;
					++c->room->game->lose_count;
					strcpy(c->room->game->lose_name[0], c->user->NICK);
					SendGameNumber(c);
					c->state = STATE::END;
					//입력못한 클라가 있을 경우
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
			//이전숫자가 마지막 생존 숫자였다면 남은 클라는 자동 탈락
			else if (c->game_number == LIMITNUM - 1 && !c->room->game->loseresult)
			{
				c->room->game->game_number = c->game_number;
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
				/*for (int i = 0; i < c->room->attend_count; ++i)
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
				}*/
				for (int i = 0; i < c->room->attend_count; ++i)
				{
					SetEvent(c->room->client[i]->hWaitEvent);
				}
				LeaveCriticalSection(&cs);
				return;
			}
		}//처음 들어온 값이 1이 아닌경우
		else
		{
			if (c->game_number != 1)
			{
				c->room->game->loseresult = true;
				c->room->game->start_time = clock();
				c->room->game->game_number = c->game_number;
				++c->room->game->lose_count;
				strcpy(c->room->game->lose_name[0], c->user->NICK);
				SendGameNumber(c);
				c->state = STATE::END;
				//입력못한 클라가 있을 경우
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
		//탈락이 아닐 시
		if (!c->room->game->loseresult)
		{
			c->room->game->game_number = c->game_number;
			c->room->game->start_time = clock();
			//if (c->room->game->Next || c->room->game->befor_client == NULL)
			//{
			c->room->game->Next = false;
			c->room->game->NowTimer = LIMITTIME;
			SetEvent(c->room->game->hTimerStartEvent[c->index]);
			//	}
			c->room->game->befor_client = c;

			SendGameNumber(c);
		}
		c->state = STATE::END;
		LeaveCriticalSection(&cs);
		return;
		break;
	case PROTOCOL::END:
		c->state = STATE::END;
		for (int i = 0; i < c->room->attend_count; ++i)
		{
			if (c->room->client[i]->protocol == PROTOCOL::SAMETIME)
			{
				flag = true;
			}
		}
		if (!flag)
		{
			if (!c->room->game->first_check)
			{
				c->room->game->first_check = true;
				for (int i = 0; i < c->room->attend_count; ++i)
				{
					SetEvent(c->room->client[i]->hTimercheck);
				}
			}
		}
		else
		{
			for (int j = 0; j < c->room->attend_count; ++j)
				SetEvent(c->room->client[j]->hWaitEvent);
		}
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
	//정해진 시간이 지나면 동일시간 중복체크를 그만 둔다.(timerthread에서 일정시간이 지나면 해당 이벤트를 켜줌)
	WaitForSingleObject(c->hTimercheck, INFINITE);
	/*패배의 종류가 samenumber일 경우 그리고 마지막 패배 등록자가 현재 클라와 같을 경우
	  입력못한 클라가 있을경우 해당 클라에 대한 wait상태 해제 시켜준다.
	  (입력안한 클라가 gamestart process에서 end protocol을 수행할때 모든 클라이언트의 wait event를 켜줌.)
	  하지만 모두 입력이 완료된 상태라면 여기서 모든 클라에 대한 wait event를 켜준다.*/
	if (c->protocol == PROTOCOL::SAMETIME && c->room->game->Last_client == c)
	{
		printf("%d\n", c->room->game->timer_event_index);
		WaitForMultipleObjects(c->room->game->timer_event_index, c->room->game->hTimerEvent, TRUE, INFINITE);
		bool find = false;
		EnterCriticalSection(&cs);
		if (c->room->game->loseresult)
		{
			c->room->game->loseresult = false;
			//입력못한 클라가 있을 경우
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
					find = true;
				}
			}
			if (!find)
			{
				for (int j = 0; j < c->room->attend_count; ++j)
					SetEvent(c->room->client[j]->hWaitEvent);
			}

		}
		LeaveCriticalSection(&cs);
	}
	//게임 종료에 대한 결과값 계산이 다 끝났다면 waitevent는 켜지므로 이 구간을 넘어간다.
	WaitForSingleObject(c->hWaitEvent, INFINITE);
	EnterCriticalSection(&cs);
	//게임 결과 전송
	for (int i = 0; i < c->room->game->lose_count; ++i)
	{
		if (!strcmp(c->user->NICK, c->room->game->lose_name[i]))
		{
			strcpy(temp, FAIMENUMBER_MSG);
			break;
		}
		else strcpy(temp, SUCCESSNUMBER_MSG);

	}
	bool flag = false;
	for (int i = 0; i < c->room->attend_count; ++i)
	{
		if (c->room->client[i]->protocol == PROTOCOL::SAMETIME)
		{
			flag = true;
		}
	}
	if (!flag)
	{
		sprintf(buf, GAMERESULT_MSG, c->room->game->game_number);
	}
	else
	{
		strcpy(buf, "눈치게임 실패자 명단(동시입력):");
	}
	strcat(temp, buf);
	for (int i = 0; i < c->room->game->lose_count; ++i)
	{
		strcat(temp, c->room->game->lose_name[i]);
		if (i != c->room->game->lose_count - 1)
			strcat(temp, ",");
	}
	LeaveCriticalSection(&cs);
	strcat(temp, "\r\n");
	//클라에 게임결과 메세지 보내기
	size = PackPacket(c->sendbuf, PROTOCOL::ROOMRESULT, MSGTYPE::GAMERESULT, temp);
	retval = send(c->sock, c->sendbuf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		c->state = STATE::EXIT;
		return;
	}
	c->state = STATE::MAINMENU;
	++waitcount;
	/*마지막 클라가 정리작업을 한다
	  1.hTimerStartEvent를 켜줌으로써 timerthread를 종료시킨다.
	  2.모든 클라이언트의 endevent를 켜준다.
	  3.endevent가 켜지면 room에 대한 정보를 정리한다.
	  4.모든 작업이 끝나면 메인화면으로 돌아간다.*/
	if (waitcount == LIMITNUM)
	{
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