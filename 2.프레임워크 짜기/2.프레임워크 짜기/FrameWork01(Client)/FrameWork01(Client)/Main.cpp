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
//할일 메인 메뉴 기능 구현해보고 서버랑 테스트하기 