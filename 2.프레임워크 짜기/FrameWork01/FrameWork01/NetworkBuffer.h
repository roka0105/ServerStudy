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
	int PackPacket(PROTOCOL protocol,char*data,int size);
	void UnPackPacket(PROTOCOL& protocol,char* data);
private:
	char buf[MAXBUF];
};

