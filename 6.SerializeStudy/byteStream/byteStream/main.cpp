#include "global.h"
#include "struct_RTTI.h"

struct Game
{
	int id;
};
class Hi
{
protected:
	void Printf()
	{
		cout << "Hi" << endl;
	}
	virtual void Printf(string name) = 0;
};
class Hello :public Hi
{
private:
	string name;
public:
	//void Printf() { Hi::Printf(); }
	void Printf(string name) { cout << name << endl; }
};
#define OffSet(c,mv) ((size_t)&(static_cast<c*>(nullptr)->mv))

STRUCT_BEGIN(YES)
STRUCT_VAR_INT(YES, number)
STRUCT_VAR_FLOAT(YES, number2)
STRUCT_END()

int main()
{
	YES yes;
	/*yes.number = 10;
	yes.number2 = 0.1f;
	//ofstream file("test.txt");
	//if (file.is_open())
	//	xml_write(file, yes);
	wofstream file("test.txt");
	if (file.is_open())
		xml_write(file, yes);
	
	*/
	ifstream file("test.txt");
	if (file.is_open())
		xml_read(file, yes);
	cout << yes.number << endl;
	cout << yes.number2 << endl;
	file.close();
    /*Hello hello;
	hello.Printf();
	hello.Printf("�ֿ���");
	*/
	/*int a = 1;
	float b = 1.2f;
	double c = 0.6;
	const char* buf = "�ȳ�";
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
	//sample temp;
	//sample temp2;
	//cout << OffSet(Player, age);
	//cout << endl;
	Player yeram("�ֿ���", 25, 70.2);
	OutPutStream outstream;
	cout << sizeof(yeram.name) << endl;
	cout << typeid(yeram.name).name() << endl;
	/*outstream.Serialize(yeram.name);
	outstream.Serialize(yeram.age);
	outstream.Serialize(yeram.weight);
	cout << "outstream ������ ����:" << outstream.GetOutStream() << endl;
	*/
	//outstream.Parsing(yeram.name,)
	//outstream.FileSave();
	InPutStream instream;
	//instream.SetBuff(outstream.GetOutStream(),outstream.GetCapacity());
	//instream.FileRoad();
	/*cout << "instream ������ ����:" << instream.GetInStream() << endl;
	Player Bye;
	instream.Serialize(Bye.name);
	instream.Serialize(Bye.age);
	instream.Serialize(Bye.weight);

	cout << Bye.name << endl;
	cout << Bye.age << endl;
	cout << Bye.weight << endl;*/
}