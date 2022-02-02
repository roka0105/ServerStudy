#pragma once
#include <iostream>
#include <map>
using namespace std;

#define interface class

class Player
{
public:
	Player() :name(" "), age(0), weight(0) {}
	Player(const char* _name, int _age, float _weight) :age(_age), weight(_weight)
	{
		strcpy(name, _name);
	}
	~Player(){}

	char name[20];
	int age;
	float weight;
};
class OutPutStream;
interface MemoryStream
{
	virtual void Serialize(void* Data, size_t Datasize) = 0;
	virtual void Serialize(char* Data) = 0;
	virtual void Parsing() = 0;
	virtual bool IsInput()const = 0;
	template <typename T>
	void Serialize(T& Data)
	{
		Serialize(&Data);
	}
};
#include "OutPutStream.h"
#include "InPutStream.h"

