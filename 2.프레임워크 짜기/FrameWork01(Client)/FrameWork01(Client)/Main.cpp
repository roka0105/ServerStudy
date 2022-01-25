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
//질문 다이어로그 박스 매개변수에 output 인자 넣을수 없나 질문하기
//다이어로그 매개변수 전달해서 값을 받아오고 싶은데 다이어로그가 static이라 변수도 static으로 선언해야하는 문제점이 있습니다.
//핸들이나 output변수들에 값을 저장해놓고 다이어로그를 이용하도록 하고싶은데 어쩌면 좋을까요?
//이거 매니저 인스턴스 가져가서 하면 될듯 내일 해보기.

//내일 할 일
//*recvbuf 사용하는 곳들에서 list에서 pop해서 가져다 쓰게 해야함.
//*sendbuf 사용하는곳들에서 send()하지 않고 list에 push 하도록 변경해야함.
//*로그인업무 수행토록 하기. 
//*파일 저장방식 구현하기.