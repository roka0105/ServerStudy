#pragma once
#include "global.h"
class NetWorkProc
{
public:
	bool Init();
	void Start();//������ ����.
	void UpDate();//���⼭ �޼��� �������� üũ�ؼ� �޼��� ó�������ϱ�.
	void End();
private:
	ListenSocket Listen;
	NetworkSocket* ClientSocket;
	CRITICAL_SECTION cs;
	WSADATA ws;
};


