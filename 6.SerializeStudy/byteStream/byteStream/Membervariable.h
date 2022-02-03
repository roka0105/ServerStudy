#include "global.h"
#include <vector>
enum class PrimitiveType
{
	Char = 1,
	String,
	Int,
	Float,
	Double,
};
class MemberVariable
{
public:
	MemberVariable(const char* _name, PrimitiveType _type, uint32_t _offset) :
		name(_name), type(_type), offset(_offset)
	{

	}
	const char* GetName()const
	{
		return name.c_str();
	}
	PrimitiveType GetType()const
	{
		return type;
	}
	unsigned int GetOffset()const
	{
		return offset;
	}
private:
	string name;
	PrimitiveType type;
	uint32_t offset;
};
class DataType
{
public:
	/*template <typename Iterator>
	DataType(Iterator begin, Iterator end) :memberVariables(begin, end)
	{

	}*/
	DataType(initializer_list<MemberVariable> inMVs) :
		memberVariables{inMVs}
	{

	}
	const vector<MemberVariable>& GetMemberVariable() const
	{
		return memberVariables;
	}
private:
	vector<MemberVariable> memberVariables;
};
#define OffSetOf(c,mv)((size_t)&(static_cast<c*>(nullptr)->mv))

class Student
{
	string name = "Yeram";
	int age = 25;
	float weight = 70.2;
	static DataType* sDataType;
	static void InitDataType()
	{
		sDataType = new DataType
		({ 
			MemberVariable("name", PrimitiveType::String, OffSetOf(Student, name)),
			MemberVariable("age", PrimitiveType::Int, OffSetOf(Student, age)),
			MemberVariable("weight", PrimitiveType::Float, OffSetOf(Student, weight))
		});
	}

};

void Serialize(MemoryStream* inmemoryStream, const DataType* inDataType, uint8_t* inData)
{
	for (auto& mv : inDataType->GetMemberVariable())
	{
		void* mvData = inData + mv.GetOffset();
		switch (е╦ют)
		{
		case Int:
			inmemoryStream->Serialize(*(int*)mvData);
		}
	}
}