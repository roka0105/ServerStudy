#include "OutPutMemoryBitStream.h"
#pragma region 0xff
/*
f 15, ff (15*16^1)+(15*16^0) =255
부호없는 정수 사용시 0~255 표현가능
부호있는 정수 사용시 256/2=128개씩 표현가능한데, -128 표현하고 양수는 0~127(128개)가 되어서 
범위는 -128~127이 된다.

만약 int를 byte로 128을 표현하려하면 강제 형변환이 일어남.
1byte(8bit) 128=1000 0000 인데 맨 맨 앞에 있는 비트는 부호비트이기 때문이다.
*/
//비트 스트림까지는 아직 안해도 된다해서 다음 공부로 넘어가기.
#pragma endregion

void OutPutMemoryBitStream::WriteBits(uint8_t Data, size_t inBitCount)
{
	uint32_t nextHead = m_Head + static_cast<uint32_t>(inBitCount);
	if (nextHead > m_Capacity) ReallocBuffer(max(m_Capacity * 2, nextHead));
	
	uint32_t byteOffset = m_Head >> 3;
	uint32_t bitOffset = m_Head & 0x7;
}
void OutPutMemoryBitStream::WriteBits(const void* Data, size_t inBitCount)
{

}
void OutPutMemoryBitStream::ReallocBuffer(uint32_t size)
{

}