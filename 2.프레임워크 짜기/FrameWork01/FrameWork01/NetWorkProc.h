#pragma once
#include "global.h"
class NetWorkProc
{
public:
	bool Init();
	void Start();//������ ����.
	SOCKET UpDate(SOCKADDR_IN& output);//���⼭ �޼��� �������� üũ�ؼ� �޼��� ó�������ϱ�.
	void End();
private:
	ListenSocket Listen;
	//Ŭ���̾�Ʈ ���� �߰��ϴ� controll ���� controll.add(Ŭ�����)�ϱ�.
	CRITICAL_SECTION cs;
	WSADATA ws;
};


