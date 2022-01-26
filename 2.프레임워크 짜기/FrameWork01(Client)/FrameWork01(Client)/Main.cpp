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
//���� ���̾�α� �ڽ� �Ű������� output ���� ������ ���� �����ϱ�
//���̾�α� �Ű����� �����ؼ� ���� �޾ƿ��� ������ ���̾�αװ� static�̶� ������ static���� �����ؾ��ϴ� �������� �ֽ��ϴ�.
//�ڵ��̳� output�����鿡 ���� �����س��� ���̾�α׸� �̿��ϵ��� �ϰ������ ��¼�� �������?
//�̰� �Ŵ��� �ν��Ͻ� �������� �ϸ� �ɵ� ���� �غ���.

//�����Ұ� �����̿��� �Ǵ� ������ �����Բ�
// 1.�� ������ ���� ���ú�,���� �����忡���� �Ѱ��� �ϸ� �ϴϱ� �������� �����Ͻô� Ž������ �����尡 �Ǵ°��� �ƴѰ���?
// 2.���̾�α�â�� ������ ������ ������ Ŭ���̾�Ʈ ���� �����尡 ���� ����� ���� ���� �ʱ� ������ �̱����� ���� 
//   ���̾�α�â�� ������ �ݰ� -> �����͸� ��ŷ�ؼ� sendpacketlist�� �ְ� sendthread�� send�ϴ� ������ �����ϴµ�
//   �ٸ� ���� ����� �������.
//���� �� ��
//*recvbuf ����ϴ� ���鿡�� list���� pop�ؼ� ������ ���� �ؾ���.
//*sendbuf ����ϴ°��鿡�� send()���� �ʰ� list�� push �ϵ��� �����ؾ���.
//*�α��ξ��� ������� �ϱ�. 
//*���� ������ �����ϱ�.