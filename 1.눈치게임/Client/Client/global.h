#pragma once
#pragma comment (lib,"ws2_32")
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include "resource.h"

#define MAXSTACKBUF 4096
#define MAXNUM 100
#define MAXBUF 256
#define LIMITNUM 3
#define SERVERPORT 9000
#define SERVERIP "127.0.0.1"

enum class MSGTYPE
{
	NONE = -1,
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
	NONE = -1,
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
	END,
	EXIT,
	MAX
};
enum class STATE
{
	NONE = -1,
	MAINMENU,
	LOGIN,
	JOIN,
	ROOMLIST,
	ROOM,
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
	ClientInfo(SOCKET _sock,SOCKADDR_IN _addr)
	{
		sock = _sock;
		memcpy(&addr, &_addr, sizeof(SOCKADDR_IN));
		ZeroMemory(sendbuf, MAXBUF);
		ZeroMemory(recvbuf, MAXBUF);
		user = new UserInfo();
	}

	SOCKET sock;
	SOCKADDR_IN addr;
	STATE state;
	PROTOCOL protocol;
	char sendbuf[MAXSTACKBUF];
	char recvbuf[MAXSTACKBUF];
	UserInfo* user;
	RoomInfo* room;
	int game_number;
	HANDLE hWaitEvent, hEndEvent;
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
	int number;
	int attend_count;
	ClientInfo* client[LIMITNUM];
	char name[MAXBUF];
};
void err_quit(char* msg);
void err_display(char* msg);
int recvn(SOCKET s, char* buf, int len, bool flag);

bool RecvPacket(SOCKET s, char* recvbuf);
PROTOCOL GetProtocol(char* recvbuf);
int PackPacket(char* sendbuf, const PROTOCOL protocol, const int number);
int PackPacket(char* sendbuf, const PROTOCOL protocol, const char* id, const char* pw, const char* nickname);
int PackPacket(char* sendbuf, const PROTOCOL protocol);
void UnPackPacket(const char* recvbuf, MSGTYPE* msgtype, char* msg);
void UnPackPacket(const char* recvbuf, int& roomcount, int& limitattendcount, RoomInfo* roomlist,UserInfo* userinfo);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow);
DWORD WINAPI ClientMain(LPVOID arg);
DWORD CALLBACK RecvThread(LPVOID arg);

INT_PTR CALLBACK DlgProc1(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);//메인메뉴
INT_PTR CALLBACK DlgProc2(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);//로그인
INT_PTR CALLBACK DlgProc3(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);//회원가입
INT_PTR CALLBACK DlgProc4(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);//방 리스트
INT_PTR CALLBACK DlgProc5(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);//방 입장
#ifdef  MAIN
ClientInfo* Client;
HANDLE hThread[2];
HANDLE hWriteEvent, hReadEvent, hWaitEvent;
int MenuSelect = 0,Roomindex=0;
char buf[MAXBUF], buf2[MAXBUF], buf3[MAXBUF],buf4[MAXBUF];
HWND hErrStr, _hDlg,hLogOutBtn,hList, hUserinfo_nick,hEdit1, hBtn[LIMITNUM],hOkBtn;
HINSTANCE hInst;
RoomInfo roominfo[MAXBUF];
int Roomcount, LimitAttend;
#else 
extern ClientInfo* Client;
extern HANDLE hThread[2];
extern HANDLE hWriteEvent, hReadEvent,hWaitEvent;
extern int MenuSelect,Roomindex;
extern HINSTANCE hInst;
extern char buf[MAXBUF], buf2[MAXBUF], buf3[MAXBUF],buf4[MAXBUF];
extern HWND hErrStr,_hDlg, hLogOutBtn, hList,hUserinfo_nick, hEdit1,hBtn[LIMITNUM], hOkBtn;
extern RoomInfo roominfo[MAXBUF];
extern int Roomcount, LimitAttend;
#endif

