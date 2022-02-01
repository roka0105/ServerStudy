#pragma once
#include "global.h"

#define LITTLE_ENDIAN 0xFF
#pragma region 바이트 스와핑 부호없는 정수 사용 이유
/* 바이트 스와핑은 unsigned 에서 사용한다.
부호가 있는 정수 데이터에 사용시 >> 할때
부호를 나타내는 비트가 복제된다
예)1(-)000 0001 >>4 ==1111 1000
예)0(+)111 1111 >>3 ==0000 1111
부호가 있는 정수 데이터 사용시 << 할때
부호비트가 변경되며 오버플로우 발생할 수 있다.
예)0111 1111 <<1 == 1111 1110 (부호가 -가 되어버림)
(참고**)-는 비트 값이 클수록 0에 가깝다. (-1 은 모두 1로 채워짐)
따라서 부호값이 바뀌면 값도 크게 달라짐.
그렇기 때문에 바이트 스와핑시 부호없는 정수 unsignedint를 쓴다.
TypeAliaser는 어떤 자료형이 와도 처리하도록
바이트 스와핑시 T->uint
스와핑된 결과 리턴시 uint->T 변환.

*/
#pragma endregion

#pragma region 리틀/빅 엔디안 
/*
* https://jhnyang.tistory.com/226
TCP / IP, XNS, SNA 규약은 16bit, 32bit 정수에서 빅 엔디안 방식을 사용함.(서버는 보통 빅 엔디안 쓰는듯)
수치 계산시 리틀엔디안이 더 빠르고 비교연산 및 디버깅에서는 빅엔디안이 더 좋다.]

**리틀 엔디안 장점 1)수학적 연산 빠르다.
리틀 엔디안이 수학적 연산이 쉬운 이유
1234+5678 연산시 뒤에서부터 계산하며 10이상의 값이 나올시 1씩 앞자리에 carry 하면서 연산하는데
hex 1234 hex 5678 은 각각 리틀 엔디안 표기시
(12) 00010010 (34) 00110100   <-메모리시작주소
(56) 01010110 (78) 01111000   <-메모리시작주소
+===================================
	 01101000      10101100
	 carry 하면서 수학적 연산이 쉽다.

**리틀 엔디안 장점 2)형변환
어떤 타입의 시작 주소에 값이 들어감.
int 0x12 저장시 int 4byte =32bit=8bit*4
int(32bit)를 int8_t(8bit)로 변환하고 싶을때 앞의 8bit만 잘라오면 됨.
00000000 - - - - 00010010 (32bit)
00010010(8bit)

**빅 엔디안 장점 1) 숫자 비교
숫자 비교는 연산처럼 뒤에서 부터가 아니라 앞에서 부터 한다.
빅엔디안은 앞에서부터 차곡차곡 스택에 쌓지만 리틀은 뒤에서 부터 숫자를 쌓기때문에 속도가 느림.

**빅 엔디안 장점 2)디버깅 확인
사람이 숫자를 읽고 쓰는 방법과 같기 때문에 메모리값을 보기 편함.
*/
#pragma endregion
#pragma region 엔디안 변환함수
/* htons,htonl => 호스트의 리틀 앤디안을 네트워크로 가져오면서 빅 앤디안으로 변환한다. s short l long
   ntohs,ntohl => 네트워크 바이트 순서에서 호스트 바이트 순서로 메모리값 변환
   s는 2byte 포트정보 변환할때 사용 l은 4byte ip주소 변환할때 사용.*/
#pragma endregion
#pragma region *(unsigned char*)&num 이런식으로 접근 하는 이유
   /*
	 https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=kgg1959&logNo=220187126219
	 이렇게 하는 이유는 num에 0x00ff값을 넣었을때 리틀 엔디안이면 ff 부터 저장될 것이다.
	 현재 num에는 16bit가 저장되어있는데 읽어야 할 메모리는 하위비트 8bit(1byte) 이다.
	 메모리 시작에 위치한 1byte 메모리 주소만을 떼어와서 해당 주소의 값으로 저장된 FF를 읽었을때
	 Little_endian(0xff)와 같다면 이는 메모리 저장방식이 little 방식이라는것이 된다.
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
