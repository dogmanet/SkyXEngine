
/*
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
license MIT see in LICENSE or
https://s4g.su/
*/

#define S4G_PRE_COND(s4gm, retval) if (s4gm == 0){s4g_gen_msg(s4gm, S4G_ERROR, "!!!", "script system is not init, api function [%s]",__FUNCTION__);	return retval;}

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
	S4G_PRE_COND(s4gm);

	mem_delete(s4gm);
}

void s4g_clear(s4g_main* s4gm)
{
	s4gm->clear();
}

void s4g_call_gc(s4g_main* s4gm)
{
	S4G_PRE_COND(s4gm);
	s4gm->gc->clear();
}

long s4g_gc_mem_busy(s4g_main* s4gm)
{
	S4G_PRE_COND(s4gm, -1);
	return s4gm->gc->get_count_mem_busy();
}

long s4g_gc_mem_allocated(s4g_main* s4gm)
{
	S4G_PRE_COND(s4gm, -1);
	return s4gm->gc->get_count_mem_allocated();
}

s4g_value* s4g_gcget_null(s4g_main* s4gm)
{
	S4G_PRE_COND(s4gm, 0);
	return s4gm->gc->get_val_null();

}

s4g_value* s4g_gcget_bool(s4g_main* s4gm, bool bf)
{
	S4G_PRE_COND(s4gm, 0);
	return s4gm->gc->get_bool(bf);
}

s4g_value* s4g_gcget_true(s4g_main* s4gm)
{
	S4G_PRE_COND(s4gm, 0);
	return s4gm->gc->get_bool(true);
}

s4g_value* s4g_gcget_false(s4g_main* s4gm)
{
	S4G_PRE_COND(s4gm, 0);
	return s4gm->gc->get_bool(false);
}

int s4g_load_file(s4g_main* s4gm, const char* file)
{
	S4G_PRE_COND(s4gm, -1);
	s4gm->gc->begin_of_const_data();
	int status = s4gm->arr_lex->read(file);
	if (status != 0)
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "lexer: %s", s4gm->arr_lex->strerror);
		sprintf(s4gm->strerror, "%s", s4gm->arr_lex->strerror);
		return status;
	}

	s4gm->gnode = s4gm->bst->s4g_gen_tree();
#if defined(_DEBUG)
	{
		if (s4gm->gnode)
		{
			String tree = String("var s4g_ast = ") + s4gm->gnode->Dump();
			FILE * pf = fopen("./Debug/ast/ast.json", "wb");
			if (pf)
			{
				fwrite(tree.c_str(), 1, tree.length(), pf);
				fclose(pf);
			}
		}
	}
	//exit(0);
#endif
	if (s4gm->bst->status != 0)
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "parser: %s", s4gm->bst->error);
		sprintf(s4gm->strerror, "%s", s4gm->bst->error);
		return s4gm->bst->status;
	}
	
	s4gm->compiler->compile(s4gm->gnode, s4gm->commands);
	s4gm->compiler->comms = 0;
#ifdef S4G_CLEAR_AST
	s4gm->bst->NodePool.clear();
#endif
	s4gm->gc->end_of_const_data();
	return 0;
}

int s4g_load_str(s4g_main* s4gm, const char* str)
{
	S4G_PRE_COND(s4gm, -1);
	s4gm->gc->begin_of_const_data();
	int status = s4gm->arr_lex->read(str,false);
	if (status != 0)
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "lexer: %s", s4gm->arr_lex->strerror);
		sprintf(s4gm->strerror, "%s", s4gm->arr_lex->strerror);
		return status;
	}

	s4gm->gnode = s4gm->bst->s4g_gen_tree();
	if (s4gm->bst->status != 0)
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "parser: %s", s4gm->bst->error);
		sprintf(s4gm->strerror, "%s", s4gm->bst->error);
		return s4gm->bst->status;
	}
	s4gm->compiler->compile(s4gm->gnode, s4gm->commands);
#ifdef S4G_CLEAR_AST
	s4gm->bst->NodePool.clear();
#endif
	s4gm->gc->end_of_const_data();
	return 0;
}

void s4g_call(s4g_main* s4gm, bool call_func)
{
	S4G_PRE_COND(s4gm);

	if (call_func)
	{
		if (s4gm->vmachine->execute.count_obj > 0)
		{
			s4gm->vmachine->com_call();
			if (s4gm->vmachine->error < 0)
			{
				s4g_gen_msg(s4gm, S4G_ERROR, s4gm->vmachine->strerror);
				return;
			}

			s4gm->vmachine->run(s4gm->vmachine->curr_comm, (s4gm->vmachine->curr_vars));
			if (s4gm->vmachine->error < 0)
			{
				s4g_gen_msg(s4gm, S4G_ERROR, s4gm->vmachine->strerror);
			}
		}
	}
	else
	{
		s4gm->vmachine->run(s4gm->commands, (s4gm->vmachine->gvars));
		if (s4gm->vmachine->error < 0)
		{
			s4g_gen_msg(s4gm, S4G_ERROR, s4gm->vmachine->strerror);
		}
	}
}


void s4g_spush_table_null(s4g_main* s4gm, int count_elem)
{
	S4G_PRE_COND(s4gm);

	s4g_value* ttable = s4gm->vmachine->gc->cr_val_table_null();
	s4g_table* tt = s4gm->vmachine->gc->get_table(ttable);
	tt->reserve(count_elem);
	s4gm->vmachine->execute.push(ttable);
	//s4gm->vmachine->user_comm.get(s4gm->vmachine->user_comm.count_obj);
}

void s4g_spush_c_func(s4g_main* s4gm, s4g_c_function func)
{
	S4G_PRE_COND(s4gm);

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_c_func(func));
}

void s4g_spush_int(s4g_main* s4gm, s4g_int num)
{
	S4G_PRE_COND(s4gm);

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_int(num));
}

void s4g_spush_uint(s4g_main* s4gm, s4g_uint num)
{
	S4G_PRE_COND(s4gm);

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_uint(num));
}

void s4g_spush_float(s4g_main* s4gm, s4g_float num)
{
	S4G_PRE_COND(s4gm);

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_float(num));
}

void s4g_spush_str(s4g_main* s4gm, const char* str)
{
	S4G_PRE_COND(s4gm);

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_str(str));
}

void s4g_spush_bool(s4g_main* s4gm, s4g_bool bf)
{
	S4G_PRE_COND(s4gm);

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_bool(bf));
}

void s4g_spush_pdata(s4g_main* s4gm, s4g_pdata pdata)
{
	S4G_PRE_COND(s4gm);

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_pdata(pdata));
}

void s4g_spush_null(s4g_main* s4gm)
{
	S4G_PRE_COND(s4gm);

	s4gm->vmachine->execute.push(s4gm->vmachine->gc->cr_val_null());
}

void s4g_spush_precall(s4g_main* s4gm)
{
	S4G_PRE_COND(s4gm);

	s4gm->vmachine->sr.setn_first_free(s4gm->vmachine->execute.count_obj);
}

void s4g_spush_value(s4g_main* s4gm, s4g_value* val)
{
	S4G_PRE_COND(s4gm);

	s4gm->vmachine->execute.push(val);
}

void s4g_sstore(s4g_main* s4gm, int index, const char* name)
{
	S4G_PRE_COND(s4gm);

	if (index == S4G_NM_GLOBAL)
	{
		if (s4gm->vmachine->execute.count() >= 1)
		{
			s4g_value* tval = s4gm->vmachine->execute.get(-1);
				strcpy(tval->name, name);
				s4gm->vmachine->gvars->add_val_s(name, tval);
			s4gm->vmachine->execute.pop(1);
		}
		else
		{
				s4g_gen_msg(s4gm, S4G_ERROR, "stack is empty, api function [%s]", __FUNCTION__);
		}
	}
	else if (index == S4G_NM_SYS)
	{
		int qwert = 0;
		if (s4gm->vmachine->execute.count() > 0)
		{
			s4g_value* tval = s4gm->vmachine->execute.get(-1);
			strcpy(tval->name, name);
			s4gm->gc->add_in_const_ctx(name, tval);
			s4gm->vmachine->execute.pop(1);
		}
		else
		{
			s4g_gen_msg(s4gm, S4G_ERROR, "stack is empty, api function [%s]", __FUNCTION__);
		}
	}
	else if (index >= 0)
	{
		if (s4gm->vmachine->execute.count() >= index)
		{
			s4g_gen_msg(s4gm, S4G_ERROR, "invalid index '%d', size stack '%d', api function [%s]", index, s4gm->vmachine->execute.count(), __FUNCTION__);
			return;
		}

		s4g_value* tval = s4gm->vmachine->execute.get(index);
		if (s4gm->gc->get_type(tval) == t_table)
		{
			s4g_table* ttable = s4gm->gc->get_table(tval);
			s4g_value* tval2 = s4gm->vmachine->execute.get(-1);
			strcpy(tval2->name, name);
			ttable->add_val_s(name, tval2);
			s4gm->vmachine->execute.pop(1);
		}
		else
		{
			s4g_gen_msg(s4gm, S4G_ERROR, "stack element at '%d' is not a table, api function [%s]", index, __FUNCTION__);
		}
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "invalid index '%d', api function [%s]", index, __FUNCTION__);
	}
}


void s4g_sget(s4g_main* s4gm, int index, const char* name)
{
	S4G_PRE_COND(s4gm);

	if (index == S4G_NM_GLOBAL)
	{
		s4g_value* tval = s4gm->vmachine->gvars->gets(name);
		if (!tval)
		{
				s4g_gen_msg(s4gm, S4G_ERROR, "global variable '%s' is not found, api function [%s]", name, __FUNCTION__);
			return;
		}
		s4gm->vmachine->execute.push(tval);
	}
	else if (index == S4G_NM_SYS)
	{
		s4g_value* tval = s4gm->gc->get_out_const_ctx(name);
		if (tval)
		{
			s4gm->vmachine->execute.push(tval);
		}
		else
		{
			s4g_gen_msg(s4gm, S4G_ERROR, "sys_ctx variable '%s' is not found, api function [%s]", name, __FUNCTION__);
			return;
		}
	}
	else if (index >= 0)
	{
		if (s4gm->vmachine->execute.count() >= index)
		{
			s4g_gen_msg(s4gm, S4G_ERROR, "invalid index '%d', size stack '%d', api function [%s]", index, s4gm->vmachine->execute.count(), __FUNCTION__);
			return;
		}

		s4g_value* tval = s4gm->vmachine->execute.get(index);
		if (s4gm->gc->get_type(tval) == t_table)
		{
			s4g_table* ttable = s4gm->gc->get_table(tval);
			s4g_value* tval2 = ttable->gets(name);
			if (!tval2)
			{
				s4g_gen_msg(s4gm, S4G_ERROR, "table var '%s' is not found, api function [%s]", name, __FUNCTION__);
				return;
			}
			s4gm->vmachine->execute.push(tval2);
		}
		else
		{
			s4g_gen_msg(s4gm, S4G_ERROR, "stack element at '%d' is not a table, api function [%s]", index, __FUNCTION__);
		}
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "invalid index '%d', api function [%s]", index, __FUNCTION__);
	}
}


int s4g_sis_int(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm,- 1);

	return (s4g_sget_type(s4gm, index) == t_int ? 1 : 0);
}

int s4g_sis_uint(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm,- 1);

	return (s4g_sget_type(s4gm, index) == t_uint ? 1 : 0);
}

int s4g_sis_float(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm, -1);

	return (s4g_sget_type(s4gm, index) == t_float ? 1 : 0);
}

int s4g_sis_str(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm, -1);

	return (s4g_sget_type(s4gm, index) == t_string ? 1 : 0);
}

int s4g_sis_bool(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm, -1);

	return (s4g_sget_type(s4gm, index) == t_bool ? 1 : 0);
}

int s4g_sis_c_func(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm, -1);

	return (s4g_sget_type(s4gm, index) == t_cfunc ? 1 : 0);
}

int s4g_sis_s_func(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm, -1);

	return (s4g_sget_type(s4gm, index) == t_sfunc ? 1 : 0);
}

int s4g_sis_table(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm, -1);

	return (s4g_sget_type(s4gm, index) == t_table ? 1 : 0);
}

int s4g_sis_pdata(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm, -1);

	return (s4g_sget_type(s4gm, index) == t_pdata ? 1 : 0);
}

int s4g_sis_null(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm, -1);

	return (s4g_sget_type(s4gm, index) == t_null ? 1 : 0);
}


s4g_type s4g_sget_type(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm, (s4g_type)-1);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_type(s4gm->vmachine->execute.get(index));
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "stack is empty, api function [%s]", __FUNCTION__);
		return (s4g_type)-1;
	}
}


s4g_int s4g_sget_int(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm, -1);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_int(s4gm->vmachine->execute.get(index));
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "stack is empty, api function [%s]", __FUNCTION__);
		return 0;
	}
}

s4g_uint s4g_sget_uint(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm, -1);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_uint(s4gm->vmachine->execute.get(index));
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "stack is empty, api function [%s]", __FUNCTION__);
		return 0;
	}
}

s4g_float s4g_sget_float(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm, -1);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_float(s4gm->vmachine->execute.get(index));
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "stack is empty, api function [%s]", __FUNCTION__);
		return 0;
	}
}
s4g_bool s4g_sget_bool(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm, -1);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_bool(s4gm->vmachine->execute.get(index));
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "stack is empty, api function [%s]", __FUNCTION__);
		return 0;
	}
}

const char* s4g_sget_str(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm, 0);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_str(s4gm->vmachine->execute.get(index));
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "stack is empty, api function [%s]", __FUNCTION__);
		return 0;
	}
}

s4g_c_function s4g_sget_cfunc(s4g_main* s4gm, int index)
{

	S4G_PRE_COND(s4gm, 0);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_c_func(s4gm->vmachine->execute.get(index));
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "stack is empty, api function [%s]", __FUNCTION__);
		return 0;
	}
}

s4g_pdata s4g_sget_pdata(s4g_main* s4gm, int index)
{
	S4G_PRE_COND(s4gm, 0);

	if (s4gm->vmachine->execute.count() >= index)
	{
		return s4gm->gc->get_pdata(s4gm->vmachine->execute.get(index));
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "stack is empty, api function [%s]", __FUNCTION__);
		return 0;
	}
}


void s4g_spop(s4g_main* s4gm, int count)
{
	S4G_PRE_COND(s4gm);

	s4gm->vmachine->execute.pop(count);
}

int s4g_sgettop(s4g_main* s4gm)
{
	S4G_PRE_COND(s4gm, -1);

	return s4gm->vmachine->execute.count();
}

/////////

int s4g_cfcount_arg(s4g_main* s4gm)
{
	S4G_PRE_COND(s4gm, -1);

	return s4gm->vmachine->stackarg.count_obj;
}

s4g_value* s4g_cfget_arg(s4g_main* s4gm, int narg)
{
	S4G_PRE_COND(s4gm, 0);

	if (narg <= 0)
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "argument number '%d' must be > 0, api function [%s]", narg, __FUNCTION__);
		return 0;
	}

	if (s4gm->vmachine->stackarg.count_obj > 0)
	{
		if (s4gm->vmachine->stackarg.count_obj >= narg - 1)
			return s4gm->vmachine->stackarg.get(narg - 1);
		else
		{
			s4g_gen_msg(s4gm, S4G_ERROR, "argument number '%d' greater than the total number of arguments, api function [%s]", narg, __FUNCTION__);
			return 0;
		}
	}
	else if (s4gm->vmachine->stackarg.count_obj == 0)
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "there is no arguments, api function [%s]", __FUNCTION__);
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

s4g_pdata s4g_cfget_pdata(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return s4gm->gc->get_pdata(tval);

	return 0;
}


int s4g_cfis_null(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return (s4gm->vmachine->gc->get_type(tval) == t_null);

	return 0;
}

int s4g_cfis_int(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return (s4gm->vmachine->gc->get_type(tval) == t_int);

	return 0;
}

int s4g_cfis_uint(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return (s4gm->vmachine->gc->get_type(tval) == t_uint);

	return 0;
}

int s4g_cfis_float(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return (s4gm->vmachine->gc->get_type(tval) == t_float);

	return 0;
}

int s4g_cfis_bool(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return (s4gm->vmachine->gc->get_type(tval) == t_bool);

	return 0;
}

int s4g_cfis_str(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return (s4gm->vmachine->gc->get_type(tval) == t_string);

	return 0;
}

int s4g_cfis_table(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return (s4gm->vmachine->gc->get_type(tval) == t_table);

	return 0;
}

int s4g_cfis_cfunc(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return (s4gm->vmachine->gc->get_type(tval) == t_cfunc);

	return 0;
}

int s4g_cfis_pdata(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);

	if (tval)
		return (s4gm->vmachine->gc->get_type(tval) == t_pdata);

	return 0;
}

s4g_type s4g_cfget_type(s4g_main* s4gm, int narg)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);
	if (tval)
		return tval->pdata->type;

	return t_none;
}

const char* s4g_cfget_str_type(s4g_main* s4gm, int narg, char* str)
{
	s4g_value* tval = s4g_cfget_arg(s4gm, narg);
	if (tval)
		return s4g_get_str_type(tval->pdata->type, str);

	return "#error";
}

/////////////

const char* s4g_value_name(s4g_value* value)
{
	if (value)
		return value->name;

	return 0;
}

long s4g_table_size(s4g_table* ttable)
{
	if (ttable)
		return ttable->size();

	return -1;
}

s4g_value* s4g_table_get(s4g_table* ttable, long key)
{
	if (ttable && key < ttable->size())
		return ttable->getn(key);

	return 0;
}

///////////

const char* s4g_stack_trace(s4g_main* s4gm)
{
	S4G_PRE_COND(s4gm, 0);
	return s4gm->vmachine->stack_trace();
}

const char* s4g_dbg_get_curr_file(s4g_main* s4gm,char* str)
{
	S4G_PRE_COND(s4gm, 0);
	return s4gm->vmachine->get_curr_file(str);
}

long s4g_dbg_get_curr_str(s4g_main* s4gm, char* str)
{
	S4G_PRE_COND(s4gm, -1);
	return s4gm->vmachine->get_curr_str(str);
}

const char* s4g_dbg_get_curr_func(s4g_main* s4gm, char* str)
{
	S4G_PRE_COND(s4gm, 0);
	return s4gm->vmachine->get_curr_func(str);
}

/////////////

bool s4g_pp_is_def(s4g_main* s4gm, const char* name_def)
{
	S4G_PRE_COND(s4gm, 0);
	return(s4gm->arr_lex->preproc.isDefined(name_def));
}
void s4g_pp_def(s4g_main* s4gm, const char* def_str)
{
	S4G_PRE_COND(s4gm);
	s4gm->arr_lex->preproc.Define(def_str);
}
void s4g_pp_undef(s4g_main* s4gm, const char* name_def)
{
	S4G_PRE_COND(s4gm);
	s4gm->arr_lex->preproc.Undef(name_def);
}
void s4g_pp_add_inc_path(s4g_main* s4gm, const char* path)
{
	S4G_PRE_COND(s4gm);
	s4gm->arr_lex->preproc.AddIncPath(path);
}