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
//���� ���� �޴� ��� �����غ��� ������ �׽�Ʈ�ϱ� 
//���� ���̾�α� �ڽ� �Ű������� output ���� ������ ���� �����ϱ�