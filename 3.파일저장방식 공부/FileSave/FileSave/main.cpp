#include <iostream>
#include <fstream>
using namespace std;
#define MAXBUF 5000
struct Data
{
	int num1;
	int num2;
	int num3;
	int num4;
};
char* Alloc_Buffer(int size);
//포인터 객체를 넘기는것도 값복사가 되기 때문에 매개변수로 부른 char*와 main에서 넣은 str2은 
//서로 다르지만 같은 공간을 가르키고 있는것인데, 함수 내에서 data에 대한 주소값을 바꾸게 되면
//data는 buffer를 가르키지만 main의 str2는 그대로 null 을 가르키게 된다.
//따라서 주소를 연결하고자 할때는 참조를 붙여서 str2 자기 자신을 보내야한다.
//여태 *로 넘긴 매개변수를 잘만 사용했던것은 복사된 data와 str2가 같은 주소를 가르켰기 때문이다,
//와 너무 신기하다 좋은거 하나 깨달았다.
bool ReadToFile(string filepath, char* &data, int* datasize)
{
	ifstream file(filepath, ifstream::binary);
	if (file)
	{
		file.seekg(0, file.end);
		int length = (int)file.tellg();
		file.seekg(0, file.beg);
		
		char* buffer = Alloc_Buffer(length);
		char* ptr = buffer;
		while (!file.eof())
		{
			file.get(*ptr);	
			cout << *ptr << endl;
			ptr += sizeof(char);
			//cout << file.cur<<endl<<buffer<<endl;
		   // file.seekg(file.cur,)
		}
		//file.read((char*)buffer, 1);
		file.close();
		//cout << buffer << endl;
		data = buffer;
		*datasize = length;
	}
	return true;
}
bool ReadToFile2(string filepath, Data* data, int* datasize)
{
	ifstream file(filepath, ifstream::binary);
	if (file)
	{
		file.seekg(0, file.end);
		int length = (int)file.tellg();
		file.seekg(0, file.beg);

		char* buffer = Alloc_Buffer(length);
		char* ptr = buffer;
		while (!file.eof())
		{
			file.get(*ptr);
			data->num1 = *ptr;
			cout << *ptr << endl;
			ptr += sizeof(char);
			//cout << file.cur<<endl<<buffer<<endl;
		   // file.seekg(file.cur,)
		}
		//file.read((char*)buffer, 1);
		file.close();
		//cout << buffer << endl;
		//data = buffer;
		*datasize = length;
	}
	return true;
}
int WriteToFile2(const char* filepath,Data* data, int* datasize)
{
	FILE* fp = fopen(filepath, "wb");
	if (fp == NULL)return 0;
	fwrite(data, sizeof(Data), 1, fp);
	fclose(fp);
	return 0;
}
int WriteToFile(string filepath, char* data, int* datasize)
{
	ofstream file(filepath, ios::out | ios::binary);
	if (file.is_open())
	{
		file.write((const char*)data, *datasize);
		file.close();
	}
	return 0;
}
char* Alloc_Buffer(int size)
{
	char* buffer = new char[size];
	memset(buffer, '\0', size);
	return buffer;
}

int main()
{
	string filename = "Data.bin";
	Data d1;
	d1.num1 = 100;
	d1.num2 = 200;
	d1.num3 = 300;
	d1.num4 = 400;
	char* str2=nullptr;
	int datasize = MAXBUF;
	//ReadToFile(filename, str2, &datasize);
	//cout << str2 << endl;
	datasize = sizeof(struct Data);
	WriteToFile2("Data.bin",&d1,&datasize);
	Data d2;
	ReadToFile2(filename,&d2, &datasize);

	cout << d2.num1 << endl;
}
//구조체 정보가 16진수가 아니라 문자로 저장되는 문제를 어떻게 해결할까