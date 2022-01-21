#pragma once
#pragma comment (lib,"ws2_32")
#include <winsock2.h>
#include <iostream>
#include <stdlib.h>
#include <map>

#define SERVERPORT 9000
#define MAXBUF 4026
#define MAXPERSONNEL 100
#define SERVERIP "127.0.0.1"

using namespace std;
enum class MENU
{  
	None=-1,
	MAIN = 0,
	LOGIN,
	JOIN,
	END,
	MAX
};
enum class STATE
{
	NONE = -1,

	MAIN,
	LOGIN,
	JOIN,
	EXIT,
	END,
	MAX
};
enum class PROTOCOL
{
	None = -1,
	MENU_SELECT=1,
	MENU_RESULT,
	LOGININFO,
	LOGOUT,
	LOGINRESULT,
	JOININFO,
	JOINRESULT,
	ENDPROGRAM,
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

