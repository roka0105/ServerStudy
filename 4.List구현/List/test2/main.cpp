#include "_List.h"
//���庼�� operator[] �� input output���� �����ϰ� ������ ���� �Ű������� ���Ƽ� ������ ���� ����µ� 
//��� �ϴ°� �������? �ذ��Ѱ� ����!
//���� 2: ���Կ����� �����ε� ������ �ϰ������ ��� �ؾ��ϳ���?

//���� ���� operator ������ �� �� ������ ����. 
//1.
//�Ű������� �ΰ� ���� ������ ���� Ÿ�԰� ������ ���� Ÿ���ε�
//�Ű������� �Ѱ��� ������ ����,�ȳ����� ����,�Ѵ� ������ ���������� ���������� ������ �� �ִ°� ����.
//2.
//cout << ������ ���� operator�� ostream���� ������ �����Ű������� �ְ� return �� os�� main�� cout�� ó���Ѵ�.
//3.
// operator <<�� friend�� �� ������ intList���ο��� ���ÿ��� node���� ����,�Լ��� public������ �ܺο���
// ���ٽ� private�� �ǵ��� ������ ���� �����ΰͰ���. �׷��� intList ��� �Լ��� friend�� operator<<�� �����Ͽ�
// ������ ������� �ߴ�. friend�� �����ϴ� ����.


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
	//������ ���Կ����� ���ǰ� �ȵǾ���� �ӽ÷� << �� �����.
	number << intList[1];
	cout << number << endl;
	*/

	//==============================================================
	// *�� list �׽�Ʈ
	_List<int*> pointList;
	pointList.Push_back(new int(3));
	pointList.Push_back(new int(4));
	pointList.Push_back(new int(5));

	for (int i = 0; i < pointList.size(); ++i)
	{
		cout << "==========����Ʈ ��(�ּ�) ���=========" << endl;
		cout << pointList[i] << endl;
		cout << "=============��� �ּ� ���===========" << endl;
		cout << &(pointList[i]) << endl;
		cout << "==========����Ʈ ��(�ּ�)�� �� ���=========" << endl;
		int* temp;
		temp << pointList[i];
		cout << *temp << endl;
	}
	cout <<endl<<endl<< "***************************************************" << endl;

	cout << "==========����Ʈ ��(�ּ�)�� �޸�����=========" << endl;
	for (int i = 0; i < pointList.size(); ++i)
	{
		int* temp;
		temp << pointList[i];
		cout << temp << " delete" << endl;
		delete temp;
	}
	cout << "==========����Ʈ ��� �޸�����=========" << endl;
	pointList.Clear();
	return 0;
}