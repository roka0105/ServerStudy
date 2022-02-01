#include "OutPutMemoryBitStream.h"
#pragma region 0xff
/*
f 15, ff (15*16^1)+(15*16^0) =255
��ȣ���� ���� ���� 0~255 ǥ������
��ȣ�ִ� ���� ���� 256/2=128���� ǥ�������ѵ�, -128 ǥ���ϰ� ����� 0~127(128��)�� �Ǿ 
������ -128~127�� �ȴ�.

���� int�� byte�� 128�� ǥ���Ϸ��ϸ� ���� ����ȯ�� �Ͼ.
1byte(8bit) 128=1000 0000 �ε� �� �� �տ� �ִ� ��Ʈ�� ��ȣ��Ʈ�̱� �����̴�.
*/
//��Ʈ ��Ʈ�������� ���� ���ص� �ȴ��ؼ� ���� ���η� �Ѿ��.
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