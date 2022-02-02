#include "global.h"
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
	/*int a = 1;
	float b = 1.2f;
	double c = 0.6;
	const char* buf = "¾È³ç";
	char* buf2 = new char[100];
	Game game;
	memcpy(buf2, "hi", sizeof("hi"));
	cout << sizeof(buf2) << endl;
	cout << strlen(buf2) << endl;
	cout << typeid(a).name() << endl;
	cout << typeid(b).name() << endl;
	cout << typeid(c).name() << endl;
	cout << typeid(buf).name() << endl;
	cout << typeid(buf2).name() << endl;
	cout << typeid(game).name() << endl;
	cout << typeid(Hello).name() << endl;
	cout << sizeof(Game) << endl;
	cout << sizeof(game) << endl;*/

	Player yeram("ÃÖ¿¹¶÷",25,70.2);
	OutPutStream outstream;
	cout << typeid(yeram.name).name() << endl;
	outstream.Serialize(yeram.name);
	outstream.Serialize((void*)&yeram.age,sizeof(int));
	outstream.Serialize((void*)&yeram.weight,sizeof(float));

	InPutStream instream;
	instream.SetBuff(outstream.GetOutStream(),outstream.GetCapacity());
	Player Bye;
	instream.Read(Bye.name, 6);
	instream.Read(&Bye.age, sizeof(int));
	instream.Read(&Bye.weight, sizeof(float));

	cout << Bye.name << endl;
	cout << Bye.age << endl;
	cout << Bye.weight << endl;
}