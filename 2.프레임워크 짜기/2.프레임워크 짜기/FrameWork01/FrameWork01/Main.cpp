#include "ProgramManager.h"
int main(int argv,char*argc[])
{
	ProgramManager* Program_Manager=ProgramManager::Instance();
	
	Program_Manager->MainThread();
	return 0;
}
/*질문할 것
* 1.함수포인터 연결할때 login,main,join함수를 static으로 안하고싶습니다.
*   =>static으로 안하면 함수포인터 정의에서 (클래스::*func)(매개변수)
*     이렇게 작성해야하는데 클래스를 특정 지으면 다른 클래스는 또 함수포인터 따로 만들어야하는 문제로 인해 
*     static 멤버함수로 구현했습니다.
*   =>하지만 해당 기능 매니저에서 unpackpacket을 구현 후 필요한 데이터 언팩 처리를 위한 함수를 사용하려 할 때
*     해당 프로그램이 static으로 선언되어있어 예를들어 mainmanager 의 mainprogram 함수에서 메인메뉴 기능을
*     수행할때 unpack으로 menu 번호를 받아오도록 할때 mainprogram이 static이기 때문에 unpack멤버함수를 사용 할 수 없음
* 2.구조체를 바이너리화 해서 전송 하고 싶습니다.
*/