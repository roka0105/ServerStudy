#include "SystemManager.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	SystemManager::Create();
	SystemManager* system=SystemManager::Instance();
	system->Init(hInstance);
	system->Start();
	system->End();
	SystemManager::Destroy();
	return 0;
}
//할일 메인 메뉴 기능 구현해보고 서버랑 테스트하기 
//질문 다이어로그 박스 매개변수에 output 인자 넣을수 없나 질문하기