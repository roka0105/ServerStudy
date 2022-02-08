#include "global.h"
#include "structRTTI.h"
#include <fstream>
STRUCT_BEGIN(yeram)
STRUCT_VAR_INT(yeram,age)
STRUCT_VAR_FLOAT(yeram,weight)
STRUCT_VAR_STRING(yeram,username)
STRUCT_END()

int main()
{
	yeram myname;
	/*myname.username = "최예람";
	myname.age = 25;
	myname.weight = 67.01f;

	ofstream file("test.txt");
	if (!file.is_open())
	{
		cout << "파일열기실패" << endl;
	}
	xml_write(file, myname);*/
	ifstream file("test.txt");
	if(!file.is_open())
	{
		cout << "파일읽기 실패" << endl;
		return 0;
	}
	xml_read(file, myname);
	cout << myname.age << myname.username << myname.weight << endl;
	return 0;
}