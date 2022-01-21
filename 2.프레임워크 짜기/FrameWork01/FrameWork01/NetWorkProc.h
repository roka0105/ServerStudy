#pragma once
#include "ListenSocket.h"
class NetWorkProc
{
private:
	static NetWorkProc* instance;
public:
	static NetWorkProc* Instance();
	static void Create();
	static void Destroy();
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



