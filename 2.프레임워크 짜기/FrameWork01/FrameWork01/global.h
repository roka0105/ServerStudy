#pragma once
#pragma comment (lib,"ws2_32")
#include <winsock2.h>
#include <iostream>
#include <stdlib.h>
#include <map>


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

	MAX
};
enum class PROTOCOL
{
	None = -1,
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

