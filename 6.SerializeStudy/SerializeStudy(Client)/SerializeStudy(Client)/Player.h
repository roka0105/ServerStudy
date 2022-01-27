#pragma once
#include "global.h"
typedef struct ITEM
{
	ITEM()
	{
		ZeroMemory(name, MAXBUF);
		atk = 0;
	}
	char name[MAXBUF];
	int atk;
};
class Player
{
private:
	int mHp;
	int mMp;
	float mExp;
	char name[MAXBUF];
	ITEM* item;
	vector<int> number;
public:
	Player();
	Player(int hp, int mp, float exp,const char* _name);
	~Player();
	void Write(OutPutMemoryStream& oStream)const;
	void Read(InPutMemoryStream& iStream)const;
	void Print();
};

