#pragma once
#include "global.h"
class NetWorkProc
{
public:
	bool Init();
	void Start();//스레드 생성.
	void UpDate();//여기서 메세지 들어오는지 체크해서 메세지 처리까지하기.
	void End();
private:
	ListenSocket Listen;
	NetworkSocket* ClientSocket;
	CRITICAL_SECTION cs;
	WSADATA ws;
};


