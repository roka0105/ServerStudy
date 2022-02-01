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
	oStream.Write(mExp);
	oStream.Write(name, MAXBUF);
}
void Player::Read(InPutMemoryStream& iStream)const
{
	iStream.Read(mHp);
	iStream.Read(mMp);
	iStream.Read(mExp);
	iStream.Read((void*)name,100);
}
void Player::Print()
{
	cout << "데이터 출력" << endl;
	cout << mHp <<endl<< mMp <<endl<< (float)mExp <<endl<< name << endl;
	cout<<typeid(mExp).name() << endl;
}