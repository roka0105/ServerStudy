#include <iostream>
#include <fstream>
using namespace std;
#define MAXBUF 90000
struct Data
{
	int num1;
	int num2;
	int num3;
	int num4;
	char name[MAXBUF];
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
bool ReadToFile2(const char* filepath, char* data, int* datasize)
{
	FILE* fp = fopen(filepath, "rb");
	if (fp == NULL)return false;
	fread(data, *datasize, 1, fp);
	return true;
}
bool ReadToFile3(const char* filepath, char* data, int* datasize)
{
	FILE* fp = fopen(filepath, "rt");
	if (fp == NULL)return false;
	fread(data, *datasize, 1, fp);
	return true;
}
int WriteToFile2(const char* filepath,char* data, int* datasize)
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
//txt
int WriteToFile3(const char* filepath, char* data, int* datasize)
{
	FILE* fp = fopen(filepath, "wt");
	if (fp == NULL)return 0;
	fwrite(data, *datasize, 1, fp);
	fclose(fp);
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
	//���̳ʸ��� ����ü ����.
	string filename = "Image.jpg";
	Data d1;
	d1.num1 = 100;
	d1.num2 = 200;
	d1.num3 = 300;
	d1.num4 = 400;
	memset(d1.name, '\0', MAXBUF);
	strcpy(d1.name, "���̸��� ����!");
	char* str2=nullptr;
	int datasize = MAXBUF;
	//ReadToFile(filename, str2, &datasize);
	//cout << str2 << endl;
	datasize = sizeof(struct Data);
	WriteToFile2("Data.bin",(char*)&d1,&datasize);
	Data d2;
	ReadToFile2("Data.bin",(char*)&d2, &datasize);


	str2 = new char[MAXBUF];
	//���� ���̳ʸ��� �о�ͼ� ���� ����.
	ReadToFile2("Image.jpg", str2, &datasize);
	WriteToFile2("Dog.jpg", str2, &datasize);
	
	//text�� ������ ����.
	//�̰� �׳� �����ϰ� �о���°�. 
	//? ������ü�� ���̳ʶ������̶� �Ӱ� �ٸ��� ������ �Ȱ��� ǥ�� �Ǵµ�..
	//char�� �����ؼ� ���� �����°��� char 1 int 4 �̴ϱ�.
	//���������� ������ struct ����.
    WriteToFile3("Txt.txt", (char*)&d1, &datasize);
	ReadToFile3("Txt.txt", (char*)&d2, &datasize);
	cout << d2.num1 << endl;
	cout << d2.num2 << endl;
	cout << d2.num3 << endl;
	cout << d2.num4 << endl;
	cout << d2.name << endl;
	// ,�� ������ ���� ;���� ���� �з� �����ϱ�.(csv)


}
