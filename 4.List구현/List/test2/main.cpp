#include "_List.h"
//���庼�� operator[] �� input output���� �����ϰ� ������ ���� �Ű������� ���Ƽ� ������ ���� ����µ� 
//��� �ϴ°� �������? �ذ��Ѱ� ����!

//���� ���� operator ������ �� �� ������ ����. 
//1.
//�Ű������� �ΰ� ���� ������ ���� Ÿ�԰� ������ ���� Ÿ���ε�
//�Ű������� �Ѱ��� ������ ����,�ȳ����� ����,�Ѵ� ������ ���������� ���������� ������ �� �ִ°� ����.
//2.
//cout << ������ ���� operator�� ostream���� ������ �����Ű������� �ְ� return �� os�� main�� cout�� ó���Ѵ�.
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