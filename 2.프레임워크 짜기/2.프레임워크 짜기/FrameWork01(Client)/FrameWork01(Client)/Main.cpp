#include "SystemManager.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	SystemManager system;
	system.Init(hInstance);
	system.Start();
	system.End();
	return;
}
//���� ���� �޴� ��� �����غ��� ������ �׽�Ʈ�ϱ� 