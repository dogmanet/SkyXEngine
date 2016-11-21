
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
	type = t_null;
	data = 0;
}

s4g_value::s4g_value(s4g_value* val)
{
	
}


s4g_value::s4g_value(int _type,const char* _val)
{
	type = t_null;
	data = 0;
		if(_val)
		{
				if(_type == t_int)
				{
					set_long(atol(_val));
				}
				else if(_type == t_string)
				{
					set_string(_val);
				}
				else if(_type == t_table)
				{
					set_table();
				}
		}
		else
		{
			type = (s4g_type)_type;
		}
}

s4g_value::s4g_value(long num)
{
	type = t_null;
	data = 0;
	set_long(num);
}

inline s4g_type s4g_value::get_type()
{
	return type;
}

void s4g_value::set_long(long num)
{
	long* tmpdata = 0;
		if(type == t_int)
		{
			tmpdata = (long*)data;
		}
		else if(type == t_string)
		{
			tmpdata = (long*)data;
			((String*)tmpdata)->Release();
			delete ((String*)tmpdata);tmpdata = 0;
		}

		if(tmpdata == 0)
			tmpdata = new long;
	*tmpdata = num;
	data = tmpdata;
	type = t_int;
}

inline void s4g_value::set_table()
{
		if(type == t_int)
		{
			long* tmpdata;
			tmpdata = (long*)data;
			delete tmpdata; tmpdata = 0;
		}
		else if(type == t_string)
		{
			String* tmpdata = (String*)data;
			tmpdata->Release();
			delete tmpdata;tmpdata = 0;
		}

		if(data == 0)
			data = new s4g_table();
	type = t_table;
}

inline void s4g_value::set_table(s4g_table* tt)
{
	s4g_table* tmpdata = 0;
		if(type == t_table)
		{
			tmpdata = (s4g_table*)data;
		}
		else if(type == t_int)
		{
			tmpdata = (s4g_table*)data;
			delete (long*)tmpdata;tmpdata = 0;
		}
		else if(type == t_string)
		{
			tmpdata = (s4g_table*)data;
			((String*)tmpdata)->Release();
			delete (String*)tmpdata;tmpdata = 0;
		}

		if(tmpdata == 0)
			tmpdata = new s4g_table;
	tmpdata = tt;
	data = tmpdata;
	type = t_table;
}

inline void s4g_value::set_s_function(s4g_s_function* func)
{
	s4g_s_function* tmpdata = 0;
	delete data;data = 0;
	data = new s4g_s_function;
	data = func;
	type = t_sfunc;
}

inline void s4g_value::set_c_function(s4g_c_function func)
{
	s4g_c_function* tmpdata = 0;
	delete data;data = 0;
	data = new s4g_c_function;
	data = func;
	type = t_cfunc;
}

inline void s4g_value::set_string(const char* str)
{
	String* tmpdata = 0;
		if(type == t_string)
		{
			tmpdata = (String*)data;
		}
		else if(type == t_int)
		{
			tmpdata = (String*)data;
			delete (long*)tmpdata;tmpdata = 0;
		}
		else if(type == t_table)
		{
			tmpdata = (String*)data;
			delete (s4g_table*)tmpdata;tmpdata = 0;
		}

		if(tmpdata == 0)
			tmpdata = new String;
	*tmpdata = str;
	data = tmpdata;
	type = t_string;
}

long s4g_value::get_long()
{
	return *(long*)data;
}

String & s4g_value::get_string()
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

void s4g_table::add_value(const char* name,s4g_value* val)
{
		if(name != 0)
			memcpy(arr_value[name],val,sizeof(s4g_value));
			//arr_value[name] = new s4g_value(val);
		else 
			arr_value.Add(val);
}

void s4g_table::add_value(const char* name,long val)
{
	arr_value[name] = new s4g_value(val);
}

void s4g_table::add_value(const char* name,double val)
{

}

void s4g_table::add_value(const char* name,UINT val)
{

}

void s4g_table::add_value(const char* name,const char* val)
{

}

void s4g_table::add_value(const char* name,bool val)
{

}

void s4g_table::add_value(const char* name)
{
	arr_value[name] = new s4g_value();
}

void s4g_table::add_value(const char* name,s4g_table* val)
{

}

void s4g_table::create_table(const char* name)
{

}

void s4g_table::add_key_null(long key)
{
	arr_value[key] = new s4g_value();
}

///////////////////////

s4g_main* s4g_init()
{
	return new s4g_main();
}

int s4g_load_file(s4g_main* s4gm,const char* file)
{
	/*int len = strlen(file);
		while(file[len--] != '\\')
		{
				if(file[len-1] == '\\')
				{
					//len--;
					memcpy(s4gm->stdpath,file,len);
					s4gm->stdpath[len] = 0;
				}
		}*/

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
	tval->set_c_function(func);
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
			ttable->add_value(name);
			vm->execute.push((ttable->gets(name)));
		}

	s4g_value* tvalue = vm->execute.get(vm->execute.count());
	s4g_value* tvalue2 = vm->execute.get(vm->execute.count()-1);
	memcpy(tvalue,tvalue2,sizeof(s4g_value));			
	vm->execute.pop(2);
}