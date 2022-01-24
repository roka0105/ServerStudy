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
	//바이너리로 구조체 저장.
	string filename = "Image.jpg";
	Data d1;
	d1.num1 = 100;
	d1.num2 = 200;
	d1.num3 = 300;
	d1.num4 = 400;
	memset(d1.name, '\0', MAXBUF);
	strcpy(d1.name, "내이름은 예람!");
	char* str2=nullptr;
	int datasize = MAXBUF;
	//ReadToFile(filename, str2, &datasize);
	//cout << str2 << endl;
	datasize = sizeof(struct Data);
	WriteToFile2("Data.bin",(char*)&d1,&datasize);
	Data d2;
	ReadToFile2("Data.bin",(char*)&d2, &datasize);


	str2 = new char[MAXBUF];
	//사진 바이너리로 읽어와서 파일 쓰기.
	ReadToFile2("Image.jpg", str2, &datasize);
	WriteToFile2("Dog.jpg", str2, &datasize);
	
	//text로 데이터 저장.
	//이건 그냥 저장하고 읽어오는거. 
	//? 파일자체는 바이너라파일이랑 머가 다르지 내용이 똑같이 표기 되는데..
	//char로 저장해서 값이 깨지는거임 char 1 int 4 이니까.
	//구문단위로 나눠서 struct 저장.
    WriteToFile3("Txt.txt", (char*)&d1, &datasize);
	ReadToFile3("Txt.txt", (char*)&d2, &datasize);
	cout << d2.num1 << endl;
	cout << d2.num2 << endl;
	cout << d2.num3 << endl;
	cout << d2.num4 << endl;
	cout << d2.name << endl;
	// ,로 데이터 구분 ;으로 묶음 분류 구현하기.(csv)


}
