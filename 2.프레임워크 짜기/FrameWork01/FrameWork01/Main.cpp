#include "global.h"
int main()
{
	NetWorkProc NetWorkManager;
	if (!NetWorkManager.Init())//네트워크 초기화
		return 0;
	NetWorkManager.Start();//프로그램 시작.
	NetWorkManager.End();//마무리작업.
	return 0;
}