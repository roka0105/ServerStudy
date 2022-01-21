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
//������ ��ü�� �ѱ�°͵� �����簡 �Ǳ� ������ �Ű������� �θ� char*�� main���� ���� str2�� 
//���� �ٸ����� ���� ������ ����Ű�� �ִ°��ε�, �Լ� ������ data�� ���� �ּҰ��� �ٲٰ� �Ǹ�
//data�� buffer�� ����Ű���� main�� str2�� �״�� null �� ����Ű�� �ȴ�.
//���� �ּҸ� �����ϰ��� �Ҷ��� ������ �ٿ��� str2 �ڱ� �ڽ��� �������Ѵ�.
//���� *�� �ѱ� �Ű������� �߸� ����ߴ����� ����� data�� str2�� ���� �ּҸ� �����ױ� �����̴�,
//�� �ʹ� �ű��ϴ� ������ �ϳ� ���޾Ҵ�.
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
//����ü ������ 16������ �ƴ϶� ���ڷ� ����Ǵ� ������ ��� �ذ��ұ�