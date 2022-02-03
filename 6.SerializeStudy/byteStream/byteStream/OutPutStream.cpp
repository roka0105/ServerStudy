#include "OutPutStream.h"
void OutPutStream::Write(void* Data, size_t Datasize)
{
	uint32_t result = m_Head + Datasize+sizeof(int);
	if (m_Capacity < result)
	{
		ReAllocBuffer(max(m_Capacity * 2, result));
	}
	memcpy(m_Buff + m_Head, &Datasize, sizeof(int));
	memcpy(m_Buff + m_Head+sizeof(int), Data, Datasize);
	m_Head = result;
}
void OutPutStream::Serialize(void* Data,size_t Datasize)
{
	Write(Data,Datasize);
}
void OutPutStream::Parsing(void* Data, size_t Datasize)
{
	memcpy(m_Buff+m_Head, Data, Datasize);
	m_Head += Datasize;
}
//void OutPutStream::Serialize(char* Data)
//{
//	Write(Data);
//}
bool OutPutStream::IsInput()const
{
	return false;
}
//template <typename T>
//void OutPutStream::Write(T Data)
//{
//	//static_assert(is_arithmetic<T>::value || is_enum<T>::value, "Generic  only supports primitive data types");
//	char* temp = (char*)typeid(T).name();
//	char* buf = strtok(temp," ");
//	
//	if (!strcmp(buf, "char"))
//	{
//		size_t size = strlen((const char*)Data);
//		Write(Data,size);
//	}
//	else Write(&Data, sizeof(Data));
//}
//void OutPutStream::Write(char* Data)
//{
//	uint32_t size = strlen(Data);
//	Write(Data, size);
//}
//void OutPutStream::Write(char* Data, char* type_name)
//{
//	if (type_name != nullptr)
//	{
//		uint32_t typename_size = strlen(type_name);
//		Write(type_name, typename_size);
//	}
//	uint32_t datasize = strlen(Data);
//	Write(Data, datasize);
//}
void OutPutStream::FileSave()
{
	FILE* fp = fopen("binary.txt", "wb");
	if (fp == NULL)
	{
		cout << "파일열기실패!" << endl;
		exit(-1);
	}
	fwrite(m_Buff, m_Capacity, 1, fp);
	fclose(fp);
}
void OutPutStream::ReAllocBuffer(uint32_t size)
{
	if (m_Buff != NULL)
	{
		char* temp = new char[size];
		memset(temp, '\0', size);
		memcpy(temp, m_Buff, size);
		delete m_Buff;
		m_Buff = temp;
	}
	else
	{
		m_Buff = new char[size];
		memset(m_Buff, '\0', size);
	}
	//m_Buff = (char*)realloc(m_Buff, size);
	if (m_Buff == NULL)
	{
		cout << "메모리할당 실패" << endl;
		exit(-1);
	}
	m_Capacity = size;
}