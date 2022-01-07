#pragma once
#include "global.h"
class NetworkBuffer
{
public:
	NetworkBuffer();
	~NetworkBuffer();
	bool is_empty();
	void MemoryZero();
	char* Data_Pop();
	int Size_Pop();
	//getprotocol 분리해서 반환하는거 구현하기.(ProtocolPop)
	//패킹하는 함수 구현하기.
	//언팩하는 함수 구현하기.
private:
	char buf[MAXBUF];
	int size;
};

