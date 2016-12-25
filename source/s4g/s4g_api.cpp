
#include <s4g\s4g_api.h>

#define S4G_PRE_COND(retval) if (s4gm == 0){s4g_report(2, "!!!", "script system is not init, api function [%s]",__FUNCTION__);	return retval;}

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
		else if (tt == t_pdata)
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

int s4g_call_gc(s4g_main* s4gm, DWORD mls)
{
	s4gm->gc->clear(mls);
	return 0;
}

int s4g_load_file(s4g_main* s4gm, const char* file)
{
	S4G_PRE_COND(-1);
	s4gm->gc->begin_of_const_data();
	int status = s4gm->arr_lex->read(file);
	if (status != 0)
	{
		s4g_rf(2, s4gm->name, "lexer: %s", s4gm->arr_lex->strerror);
		sprintf(s4gm->strerror, "%s", s4gm->arr_lex->strerror);
		return status;
	}
	s4gm->gc->typedata = 1;

	s4gm->gnode = s4gm->bst->s4g_gen_tree();
	if (s4gm->bst->status != 0)
	{
		s4g_rf(2, s4gm->name, "parser: %s", s4gm->bst->error);
		sprintf(s4gm->strerror, "%s", s4gm->bst->error);
		return s4gm->bst->status;
	}
	s4gm->compiler->compile(s4gm->gnode, s4gm->commands);
	//s4gm->gc->typedata = 0;
	s4gm->gc->end_of_const_data();
	return 0;
}

int s4g_load_str(s4g_main* s4gm, const char* str)
{
	S4G_PRE_COND(-1);
	s4gm->gc->begin_of_const_data();
	int status = s4gm->arr_lex->read(str,false);
	if (status != 0)
	{
		s4g_rf(2, s4gm->name, "lexer: %s", s4gm->arr_lex->strerror);
		sprintf(s4gm->strerror, "%s", s4gm->arr_lex->strerror);
		return status;
	}
	s4gm->gc->typedata = 1;

	s4gm->gnode = s4gm->bst->s4g_gen_tree();
	if (s4gm->bst->status != 0)
	{
		s4g_rf(2, s4gm->name, "parser: %s", s4gm->bst->error);
		sprintf(s4gm->strerror, "%s", s4gm->bst->error);
		return s4gm->bst->status;
	}
	s4gm->compiler->compile(s4gm->gnode, s4gm->commands);
	//s4gm->gc->typedata = 0;
	s4gm->gc->end_of_const_data();
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
		s4gm->vmachine->run(s4gm->commands, (s4gm->vmachine->gvars));
		if (s4gm->vmachine->error != 0)
		{
			s4g_rf(2, s4gm->name, s4gm->vmachine->strerror);
		}
	}
}


void s4g_spush_table_null(s4g_main* s4gm, int count_elem)
{
	S4G_PRE_COND();

	s4g_value* ttable = s4gm->vmachine->gc->cr_val_table_null();
	s4g_table* tt = s4gm->vmachine->gc->get_table(ttable);
	tt->reserve(count_elem);
	s4gm->vmachine->execute.push(ttable);
	//s4gm->vmachine->user_comm.get(s4gm->vmachine->user_comm.count_obj);
}

void s4g_spush_c_func(s4g_main* s4gm, s4g_c_function func)
{
	S4G_PRE_COND();

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_c_func(func));
}

void s4g_spush_int(s4g_main* s4gm, s4g_int num)
{
	S4G_PRE_COND();

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_int(num));
}

void s4g_spush_uint(s4g_main* s4gm, s4g_uint num)
{
	S4G_PRE_COND();

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_uint(num));
}

void s4g_spush_float(s4g_main* s4gm, s4g_float num)
{
	S4G_PRE_COND();

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_float(num));
}

void s4g_spush_str(s4g_main* s4gm, const char* str)
{
	S4G_PRE_COND();

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_str(str));
}

void s4g_spush_bool(s4g_main* s4gm, s4g_bool bf)
{
	S4G_PRE_COND();

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_bool(bf));
}

void s4g_spush_pdata(s4g_main* s4gm, s4g_pdata pdata)
{
	S4G_PRE_COND();

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_pdata(pdata));
}

void s4g_spush_null(s4g_main* s4gm)
{
	S4G_PRE_COND();

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_null());
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

int s4g_sis_pdata(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(-1);

	return (s4g_sget_type(s4gm, index) == t_pdata ? 1 : 0);
}

int s4g_sis_null(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(-1);

	return (s4g_sget_type(s4gm, index) == t_null ? 1 : 0);
}


s4g_type s4g_sget_type(s4g_main* s4gm, int index)
{
	S4G_PRE_COND((s4g_type)-1);

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

s4g_pdata s4g_sget_pdata(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(0);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_pdata(s4gm->vmachine->execute.get(index));
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
