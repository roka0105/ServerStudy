#pragma once
#pragma comment (lib,"ws2_32")
#include <winsock2.h>
#include <iostream>


#define SERVERPORT 9000
#define MAXBUF 4026


#include "ListenSocket.h"
#include "NetworkSocket.h"
#include "NetworkBuffer.h"
#include "NetWorkProc.h"
using namespace std;
