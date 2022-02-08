#pragma once
#include "global.h"
class field_t
{
public:
	field_t(string _name) :field_name(_name) {}
	template <typename FieldTy, typename ObjTy>
	FieldTy& ref(ObjTy* obj)
	{
		FieldTy* addr = reinterpret_cast<FieldTy*>(ptr(obj));
		return *addr;
	}
	const char* Getname() { return field_name.c_str(); }
protected:
	virtual void* ptr(void* obj)const = 0;
protected:
	string field_name;
};
template <typename FieldTy, typename ObjTy, typename FieldTy(ObjTy::* Field)>
class field_impl_t :public field_t
{
public:
	field_impl_t(string _name) :field_t(_name) {}
protected:
	void* ptr(void* obj)const
	{
		ObjTy* object = reinterpret_cast<ObjTy*>(obj);
		return &(object->*Field);
	}
};
template <typename _Ty>
struct field_info_map_t
{
protected:
	typedef _Ty impl_type;
	typedef map<string, field_t*> fieldmap;
	static fieldmap fieldInfo_map;
	template <typename FieldTy, typename FieldTy(_Ty::* Field)>
	static void register_field(string name)
	{
		fieldInfo_map.insert(make_pair(name, new field_impl_t<FieldTy, _Ty, Field>(name)));
	}
public:
	typedef map<string, field_t*>::iterator fieldInfo_Iterator;
	static field_t* GetField(string name)
	{
		return fieldInfo_map[name];
	}
	static fieldInfo_Iterator get_field_begin()
	{
		return fieldInfo_map.begin();
	}
	static fieldInfo_Iterator get_field_end()
	{
		return fieldInfo_map.end();
	}
};
template <typename _Ty>
typename field_info_map_t<_Ty>::fieldmap field_info_map_t<_Ty>::fieldInfo_map;

#define REGISTER_FIELD_BEGIN()\
struct __register\
{\
__register()\
{\
static bool init=false;\
if(init)return;

#define REGISTER_FIELD_END()\
}\
}_register_auto;

#define REGISTER_FIELD(type,name)\
impl_type::register_field<type,&impl_type::name>(#name);

struct user_t :field_info_map_t<user_t>
{
	int index;
	string name;

	REGISTER_FIELD_BEGIN()
		REGISTER_FIELD(int, index)
		REGISTER_FIELD(string, name)
	REGISTER_FIELD_END()
};
