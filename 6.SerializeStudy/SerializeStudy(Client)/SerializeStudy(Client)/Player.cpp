#include "Player.h"
Player::Player():mHp(0),mMp(0),mExp(0),item(nullptr)
{
	ZeroMemory(name, MAXBUF);
}
Player::Player(int hp, int mp, float exp,const char* _name): mHp(hp), mMp(mp), mExp(exp), item(nullptr)
{
	strcpy(name, _name);
}
Player::~Player()
{
	delete item;
}
void Player::Write(OutPutMemoryStream& oStream)const
{
	oStream.Write(mHp);
	oStream.Write(mMp);
	oStream.Write((int)mExp);
	oStream.Write(name, MAXBUF);
}
void Player::Read(InPutMemoryStream& iStream)const
{
	iStream.Read((int*)&mHp);
	iStream.Read((int*)&mMp);
	iStream.Read((int*)&mExp);
	iStream.Read((void*)name,100);
}
void Player::Print()
{
	cout << "데이터 출력" << endl;
	cout << mHp << mMp << mExp << name << endl;
}