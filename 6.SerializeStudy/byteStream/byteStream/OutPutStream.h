#pragma once
#include "global.h"

class OutPutStream:public MemoryStream
{
public:
	OutPutStream() :m_Buff(nullptr), m_Head(0), m_Capacity(32)
	{
		ReAllocBuffer(32);
	}
	~OutPutStream() { delete m_Buff; }
	char* GetOutStream() { return m_Buff; }
	uint32_t GetCapacity() { return m_Capacity; }
	void Serialize(void* Data,size_t Datasize);
	void Serialize(char* Data);
	void Parsing(){}
	bool IsInput()const;
	void Write(void* Data, size_t Datasize);
	template <typename T>
	void Write(T Data);
	//void Write(char* Data);
	//void Write(char* Data,char* type_name=nullptr);
	void FileSave();
private:
	void ReAllocBuffer(uint32_t size);
	char* m_Buff;
	uint32_t m_Head;
	uint32_t m_Capacity;
};
