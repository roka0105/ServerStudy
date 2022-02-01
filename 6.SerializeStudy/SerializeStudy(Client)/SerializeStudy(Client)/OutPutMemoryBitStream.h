#pragma once
#include  "global.h"
class OutPutMemoryBitStream
{
public:
	OutPutMemoryBitStream() { ReallocBuffer(256); }
	~OutPutMemoryBitStream() { free(m_Buffer); }
	void WriteBits(uint8_t Data, size_t inBitCount);
	void WriteBits(const void* Data, size_t inBitCount);
	const char* GetBufferPtr()const { return m_Buffer; }
	uint32_t GetBitLength()const { return m_Head; }
	uint32_t GetByteLength()const { return (m_Head + 7) >> 3; }
	void WriteBytes(const void* Data, size_t inByteCount)
	{
		WriteBits(Data, inByteCount << 3);
	}
private:
	void ReallocBuffer(uint32_t size);
	char* m_Buffer;
	int m_Head;
	int m_Capacity;
};

