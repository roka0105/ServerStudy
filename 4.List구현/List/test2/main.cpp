#include "_List.h"
//여쭤볼것 operator[] 을 input output으로 구현하고 싶은데 둘이 매개변수가 같아서 재정의 오류 생기는데 
//어떻게 하는게 좋을까요? 해결한것 같다!
//질문 2: 대입연산자 오버로딩 전위로 하고싶을때 어떻게 해야하나요?

//오늘 배운것 operator 개념을 좀 더 잡은것 같다. 
//1.
//매개변수가 두개 들어가면 전위에 들어온 타입과 후위에 들어온 타입인데
//매개변수를 한개만 넣으면 후위,안넣으면 전위,둘다 넣으면 후위로할지 전위로할지 지정할 수 있는것 같다.
//2.
//cout << 연산을 위한 operator는 ostream으로 선언한 참조매개변수에 넣고 return 한 os를 main의 cout이 처리한다.
//3.
// operator <<에 friend를 한 이유는 intList내부에서 사용시에는 node관련 변수,함수가 public이지만 외부에서
// 접근시 private가 되도록 설정해 놨기 때문인것같다. 그래서 intList 멤버 함수에 friend로 operator<<를 구현하여
// 접근이 가능토록 했다. friend는 공유하는 느낌.


int main()
{
	/*
	_List<int> intList;
	intList.Push_back(1);
	intList.Push_back(2);
	intList.Push_back(3);
	*/
	/*cout << "=========pushback===========" << endl;
	for (_List<int>::_Iterator itr = intList.begin(); itr != intList.end(); ++itr)
	{
		cout << *itr << endl;
	}
	int number=intList.Pop_front();
	cout << "=========popfront===========" << endl;
	cout << number << endl;
	cout << "==========erase(point)==========" << endl;
	for (_List<int>::_Iterator itr = intList.begin(); itr != intList.end(); ++itr)
	{
		if (*itr == number)
		{
			intList.erase(itr);
		}
	}
	for (_List<int>::_Iterator itr = intList.begin(); itr != intList.end(); ++itr)
	{
		cout << *itr << endl;
	}
	cout << "=========insert===========" << endl;
	for (_List<int>::_Iterator itr = intList.begin(); itr != intList.end(); ++itr)
	{
		if (*itr == 3)
		{
			intList.insert(itr,5);
			intList.insert(itr,5);
			intList.insert(itr, 4);
		}
	}
	for (_List<int>::_Iterator itr = intList.begin(); itr != intList.end(); ++itr)
	{
		cout << *itr << endl;
	}
	cout << "==========erase(number)==========" << endl;
	intList.erase(5);
	for (_List<int>::_Iterator itr = intList.begin(); itr != intList.end(); ++itr)
	{
		cout << *itr << endl;
	}*/
	/*
	for (int i = 0; i < intList.size(); ++i)
	{
		cout << intList[i];
	}

	intList[0] = 41;
	cout << intList[0];
	cout << "====================" << endl;
	for (int i = 0; i < intList.size(); ++i)
	{
		cout << intList[i];
	}*/
	/*cout << "=========Clear===========" << endl;
	intList.Clear();
	for (int i = 0; i < intList.size(); ++i)
	{
		cout << intList[i];
	}*/
	/*
	cout << "=========node =operator===========" << endl;
	int number;
	//전위로 대입연산자 정의가 안되어가지고 임시로 << 를 사용함.
	number << intList[1];
	cout << number << endl;
	*/

	//==============================================================
	// *형 list 테스트
	_List<int*> pointList;
	pointList.Push_back(new int(3));
	pointList.Push_back(new int(4));
	pointList.Push_back(new int(5));

	for (int i = 0; i < pointList.size(); ++i)
	{
		cout << "==========리스트 값(주소) 출력=========" << endl;
		cout << pointList[i] << endl;
		cout << "=============노드 주소 출력===========" << endl;
		cout << &(pointList[i]) << endl;
		cout << "==========리스트 값(주소)의 값 출력=========" << endl;
		int* temp;
		temp << pointList[i];
		cout << *temp << endl;
	}
	cout <<endl<<endl<< "***************************************************" << endl;

	cout << "==========리스트 값(주소)의 메모리해제=========" << endl;
	for (int i = 0; i < pointList.size(); ++i)
	{
		int* temp;
		temp << pointList[i];
		cout << temp << " delete" << endl;
		delete temp;
	}
	cout << "==========리스트 노드 메모리해제=========" << endl;
	pointList.Clear();
	return 0;
}