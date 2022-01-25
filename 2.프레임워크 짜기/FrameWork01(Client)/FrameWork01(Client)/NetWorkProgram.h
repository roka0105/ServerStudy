#pragma once
#include "ClientInfo.h"
#include "FuntionManager.h"
#include "_List.h"
class NetWorkProgram
{
public:
	static NetWorkProgram* Instance();
	static void Create();
	static void Destroy();
	void Init();
	ClientInfo* Connect();
	void End();
	char* R_Packet_Pop();
	void R_Packet_Push(char* data);
	char* S_Packet_Pop();
	void S_Packet_Push(char* sendbuf);
	//static DWORD WINAPI SendThread(LPVOID arg);
	static DWORD WINAPI NetworkThread(LPVOID arg);
private:
	_List<char*> R_PacketList;
	_List<char*> S_PacketList;
	static NetWorkProgram* instance;
	NetWorkProgram();
	~NetWorkProgram();
};

