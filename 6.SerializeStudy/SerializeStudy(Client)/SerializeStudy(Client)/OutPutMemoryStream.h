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
	//void Write(unsigned int inData);
	//void Write(int inData);
	template<typename T> void Write(const T& inData)
	{
		static_assert(is_arithmetic<T>::value || is_enum<T>::value, "Generic Write only supports primitive data types");
		if(STREAM_ENDIAN==PLATFORM_ENDIAN)
		Write((void*)&inData, sizeof(inData));
		else
		{
			T swapData = ByteSwap(inData);
			Write(&swapData, sizeof(swapData));
		}
	}
private:
	void ReallocBuffer(unsigned int size);
};

