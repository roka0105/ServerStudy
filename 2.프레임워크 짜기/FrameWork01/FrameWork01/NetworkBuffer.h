#pragma once
#include "global.h"
class NetworkBuffer
{
public:
	NetworkBuffer();
	~NetworkBuffer();
	bool is_empty();
	void MemoryZero();
	const char* Data_Pop();
	void Data_Push(char* data);
	int Size_Pop();
	PROTOCOL GetProtocol();
	//getprotocol 분리해서 반환하는거 구현하기.(ProtocolPop)
	//패킹하는 함수 구현하기.
	void PackPacket(PROTOCOL protocol,const char* result);
	//언팩하는 함수 구현하기.
	void UnPackPacket(PROTOCOL protocol,char*id,char*pw);
private:
	char buf[MAXBUF];
	int size;
};

