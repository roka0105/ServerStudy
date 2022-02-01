#pragma once
#include "global.h"

#define LITTLE_ENDIAN 0xFF
#pragma region ����Ʈ ������ ��ȣ���� ���� ��� ����
/* ����Ʈ �������� unsigned ���� ����Ѵ�.
��ȣ�� �ִ� ���� �����Ϳ� ���� >> �Ҷ�
��ȣ�� ��Ÿ���� ��Ʈ�� �����ȴ�
��)1(-)000 0001 >>4 ==1111 1000
��)0(+)111 1111 >>3 ==0000 1111
��ȣ�� �ִ� ���� ������ ���� << �Ҷ�
��ȣ��Ʈ�� ����Ǹ� �����÷ο� �߻��� �� �ִ�.
��)0111 1111 <<1 == 1111 1110 (��ȣ�� -�� �Ǿ����)
(����**)-�� ��Ʈ ���� Ŭ���� 0�� ������. (-1 �� ��� 1�� ä����)
���� ��ȣ���� �ٲ�� ���� ũ�� �޶���.
�׷��� ������ ����Ʈ �����ν� ��ȣ���� ���� unsignedint�� ����.
TypeAliaser�� � �ڷ����� �͵� ó���ϵ���
����Ʈ �����ν� T->uint
�����ε� ��� ���Ͻ� uint->T ��ȯ.

*/
#pragma endregion

#pragma region ��Ʋ/�� ����� 
/*
* https://jhnyang.tistory.com/226
TCP / IP, XNS, SNA �Ծ��� 16bit, 32bit �������� �� ����� ����� �����.(������ ���� �� ����� ���µ�)
��ġ ���� ��Ʋ������� �� ������ �񱳿��� �� ����뿡���� �򿣵���� �� ����.]

**��Ʋ ����� ���� 1)������ ���� ������.
��Ʋ ������� ������ ������ ���� ����
1234+5678 ����� �ڿ������� ����ϸ� 10�̻��� ���� ���ý� 1�� ���ڸ��� carry �ϸ鼭 �����ϴµ�
hex 1234 hex 5678 �� ���� ��Ʋ ����� ǥ���
(12) 00010010 (34) 00110100   <-�޸𸮽����ּ�
(56) 01010110 (78) 01111000   <-�޸𸮽����ּ�
+===================================
	 01101000      10101100
	 carry �ϸ鼭 ������ ������ ����.

**��Ʋ ����� ���� 2)����ȯ
� Ÿ���� ���� �ּҿ� ���� ��.
int 0x12 ����� int 4byte =32bit=8bit*4
int(32bit)�� int8_t(8bit)�� ��ȯ�ϰ� ������ ���� 8bit�� �߶���� ��.
00000000 - - - - 00010010 (32bit)
00010010(8bit)

**�� ����� ���� 1) ���� ��
���� �񱳴� ����ó�� �ڿ��� ���Ͱ� �ƴ϶� �տ��� ���� �Ѵ�.
�򿣵���� �տ������� �������� ���ÿ� ������ ��Ʋ�� �ڿ��� ���� ���ڸ� �ױ⶧���� �ӵ��� ����.

**�� ����� ���� 2)����� Ȯ��
����� ���ڸ� �а� ���� ����� ���� ������ �޸𸮰��� ���� ����.
*/
#pragma endregion
#pragma region ����� ��ȯ�Լ�
/* htons,htonl => ȣ��Ʈ�� ��Ʋ �ص���� ��Ʈ��ũ�� �������鼭 �� �ص������ ��ȯ�Ѵ�. s short l long
   ntohs,ntohl => ��Ʈ��ũ ����Ʈ �������� ȣ��Ʈ ����Ʈ ������ �޸𸮰� ��ȯ
   s�� 2byte ��Ʈ���� ��ȯ�Ҷ� ��� l�� 4byte ip�ּ� ��ȯ�Ҷ� ���.*/
#pragma endregion
#pragma region *(unsigned char*)&num �̷������� ���� �ϴ� ����
   /*
	 https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=kgg1959&logNo=220187126219
	 �̷��� �ϴ� ������ num�� 0x00ff���� �־����� ��Ʋ ������̸� ff ���� ����� ���̴�.
	 ���� num���� 16bit�� ����Ǿ��ִµ� �о�� �� �޸𸮴� ������Ʈ 8bit(1byte) �̴�.
	 �޸� ���ۿ� ��ġ�� 1byte �޸� �ּҸ��� ����ͼ� �ش� �ּ��� ������ ����� FF�� �о�����
	 Little_endian(0xff)�� ���ٸ� �̴� �޸� �������� little ����̶�°��� �ȴ�.
	*/
#pragma endregion
inline bool Check_Endian()
{
	unsigned int num = 0x00FF;
	return (*(unsigned char*)&num == LITTLE_ENDIAN);
}
inline uint16_t ByteSwap2(uint16_t Data)
{
	return (Data >> 8) | (Data << 8);
}
inline uint32_t ByteSwap4(uint32_t Data)
{
	return ((Data >> 24) & 0x000000FF) |
		((Data >> 8) & 0x0000FF00) |
		((Data << 8) & 0x00FF0000) |
		((Data << 24) & 0xFF000000);
}
inline uint64_t ByteSwap8(uint64_t Data)
{
	return ((Data >> 56) & 0x00000000000000FF) |
		((Data >> 40) & 0x000000000000FF00) |
		((Data >> 24) & 0x0000000000FF0000) |
		((Data >> 8) & 0x00000000FF000000) |
		((Data << 8) & 0x000000FF00000000) |
		((Data << 24) & 0x0000FF0000000000) |
		((Data << 40) & 0x00FF000000000000) |
		((Data << 56) & 0xFF00000000000000);
}
template <typename tFrom, typename tTo>
class TypeAliaser
{
public:
	TypeAliaser(tFrom inFromValue) :mAsFromType(inFromValue) {}
	tTo& Get() { return mAsToType; }
	union
	{
		tFrom mAsFromType;
		tTo mAsToType;
	};
};
template <typename T, size_t SIZE> class ByteSwapper;
template <typename T>
class ByteSwapper<T, 2>
{
public:
	T Swap(T Data)const
	{
		uint16_t result =
			ByteSwap2(TypeAliaser<T, uint16_t>(Data).Get());
		return TypeAliaser<uint16_t, T>(result).Get();
	}

};
template <typename T>
class ByteSwapper<T, 4>
{
public:
	T Swap(T Data)const
	{
		uint32_t result = ByteSwap4(TypeAliaser<T, uint32_t>(Data).Get());
		return TypeAliaser<uint32_t, T>(result).Get();
	}
};
template <typename T>
class ByteSwapper<T, 8>
{
public:
	T Swap(T Data)const
	{
		uint64_t result = ByteSwap8(TypeAliaser<T, uint64_t>(Data).Get());
		return TypeAliaser<uint64_t, T>(result).Get();
	}
};
template <typename T>
T ByteSwap(T Data)
{
	return ByteSwapper<T, sizeof(T)>().Swap(Data);
}
