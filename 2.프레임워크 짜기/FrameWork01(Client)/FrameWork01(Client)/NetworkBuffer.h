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
	//void Data_Push(char* data);
	void Data_Push(char* data, int _size);
	int& Size_Pop();
	void Size_Push(int _size);
	void PackPacket(PROTOCOL protocol);
	void PackPacket(PROTOCOL protocol,const char*data,int datasize);
	void UnPackPacket(PROTOCOL& protocol);
private:
	char buf[MAXBUF];
	int size;
};

