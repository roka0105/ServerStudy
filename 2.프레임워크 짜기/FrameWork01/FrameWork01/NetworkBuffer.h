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
	//getprotocol �и��ؼ� ��ȯ�ϴ°� �����ϱ�.(ProtocolPop)
	//��ŷ�ϴ� �Լ� �����ϱ�.
	//�����ϴ� �Լ� �����ϱ�.
private:
	char buf[MAXBUF];
	int size;
};

