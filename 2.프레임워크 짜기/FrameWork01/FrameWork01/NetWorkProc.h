#pragma once
#include "global.h"
class NetWorkProc
{
public:
	bool Init();
	void Start();//������ ����.
	void UpDate();//���⼭ �޼��� �������� üũ�ؼ� �޼��� ó�������ϱ�.
	void End();
	NetworkSocket* GetClientSock();
private:
	ListenSocket Listen;
	NetworkSocket* ClientSocket;
	//Ŭ���̾�Ʈ ���� �߰��ϴ� controll ���� controll.add(Ŭ�����)�ϱ�.
	CRITICAL_SECTION cs;
	WSADATA ws;
};


