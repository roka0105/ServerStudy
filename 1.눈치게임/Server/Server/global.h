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

#define NULL_MSG "������ �Է��ϼ���\r\n"
#define LOGIN_NONE_MSG "��ġ�ϴ� ������ �����ϴ�.\n"
#define LOGIN_SUCCESS_MSG "�α��ο� �����ϼ̽��ϴ�\n"
#define LOGING_TRUE_MSG "�̹� �α��� ���� ���̵� �Դϴ�.\n"
#define JOIN_SUCCESS_MSG "ȸ������ ����!\n"
#define JOIN_SAMEID_MSG "����Ͻ� �� ���� ���̵� �Դϴ�.\n�ٸ� ���̵� �Է��ϼ���!\n"
#define JOIN_SAMENICK_MSG "����Ͻ� �� ���� �г��� �Դϴ�.\n�ٸ� �г����� �Է��ϼ���!\n"
#define IS_LOGINTRUE_ERR_MSG "�α����߿��� �̿��Ͻ� �� �����ϴ�\n"
#define LOGOUT_MSG "�α׾ƿ� �ϼ̽��ϴ�\n"
#define IS_FULL_ROOM_ERR_MSG "���� ���� á���ϴ�\n�ٸ� ���� �������ּ���\n"
#define IS_LOGINFALSE_ERR_MSG "�α��� �� �̿� �����մϴ�.\n"
#define STARTGAME_MSG "�÷��̾� ������ �Ϸ�Ǿ����ϴ�!\r\n��ġ ������ �����մϴ�\r\n"
#define WAITGAME_MSG "�÷��̾ �� ���϶� ���� ����մϴ�\r\n"
#define GAMENUMBER_MSG "%s���� %d�� �Է��ϼ̽��ϴ�\r\n"
#define SUCCESSNUMBER_MSG "����� ��ġ���ӿ��� �¸��ϼ̽��ϴ�\r\n"
#define FAIMENUMBER_MSG "����� ��ġ���ӿ��� �й��ϼ̽��ϴ�\r\n"
#define GAMERESULT_MSG "��ġ���� ���������� ���(%d�Է�):"
#define WRONG_MSG  "�������� ���ڷ� �ٽ� �Է��ϼ���\r\n"
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
	NULL_FAIL,//�ƹ��͵� �Է� ���� ����.
	LOGINSUCCESS,
	LOGIN_NONE_FAIL,//�ش� ���� ����.
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
	END2,//���� �Է����� ���� Ŭ�� ���� �帧 ��� ����.
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
	ClientInfo* befor_client;//������ ������ ��ģ Ŭ���̾�Ʈ.
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
/*�����Ϳ� const�� ���̸� �ش� �ּҸ� �������� �ʰ� ����ϰ� �Ǵ°������� �ذ� ���� ���� �����͸� �޾ƿ� �� �迭�� ����ִ� ������ ������ �а��� �� ����
const�� ���̸� �ȵȴ�.
const func(const* v)�϶� ���� const�� v�� ���� �������� �ʰڴٴ� �ǹ��̰�, ���� const�� v�� �ּҸ� �������� �ʰڴٴ� �ǹ��̴�.
��ó:https://adnoctum.tistory.com/927*/
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