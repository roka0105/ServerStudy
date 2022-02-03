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
	template <typename T>
	void Serialize(T& Data)
	{
		MemoryStream::Serialize(Data);
	}
	void Serialize(void* Data,size_t Datasize);
	//void Serialize(char* Data);
	template <typename T>
	void Parsing(T& Data, char* param_name)
	{
		MemoryStream::Parsing(Data, param_name);
	}
	void Parsing(void* Data, size_t Datasize);
	void StartParsing(char* Data)
	{
		char* buf = nullptr;
		sprintf(buf, "<%s>",Data);
		size_t size = strlen(buf);
		memcpy(m_Buff + m_Head, buf, size);
		m_Head += size;
	}
	void EndParsing(char* Data)
	{
		char* buf = nullptr;
		sprintf(buf, "</%s>", Data);
		size_t size = strlen(buf);
		memcpy(m_Buff + m_Head, buf, size);
		m_Head += size;
	}
	void DataParsing(void* Data)
	{
		char* buf = nullptr;
		sprintf(buf, "\"%s\"",(char*)Data);
		size_t size = strlen(buf);
		memcpy(m_Buff + m_Head, buf, size);
		m_Head += size;
	}
	bool IsInput()const;
	void Write(void* Data, size_t Datasize);
	//template <typename T>
	//void Write(T Data);
	//void Write(char* Data);
	//void Write(char* Data,char* type_name=nullptr);
	void FileSave();
private:
	void ReAllocBuffer(uint32_t size);
	char* m_Buff;
	uint32_t m_Head;
	uint32_t m_Capacity;
};
