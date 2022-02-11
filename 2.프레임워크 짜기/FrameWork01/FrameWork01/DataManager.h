#pragma once
#define interface class
interface DataSystem
{
public:
	virtual void Serialize(bool is_save) = 0;
	virtual void Parsing(bool is_save) = 0;
};
class DataManager:DataSystem
{
public:
	virtual void Serialize(bool is_save)
	{
		if (is_save)
		{
			//파일저장
		}
		else
		{
			//파일읽기
		}
	}
	virtual void Parsing(bool is_save)
	{
		if (is_save)
		{
			//파일저장
		}
		else
		{
			//파일읽기
		}
	}
};

