
#pragma once 
#include <string/string.cpp>
#include <s4g\s4g.h>
#include <s4g\s4g_parser.cpp>

void s4g_report(int level, const char* name_ss, const char* format, ...)
{
#if defined(_WINDOWS)
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);
#endif
	char buf[4096];
	va_list va; 
	va_start(va, format); 
	vsprintf_s(buf, sizeof(buf), format, va);
	va_end(va);

	if (level == 0)
		fprintf(stdout, "s4g %s notice: %s\n", name_ss, buf);
	else if (level == 1)
		fprintf(stdout, "s4g %s warning: %s\n", name_ss, buf);
	else
	{
		fprintf(stdout, "s4g %s error: %s\n", name_ss, buf);
		Sleep(5000);
		exit(1);
	}
}

s4g_report_func s4g_rf = s4g_report;

s4g_main::s4g_main(const char* _name)
{
	arr_lex = new s4g_arr_lex();
	gc = new s4g_gc();
	gnode = 0;
	bst = new s4g_builder_syntax_tree();
	bst->arr_lex = arr_lex;
	bst->gc = gc;
	commands = new Stack<s4g_command>();
	compiler = new s4g_compiler();
	compiler->gc = gc;
	vmachine = new s4g_vm(gc);
	vmachine->arr_lex = arr_lex;
	vmachine->gc = gc;
	vmachine->s4gm = this;
	strerror[0] = 0;
	if (_name)
		strcpy(name, _name);
	else
		name[0] = 0;
}


s4g_value::s4g_value(/*long lexid*/)
{
	/*nlexid = lexid;*/
	typedata = 0;
	isdelete = false;
}

s4g_value::~s4g_value()
{

}

inline s4g_value* s4g_gc::cr_val_null(/*long lexid*/)
{ 
	s4g_value* tval = 0;/*MemValue.Alloc();// new s4g_value(lexid);
	tval->typedata = typedata;

	tval->idvar = arrvar.size();
	arrvar.push_back(tval);

	s4g_data* tdata = MemData.Alloc(); //new s4g_data();
	tdata->typedata = typedata;

	tval->iddata = arrdata.size();
	arrdata.push_back(tdata);*/

	def_cr_val_null(tval);

	/*if (arrnewvals.count_obj == arrnewvals.Arr.Size)
		expansion_val(); 
		tval = arrnewvals.Arr[arrnewvals.count_obj]; 
		tval->typedata = typedata; 
		s4g_data* tdata = arrdatavals.Arr[arrdatavals.count_obj]; 
		tdata->typedata = typedata; */

	return tval;
};

inline s4g_value* s4g_gc::cr_val_table_null(/*long lexid*/)
{ 
	s4g_value* tmpval = cr_val_null(/*lexid*/);
	arrdata[tmpval->iddata]->data = new s4g_table();
	arrdata[tmpval->iddata]->type = t_table;
	return tmpval; 
};

inline s4g_value* s4g_gc::cr_val_int(s4g_int num/*, long lexid*/)
{ 
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval);

	s4g_int* tmpvv = MemInt.Alloc();
	*tmpvv = num;

	tdata->data = tmpvv;
	tdata->type = t_int;
	return tmpval; 
};

inline s4g_value* s4g_gc::cr_val_uint(s4g_uint num)
{
	s4g_value* tmpval = cr_val_null();
	s4g_uint* tmpvv = MemUInt.Alloc();
	*tmpvv = num;
	arrdata[tmpval->iddata]->data = tmpvv;
	arrdata[tmpval->iddata]->type = t_int;
	return tmpval;
}

inline s4g_value* s4g_gc::cr_val_float(s4g_float num)
{ 
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval);
	s4g_float* tmpvv = MemFloat.Alloc();
	*tmpvv = num;
	arrdata[tmpval->iddata]->data = tmpvv;
	arrdata[tmpval->iddata]->type = t_float;
	return tmpval; 
};

inline s4g_value* s4g_gc::cr_val_bool(s4g_bool bf)
{ 
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval);
	s4g_bool* tmpvv = MemBool.Alloc();
	*tmpvv = bf;
	arrdata[tmpval->iddata]->data = tmpvv;
	arrdata[tmpval->iddata]->type = t_bool;
	return tmpval; 
};

inline s4g_value* s4g_gc::cr_val_str(const char* str)
{ 
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval);
	String* tmpvv = new String(str);
	arrdata[tmpval->iddata]->data = tmpvv;
	arrdata[tmpval->iddata]->type = t_string;
	return tmpval;
};

inline s4g_value* s4g_gc::cr_val_table(s4g_table* tt)
{ 
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval);
	
	arrdata[tmpval->iddata]->data = tt;
	arrdata[tmpval->iddata]->type = t_table;
	return tmpval;
};

inline s4g_value* s4g_gc::cr_val_s_func(s4g_s_function* func)
{ 
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval);

	arrdata[tmpval->iddata]->data = func;
	arrdata[tmpval->iddata]->type = t_sfunc;
	return tmpval;
};

inline s4g_value* s4g_gc::cr_val_c_func(s4g_c_function func)
{ 
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval);

	arrdata[tmpval->iddata]->data = func;
	arrdata[tmpval->iddata]->type = t_cfunc;
	return tmpval;
};

inline s4g_value* s4g_gc::cr_val_nn()
{
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval);

	arrdata[tmpval->iddata]->data = 0;
	arrdata[tmpval->iddata]->type = t_nnull;
	return tmpval;
}

void s4g_gc::c_val(s4g_value* dest, s4g_value* src, bool incr)
{
	if (dest->iddata >= arrdata.count_obj && src->iddata >= arrdata.count_obj)
	{
		return;
	}

	arrdata[dest->iddata]->ref--;
	
	//long tmpdestref = arrdata[dest->iddata]->ref;
	dest->iddata = src->iddata;

	if (incr)
		arrdata[src->iddata]->ref++;
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

inline long s4g_table::is_exists_s2(const char* str, s4g_value** tval)
{
	return arr_value.IsExists2(str, tval);
}

inline bool s4g_table::is_exists_n2(DWORD key, s4g_value** tval)
{
	if (key - 1 >= 0 && key - 1 < arr_value.GetSize())
	{
		*tval = arr_value[key - 1];
		return true;
	}

	return false;
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
		
		else if(id-1 >= arr_value.GetSize())
		{
			return arr_value[id-1];
		}
	return 0;
}

//

void s4g_table::add_val_s(const char* name, s4g_value* val/*, long lexid*/)
{
	arr_value.Add(name,val);
}

inline void s4g_table::add_val_n(long num, s4g_value* val/*, long lexid*/)
{
	arr_value.Add(num, val);
}


inline void s4g_table::add_val(s4g_value* val/*, long lexid*/)
{
	arr_value.Add(val);
}

inline long s4g_table::size()
{
	return arr_value.GetSize();
}

inline const char* s4g_table::get_name_id(long id)
{
	return arr_value.GetNameID(id);
}

inline void s4g_table::reserve(int count_elem)
{
	arr_value.reserve(count_elem);
}

///////////////////////

inline s4g_value* s4g_gc::cr_val(int _type, const char* _val/*, long lexid*/)
{
	s4g_value* tmpval = 0;// new s4g_value();

	if (_val)
	{
		if (_type == t_int)
		{
			tmpval = cr_val_int(atol(_val)/*, lexid*/);
		}
		else if (_type == t_uint)
		{
			UINT num;
			sscanf(_val, "%u", &num);
			tmpval = cr_val_uint(num/*, lexid*/);
		}
		else if (_type == t_float)
		{
			tmpval = cr_val_float(atof(_val)/*, lexid*/);
		}
		else if (_type == t_bool)
		{
			bool bf = false;
			if (strcmp(_val, "true") == 0 || atol(_val) != 0)
				bf = true;

			tmpval = cr_val_bool(bf/*, lexid*/);
		}
		else if (_type == t_string)
		{
			tmpval = cr_val_str(_val/*, lexid*/);
		}
		else if (_type == t_table)
		{
			tmpval = cr_val_table_null(/*lexid*/);
		}
		else if (_type == t_nnull)
		{
			tmpval = cr_val_nn(/*lexid*/);
		}
	}
	else
	{
		if (_type == t_table)
		{
			tmpval = cr_val_table_null(/*lexid*/);
		}
		else if (_type == t_nnull)
		{
			tmpval = cr_val_nn(/*lexid*/);
		}
	}
	return tmpval;
}

s4g_data::s4g_data()
{ 
	data = 0; 
	ref = 0;
	type = t_null;
	typedata = 0;
}

s4g_data::~s4g_data()
{ 

}

///////////////////////

s4g_gc::s4g_gc()
{
	typedata = 1;
	curr_num_top_ctx = 0;
	for (int i = 0; i < S4G_MAX_CALL; i++)
	{
		s4g_context* tcontext = MemCtx.Alloc();
		tcontext->table = MemTable.Alloc();
		tcontext->valid = false;
		arrcurrcontexts.push_back(tcontext);
	}
}

inline s4g_int s4g_gc::get_int(s4g_value* val)
{
	return *(s4g_int*)(arrdata[val->iddata]->data);
}

inline s4g_uint s4g_gc::get_uint(s4g_value* val)
{
	return *(s4g_uint*)(arrdata[val->iddata]->data);;
}
inline s4g_float s4g_gc::get_float(s4g_value* val)
{
	return *(s4g_float*)(arrdata[val->iddata]->data);;
}
inline const char* s4g_gc::get_str(s4g_value* val)
{
	return ((String*)(arrdata[val->iddata]->data))->c_str();
}
inline s4g_table* s4g_gc::get_table(s4g_value* val)
{
	return (s4g_table*)(arrdata[val->iddata]->data);
}
inline s4g_s_function* s4g_gc::get_s_func(s4g_value* val)
{
	return (s4g_s_function*)(arrdata[val->iddata]->data);
}
inline s4g_c_function s4g_gc::get_c_func(s4g_value* val)
{
	return (s4g_c_function)(arrdata[val->iddata]->data);
}
inline s4g_bool s4g_gc::get_bool(s4g_value* val)
{
	return *(s4g_bool*)(arrdata[val->iddata]->data);
}

inline s4g_type s4g_gc::get_type(s4g_value* val)
{
	return arrdata[val->iddata]->type;
}


void s4g_gc::clear()
{
	int qwert = 0;

	/*for (int i = 0; i < oldarrcontexts.size(); i++)
	{
		if (oldarrcontexts[i] != 0)
		{
			s4g_context* tmpctx = oldarrcontexts[i];
			mem_delete(oldarrcontexts[i]->table);
			mem_delete(oldarrcontexts[i]);
		}
	}*/

	int countdeldata = 0;
	for (int i = 0; i < arrdata.size(); i++)
	{
		if (arrdata[i] != 0)
		{
			s4g_type ttype0 = arrdata[i]->type;
			s4g_data* ttmmpp = arrdata[i];
			if (arrdata[i] && arrdata[i]->typedata == 0 && arrdata[i]->ref < 1)
			{
				s4g_type ttype = arrdata[i]->type;

				if (ttype == t_table)
				{
					s4g_table* ttable = (s4g_table*)arrdata[i]->data;
					mem_delete(ttable);
				}
				else if (ttype == t_sfunc)
				{
					s4g_s_function* tsf = (s4g_s_function*)arrdata[i]->data;
					mem_delete(tsf);
				}
				else if (ttype == t_int)
				{
					s4g_int* tint = (s4g_int*)arrdata[i]->data;
					mem_delete(tint);
				}
				else if (ttype == t_uint)
				{
					s4g_uint* tuint = (s4g_uint*)arrdata[i]->data;
					mem_delete(tuint);
				}
				else if (ttype == t_float)
				{
					s4g_float* tfloat = (s4g_float*)arrdata[i]->data;
					mem_delete(tfloat);
				}
				else if (ttype == t_bool)
				{
					s4g_bool* tbool = (s4g_bool*)arrdata[i]->data;
					mem_delete(tbool);
				}
				else if (ttype == t_string)
				{
					String* tstr = (String*)arrdata[i]->data;
					mem_delete(tstr);
				}
				
				mem_delete(arrdata[i]);

				countdeldata++;
			}
		}
	}

	int countdelvar = 0;
	for (int i = 0; i < arrvar.size(); i++)
	{
		if (arrvar[i] != 0)
		{
			
			if (arrdata[arrvar[i]->iddata] == 0 || (arrvar[i]->typedata == 0 && arrdata[arrvar[i]->iddata]->ref < 1) || (arrvar[i]->isdelete && arrvar[i]->typedata == 0))
			{
				s4g_value* tmpdata = arrvar[i];
				mem_delete(arrvar[i]);
				
				countdelvar++;
			}
		}
	}
	
	//oldarrcontexts.clear();
}

inline void s4g_gc::add_mem_contexts()
{
	s4g_context* tcontext = 0;
	for (int i = 0; i < S4G_ADD_MEM_CONTEXTS; ++i)
	{
		tcontext = MemCtx.Alloc();
		tcontext->valid = true;
		tcontext->table = MemTable.Alloc();
		arrcurrcontexts.push(tcontext);
	}
}

long s4g_gc::add_new_context(s4g_table** tt)
{
	if (curr_num_top_ctx >= arrcurrcontexts.count_obj)
	{
		//add_mem_contexts();
		s4g_context* tcontext = 0;
		for (int i = 0; i < S4G_ADD_MEM_CONTEXTS; ++i)
		{
			tcontext = MemCtx.Alloc();
			tcontext->valid = true;
			tcontext->table = MemTable.Alloc();
			arrcurrcontexts.push(tcontext);
		}
	}
		
	s4g_context* tcontext = arrcurrcontexts[curr_num_top_ctx];
	if (!tcontext->table)
		tcontext->table = MemTable.Alloc();
	*tt = tcontext->table;
	tcontext->valid = true;
	++curr_num_top_ctx;
	return curr_num_top_ctx - 1;
}

long s4g_gc::add_context(s4g_table* tt)
{
	s4g_context* tcontext = 0;
	if (curr_num_top_ctx >= arrcurrcontexts.count_obj)
	{
		
		tcontext = MemCtx.Alloc();
		tcontext->valid = true;
		tcontext->table = 0;// MemTable.Alloc();
		arrcurrcontexts.push(tcontext);
	}
	else
		tcontext = arrcurrcontexts[curr_num_top_ctx];

	//s4g_context* tcontext = MemCtx.Alloc();
	tcontext->valid = true;
	tcontext->table = tt;
	//arrcurrcontexts.push(tcontext);
	++curr_num_top_ctx;
	return curr_num_top_ctx - 1;
}

void s4g_gc::remove_context(long id)
{
	if (id < arrcurrcontexts.count_obj && id >= 0)
	{
		arrcurrcontexts[id]->table = 0;
		//farrcurrcontexts.erase(id);
	}
}

void s4g_gc::activate_prev(long lastidctx)
{
	long tmplastidctx = lastidctx;
	if (tmplastidctx == -1)
		tmplastidctx = 1;
	for (int i = lastidctx; i < curr_num_top_ctx; i++)
	{
		arrcurrcontexts[i]->valid = true;
	}
}

long s4g_gc::deactivate_prev()
{
	for (int i = curr_num_top_ctx - 1; i >= 0; i--)
	{
		if (arrcurrcontexts[i]->valid)
			arrcurrcontexts[i]->valid = false;
		else
			return i;
	}
	return 0;
}

void s4g_gc::del_context(long id)
{
	//if (id < arrcurrcontexts.size() && id >= 0)
	//{
		/*oldarrcontexts.push_back(arrcurrcontexts[id]);
		arrcurrcontexts.erase(id);*/
		--curr_num_top_ctx;
	//}
}

inline long s4g_gc::ctx_is_exists_s(const char* str, s4g_value** val)
{
	s4g_context* ctx = 0;
	for (int i = curr_num_top_ctx - 1; i >= 0; i--)
	{
		ctx = arrcurrcontexts[i];
		if (ctx->valid && ctx->table && ctx->table->is_exists_s2(str, val) != -1)
		{
			return i;
		}
	}
	return -1;
}

inline void s4g_gc::set_td_data(s4g_value* val, int td)
{
	if (val && val->iddata >= 0 && val->iddata < arrdata.size() && arrdata[val->iddata])
		arrdata[val->iddata]->typedata = td;
}

inline void s4g_gc::set_ctx_for_del(s4g_context* ctx)
{
	oldarrcontexts.push(ctx);
}

///////////////////////

void s4g_get_str_type(s4g_type tt, char* str_type)
{
	if (str_type)
	{
		if (tt == t_none)
			strcpy(str_type, s4g_str_type[0]);
		else if (tt == t_null)
			strcpy(str_type, s4g_str_type[1]);
		else if (tt == t_table)
			strcpy(str_type, s4g_str_type[2]);
		else if (tt == t_string)
			strcpy(str_type, s4g_str_type[3]);
		else if (tt == t_float)
			strcpy(str_type, s4g_str_type[4]);
		else if (tt == t_int)
			strcpy(str_type, s4g_str_type[5]);
		else if (tt == t_uint)
			strcpy(str_type, s4g_str_type[6]);
		else if (tt == t_bool)
			strcpy(str_type, s4g_str_type[7]);
		else if (tt == t_user_data)
			strcpy(str_type, s4g_str_type[8]);
		else if (tt == t_cfunc)
			strcpy(str_type, s4g_str_type[9]);
		else if (tt == t_sfunc)
			strcpy(str_type, s4g_str_type[10]);
		else if (tt == t_nnull)
			strcpy(str_type, s4g_str_type[11]);
	}
}

s4g_main* s4g_init(const char* name)
{
	return new s4g_main(name);
}

void s4g_set_rf(s4g_report_func rf)
{
	s4g_rf = rf;
}

void s4g_kill(s4g_main* s4gm)
{
	S4G_PRE_COND();

	mem_delete(s4gm);
}

int s4g_load_file(s4g_main* s4gm,const char* file)
{
	S4G_PRE_COND(-1);

	int status = s4gm->arr_lex->read(file);
		if(status != 0)
		{
			s4g_rf(2, s4gm->name, "lexer: %s", s4gm->arr_lex->strerror);
			sprintf(s4gm->strerror,"%s",s4gm->arr_lex->strerror);
			return status;
		}
	s4gm->gc->typedata = 1;

	s4gm->gnode = s4gm->bst->s4g_gen_tree();
		if(s4gm->bst->status != 0)
		{
			s4g_rf(2, s4gm->name, "parser: %s", s4gm->bst->error);
			sprintf(s4gm->strerror,"%s",s4gm->bst->error);
			return s4gm->bst->status;
		}
	s4gm->compiler->compile(s4gm->gnode,s4gm->commands);
	s4gm->gc->typedata = 0;

	return 0;
}

void s4g_call(s4g_main* s4gm, bool call_func)
{
	S4G_PRE_COND();

	if (call_func)
	{
		if (s4gm->vmachine->execute.count() > 0)
		{
			s4gm->vmachine->com_call();
			if (s4gm->vmachine->error != 0)
			{
				s4g_rf(2, s4gm->name, s4gm->vmachine->strerror);
				return;
			}
		
			s4gm->vmachine->run(s4gm->vmachine->curr_comm, (s4gm->vmachine->curr_vars));
			if (s4gm->vmachine->error != 0)
			{
				s4g_rf(2, s4gm->name, s4gm->vmachine->strerror);
			}
		}
	}
	else 
	{
		//s4gm->vmachine->run(s4gm->commands, (s4gm->vmachine->gvars));
			if (s4gm->vmachine->error != 0)
			{
				s4g_rf(2, s4gm->name, s4gm->vmachine->strerror);
			}
	}
}


void s4g_spush_table_null(s4g_main* s4gm, int count_elem)
{
	S4G_PRE_COND();

	s4g_value* ttable = s4gm->vmachine->gc->cr_val_table_null(/*-5*/);
	s4g_table* tt = s4gm->vmachine->gc->get_table(ttable);
	tt->reserve(count_elem);
	s4gm->vmachine->execute.push(ttable);
}

void s4g_spush_c_func(s4g_main* s4gm, s4g_c_function func)
{
	S4G_PRE_COND();

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_c_func(func/*, -5*/));
}

void s4g_spush_int(s4g_main* s4gm, s4g_int num)
{
	S4G_PRE_COND();
	stack_push(s4gm->vmachine->execute, s4gm->vmachine->gc->cr_val_int(num));
	//s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_int(num/*, -5*/));
}

void s4g_spush_uint(s4g_main* s4gm, s4g_uint num)
{
	S4G_PRE_COND();

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_uint(num/*, -5*/));
}

void s4g_spush_float(s4g_main* s4gm, s4g_float num)
{
	S4G_PRE_COND();

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_float(num/*, -5*/));
}

void s4g_spush_str(s4g_main* s4gm, const char* str)
{
	S4G_PRE_COND();

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_str(str/*, -5*/));
}

void s4g_spush_bool(s4g_main* s4gm, s4g_bool bf)
{
	S4G_PRE_COND();

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_bool(bf/*, -5*/));
}

void s4g_spush_null(s4g_main* s4gm)
{
	S4G_PRE_COND();

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_null(/*-5*/));
}

void s4g_spush_precall(s4g_main* s4gm)
{
	S4G_PRE_COND();

	s4gm->vmachine->sr.setn_first_free(s4gm->vmachine->execute.count_obj);
}

void s4g_sstore_g(s4g_main* s4gm, const char* name)
{
	S4G_PRE_COND();

	if (s4gm->vmachine->execute.count() >= 1)
	{
		s4g_value* tval = s4gm->vmachine->execute.get(-1);
		s4gm->vmachine->gvars->add_val_s(name, tval/*, -5*/);
		s4gm->vmachine->execute.pop(1);
	}
	else
	{
		s4g_rf(2, s4gm->name, "stack is empty, api function [%s]", __FUNCTION__);
	}
}

void s4g_sstore(s4g_main* s4gm, int index, const char* name)
{
	S4G_PRE_COND();

	if (s4gm->vmachine->execute.count() >= index)
	{
		s4g_value* tval = s4gm->vmachine->execute.get(index);
		if (s4gm->gc->get_type(tval) == t_table)
		{
			s4g_table* ttable = s4gm->gc->get_table(tval);
			ttable->add_val_s(name, s4gm->vmachine->execute.get(-1)/*, -5*/);
			s4gm->vmachine->execute.pop(1);
		}
		else
		{
			s4g_rf(2, s4gm->name, "value of stack at index '%d' is not table, api function [%s]", index, __FUNCTION__);
		}
	}
	else
	{
		s4g_rf(2, s4gm->name, "stack is empty, api function [%s]", __FUNCTION__);
	}
}


void s4g_sget_g(s4g_main* s4gm, const char* name)
{
	S4G_PRE_COND();

	s4g_value* tval = s4gm->vmachine->gvars->gets(name);
	if (!tval)
	{
		s4g_rf(2, s4gm->name, "var '%s' is not found in global, api function [%s]", name, __FUNCTION__);
		return;
	}
	s4gm->vmachine->execute.push(tval);
}

void s4g_sget(s4g_main* s4gm, int index, const char* name)
{
	S4G_PRE_COND();

	if (s4gm->vmachine->execute.count() >= index)
	{
		s4g_value* tval = s4gm->vmachine->execute.get(index);
		if (s4gm->gc->get_type(tval) == t_table)
		{
			s4g_table* ttable = s4gm->gc->get_table(tval);
			s4g_value* tval2 = ttable->gets(name);
			if (!tval2)
			{
				s4g_rf(2, s4gm->name, "var '%s' is not found in table, api function [%s]", name, __FUNCTION__);
				return;
			}
			s4gm->vmachine->execute.push(tval2);
		}
		else
		{
			s4g_rf(2, s4gm->name, "value of stack at index '%d' is not table, api function [%s]", index, __FUNCTION__);
		}
	}
	else
	{
		s4g_rf(2, s4gm->name, "stack is empty, api function [%s]", __FUNCTION__);
	}
}


int s4g_sis_int(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(-1);

	return (s4g_sget_type(s4gm, index) == t_int ? 1 : 0);
}

int s4g_sis_uint(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(-1);

	return (s4g_sget_type(s4gm, index) == t_uint ? 1 : 0);
}

int s4g_sis_float(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(-1);

	return (s4g_sget_type(s4gm, index) == t_float ? 1 : 0);
}

int s4g_sis_str(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(-1);

	return (s4g_sget_type(s4gm, index) == t_string ? 1 : 0);
}

int s4g_sis_bool(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(-1);

	return (s4g_sget_type(s4gm, index) == t_bool ? 1 : 0);
}

int s4g_sis_c_func(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(-1);

	return (s4g_sget_type(s4gm, index) == t_cfunc ? 1 : 0);
}

int s4g_sis_s_func(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(-1);

	return (s4g_sget_type(s4gm, index) == t_sfunc ? 1 : 0);
}

int s4g_sis_table(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(-1);

	return (s4g_sget_type(s4gm, index) == t_table ? 1 : 0);
}

int s4g_sis_null(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(-1);

	return (s4g_sget_type(s4gm, index) == t_null ? 1 : 0);
}


s4g_type s4g_sget_type(s4g_main* s4gm, int index)
{
	S4G_PRE_COND((s4g_type )- 1);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_type(s4gm->vmachine->execute.get(index));
	}
	else
	{
		s4g_rf(1, s4gm->name, "stack is empty, api function [%s]", __FUNCTION__);
		return (s4g_type)-1;
	}
}


s4g_int s4g_sget_int(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(-1);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_int(s4gm->vmachine->execute.get(index));
	}
	else
	{
		s4g_rf(2, s4gm->name, "stack is empty, api function [%s]", __FUNCTION__);
		return 0;
	}
}

s4g_uint s4g_sget_uint(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(-1);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_uint(s4gm->vmachine->execute.get(index));
	}
	else
	{
		s4g_rf(2, s4gm->name, "stack is empty, api function [%s]", __FUNCTION__);
		return 0;
	}
}

s4g_float s4g_sget_float(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(-1);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_float(s4gm->vmachine->execute.get(index));
	}
	else
	{
		s4g_rf(2, s4gm->name, "stack is empty, api function [%s]", __FUNCTION__);
		return 0;
	}
}
s4g_bool s4g_sget_bool(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(-1);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_bool(s4gm->vmachine->execute.get(index));
	}
	else
	{
		s4g_rf(2, s4gm->name, "stack is empty, api function [%s]", __FUNCTION__);
		return 0;
	}
}

const char* s4g_sget_str(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(0);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_str(s4gm->vmachine->execute.get(index));
	}
	else
	{
		s4g_rf(2, s4gm->name, "stack is empty, api function [%s]", __FUNCTION__);
		return 0;
	}
}

s4g_c_function s4g_sget_cfunc(s4g_main* s4gm, int index)
{

	S4G_PRE_COND(0);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_c_func(s4gm->vmachine->execute.get(index));
	}
	else
	{
		s4g_rf(2, s4gm->name, "stack is empty, api function [%s]", __FUNCTION__);
		return 0;
	}
}


void s4g_pop(s4g_main* s4gm, int count)
{
	S4G_PRE_COND();

	s4gm->vmachine->execute.pop(count);
}

int s4g_gettop(s4g_main* s4gm)
{
	S4G_PRE_COND(-1);

	return s4gm->vmachine->execute.count();
}

/////////

int s4g_cfcount_arg(s4g_main* s4gm)
{
	S4G_PRE_COND(-1);

	return s4gm->vmachine->CurrCountArg;
}

s4g_value* s4g_cfget_arg(s4g_main* s4gm, int narg)
{
	S4G_PRE_COND(0);

	if (narg <= 0)
	{
		s4g_rf(2, s4gm->name, "narg '%d' must be greater than 0, api function [%s]", narg, __FUNCTION__);
		return 0;
	}

	if (s4gm->vmachine->CurrCountArg > 0)
	{
		int index = ((s4gm->vmachine->execute.count() - s4gm->vmachine->CurrCountArg)) + narg;
		if (s4gm->vmachine->execute.count() >= index)
			return s4gm->vmachine->execute.get(index);
		else
		{
			s4g_rf(2, s4gm->name, "argument number '%d' greater than the total number of arguments, api function [%s]", narg, __FUNCTION__);
			return 0;
		}
	}
	else if (s4gm->vmachine->CurrCountArg == 0)
	{
		s4g_rf(2, s4gm->name, "in function arguments have not been sent, api function [%s]", __FUNCTION__);
		return 0;
	}
	else if (s4gm->vmachine->CurrCountArg < 0)
	{
		s4g_rf(2, s4gm->name, "function is not called, api function [%s]", __FUNCTION__);
		return 0;
	}
}

s4g_int s4g_cfget_int(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);
	
	if (tval)
		return s4gm->gc->get_int(tval);

	return 0;
}

s4g_uint s4g_cfget_uint(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return s4gm->gc->get_uint(tval);

	return 0;
}

s4g_float s4g_cfget_float(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return s4gm->gc->get_float(tval);

	return 0;
}

s4g_bool s4g_cfget_bool(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return s4gm->gc->get_bool(tval);

	return 0;
}

const char* s4g_cfget_str(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return s4gm->gc->get_str(tval);

	return 0;
}

s4g_c_function s4g_cfget_cfunc(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return s4gm->gc->get_c_func(tval);

	return 0;
}


int s4g_cfis_int(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return (s4gm->vmachine->gc->get_type(tval) == t_int ? 1 : 0);

	return 0;
}

int s4g_cfis_uint(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return (s4gm->vmachine->gc->get_type(tval) == t_uint ? 1 : 0);

	return 0;
}

int s4g_cfis_float(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return (s4gm->vmachine->gc->get_type(tval) == t_float ? 1 : 0);

	return 0;
}

int s4g_cfis_bool(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return (s4gm->vmachine->gc->get_type(tval) == t_bool ? 1 : 0);

	return 0;
}

int s4g_cfis_str(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return (s4gm->vmachine->gc->get_type(tval) == t_string ? 1 : 0);

	return 0;
}

int s4g_cfis_cfunc(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return (s4gm->vmachine->gc->get_type(tval) == t_cfunc ? 1 : 0);

	return 0;
}
