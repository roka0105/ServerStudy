#include "OutPutMemoryStream.h"
OutPutMemoryStream::OutPutMemoryStream()
	:mBuffer(nullptr), mHead(0), mCapacity(0)
{
	ReallocBuffer(32);
}
OutPutMemoryStream::~OutPutMemoryStream()
{
	free(mBuffer);
}
char* OutPutMemoryStream::GetStreamPtr()
{
	return mBuffer;
}
int OutPutMemoryStream::GetLength()
{
	return mHead;
}
void OutPutMemoryStream::Write(const void* inData, size_t inByteCount)
{
	unsigned int resulthead = mHead + inByteCount;
	if (mCapacity < resulthead)
	{
		ReallocBuffer(max(mCapacity * 2, resulthead));
	}
	memcpy(mBuffer + mHead, inData, inByteCount);
	mHead = resulthead;
}
//void OutPutMemoryStream::Write(unsigned int inData)
//{
//	Write(&inData, sizeof(inData));
//}
//void OutPutMemoryStream::Write(int inData)
//{
//	//static_assert(is_arithmetic<T>::value || is_enum<T>::value), "Generic Write only supports primitive data types");
//	Write(&inData, sizeof(inData));
//}
void OutPutMemoryStream::ReallocBuffer(unsigned int size)
{
	mBuffer = (char*)realloc(mBuffer, size);
	if (mBuffer == NULL)
	{
		cout << "메모리할당 실패" << endl;
		exit(1);
	}
	mCapacity = size;
}