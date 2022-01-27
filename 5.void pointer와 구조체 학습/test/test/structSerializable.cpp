#include <iostream>
using namespace std;
typedef struct Data
{
	int age;
	char name[100];
	float money;
};
#define interface class 
template <typename T>
interface ISerialize
{
	virtual void SerializeType1(T data) = 0;
};
class DataPacking :public ISerialize<Data>
{
	void SerializeType1(Data data)
	{
		char buf[100];
		char* ptr = buf;
		memcpy(ptr, &data.age, sizeof(int));
		ptr += sizeof(int);

		memcpy(ptr,)
	}
};

int main()
{

}