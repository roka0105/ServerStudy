#include "InPutMemoryStream.h"
InPutMemoryStream::InPutMemoryStream(char* buf, int size):mBuffer(buf),mHead(0),mCapacity(size)
{

}
InPutMemoryStream::~InPutMemoryStream()
{
	delete mBuffer;
}
void InPutMemoryStream::Read(void* Data, int size)
{
	if (mBuffer == NULL)return;
	if (mHead >= mCapacity)
	{
		memset(mBuffer,'\0', mCapacity);
		mHead = 0;
	}
	memcpy(Data, mBuffer + mHead, size);
	mHead += size;
}
void InPutMemoryStream::Read(unsigned int* Data)
{
	Read(Data, sizeof(Data));
}
void InPutMemoryStream::Read(int* Data)
{
	Read(Data, sizeof(int));
}