#include "global.h"
int main()
{
	NetWorkProc NetWorkManager;
	if (!NetWorkManager.Init())//��Ʈ��ũ �ʱ�ȭ
		return 0;
	NetWorkManager.Start();//���α׷� ����.
	NetWorkManager.End();//�������۾�.
	return 0;
}