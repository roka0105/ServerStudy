#pragma once
#include <iostream>
#include <map>
#include <tchar.h>
#include <fstream>
#include <cassert>
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
	~Player() {}

	char name[20];
	string id;
	int age;
	float weight;
};
interface MemoryStream
{
protected:
	virtual void Serialize(void* Data, size_t Datasize) = 0;
	//virtual void Serialize(char* Data) = 0;+60
	virtual void Parsing(void* Data, size_t Datasize) = 0;
	virtual bool IsInput()const = 0;
	template <typename T>
	void Serialize(T& Data)
	{
		char* buf = (char*)typeid(T).name();
		char* temp = strtok(buf, " ");
		if (IsInput())
			Serialize(&Data,0);
		else if (!strcmp(temp, "char"))
			Serialize(&Data, strlen((const char*)&Data));
		else Serialize(&Data, sizeof(T));
	}
	template <typename T>
	void Parsing(T& Data,char* param_name)
	{
		/*if (IsInput())
			Parsing(Data);
		else if (!strcmp(param_name, ";"))
		{
			Parsing(";\n");
		}
		else 
		{   */
			size_t size = 0;
			char* type_name= (char*)typeid(T).name();
			type_name = strtok(type_name, " ");
			if (!strcmp(type_name, ";"))
			{
				strcpy(type_name, ";\n");
			}
			if (!strcmp(type_name, "char"))
			{
				size = strlen(&Data);
			}
			else size = sizeof(T);
			char* buf = nullptr;
			strcpy(type_name, param_name);
			sprintf(buf, "<%s>[%d]\"%s\"", type_name,size,&Data);
			size = strlen(buf);
			Parsing(buf,size);
		//}
	}
};
#include "OutPutStream.h"
#include "InPutStream.h"

