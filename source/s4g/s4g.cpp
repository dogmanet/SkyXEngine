
#pragma once 
#include <core\string.cpp>
#include <s4g\s4g.h>
#include <s4g\s4g_parser.cpp>

s4g_main::s4g_main()
{
	arr_lex = new s4g_arr_lex();
	gnode = 0;
	bst = new s4g_builder_syntax_tree();
	bst->arr_lex = arr_lex;
	commands = new Stack<s4g_command>();
	compiler = new s4g_compiler();
	vmachine = new s4g_vm();
	strerror[0] = 0;
}


s4g_value::s4g_value()
{
	data = 0;
	cr_null();
}

s4g_value::~s4g_value()
{
	mem_delete(data);
}

inline void s4g_value::cr_null()
{
	mem_delete(data);
	type = t_null;
}

inline void s4g_value::cr_nn()
{
	mem_delete(data);
	type = t_nn;
}

inline s4g_type s4g_value::get_type()
{
	return type;
}

inline void s4g_value::cr_value(s4g_value* val)
{
	mem_delete(data);

	if (val->get_type() == t_null)
		cr_null();
	else if (val->get_type() == t_table)
		init_table(val->get_table());
	else if (val->get_type() == t_string)
		init_str(val->get_str().c_str());
	else if (val->get_type() == t_float)
		init_float(val->get_float());
	else if (val->get_type() == t_int)
		init_long(val->get_long());
	else if (val->get_type() == t_bool)
		init_bool(val->get_bool());
	else if (val->get_type() == t_sfunc)
		init_s_func(val->get_s_func());
	else if (val->get_type() == t_nn)
		cr_nn();
}

inline void s4g_value::set_value(s4g_value* val)
{
	mem_delete(data);
	data = val->data;
	type = val->get_type();
}

void s4g_value::init_long(long num)
{
	long* tmpdata = 0;
	mem_delete(data);

	tmpdata = new long;
	*tmpdata = num;
	data = tmpdata;
	type = t_int;
}

inline void s4g_value::cr_table()
{
	mem_delete(data);
	data = new s4g_table();
	type = t_table;
}

inline void s4g_value::init_table(s4g_table* tt)
{
	mem_delete(data);
	data = new s4g_table;
	data = tt;
	type = t_table;
}

inline void s4g_value::init_s_func(s4g_s_function* func)
{
	mem_delete(data);
	data = new s4g_s_function;
	data = func;
	type = t_sfunc;
}

inline void s4g_value::init_c_func(s4g_c_function func)
{
	mem_delete(data);
	data = new s4g_c_function;
	data = func;
	type = t_cfunc;
}

inline void s4g_value::init_str(const char* str)
{
	mem_delete(data);

	data = new String(str);
	type = t_string;
}

inline void s4g_value::init_float(double num)
{
	mem_delete(data);
	double* tmpdata = new double;
	*tmpdata = num;
	data = tmpdata;
	type = t_float;
}

inline void s4g_value::init_bool(int bf)
{
	mem_delete(data);
	bool* tmpdata = new bool;
	*tmpdata = bf;
	data = tmpdata;
	type = t_bool;
}


long s4g_value::get_long()
{
	return *(long*)data;
}

double s4g_value::get_float()
{
	return *(double*)data;
}

int s4g_value::get_bool()
{
	return *(bool*)data;
}

String & s4g_value::get_str()
{
	return *(String*)data;
}

inline s4g_table* s4g_value::get_table()
{
	return (s4g_table*)data;
}

inline s4g_s_function* s4g_value::get_s_func()
{
	return (s4g_s_function*)data;
}

inline s4g_c_function s4g_value::get_c_func()
{
	return (s4g_c_function)data;
}



//////////////

inline s4g_value* cr_val_null()
{
	return new s4g_value();
}

inline s4g_value* cr_val_table_null()
{
	s4g_value* tmpval = new s4g_value();
	tmpval->cr_table();
	return tmpval;
}

inline s4g_value* cr_val_long(long num)
{
	s4g_value* tmpval = new s4g_value();
	tmpval->init_long(num);
	return tmpval;
}

inline s4g_value* cr_val_float(float num)
{
	s4g_value* tmpval = new s4g_value();
	tmpval->init_float(num);
	return tmpval;
}

inline s4g_value* cr_val_bool(bool bf)
{
	s4g_value* tmpval = new s4g_value();
	tmpval->init_bool(bf);
	return tmpval;
}

inline s4g_value* cr_val_str(const char* str)
{
	s4g_value* tmpval = new s4g_value();
	tmpval->init_str(str);
	return tmpval;
}

inline s4g_value* cr_val_table(s4g_table* tt)
{
	s4g_value* tmpval = new s4g_value();
	tmpval->init_table(tt);
	return tmpval;
}

inline s4g_value* cr_val_s_func(s4g_s_function* func)
{
	s4g_value* tmpval = new s4g_value();
	tmpval->init_s_func(func);
	return tmpval;
}

inline s4g_value* cr_val_c_func(s4g_c_function func)
{
	s4g_value* tmpval = new s4g_value();
	tmpval->init_c_func(func);
	return tmpval;
}

inline s4g_value* cr_val(int _type, const char* _val)
{
	s4g_value* tmpval = new s4g_value();

	if (_val)
	{
		if (_type == t_int)
		{
			tmpval->init_long(atol(_val));
		}
		else if (_type == t_string)
		{
			tmpval->init_str(_val);
		}
		else if (_type == t_table)
		{
			tmpval->cr_table();
		}
		else if (_type == t_nn)
		{
			tmpval->cr_nn();
		}
	}
	else
	{
		if (_type == t_table)
		{
			tmpval->cr_table();
		}
		else if (_type == t_nn)
		{
			tmpval->cr_nn();
		}
	}
	return tmpval;
}

//////////////

s4g_table::s4g_table()
{

}

bool s4g_table::is_exists_s(const char* str)
{
	return arr_value.IsExists(str);
}

bool s4g_table::is_exists_n(DWORD key)
{
	return (arr_value.GetSize() >= key);
}

s4g_value* s4g_table::gets(const char* str)
{
	return arr_value[str];
}

s4g_value* s4g_table::getn(DWORD id)
{
		if(id-1 >= 0 && id-1 < arr_value.GetSize())
		{
			return arr_value[id-1];
		}
		/*else if(id < 0 && abs(id)-1 < arr_value.GetSize())
		{
			return arr_value[arr_value.GetSize()+id];
		}*/
		else if(id-1 >= arr_value.GetSize())
		{
			return arr_value[id-1];
		}
	return 0;
}

//

void s4g_table::set_val_s(const char* name, s4g_value* val)
{
	arr_value[name]->set_value(val);
}

void s4g_table::add_val_s(const char* name, s4g_value* val)
{
	arr_value[name]->cr_value(val);
}

//

inline void s4g_table::set_val_n(long num, s4g_value* val)
{
	arr_value[num]->set_value(val);
}

inline void s4g_table::add_val_n(long num, s4g_value* val)
{
	arr_value[num]->cr_value(val);
}

inline void s4g_table::set_val(s4g_value* val)
{
	arr_value[arr_value.GetSize()]->set_value(val);
}

inline void s4g_table::add_val(s4g_value* val)
{
	arr_value.Add(val);
}

//

void s4g_table::add_s(const char* name,long val)
{
	arr_value[name] = cr_val_long(val);
}

void s4g_table::add_s(const char* name,double val)
{
	arr_value[name] = cr_val_float(val);
}

void s4g_table::add_s(const char* name,const char* val)
{
	arr_value[name] = cr_val_str(val);
}

void s4g_table::add_s(const char* name,bool val)
{
	arr_value[name] = cr_val_bool(val);
}

void s4g_table::add_s(const char* name, s4g_s_function* val)
{
	arr_value[name] = cr_val_s_func(val);
}

void s4g_table::add_s(const char* name, s4g_c_function val)
{
	arr_value[name] = cr_val_c_func(val);
}

void s4g_table::add_s(const char* name,s4g_table* val)
{
	arr_value[name] = cr_val_table(val);
}

void s4g_table::add_s(const char* name)
{
	arr_value[name] = new s4g_value();
}

void s4g_table::add_t_s(const char* name)
{
	arr_value[name] = cr_val_table_null();
}

//

void s4g_table::add_n(long num, long val)
{
	arr_value[num] = cr_val_long(val);
}

void s4g_table::add_n(long num, double val)
{
	arr_value[num] = cr_val_float(val);
}

void s4g_table::add_n(long num, const char* val)
{
	arr_value[num] = cr_val_str(val);
}

void s4g_table::add_n(long num, bool val)
{
	arr_value[num] = cr_val_bool(val);
}

void s4g_table::add_n(long num, s4g_s_function* val)
{
	arr_value[num] = cr_val_s_func(val);
}

void s4g_table::add_n(long num, s4g_c_function val)
{
	arr_value[num] = cr_val_c_func(val);
}

void s4g_table::add_n(long num, s4g_table* val)
{
	arr_value[num] = cr_val_table(val);
}

void s4g_table::add_n(long num)
{
	arr_value[num] = new s4g_value();
}

void s4g_table::add_t_n(long num)
{
	arr_value[num] = cr_val_table_null();
}

//

void s4g_table::add(long val)
{
	arr_value.Add(cr_val_long(val));
}

void s4g_table::add(double val)
{
	arr_value.Add(cr_val_float(val));
}

void s4g_table::add(const char* val)
{
	arr_value.Add(cr_val_str(val));
}

void s4g_table::add(bool val)
{
	arr_value.Add(cr_val_bool(val));
}

void s4g_table::add(s4g_s_function* val)
{
	arr_value.Add(cr_val_s_func(val));
}

void s4g_table::add(s4g_c_function val)
{
	arr_value.Add(cr_val_c_func(val));
}

void s4g_table::add(s4g_table* val)
{
	arr_value.Add(cr_val_table(val));
}

void s4g_table::add()
{
	arr_value.Add(new s4g_value());
}

void s4g_table::add_t()
{
	arr_value.Add(cr_val_table_null());
}


///////////////////////

s4g_main* s4g_init()
{
	return new s4g_main();
}

int s4g_load_file(s4g_main* s4gm,const char* file)
{
	int status = s4gm->arr_lex->read(file);
		if(status != 0)
		{
			sprintf(s4gm->strerror,"%s",s4gm->arr_lex->strerror);
			return status;
		}
	//status = s4gm->prep->run_pp(s4gm->stdpath);
	s4gm->gnode = s4gm->bst->s4g_gen_tree();
		if(s4gm->bst->status != 0)
		{
			sprintf(s4gm->strerror,"%s",s4gm->bst->error);
			return s4gm->bst->status;
		}
	s4gm->compiler->compile(s4gm->gnode,s4gm->commands);
	s4gm->vmachine->run(s4gm->commands,&(s4gm->vmachine->gvars));
	return 0;
}

void s4g_push_c_func(s4g_vm* vm,s4g_c_function func)
{
	s4g_value* tval = new s4g_value();
	tval->init_c_func(func);
	vm->execute.push(tval);
}

void s4g_store(s4g_vm* vm,const char* name)
{
	s4g_table* ttable = &(vm->gvars);
			
		if(ttable->is_exists_s(name))
		{
			vm->execute.push((ttable->gets(name)));
		}
		else
		{
			int qwert = 0;
			ttable->add_s(name);
			vm->execute.push((ttable->gets(name)));
		}

	s4g_value* tvalue = vm->execute.get(vm->execute.count());
	s4g_value* tvalue2 = vm->execute.get(vm->execute.count()-1);
	memcpy(tvalue,tvalue2,sizeof(s4g_value));			
	vm->execute.pop(2);
}