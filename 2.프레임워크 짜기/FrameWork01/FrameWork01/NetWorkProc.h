#pragma once
#include "global.h"
class NetWorkProc
{
public:
	bool Init();
	void Start();//스레드 생성.
	SOCKET UpDate(SOCKADDR_IN& output);//여기서 메세지 들어오는지 체크해서 메세지 처리까지하기.
	void End();
private:
	ListenSocket Listen;
	//클라이언트 정보 추가하는 controll 선언 controll.add(클라소켓)하기.
	CRITICAL_SECTION cs;
	WSADATA ws;
};


