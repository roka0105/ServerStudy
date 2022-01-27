#pragma once
#include <iostream>
using namespace std;
class OutPutMemoryStream
{
private:
	unsigned int mHead;
	unsigned int mCapacity;
	char* mBuffer;
public:
	OutPutMemoryStream();
	~OutPutMemoryStream();
	char* GetStreamPtr();
	int GetLength();
	void Write(const void* inData, size_t inByteCount);
	void Write(unsigned int inData);
	void Write(int inData);
private:
	void ReallocBuffer(unsigned int size);
};

