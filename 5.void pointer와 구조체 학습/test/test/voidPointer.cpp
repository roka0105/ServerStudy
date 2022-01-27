#include <iostream>
using namespace std;

typedef struct Test
{
	Test()
	{
		number = 0;
		name = 'a';
		number2 = 0.1f;
		number3 = 0.01;
	}
	int number;
	char name;
	float number2;
	double number3;
};
int main()
{
	void* _void[10];
	Test test;
	_void[0] = new int(test.number);
	_void[1] = new char(test.name);
	_void[2] = new float(test.number2);
	_void[3] = new double(test.number3);
	int data = *((int*)_void[0]);

	printf("%d\n", test.number);
	printf("%d\n", *((int*)_void[0]));
	for (int i = 0; i < 4; ++i)
	{
		auto num = (_void[i]);
		printf("%s,", typeid(Test::number).name());
		printf("%d\n", (int*)_void[i]);

	}
}