#include <iostream>
using namespace std;
struct Game
{
	int id;
};
class Hello
{
private:
	string name;
};
int main()
{
	int a = 1;
	float b = 1.2f;
	double c = 0.6;
	const char* buf = "¾È³ç";
	char* buf2 = new char[3];

	memcpy(buf2, "hi", sizeof("hi"));
	cout << typeid(a).name() << endl;
	cout << typeid(b).name() << endl;
	cout << typeid(c).name() << endl;
	cout << typeid(buf).name() << endl;
	cout << typeid(buf2).name() << endl;
	cout << typeid(Game).name() << endl;
	cout << typeid(Hello).name() << endl;
}