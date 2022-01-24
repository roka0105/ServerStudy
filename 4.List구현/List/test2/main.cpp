#include "_List.h"
//여쭤볼것 operator[] 을 input output으로 구현하고 싶은데 둘이 매개변수가 같아서 재정의 오류 생기는데 
//어떻게 하는게 좋을까요? 해결한것 같다!

//오늘 배운것 operator 개념을 좀 더 잡은것 같다. 
//1.
//매개변수가 두개 들어가면 전위에 들어온 타입과 후위에 들어온 타입인데
//매개변수를 한개만 넣으면 후위,안넣으면 전위,둘다 넣으면 후위로할지 전위로할지 지정할 수 있는것 같다.
//2.
//cout << 연산을 위한 operator는 ostream으로 선언한 참조매개변수에 넣고 return 한 os를 main의 cout이 처리한다.
int main()
{
	_List<int> intList;
	intList.Push_back(1);
	intList.Push_back(2);
	intList.Push_back(3);
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
	}
	cout << "=========Clear===========" << endl;
	intList.Clear();
	for (int i = 0; i < intList.size(); ++i)
	{
		cout << intList[i];
	}
	return 0;
}