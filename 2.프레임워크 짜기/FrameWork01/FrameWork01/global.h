#pragma once
#pragma comment (lib,"ws2_32")
#include <winsock2.h>
#include <iostream>
#include <map>


#define SERVERPORT 9000
#define MAXBUF 4026
#define MAXPERSONNEL 100


#include "ListenSocket.h"
#include "NetworkSocket.h"
#include "NetworkBuffer.h"
#include "NetWorkProc.h"
#include "ClientInfo.h"
#include "ClientManager.h"
#include "ProgramManager.h"
using namespace std;

enum PROTOCOL
{
	None=-1,
	LOGININFO,
	LOGOUT,
	LOGINRESULT,
	Max
};
