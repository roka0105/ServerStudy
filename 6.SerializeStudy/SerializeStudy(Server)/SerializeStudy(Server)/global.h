#pragma once
#pragma comment (lib,"ws2_32")
#include <iostream>
#include <winsock2.h>
#include <vector>

using namespace std;

#define SERVERPORT 9000
#define SERVERIP "127.0.0.1"
#define MAXBUF 100

class Player;
void err_quit(char* msg);
void err_display(char* msg);
int recvn(SOCKET sock, char* buf, int len, bool flag);
void SendPlayerInfo(SOCKET sock, const Player* player);
void RecvPlayerInfo(SOCKET sock, Player* player);
#include "OutPutMemoryStream.h"
#include "InPutMemoryStream.h"
#include "Player.h"