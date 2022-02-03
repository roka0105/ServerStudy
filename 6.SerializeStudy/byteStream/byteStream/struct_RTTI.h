#pragma once
#include "global.h"
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
#define OFFSET_OF(_struct,_var)((size_t)&(static_cast<_struct*>(nullptr)->_var)

struct sample
{
	struct _temp
	{

		_temp()
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

#define REGISTAR_RTTI_STRUCT(_name)\
REGISTER_ONLY_ONCE(rtti,_name,(struct_info(_T(#_name))))

#define REGISTAR_RTTI_VAR(_struct,_var,_type)\
REGISTER_ONLY_ONCE(rtti[_T(#_struct)].varmap,_var,(var_info(_T(#_var),_type,OFFSET_OF(_struct,_var))))

#define 