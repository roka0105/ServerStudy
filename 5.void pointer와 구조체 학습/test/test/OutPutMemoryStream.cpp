#include <iostream>
#include <algorithm>
using namespace std;

class OutPutMemoryStream
{
public:
	OutPutMemoryStream() :mBuffer(nullptr), mHead(0), mCapacity(0)
	{
		ReallocBuffer(32);
	}
	~OutPutMemoryStream() { free(mBuffer); }
	const char* GetBufferPtr() { return mBuffer; }
	int GetLength() { return mHead; }
	void Write(const void* inData, size_t inByteCount);
	void Write(unsigned int inData) { Write(&inData, sizeof(inData)); }
	void Write(int inData) { Write(&inData, sizeof(inData)); }
private:
	void ReallocBuffer(int size);
	char* mBuffer;
	int mHead;
	int mCapacity;
};
void OutPutMemoryStream::Write(const void* inData, size_t inByteCount)
{
	unsigned int resultHead = mHead + (unsigned int)inByteCount;
	if (resultHead > mCapacity)
	{
		ReallocBuffer(max(mCapacity*2,(int)resultHead));
	}
	memcpy(mBuffer + mHead, inData, inByteCount);
	mHead = resultHead;
}
void OutPutMemoryStream::ReallocBuffer(int size)
{
	mBuffer = (char*)realloc(mBuffer, size);
	if (mBuffer == NULL)
	{
		cout << "할당할 수 있는 메모리가 없습니다." << endl;
		exit(1);
	}
	mCapacity = size;
}