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
	void Data_Push(char* data,int size);
	void Data_Push(char* data);
	int Size_Pop();
	void Size_Push(int number);
	int PackPacket(PROTOCOL protocol);
	int PackPacket(PROTOCOL protocol,char*data,int size);
	void UnPackPacket(PROTOCOL& protocol);
private:
	char buf[MAXBUF];
	int size;
};

