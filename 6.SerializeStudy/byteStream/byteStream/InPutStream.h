#pragma once
#include "global.h"
class InPutStream:public MemoryStream
{
public:
	InPutStream() :m_Buff(nullptr), m_Head(0), m_Capacity(32)
	{
		ReAllocBuffer(32);
	}
	~InPutStream() { delete m_Buff; }
	void Serialize(void* Data, size_t Datasize)
	{
		Read(Data, Datasize);
	}
	void Serialize(char* Data){}
	void Parsing(){}
	bool IsInput()const { return true; }
	//임시
	void SetBuff(char* buf,uint32_t size)
	{
		if (m_Capacity < size)
		{
			ReAllocBuffer(size);
		}
		memcpy(m_Buff, buf, size);
		m_Capacity = size;
	}
	void Read(void* Data, size_t Datasize)
	{
		memcpy(Data, m_Buff+m_Head, Datasize);
		m_Head += Datasize;
	}
	
private:
	void ReAllocBuffer(uint32_t size)
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
	char* m_Buff;
	uint32_t m_Head;
	uint32_t m_Capacity;
};

