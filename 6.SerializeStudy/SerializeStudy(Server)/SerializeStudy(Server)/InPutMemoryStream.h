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
	/*void Read(unsigned int* Data);
	void Read(int* Data);*/
	template<typename T> void Read(T& Data)
	{
		static_assert(is_arithmetic<T>::value || is_enum<T>::value, "Generic Read only supports primitive data types");
		Read((void*)&Data, sizeof(Data));
	}
private:

};

