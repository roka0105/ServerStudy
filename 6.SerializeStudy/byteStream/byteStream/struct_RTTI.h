#pragma once
#include "global.h"
#include <string>
#include <tchar.h>
namespace std
{
	typedef wstring tstring;
	typedef wostream tostream;
	typedef wistream tistream;
}
enum type_t
{
	NONE,
	INT,
	FLOAT,
};
struct var_info
{
	var_info(const tstring& _name, const type_t& _type, size_t _offset)
		:name(_name), type(_type), offset(_offset) {}
	var_info(const var_info& o) :name(o.name), type(o.type), offset(o.offset) {}

	tstring name;
	type_t type;
	size_t offset;
};
typedef map<tstring, var_info> varmap_t;

struct struct_info
{
	struct_info(const tstring& _name) :name(_name) {}
	struct_info(const struct_info& o) :name(o.name) {}
	struct_info() {}

	tstring name;
	varmap_t varmap;
};

typedef map<tstring, struct_info> rtti_t;
rtti_t rtti;
#define OFFSET_OF(_struct,_var)((size_t)&(static_cast<_struct*>(nullptr)->_var))


struct sample
{
	struct _temp
	{
		_temp(string name)
		{
			static bool once = true;
			if (once)
			cout << "call struct _temp" << endl;
			once = false;
		}
	}__temp;

};

#define REGISTER_ONLY_ONCE(_rtti,_name,_info)\
static bool init=false;\
if(!init)\
_rtti.insert(make_pair(_T(#_name),_info));\
init=true;

#define REGISTER_RTTI_STRUCT(_name)\
REGISTER_ONLY_ONCE(rtti,_name,(struct_info(_T(#_name))))

#define REGISTER_RTTI_VAR(_struct,_var,_type)\
REGISTER_ONLY_ONCE(rtti[_T(#_struct)].varmap,_var,(var_info(_T(#_var),_type,OFFSET_OF(_struct,_var))))

#define STRUCT_BEGIN(_name)\
struct _name\
{\
private: \
struct __register_init{__register_init(){REGISTER_RTTI_STRUCT(_name);}}___register_init;\
public:\
static const tstring& name(){static tstring __name(_T(#_name));return __name;} 

#define STRUCT_VAR(_struct,_var,_ctype,_type)\
public:\
_ctype _var;\
private:\
struct __register_##_var{__register_##_var(){REGISTER_RTTI_VAR(_struct,_var,_type);}}___register_##_var;

#define STRUCT_VAR_INT(_struct,_var) STRUCT_VAR(_struct,_var,int,INT)
#define STRUCT_VAR_FLOAT(_struct,_var) STRUCT_VAR(_struct,_var,float,FLOAT)
#define STRUCT_END() };

#define GET_VAR_ADDR(_ctype,_obj,_offset)((_ctype*)(((char*)&_obj)+_offset))

#define GET_VAR(_ctype,_obj,_offset)(*(GET_VAR_ADDR(_ctype,_obj,_offset)))\


#define XML_START_TAG(name)  "<"<<name<<">"
#define XML_END_TAG(name) "</"<<name<<">"
#define XML_TAB "\t"

template <typename _stream,typename _struct>
bool xml_write(_stream& out, const _struct& obj)
{
	if (rtti.find(obj.name()) == rtti.end())return false;

	const struct_info& si = rtti[_struct::name()];
	out << XML_START_TAG(si.name) << endl;
	for (varmap_t::const_iterator i = si.varmap.begin(); i != si.varmap.end(); ++i)
	{
		const var_info& vi = i->second;
		out << XML_TAB << XML_START_TAG(vi.name);
		switch (vi.type)
		{
		case INT: out << GET_VAR(int, obj, vi.offset); break;
		case FLOAT:out << GET_VAR(float, obj, vi.offset); break;
		}
		out << XML_END_TAG(vi.name) << endl;
	}
	out << XML_END_TAG(si.name) << endl;
	return true;
}

#define COUNT_OF(_array) (sizeof(_array)/sizeof(_array[0]))

template <typename _stream,typename _struct>
bool xml_read(_stream& in, _struct& obj)
{
	if (rtti.find(obj.name()) == rtti.end())
		return false;

	const struct_info& si = rtti[_struct::name()];
	char name[1024], value[1024];
	memset(name, '\0', 1024);
	memset(value, '\0', 1024);
	in.ignore(1024, _T('>'));
	for (varmap_t::const_iterator iter = si.varmap.begin(); iter != si.varmap.end(); ++iter)
	{
		in.ignore(1024, _T('<'));
		in.getline((char*)name, COUNT_OF(name), _T('>'));
		in.getline((char*)value, COUNT_OF(value), _T('<'));
		string s_name(name);
		tstring t_name;
		t_name.assign(s_name.begin(), s_name.end());
		const var_info& vi = si.varmap.find(t_name)->second;
		switch (vi.type)
		{
		case INT:GET_VAR(int, obj, vi.offset) = atoi(value); break;
		case FLOAT:GET_VAR(float, obj, vi.offset) = static_cast<float>(atof(value)); break;
		}
	}
	return true;
}