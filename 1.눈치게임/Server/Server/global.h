#pragma once
#pragma comment (lib,"ws2_32")
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <time.h>

#define MAXSTACKBUF 4096
#define MAXNUM 100
#define MAXBUF 256
#define LIMITNUM 3
#define LIMITTIME 2.0
#define SERVERPORT 9000

#define NULL_MSG "정보를 입력하세요\r\n"
#define LOGIN_NONE_MSG "일치하는 정보가 없습니다.\n"
#define LOGIN_SUCCESS_MSG "로그인에 성공하셨습니다\n"
#define LOGING_TRUE_MSG "이미 로그인 중인 아이디 입니다.\n"
#define JOIN_SUCCESS_MSG "회원가입 성공!\n"
#define JOIN_SAMEID_MSG "사용하실 수 없는 아이디 입니다.\n다른 아이디를 입력하세요!\n"
#define JOIN_SAMENICK_MSG "사용하실 수 없는 닉네임 입니다.\n다른 닉네임을 입력하세요!\n"
#define IS_LOGINTRUE_ERR_MSG "로그인중에는 이용하실 수 없습니다\n"
#define LOGOUT_MSG "로그아웃 하셨습니다\n"
#define IS_FULL_ROOM_ERR_MSG "방이 가득 찼습니다\n다른 방을 선택해주세요\n"
#define IS_LOGINFALSE_ERR_MSG "로그인 후 이용 가능합니다.\n"
#define STARTGAME_MSG "플레이어 모집이 완료되었습니다!\r\n눈치 게임을 시작합니다\r\n"
#define WAITGAME_MSG "플레이어가 다 모일때 까지 대기합니다\r\n"
#define GAMENUMBER_MSG "%s님이 %d을 입력하셨습니다\r\n"
#define SUCCESSNUMBER_MSG "당신은 눈치게임에서 승리하셨습니다\r\n"
#define FAIMENUMBER_MSG "당신은 눈치게임에서 패배하셨습니다\r\n"
#define GAMERESULT_MSG "눈치게임 생존실패자 명단(%d입력):"
#define WRONG_MSG  "순차적인 숫자로 다시 입력하세요\r\n"
enum class MENUNUMBER
{
	NONE=-1,
	LOGIN=1,
	JOIN,
	LIST,
	END,
};
enum class MSGTYPE
{
	NONE=-1,
	NULL_FAIL,//아무것도 입력 안한 에러.
	LOGINSUCCESS,
	LOGIN_NONE_FAIL,//해당 정보 없음.
	JOINSUCCESS,
	JOIN_SAMEID_FAIL,
	JOIN_SAMENICK_FAIL,
	IS_LOGIN_TRUE_ERR,
	IS_LOGIN_FALSE_ERR,
	LOGOUT,
	IS_FULL_ROOM_ERR,
	IN_ROOM_SUCCESS,
	STARTGAME,
	WAITROOM,
	GAMENUMBER,
	GAMERESULT,
	WRONGNUMBER,
	MAX
};
enum class PROTOCOL
{
	NONE=-1,
	MENUNUMBER,
	USERINFO,
	LOGINRESULT,
	JOINRESULT,
	BACKPAGE,
	LOGOUT,
	ROOMINFO,
	SELECTROOM,
	ROOMRESULT,
	CHECKSTARTGAME,
	STARTGAME,
	CREATEROOM,
	REROOMLISTINFO,
	END,
	EXIT,
	MAX
};
enum class STATE
{
	NONE=-1,
	MAINMENU,
	LOGIN,
	JOIN,
	ROOMLIST,
	ROOM,
	CREATEROOM,
	REROOMLISTINFO,
	GAMESTART,
	BACKPAGE,
	LOGOUT,
	END,
	END2,//아직 입력하지 않은 클라에 대한 흐름 제어를 위함.
	EXIT,
	MAX
};

typedef struct UserInfo
{
	UserInfo()
	{
		ZeroMemory(ID, MAXBUF);
		ZeroMemory(PW, MAXBUF);
		ZeroMemory(NICK, MAXBUF);
		loging = false;
	}
	char ID[MAXBUF];
	char PW[MAXBUF];
	char NICK[MAXBUF];
	bool loging;
};
extern struct RoomInfo;
extern struct GameInfo;
typedef struct ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;
	STATE state;
	PROTOCOL protocol;
	char sendbuf[MAXSTACKBUF];
	char recvbuf[MAXSTACKBUF];
	UserInfo* user;
	RoomInfo* room;
	int game_number;
	HANDLE hWaitEvent,hEndEvent;
};
typedef struct GameInfo
{
	ClientInfo* befor_client;//이전에 정답을 외친 클라이언트.
	int game_number;
	float start_time;
	float end_time;
	char lose_name[LIMITNUM][MAXBUF];
	int lose_count;
	bool sametime_check;
};
typedef struct RoomInfo
{
	RoomInfo()
	{
		number = -1;
		attend_count = 0;
		for (int i = 0; i < LIMITNUM; ++i)
		{
			client[i] = new ClientInfo();
		}
		ZeroMemory(name, MAXBUF);
		game = NULL;
	}
	int number;
	int attend_count;
	ClientInfo* client[LIMITNUM];
	char name[MAXBUF];
	GameInfo* game;
};
void err_quit(char* msg);
void err_display(char* msg);
int recvn(SOCKET s, char* buf, int len, bool flag);

bool RecvPacket(SOCKET s, char* recvbuf);
PROTOCOL GetProtocol(char* recvbuf);
int PackPacket(char* sendbuf, const PROTOCOL protocol, const int number);
int PackPacket(char* sendbuf, const PROTOCOL protocol, const MSGTYPE msgtype,const char* msg);
int PackPacket(char* sendbuf, const PROTOCOL protocol);
/*포인터에 const를 붙이면 해당 주소를 변경하지 않고 사용하게 되는것임으로 밑과 같이 이중 포인터를 받아와 각 배열에 들어있는 포인터 정보를 읽고자 할 경우는
const를 붙이면 안된다.
const func(const* v)일때 앞의 const는 v의 값을 변경하지 않겠다는 의미이고, 뒤의 const는 v의 주소를 변경하지 않겠다는 의미이다.
출처:https://adnoctum.tistory.com/927*/
int PackPacket(char* sendbuf, const PROTOCOL protocol, const int roomcount, const int limitattendcount,RoomInfo** roomlist,UserInfo* user);
void UnPackPacket(const char* recvbuf, char* id, char* pw, char* nickname);
void UnPackPacket(const char* recvbuf, int& number);
void UnPackPacket(const char* recvbuf, char* roomname);

void AddClient(ClientInfo* c);
void RemoveClient(ClientInfo* c);
void AddUser(UserInfo* u);
void FileSave();
void FileLoad();
bool LoginCheck(ClientInfo* c,UserInfo* userinfo);
bool JoinCheck(ClientInfo* c, UserInfo* userinfo);
char* RandomRoom_Setname(); 
void InitRoom();
bool CheckInRoom(ClientInfo* c,int roomindex);
void RemoveRoom(RoomInfo* room);
void CreateRoom(char* roomname);
void SendGameNumber(ClientInfo* c);
void UserLogOut(bool all=false, ClientInfo* c=NULL);

void MenuProcess(ClientInfo* c);
void LoginProcess(ClientInfo* c);
void JoinProcess(ClientInfo* c);
void RoomListProcess(ClientInfo* c);
void RoomProcess(ClientInfo* c);
void GameStartProcess(ClientInfo* c);
void EndProcess(ClientInfo* c);
void ExitProcess(ClientInfo* c);

DWORD CALLBACK ClientThread(LPVOID arg);
#ifdef MAIN
CRITICAL_SECTION cs;
RoomInfo* Room[MAXNUM];
int RoomCount = 0;
ClientInfo* Client[MAXNUM];
int ClientCount = 0;
UserInfo* User[MAXNUM];
int UserCount = 0;
HANDLE hThread;
char* Roomname[LIMITNUM];
//GameInfo* game;
int waitcount=0;
#else
extern CRITICAL_SECTION cs;
extern RoomInfo* Room[MAXNUM];
extern int RoomCount;
extern ClientInfo* Client[MAXNUM];
extern int ClientCount;
extern UserInfo* User[MAXNUM];
extern int UserCount;
extern HANDLE hThread;
extern char* Roomname[LIMITNUM];
//extern GameInfo* game;
extern int waitcount;
#endif