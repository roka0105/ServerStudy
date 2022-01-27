#pragma once
#include <iostream>
using namespace std;
class InPutMemoryStream
{
private:
	unsigned int mHead;
	unsigned int mCapacity;
	char* mBuffer;
public:
	InPutMemoryStream(char* buf,int size);
	~InPutMemoryStream();
	void Read(void* Data, int size);
	void Read(unsigned int* Data);
	void Read(int* Data);
private:

};

