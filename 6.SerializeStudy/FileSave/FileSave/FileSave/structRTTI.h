#pragma once
#include "global.h"

enum class type_t
{
	NONE,
	INT,
	FLOAT,
	STRING,
	CHARARRAY,
};

struct var_info
{
public:
	type_t type;
	string name;
	size_t offset;

	var_info(string _name, type_t _type, size_t _offset) :name(_name), type(_type), offset(_offset) {}
	var_info(const var_info& ref) :name(ref.name), type(ref.type), offset(ref.offset) {}
};
typedef map<string, var_info> varmap;
struct struct_info
{
public:
	varmap _varmap;
	string name;

	struct_info(string _name):name(_name){}
	struct_info(const struct_info& ref):name(ref.name){}
	struct_info(){}
};
typedef map<string, struct_info> structmap;
structmap _structmap;

#define OFFSET_OF(_struct,_var) ((size_t)&(static_cast<_struct*>(nullptr)->_var))

#define REGISTER_ONLY_ONCE(map,var,info)\
static bool init=false;\
if(!init)\
{\
map.insert(make_pair(#var,info));\
init = true;\
}\

#define REGISTER_STRUCT(structname)\
REGISTER_ONLY_ONCE(_structmap,structname,struct_info(#structname))

#define REGISTER_VAR(structname,varname,type)\
REGISTER_ONLY_ONCE(_structmap[#structname]._varmap,varname,var_info(#varname,type,OFFSET_OF(structname,varname)))

#define STRUCT_BEGIN(name)\
struct name\
{\
private:\
struct register_init{register_init(){REGISTER_STRUCT(name);}}_register_init;\
public:\
static const string& _name(){static string __name(#name);return __name;}

#define STRUCT_VAR(_struct,_var,_ctype,_type)\
public:\
_ctype _var;\
private:\
struct register_##_var{register_##_var(){REGISTER_VAR(_struct,_var,_type);}}_register_##_var;

#define STRUCT_VAR_INT(_struct,_var) STRUCT_VAR(_struct,_var,int,type_t::INT)
#define STRUCT_VAR_FLOAT(_struct,_var) STRUCT_VAR(_struct,_var,float,type_t::FLOAT)
#define STRUCT_VAR_STRING(_struct,_var) STRUCT_VAR(_struct,_var,string,type_t::STRING)
//#define STRUCT_VAR_CHARARRAY(_struct,_var) STRUCT_VAR(_struct,_var,char*,type_t::CHARARRAY)

#define STRUCT_END() };

#define GET_VAR_ADDR(_ctype,_obj,_offset)\
((_ctype*)(((char*)&_obj)+_offset))
#define GET_VAR(_ctype,_obj,_offset)\
(*(GET_VAR_ADDR(_ctype,_obj,_offset)))

#define XML_START_TAG(name) "<"<<name<<">"
#define XML_END_TAG(name) "</"<<name<<">"
#define XML_TAB "\t"

template <typename _stream,typename _struct>
bool xml_write(_stream& outstream, _struct& structobj)
{
	if (_structmap.find(structobj._name()) == _structmap.end())return false;

	const struct_info& __struct = _structmap[_struct::_name()];
	outstream << XML_START_TAG(__struct.name) << endl;
	for (varmap::const_iterator i = __struct._varmap.begin(); i != __struct._varmap.end(); ++i)
	{
		const var_info& _var = i->second;
		outstream << XML_TAB << XML_START_TAG(_var.name);
		switch (_var.type)
		{
		case type_t::INT:
			cout << "int offset" << _var.offset << endl;
			cout << "int data" << GET_VAR(int, structobj, _var.offset)<<endl;
			outstream << GET_VAR(int, structobj, _var.offset);
			break;
		case type_t::FLOAT:
			cout << "float offset" << _var.offset << endl;
			cout << "float data" << GET_VAR(int, structobj, _var.offset) << endl;
			outstream << GET_VAR(float, structobj, _var.offset);
			break;
		case type_t::STRING:
			cout << "string offset" << _var.offset << endl;
			cout << "string data" << GET_VAR(int, structobj, _var.offset) << endl;
			outstream << GET_VAR(string, structobj, _var.offset);
			break;
		}
		outstream << XML_END_TAG(_var.name)<<endl;
	}
	outstream << XML_END_TAG(__struct.name)<<endl;
	return true;
}
#define COUNT_OF(array) (sizeof(array)/sizeof(array[0]))
template <typename _stream,typename _struct>
bool xml_read(_stream& instream, _struct& structobj)
{
	if (_structmap.find(structobj._name()) == _structmap.end())return false;

	const struct_info& structinfo = _structmap[_struct::_name()];
	
	char name[1024], value[1024];
	instream.ignore(1024, '>');
	for (varmap::const_iterator i = structinfo._varmap.begin(); i != structinfo._varmap.end(); ++i)
	{
		instream.ignore(1024, '<');
		instream.getline(name, COUNT_OF(name), '>');
		instream.getline(value, COUNT_OF(value), '<');
		string name2(name);
		const var_info& varinfo = structinfo._varmap.find(name2)->second;
		switch (varinfo.type)
		{
			case type_t::INT:
			GET_VAR(int, structobj, varinfo.offset) = atoi(value);
			break;
			case type_t::FLOAT:
			GET_VAR(float, structobj, varinfo.offset) = atof(value);
			break;
			case type_t::STRING:
			GET_VAR(string, structobj, varinfo.offset) = value;
			break;
		}
	}
	return true;
}