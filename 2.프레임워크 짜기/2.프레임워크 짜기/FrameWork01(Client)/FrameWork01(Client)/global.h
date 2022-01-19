#pragma once
#pragma comment (lib,"ws2_32")
#include <winsock2.h>
#include <iostream>
#include <stdlib.h>
#include <map>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define MAXBUF 4026
#define MAXPERSONNEL 100



using namespace std;
enum class STATE
{
	NONE = -1,

	MAIN,
	LOGIN,
	JOIN,
	EXIT,
	MAX
};
enum  PROTOCOL
{
	None = -1,
	MENU_SELECT,
	MENU_RESULT,
	LOGININFO,
	LOGOUT,
	LOGINRESULT,
	JOININFO,
	JOINRESULT,
	Max
};

//#include "ListenSocket.h"
//#include "NetworkSocket.h"
//#include "NetworkBuffer.h"
//#include "NetWorkProc.h"
//#include "ClientInfo.h"
//#include "ClientManager.h"
//#include "ProgramManager.h"
//#include "LoginManager.h"
//#include "SystemFuntion.h"

