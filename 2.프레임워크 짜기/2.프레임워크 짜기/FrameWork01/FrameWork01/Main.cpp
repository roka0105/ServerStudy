#include "ProgramManager.h"
int main(int argv,char*argc[])
{
	ProgramManager* Program_Manager=ProgramManager::Instance();
	
	Program_Manager->MainThread();
	return 0;
}
/*������ ��
* 1.�Լ������� �����Ҷ� login,main,join�Լ��� static���� ���ϰ�ͽ��ϴ�.
*   =>static���� ���ϸ� �Լ������� ���ǿ��� (Ŭ����::*func)(�Ű�����)
*     �̷��� �ۼ��ؾ��ϴµ� Ŭ������ Ư�� ������ �ٸ� Ŭ������ �� �Լ������� ���� �������ϴ� ������ ���� 
*     static ����Լ��� �����߽��ϴ�.
*   =>������ �ش� ��� �Ŵ������� unpackpacket�� ���� �� �ʿ��� ������ ���� ó���� ���� �Լ��� ����Ϸ� �� ��
*     �ش� ���α׷��� static���� ����Ǿ��־� ������� mainmanager �� mainprogram �Լ����� ���θ޴� �����
*     �����Ҷ� unpack���� menu ��ȣ�� �޾ƿ����� �Ҷ� mainprogram�� static�̱� ������ unpack����Լ��� ��� �� �� ����
* 2.����ü�� ���̳ʸ�ȭ �ؼ� ���� �ϰ� �ͽ��ϴ�.
*/