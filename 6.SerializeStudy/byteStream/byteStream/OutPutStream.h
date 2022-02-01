#pragma once
#include <iostream>
#include <map>
using namespace std;
#define interface class

interface MemoryStream
{
	virtual void Serialize(void* Data,uint32_t ByteCount) = 0;
	virtual bool IsInput()const = 0;
};
class InputMemoryStream :public MemoryStream
{

};
