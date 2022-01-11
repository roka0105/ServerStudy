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
	//getprotocol �и��ؼ� ��ȯ�ϴ°� �����ϱ�.(ProtocolPop)
	//��ŷ�ϴ� �Լ� �����ϱ�.
	void PackPacket(PROTOCOL protocol,const char* result);
	//�����ϴ� �Լ� �����ϱ�.
	void UnPackPacket(PROTOCOL protocol,char*id,char*pw);
private:
	char buf[MAXBUF];
	int size;
};

