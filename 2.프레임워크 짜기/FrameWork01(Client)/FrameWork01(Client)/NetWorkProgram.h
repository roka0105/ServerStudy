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
	void R_Packet_Pop(char* recvbuf,int& size);
	void R_Packet_Push(const char* data, int size);
	//void R_Packet_Push(char* data,int size);
	//void S_Packet_Pop(char* sendbuf,int& size);
	void S_Packet_Pop(char* buf, int& size);
	void S_Packet_Push(const char* sendbuf,int size);
	bool PacketList_IsEmpty(bool recv);
	static DWORD WINAPI SendThread(LPVOID arg);
	static DWORD WINAPI RecvThread(LPVOID arg);
private:
	_List<char*> R_PacketList;
	_List<char*> S_PacketList;
	static NetWorkProgram* instance;
	NetWorkProgram();
	~NetWorkProgram();
};

