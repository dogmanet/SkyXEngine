
#pragma once 
#include <string\string.cpp>
#include <s4g\s4g.h>
#include <s4g\s4g_api.h>

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
		Sleep(50000);
		exit(1);
	}
}

s4g_report_func s4g_rf = s4g_report;

#include <s4g\s4g_parser.cpp>


s4g_main::s4g_main(const char* _name)
{
	arr_lex = new s4g_arr_lex();
	gc = new s4g_gc();
	gnode = 0;
	bst = new s4g_builder_syntax_tree();
	bst->arr_lex = arr_lex;
	bst->gc = gc;
	commands = new s4g_stack<s4g_command>();
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


s4g_value::s4g_value()
{
	typedata = 0;
	isdelete = false;
	pdata = 0;
	strcpy(name, "#");
}

s4g_value::~s4g_value()
{

}

inline s4g_value* s4g_gc::cr_val_null(const char* name)
{ 
	s4g_value* tval = 0;
	s4g_data* tdata = 0;

	tval = MemValue.Alloc();
	tval->typedata = typedata;
	arrvar.push(tval);
	tval->idvar = arrvar.count_obj;
	tval->iddata = 0;
	tval->pdata = arrdata.Arr[0];
	if (name)
		strcpy(tval->name, name);
	else
		strcpy(tval->name, "#");
	return tval;
};

inline s4g_value* s4g_gc::cr_val_pdata(s4g_pdata pdata, const char* name)
{
	s4g_data* tdata = 0;
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval, tdata, name);
	tdata->data = pdata;
	tdata->type = t_pdata;
	tmpval->pdata = tdata;
	return tmpval;
}

inline s4g_value* s4g_gc::cr_val_table_null(const char* name)
{ 
	s4g_data* tdata = 0;
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval, tdata, name);
	tdata->data = MemTable.Alloc();
	tdata->type = t_table;
	tmpval->pdata = tdata;
	return tmpval; 
};

inline s4g_value* s4g_gc::cr_val_int(s4g_int num, const char* name)
{ 
	s4g_data* tdata = 0;
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval, tdata, name);

	s4g_int* tmpvv = MemInt.Alloc();
	*tmpvv = num;

	tdata->data = tmpvv;
	tdata->type = t_int;
	tmpval->pdata = tdata;
	return tmpval; 
};

inline s4g_value* s4g_gc::cr_val_uint(s4g_uint num, const char* name)
{
	s4g_data* tdata = 0;
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval, tdata, name);
	s4g_uint* tmpvv = MemUInt.Alloc();
	*tmpvv = num;
	tdata->data = tmpvv;
	tdata->type = t_uint;
	tmpval->pdata = tdata;
	return tmpval;
}

inline s4g_value* s4g_gc::cr_val_float(s4g_float num, const char* name)
{ 
	s4g_data* tdata = 0;
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval, tdata, name);
	s4g_float* tmpvv = MemFloat.Alloc();
	*tmpvv = num;
	tdata->data = tmpvv;
	tdata->type = t_float;
	tmpval->pdata = tdata;
	return tmpval; 
};

inline s4g_value* s4g_gc::cr_val_bool(s4g_bool bf, const char* name)
{
	s4g_data* tdata = 0;
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval, tdata, name);
	s4g_bool* tmpvv = MemBool.Alloc();
	*tmpvv = bf;
	tdata->data = tmpvv;
	tdata->type = t_bool;
	tmpval->pdata = tdata;
	return tmpval; 
};

inline s4g_value* s4g_gc::cr_val_str(const char* str, const char* name)
{
	s4g_data* tdata = 0;
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval, tdata, name);
	String* tmpvv = /*new String(str);*/ MemString.Alloc();
	*tmpvv = str;
	tdata->data = tmpvv;
	tdata->type = t_string;
	tmpval->pdata = tdata;
	return tmpval;
};

inline s4g_value* s4g_gc::cr_val_table(s4g_table* tt, const char* name)
{
	s4g_data* tdata = 0;
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval, tdata, name);
	
	tdata->data = tt;
	tdata->type = t_table;
	tmpval->pdata = tdata;
	return tmpval;
};

inline s4g_value* s4g_gc::cr_val_s_func(const char* name)
{
	s4g_data* tdata = 0;
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval, tdata, name);
	s4g_s_function* tmpvv = MemSFunc.Alloc();
	tdata->data = tmpvv;
	tdata->type = t_sfunc;
	tmpval->pdata = tdata;
	return tmpval;
}

inline s4g_value* s4g_gc::cr_val_s_func(s4g_s_function* func, const char* name)
{ 
	s4g_data* tdata = 0;
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval, tdata, name);

	tdata->data = func;
	tdata->type = t_sfunc;
	tmpval->pdata = tdata;
	return tmpval;
};

inline s4g_value* s4g_gc::cr_val_c_func(s4g_c_function func, const char* name)
{
	s4g_data* tdata = 0;
	s4g_value* tmpval = 0;// cr_val_null();
	def_cr_val_null(tmpval, tdata, name);

	tdata->data = func;
	tdata->type = t_cfunc;
	tmpval->pdata = tdata;
	return tmpval;
};

inline s4g_value* s4g_gc::cr_val_nn()
{
	s4g_data* tdata = 0;
	s4g_value* tmpval = 0;// cr_val_null();
	char* name = 0;
	def_cr_val_null(tmpval, tdata, name);

	tdata->data = 0;
	tdata->type = t_nnull;
	tmpval->pdata = tdata;
	return tmpval;
}

inline void s4g_gc::c_val(s4g_value* dest, s4g_value* src, bool incr)
{
	if (dest->iddata >= arrdata.count_obj || src->iddata >= arrdata.count_obj || arrdata.Arr.Data[dest->iddata] == 0 || arrdata.Arr.Data[src->iddata] == 0)
	{
		s4g_breakpoint;
		return;
	}

	--(arrdata.Arr.Data[dest->iddata]->ref);
	
	//long tmpdestref = arrdata[dest->iddata]->ref;
	dest->iddata = src->iddata;
	dest->pdata = src->pdata;

	if (incr)
		++(arrdata.Arr.Data[src->iddata]->ref);
}


//////////////

s4g_table::s4g_table()
{
	count_obj = 0;
}

s4g_table::~s4g_table()
{
	for (int i = 0; i < count_obj; i++)
	{
		if (Arr[i]->Value)
			Arr[i]->Value->isdelete = true;
	}
}

inline void s4g_table::clear()
{
	count_obj = 0;
}

inline int s4g_table::is_exists_s(const char* str)
{
	for (long i = 0; i<count_obj; i++)
	{
		char* tmpstr = Arr.Data[i]->Name;
		if (strcmp(Arr.Data[i]->Name, str) == 0)
		{
			return i;
		}
	}
	return -1;
}

inline bool s4g_table::is_exists_n(long key)
{
	return (count_obj >= key);
}

inline long s4g_table::is_exists_s2(const char* str, s4g_value** tval)
{
	for (long i = 0; i<count_obj; i++)
	{
		if (strcmp(Arr.Data[i]->Name, str) == 0)
		{
			*tval = Arr.Data[i]->Value;
			return i;
		}
	}
	return -1;
}

inline bool s4g_table::is_exists_n2(long key, s4g_value** tval)
{
	if (key - 1 >= 0 && key - 1 < count_obj)
	{
		*tval = Arr.Data[key - 1]->Value;
		return true;
	}

	return false;
}

inline s4g_value* s4g_table::gets(const char* str)
{
	for (long i = 0; i<count_obj; ++i)
	{
		if (strcmp(Arr.Data[i]->Name, str) == 0)
		{
			return Arr.Data[i]->Value;
		}
	}
	return 0;
}

inline s4g_value* s4g_table::getn(long id)
{
		if (id - 1 >= 0 && id - 1 < count_obj)
		{
			return Arr.Data[id - 1]->Value;
		}
		else if (id < 0)
		{
			if (abs(id) - 1 < count_obj)
				return Arr.Data[count_obj + id]->Value;
		}
		else if (id - 1 >= Arr.Size)
		{
			return Arr.Data[id - 1]->Value;
		}
	return 0;
}

//

inline long s4g_table::get_key(const char* name)
{
	for (long i = 0; i<count_obj; i++)
	{
		if (strcmp(Arr.Data[i]->Name, name) == 0)
		{
			return i;
		}
	}
	return -1;
}

inline void s4g_table::add_val_s(const char* name, s4g_value* val)
{
	long tmpkey = get_key(name);
	if (tmpkey != -1)
		Arr.Data[tmpkey]->Value = val;
	else if (count_obj < Arr.Size)
	{
		strcpy(Arr.Data[count_obj]->Name, name);
		Arr.Data[count_obj]->Value = val;
		++count_obj;
	}
	else
	{
		table_desc* tmpaa = Mem.Alloc();
		tmpaa->Value = val;
		strcpy(tmpaa->Name, name);
		Arr.push_back(tmpaa);
		++count_obj;
	}
}

inline void s4g_table::add_val_n(long num, s4g_value* val)
{
	if (num > ((UINT)-1) - 128 && num > 0)
	{
		s4g_breakpoint;
	}
	else if (num <= count_obj)
		Arr.Data[num-1]->Value = val;
	else if (num <= Arr.Size)
		Arr.Data[num - 1]->Value = val;
	else
	{
		table_desc* tmpaa = Mem.Alloc();
		tmpaa->Value = val;
		Arr[num-1] = tmpaa;
		count_obj = Arr.Size;
	}
}


inline void s4g_table::add_val(s4g_value* val)
{
	if (count_obj < Arr.Size)
	{
		Arr.Data[count_obj]->Name[0] = 0;
		Arr.Data[count_obj]->Value = val;
		++count_obj;
	}
	else
	{
		table_desc* tmpaa = Mem.Alloc();
		tmpaa->Value = val;
		Arr.push_back(tmpaa);
		++count_obj;
	}
}

inline long s4g_table::size()
{
	return count_obj;
}

inline const char* s4g_table::get_name_id(long id)
{
	if (id < count_obj && id >= 0 && Arr.Data[id])
		return Arr.Data[id]->Name;
	else
		return 0;
}

inline void s4g_table::reserve(int count_elem)
{
	Arr.reserve(count_elem);
	Mem.AllocBlock(count_elem);
}

///////////////////////

inline s4g_value* s4g_gc::cr_val(int _type, const char* _val, const char* name)
{
	s4g_value* tmpval = 0;// new s4g_value();

	if (_val)
	{
		if (_type == t_int)
		{
			tmpval = cr_val_int(atol(_val), name);
		}
		else if (_type == t_uint)
		{
			UINT num;
			sscanf(_val, "%u", &num);
			tmpval = cr_val_uint(num, name);
		}
		else if (_type == t_float)
		{
			tmpval = cr_val_float(atof(_val), name);
		}
		else if (_type == t_bool)
		{
			bool bf = false;
			if (strcmp(_val, "true") == 0 || atol(_val) != 0)
				bf = true;

			tmpval = cr_val_bool(bf, name);
		}
		else if (_type == t_string)
		{
			tmpval = cr_val_str(_val, name);
		}
		else if (_type == t_table)
		{
			tmpval = cr_val_table_null(name);
		}
		else if (_type == t_nnull)
		{
			tmpval = cr_val_nn();
		}
	}
	else
	{
		if (_type == t_table)
		{
			tmpval = cr_val_table_null(name);
		}
		else if (_type == t_nnull)
		{
			tmpval = cr_val_nn();
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
	iddata = -1;
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

	s4g_data* nulldata = MemData.Alloc();
	nulldata->typedata = 1;
	nulldata->iddata = arrdata.count_obj;
	nulldata->iddata = arrdata.count_obj;
	arrdata.push(nulldata);
}

inline s4g_int s4g_gc::get_int(s4g_value* val)
{
	S4G_COND_TYPE_12(val, 0);

	return *(s4g_int*)(val->pdata->data);
}

inline s4g_uint s4g_gc::get_uint(s4g_value* val)
{
	S4G_COND_TYPE_12(val, 0);

	return *(s4g_uint*)(val->pdata->data);
}
inline s4g_float s4g_gc::get_float(s4g_value* val)
{
	S4G_COND_TYPE_12(val, 0);

	return *(s4g_float*)(val->pdata->data);
}
inline const char* s4g_gc::get_str(s4g_value* val)
{
	S4G_COND_TYPE_12(val, 0);

	return ((String*)(val->pdata->data))->c_str();
}
inline s4g_table* s4g_gc::get_table(s4g_value* val)
{
	S4G_COND_TYPE_12(val, 0);

	return (s4g_table*)(val->pdata->data);
}
inline s4g_s_function* s4g_gc::get_s_func(s4g_value* val)
{
	S4G_COND_TYPE_12(val, 0);

	return (s4g_s_function*)(val->pdata->data);
}
inline s4g_c_function s4g_gc::get_c_func(s4g_value* val)
{
	S4G_COND_TYPE_12(val, 0);

	return (s4g_c_function)(val->pdata->data);
}
inline s4g_bool s4g_gc::get_bool(s4g_value* val)
{
	S4G_COND_TYPE_12(val, 0);

	return *(s4g_bool*)(val->pdata->data);
}

inline s4g_pdata s4g_gc::get_pdata(s4g_value* val)
{
	S4G_COND_TYPE_12(val, 0);

	return (s4g_pdata)(val->pdata->data);
}

inline s4g_type s4g_gc::get_type(s4g_value* val)
{
	return arrdata[val->iddata]->type;
}

#define def_gc_clear_del_data(tmpdata)\
if (tmpdata->type == t_table)\
{\
	MemTable.Delete((s4g_table*)tmpdata->data);\
}\
else if (tmpdata->type == t_sfunc)\
{\
	s4g_s_function* tsf = (s4g_s_function*)tmpdata->data;\
	if (tsf->externs)\
	{\
		for (int k = 0; k < tsf->externs->size(); ++k)\
		{\
			s4g_value* tmpval = tsf->externs->getn(k + 1);\
			tmpval->isdelete = true;\
			if (tmpval->iddata < arrdata.count_obj && arrdata.Arr.Data[tmpval->iddata])\
			{\
				--(arrdata.Arr.Data[tmpval->iddata]->ref);\
			}\
		}\
	}\
	tsf->args.clear();\
	tsf->commands.clear();\
	tsf->externs_strs.clear();\
	tsf->externs->clear();\
	if (tsf->externs_val && tsf->externs_val->iddata < arrdata.count_obj)\
	{\
		--(arrdata.Arr.Data[tsf->externs_val->iddata]->ref);\
		tsf->externs_val->typedata = 0;\
		tsf->externs_val->isdelete = true;\
		set_td_data(tsf->externs_val, 0);\
	}\
	MemSFunc.Delete(tsf);\
}\
else if (tmpdata->type == t_int)\
{\
	MemInt.Delete((s4g_int*)tmpdata->data);\
}\
else if (tmpdata->type == t_uint)\
{\
	MemUInt.Delete((s4g_uint*)tmpdata->data);\
}\
else if (tmpdata->type == t_float)\
{\
	MemFloat.Delete((s4g_float*)tmpdata->data);\
}\
else if (tmpdata->type == t_bool)\
{\
	MemBool.Delete((s4g_bool*)tmpdata->data);\
}\
else if (tmpdata->type == t_string)\
{\
	String* tstr = (String*)tmpdata->data;\
	MemString.Delete(tstr);\
}


void s4g_gc::clear(DWORD mls)
{
	long posend = arrdata.count_obj;
	long posend2 = arrdata.count_obj;

	int countdeldata = 0;
	int countdelvar = 0;

	s4g_value* tmpval = 0;
	s4g_data* tmpdata = 0;

	for (long i = count_nd_data; i < posend; ++i)
	{
		tmpdata = arrdata.Arr.Data[i];
			
			if (tmpdata && tmpdata->typedata == 0 && tmpdata->ref < 1)
			{
				def_gc_clear_del_data(tmpdata);
				MemData.Delete(tmpdata);
				arrdata.Arr.Data[i] = 0;
				
				++countdeldata;
				tmpdata = 0;
			}

			if (tmpdata == 0)
			{
				for (long k = 1; k < posend - count_nd_data; ++k)
				{
					if (i == (posend - k)-1)
						break;

					tmpdata = arrdata.Arr.Data[posend - k];
					if (tmpdata && tmpdata->typedata == 0 && tmpdata->ref < 1)
					{
						def_gc_clear_del_data(tmpdata);
						MemData.Delete(tmpdata);
						arrdata.Arr.Data[posend - k] = 0;
						
						++countdeldata;
						posend2 = (posend - k);
					}
					else if (!tmpdata)
					{
						posend2 = (posend - k);
					}
					else
					{
						arrdata.Arr.Data[i] = tmpdata;
						arrdata.Arr.Data[i]->iddata = i;
						posend2 = (posend - k);
						break;
					}		
				}
			}
		posend = posend2;
	}

	arrdata.count_obj = posend2;

	posend = arrvar.count_obj;
	posend2 = arrvar.count_obj;
	
	for (long i = count_nd_value; i < posend; ++i)
	{
		tmpval = arrvar.Arr.Data[i];
			if (tmpval && (arrdata.Arr.Data[tmpval->iddata] == 0 || (tmpval->typedata == 0 && arrdata.Arr.Data[tmpval->iddata]->ref < 1) || (tmpval->isdelete && tmpval->typedata == 0)))
			{
				MemValue.Delete(tmpval);
				arrvar.Arr.Data[i] = 0;
				++countdelvar;
				tmpval = 0;
			}

			if (tmpval == 0)
			{
				for (long k = 1; k < posend - count_nd_value; ++k)
				{
					if (i == (posend - k)-1)
						break;
					tmpval = arrvar.Arr.Data[posend - k];
					if (tmpval && (arrdata.Arr.Data[tmpval->iddata] == 0 || (tmpval->typedata == 0 && arrdata.Arr.Data[tmpval->iddata]->ref < 1) || (tmpval->isdelete && tmpval->typedata == 0)))
					{
						MemValue.Delete(tmpval);
						arrvar.Arr.Data[posend - k] = 0;
						++countdelvar;
						tmpval = 0;
						posend2 = (posend - k);
					}
					else if (!tmpval)
					{
						posend2 = (posend - k);
					}
					else
					{
						arrvar.Arr.Data[i] = tmpval;
						arrvar.Arr.Data[i]->idvar = i;
						posend2 = (posend - k);
						break;
					}
				}
			}
		posend = posend2;
	}

	arrvar.count_obj = posend2;
}

inline void s4g_gc::begin_of_const_data()
{
	typedata = 1;
}

inline void s4g_gc::end_of_const_data()
{
	count_nd_data = (arrdata.count_obj > 0 ? arrdata.count_obj : 1);
	count_nd_value = arrvar.count_obj;
	typedata = 0;
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
		add_mem_contexts();
	}
		
	s4g_context* tcontext = arrcurrcontexts[curr_num_top_ctx];
	if (tcontext->oldtable)
	{
		tcontext->table = tcontext->oldtable;
		tcontext->oldtable = 0;
	}
	//tcontext->table->clear();
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
		add_mem_contexts();
	}

	tcontext = arrcurrcontexts[curr_num_top_ctx];
	//tcontext->table->clear();
	
	tcontext->valid = true;
	tcontext->oldtable = tcontext->table;
	tcontext->table = tt;

	++curr_num_top_ctx;
	return curr_num_top_ctx - 1;
}

void s4g_gc::remove_context(long id)
{
	if (id < arrcurrcontexts.count_obj && id >= 0)
	{
		if (arrcurrcontexts[id]->oldtable)
		{
			arrcurrcontexts[id]->table = arrcurrcontexts[id]->oldtable;
			arrcurrcontexts[id]->oldtable = 0;
		}
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

void s4g_gc::del_top_context(bool clear)
{
	if (clear)
	{
		s4g_context* tmpctx = arrcurrcontexts[curr_num_top_ctx - 1];
		s4g_value* tmpval = 0;
		for (int i = 0; i < tmpctx->table->size(); ++i)
		{
			tmpval = 0;
			tmpval = tmpctx->table->getn(i + 1);
			tmpval->isdelete = true;
			//tmpval->typedata = 0;
			if (tmpval->pdata)
				--(tmpval->pdata->ref);
		}

		tmpctx->table->clear();
	}
	--curr_num_top_ctx;
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
	delarrcontexts.push(ctx);
}

///////////////////////

