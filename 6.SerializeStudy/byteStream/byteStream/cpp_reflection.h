#pragma once
#include "global.h"

class field_t
{
public:
	field_t(string _name) :field_name(_name) {}
	template <typename _FieldTy,typename _ObjTy>
	_FieldTy& ref(_ObjTy* obj)
	{
		_FieldTy* address = reinterpret_cast<_FieldTy*>(ptr(obj));
		return *address;
	}
	const char* name() const { return field_name.c_str(); }
protected:
	virtual void* ptr(void* obj) const = 0;
private:
	string field_name;
};

template <typename _ObjTy, typename _FieldTy, typename _FieldTy(_ObjTy::*Field)>
class field_impl_t :public field_t
{
public:
	field_impl_t(string _name):field_t(_name){}
protected:
	virtual void* ptr(void* obj_addr)const
	{
		_ObjTy* obj = reinterpret_cast<_ObjTy*>(obj_addr);
		return &(obj->*Field);
	}
};
template <typename _Ty>
struct field_info_map_t
{
protected:
	typedef _Ty impl_type;
	typedef map<string, field_t*> _info_map_t;
	static _info_map_t info_map;

	template <typename _FieldTy,typename _FieldTy(_Ty::*Field)>
	static void _register_field(string name)
	{
		cout << name << "Field 林家:" << _Ty::Field << "蔼" << _Ty::*Field << endl;
		info_map.insert(make_pair(name, new field_impl_t<_Ty, _FieldTy, Field>(name)));
	}
public:
	typedef map<string, field_t*>::iterator field_iterator_t;
	
	static field_t* get_field(string name)
	{
		return info_map[name];
	}
	static field_iterator_t Get_field_begin() { return info_map.begin(); }
	static field_iterator_t Get_field_end() { return info_map.end(); }
};
template <typename _Ty>
typename field_info_map_t<_Ty>::_info_map_t field_info_map_t<_Ty>::info_map;

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
impl_type::_register_field<type,&impl_type::name>(#name);


struct user_t :field_info_map_t<user_t>
{
	int index;
	string name;
	
	REGISTER_FIELD_BEGIN()
	REGISTER_FIELD(int, index)
	REGISTER_FIELD(string, name)
	REGISTER_FIELD_END()
public:
	void Printf()
	{
		cout << "index林家:" << &index << endl;
	}
};







class class_t
{
public:
	class_t(string _name);
	const field_t* get_field(string name)const;
	void add_field(string name, field_t* field);
	template <typename _Class>
	_Class* new_instance()const
	{
		return reinterpret_cast<_Class*>(new_instance_impl());
	}
private:
	virtual void* new_instance_impl()const = 0;
private:
	typedef map<string, field_t*>field_map_t;
	field_map_t field_map;
};

template <typename _ObjTy>
class class_impl_t :public class_t
{
public:
	class_impl_t(string name);
private:
	virtual void* new_instance_impl()const
	{
		return new _ObjTy;
	}
};

class reflection_base
{
public:
	static reflection_base& instance();
	const class_t* get_class(string name)const;
	const field_t* get_field(string class_name, string field_name)const;
public:
	void add_class(string name, class_t* class_impl);
	void add_field(string class_name, string field_name, field_t* field_impl);

	void add_class_name(const type_info* typeinfo, string class_name);
	const char* class_name_from_typeinfo(const type_info* typeinfo)const;
private:
	reflection_base(){}
	reflection_base(const reflection_base&);
	reflection_base& operator=(const reflection_base&);

private:
	typedef map<const type_info*, string> class_name_map_t;
	typedef map<string, class_t*> class_map_t;
	class_name_map_t class_name_map;
	class_map_t class_map;
};

class reflection_class_t
{
public:
	virtual ~reflection_class_t(){}
	const char* type_name()const
	{
		return typeid(*this).name();
	}
	const char* class_name()const
	{
		return reflection_base::instance().class_name_from_typeinfo(&typeid(*this));
	}
	const class_t* get_class()const
	{
		return class_t::from_name(class_name());
	}
};