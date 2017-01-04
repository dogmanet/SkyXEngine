
#include "s4g_vm.h"

s4g_vm::s4g_vm(s4g_gc* _gc)
{
	gc = _gc;
	gc->add_new_context(&gvars);
	vgvars = gc->cr_val_table(gvars, S4G_GLOBAL_NM, S4G_GC_TYPE_VAR_SYS);
	strerror[0] = 0; error = 0;
	cfetchpushstore = 0;
	curr_vars = 0;
	runexe = false;

#define GEN_OP(op) arropf[mc_ ## op] = &s4g_vm::com_ ## op;
	GEN_OP(halt)
	GEN_OP(fetch_get)
	GEN_OP(fetch)
	GEN_OP(store)
	GEN_OP(end)
	GEN_OP(mstore)
	arropf[mc_fetch_get_cr] = &s4g_vm::com_fetch_get;
	arropf[mc_fetch_cr] = &s4g_vm::com_fetch;

	GEN_OP(new_table)
	GEN_OP(add_in_table)
	GEN_OP(append_table)
	GEN_OP(push)
	GEN_OP(pop)
	GEN_OP(precall)
	GEN_OP(call)

	GEN_OP(add)
	GEN_OP(sub)
	GEN_OP(mul)
	GEN_OP(div)

	GEN_OP(block_new)
	GEN_OP(block_del)

	GEN_OP(preincr)
	GEN_OP(postincr)
	GEN_OP(predecr)
	GEN_OP(postdecr)

	GEN_OP(jz)
	GEN_OP(jnz)
	GEN_OP(jmp)

	GEN_OP(mod)
	GEN_OP(log_and)
	GEN_OP(log_or)
	GEN_OP(bit_xor)
	GEN_OP(bit_not)
	GEN_OP(bit_shiftr)
	GEN_OP(bit_shiftl)
	GEN_OP(log_neqt)
	GEN_OP(log_eq)
	GEN_OP(log_neq)
	GEN_OP(log_ge)
	GEN_OP(log_le)
	GEN_OP(log_gt)
	GEN_OP(log_lt)
	GEN_OP(log_not)
	GEN_OP(log_eqt)
	GEN_OP(bit_and)
	GEN_OP(bit_or)
#undef GEN_OP
	for (int i = 0; i < S4G_MAX_CALL; i++)
	{
		callstack.push(new s4g_call_data());
	}

	callstack.init_size(0);


	for (int i = 0; i < S4G_MAX_ENCLOSURE; i++)
	{
		blockstack.push(new s4g_call_data());
	}

	blockstack.init_size(0);
}

s4g_vm::~s4g_vm()
{
	execute.Arr.clear();
	stackarg.Arr.clear();

	for (int i = 0; i < S4G_MAX_CALL; i++)
	{
		mem_delete(callstack[i]);
	}

	for (int i = 0; i < S4G_MAX_ENCLOSURE; i++)
	{
		mem_delete(blockstack[i]);
	}

	callstack.Arr.clear();
}

////////////

inline void s4g_vm::com_block_new()
{
	ttable = 0;
	long idnewctx = gc->add_new_context(&ttable);
	tmpcd = blockstack.get(blockstack.count_obj);
	tmpcd->vars = curr_vars;
	tmpcd->idnewctx = idnewctx;
	
	curr_vars = ttable;
}

inline void s4g_vm::com_block_del()
{
	tmpcd = blockstack.get(blockstack.count_obj - 1);
	curr_vars = tmpcd->vars;
	gc->del_top_context(true);
	stack_pop(blockstack, 1);
}

inline void s4g_vm::com_fetch()
{
	is_cr = (op == mc_fetch_cr);
	cfetchpushstore = (cfetchpushstore == 0);
	
	str = gc->get_str(arg);
		if (strcmp(str, S4G_GLOBAL_NM) == 0)
		{
			execute.push(vgvars);
		}
		else
		{
			tmpval = 0;
			idctx = -1;
				if ((idctx = gc->ctx_is_exists_s(str, &tmpval)) != -1)
				{
					if (is_cr && s4gm->create_var)
					{
						error = -1;
						s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
						sprintf(this->strerror, "[%s]:%d - value '%s' is exists", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return;
					}

					execute.push(tmpval);
				}
				else
				{
					if (!is_cr && s4gm->create_var)
					{
						error = -1;
						s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
						sprintf(this->strerror, "[%s]:%d - value '%s' is not exists", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return;
					}

					tmpval = gc->cr_val_null(str);
					curr_vars->add_val_s(str, tmpval);
					execute.push(tmpval);
				}
		}

	cfetchget = 0;
}

inline void s4g_vm::com_fetch_get()
{
	is_cr = (op == mc_fetch_get_cr);
	
	int counttop = 1;
	tmpval = 0;
	if (arg)
	{
		counttop = 0;
		tmpval = arg;
	}
	else
		tmpval = execute.get(execute.count_obj - 1);

	ttable = 0;
		if(execute.count_obj >= 1+counttop)
		{
			ttype = gc->get_type(execute.get((execute.count_obj - counttop) - 1));
				if(ttype == t_table)
					ttable = gc->get_table(execute.get((execute.count_obj - counttop) - 1));
				else
				{
					char tmpfile[S4G_MAX_LEN_STR_IN_FILE];
					char tmpstr[S4G_MAX_LEN_TYPE_NAME];

					s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid - 1);
					strcpy(tmpfile, arr_lex->ArrFiles[tmplexs->fileid].c_str());
					sprintf(tmpstr, "%d", tmplexs->numstr);

					s4g_value* tval = execute.get((execute.count_obj - counttop) - 1);
					error = -1;
					char strtype[S4G_MAX_LEN_TYPE_NAME];
					s4g_get_str_type(ttype, strtype);
					sprintf(this->strerror, "[%s]:%s - value '%s' expected table but got %s", tmpfile, tmpstr, tval->name, strtype);
					return;
				}
				/*
				//если предыдущей командой было либо fetch либо fetch_get
				//то значит мы ложили на стек таблицу, однако это оказалось не так
				else if(cfetchget)	//error
					int qwert = 0;
				else
					ttable = curr_vars;*/
		}
		else
		{
			char tmpfile[S4G_MAX_LEN_STR_IN_FILE];
			char tmpstr[S4G_MAX_LEN_TYPE_NAME];

				s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid - 1);
				strcpy(tmpfile, arr_lex->ArrFiles[tmplexs->fileid].c_str());
				sprintf(tmpstr, "%d", tmplexs->numstr);


			error = -1;
			sprintf(this->strerror, "[%s]:%s - address to the table '%s', but the stack is empty", tmpfile, tmpstr, tmpval->name);
			return;
		}
			
	
		if (!arg)
		{
			//execute.pop(1);
			stack_pop(execute, 1);
		}

		//если предыдущей командой было либо fetch либо fetch_get
		//то значит мы ложили на стек таблицу, а теперь берем значение из этой таблицы и саму таблицу выталкиваем из стека
		//ложа на вершину значение из таблицы
		if (cfetchget == 1 || (oldop == mc_push && cfetchget == 2))
			stack_pop(execute, 1);
	
		if(currCom->second_data == 1)
		{
		if (tmpval->pdata->type == t_string || tmpval->pdata->type == t_int || tmpval->pdata->type == t_uint)
		{
				long index = -1;
				if (tmpval->pdata->type == t_string)
					index = atol(((String*)tmpval->pdata->data.p)->c_str());
				else if (tmpval->pdata->type == t_int)
					index = tmpval->pdata->data.i;

				tmpval = 0;

				if (ttable->is_exists_n2(index, &tmpval))
				{
					//execute.push(gc->cr_val_str(tmpval->name));
					execute.push(tmpval);
				}
				else if (ttable->size() > index)
				{
					//error
					int qwert = 0;
				}
				else
				{
					tmpval = gc->cr_val_null("");
					ttable->add_val(tmpval);
					execute.push(tmpval);
				}
			}
			else
			{
				int qwert = 0;
			}
		}
		else if (tmpval->pdata->type == t_string || tmpval->pdata->type == t_int || tmpval->pdata->type == t_uint)
		{
			str=0;
			str2[0] = 0;
			if (tmpval->pdata->type == t_string)
				str = gc->get_str(tmpval);
			else if (tmpval->pdata->type == t_int)
				str = ltoa(gc->get_int(tmpval), str2, 10);
			
			if (ttable->is_exists_s2(str, &tmpval) != -1)
			{
					if (is_cr && s4gm->create_var)
					{
						error = -1;
						s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
						sprintf(this->strerror, "[%s]:%d - key number '%s' is exists in table", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return;
					}

				execute.push(tmpval);
			}
			else
			{
					if (!is_cr && s4gm->create_var)
					{
						error = -1;
						s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
						sprintf(this->strerror, "[%s]:%d - key number '%s' is not exists in table", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return;
					}
					
					tmpval = gc->cr_val_null(str);
					ttable->add_val_s(str, tmpval);
					execute.push(tmpval);
			}
			
			//tmpval = ttable->cr_if_not_exists(str, gc);
			//execute.push(tmpval);
		}
		else
		{
			char tmpfile[S4G_MAX_LEN_STR_IN_FILE];
			char tmpstr[S4G_MAX_LEN_TYPE_NAME];

				s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid - 1);
				strcpy(tmpfile, arr_lex->ArrFiles[tmplexs->fileid].c_str());
				sprintf(tmpstr, "%d", tmplexs->numstr);

			error = -1;
			sprintf(this->strerror, "[%s]:%s - data type '%s' is unresolved address in table", tmpfile, tmpstr, tmpval->name);
			return;
		}

	cfetchget = 0;
	cfetchgetarg = (arg == 0);
}

inline void s4g_vm::com_store()
{
	tvalue = execute.get(execute.count_obj - 1);
	tvalue2 = execute.get(execute.count_obj - 2);
	
	if (tvalue->pdata->type == t_sfunc)
	{
		s4g_s_function* sf = gc->get_s_func(tvalue);
		if (sf->externs_strs.count() > 0 && !sf->externstable)
		{
			s4g_value* newsfval = gc->cr_val_s_func(tvalue2->name);
			s4g_s_function* newsf = gc->get_s_func(newsfval);
			newsf->args = sf->args;
			newsf->commands = sf->commands;
			newsf->externs_strs = sf->externs_strs;
			newsf->ismarg = sf->ismarg;
			newsf->externs_val = gc->cr_val_table_null(0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS);
			newsf->externstable = gc->get_table(newsf->externs_val);

			long tmpid = -1;
			for (int i = 0; i <= newsf->externs_strs.count(); i++)
			{
				const char* str = newsf->externs_strs.get(i).c_str();
				if ((tmpid = curr_vars->is_exists_s(str)) != -1)
				{
					tmpval = gc->cr_val_null(str);
					gc->c_val(tmpval, curr_vars->getn(tmpid),false);

					newsf->externstable->add_val_s(str, tmpval);
				}
			}
			tvalue = newsfval;
		}
	}

	gc->c_val(tvalue2, tvalue);
	
	if (oldop == mc_push && cfetchpushstore == 2)
	{
		//execute.pop(1);
		stack_pop(execute, 1);
	}
	else
	{
		//execute.pop(2);
		stack_pop(execute, 1);
	}

		if (oldop == mc_push && cfetchgetarg == 1)
		{
			//execute.pop(2);
			stack_pop(execute, 2);
		}
	cfetchgetarg = 0;
	cfetchpushstore = 0;
}

inline void s4g_vm::com_end()
{
	val_end = execute.count_obj-1;
}

inline void s4g_vm::com_mstore()
{
	long keyval = val_end;
	if (keyval == -1)
		keyval = 0;

	long countvar = (long)arg;
	long beginkeyvar = (execute.count_obj - (countvar - 1)) - 1;
	if (beginkeyvar - keyval > countvar)
		beginkeyvar = keyval + countvar;

	while (true)
	{
		if (keyval < beginkeyvar)
		{
			
			tvalue = execute.get((execute.count_obj - (countvar - 1)) - 1);
			tvalue2 = execute.get(keyval);

			gc->c_val(tvalue, tvalue2);
		}
		else
			break;

		keyval++;
		countvar--;
	}

	stack_pop(execute, execute.count_obj - val_end);
	//execute.pop(execute.count() - val_end);
}

inline void s4g_vm::com_add_in_table()
{
	ttable = gc->get_table(execute.get(execute.count_obj - 2));
	tmpval = (execute.get(execute.count_obj - 1));
	ttable->add_val(tmpval);
}

inline void s4g_vm::com_append_table()
{
	ttable = gc->get_table(execute.get(execute.count_obj - 1));
	stack_pop(execute, 1);
	tvalue = gc->cr_val_null("");
	ttable->add_val(tvalue);
	execute.push(tvalue);
}

inline void s4g_vm::com_precall()
{
	sr.setn_first_free(execute.count_obj);
}

inline void s4g_vm::com_call()
{
	//если количество вызовов не превысело установленный лимит
	if (S4G_MAX_CALL <= callstack.count())
	{
		char tmpfile[S4G_MAX_LEN_STR_IN_FILE];
		char tmpstr[S4G_MAX_LEN_TYPE_NAME];

		if (curr_comm)
		{
			s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid - 1);
			strcpy(tmpfile, arr_lex->ArrFiles[tmplexs->fileid].c_str());
			sprintf(tmpstr, "%d", tmplexs->numstr);
		}
		else
		{
			strcpy(tmpfile, "#user");
			sprintf(tmpstr, "#user");
		}

		error = -1;
		sprintf(this->strerror, "[%s]:%s - stack overflow, limit = %d", tmpfile, tmpstr, S4G_MAX_CALL);
		return;
	}

	int startpos = sr.getv_last_unfree();
	long countarg = (execute.count_obj - startpos)-1;	//получаем количество аргументов

	tvalfunc = execute.get(startpos);//в стеке лежит переменная ссылающаяся на функцию
	
		//если тип скриптовая функция
		if (tvalfunc->pdata->type == t_sfunc)
		{
			csfunc = gc->get_s_func(tvalfunc);

			//если функция не содержит кода, либо из кода там только halt
			if (csfunc->commands.count_obj <= 1)
			{
				//закачиваем работу с этой функцией
				sr.free_last_unfree();
				//execute.pop(countarg + 1); // выталкиваем из стека все что относилось к функции
				stack_pop(execute, countarg + 1);

				//если надо вернуть только один аргумент
				if ((long)arg == 1)
				{
					//вставляем в стек null
					execute.push(gc->get_val_null());
				}
				return;
			}
			long lastidctx = gc->deactivate_prev();	//деактивируем все активные возможные предыдущие контексты

			long idexternctx = -1;
			//если у нас есть подставляемые значения из другого констекста
			if (csfunc->externstable)
			{
				idexternctx = gc->add_context(csfunc->externstable);	//то устанавливаем контекст
			}

			//s4g_table* new_ctx = 0;
			ttable = 0;
			long idnewctx = gc->add_new_context(&ttable);	//создаем новый контекст
			
				//если аргументы есть
				if(countarg > 0)
				{
					long tmpargs = countarg;
					if (countarg > csfunc->args.count_obj)
						tmpargs = csfunc->args.count_obj;

						//записываем на сколько хватает аргументов
						for (int i = 0; i<tmpargs; ++i)
						{
							//tval2 = gc->cr_val_null();
							tvalue = execute.get((execute.count_obj - ((countarg - i) - 1)) - 1);
							//gc->c_val(tval2, tval, false);
							//const char* tmpstr = csfunc->args.get(i + 1).c_str();
							ttable->add_val_s(csfunc->args.get(i).c_str(), tvalue);
						}

						//если есть еще аргументы и у нас следущий аргмент это мультиаргумент
						if (countarg > tmpargs && csfunc->ismarg)
						{
								for (int i = tmpargs; i<countarg; ++i)
								{
									//tval2 = gc->cr_val_null();
									tvalue = execute.get((execute.count_obj - ((countarg - i) - 1) - 1));
									//gc->c_val(tval2, tval,false);
									csfunc->margtable->add_val(tvalue);
								}
							
								ttable->add_val_s(S4G_MARG, csfunc->marg_val);
						}
				}
			//execute.pop(countarg+1); // выталкиваем из стека все что относилось к функции
			stack_pop(execute, countarg + 1);

				//записываем в стек вызовов текущий вызов и сохранияем текущее состояние
			tmpcd = callstack.get(callstack.count_obj);
			tmpcd->coms = curr_comm;
			tmpcd->vars = curr_vars;
			tmpcd->cfetchget = cfetchget;
			tmpcd->cfetchgetarg = cfetchgetarg;
			tmpcd->cfetchpushstore = cfetchpushstore;
			tmpcd->idexternctx = idexternctx;
			tmpcd->idnewctx = idnewctx;
			tmpcd->lastidctx = lastidctx;
			tmpcd->id_curr_com = id_curr_com;
			tmpcd->valf = tvalfunc;
			if ((long)arg == 1)
				tmpcd->stack_size = execute.count_obj;
			else
				tmpcd->stack_size = -1;
			//strcpy(tmpcd->namef, tvalfunc->name);

			//устанавилваем новое окружение и новые конмады
			curr_vars = ttable;
			curr_comm = &(csfunc->commands);

			cfetchget = 3;
			cfetchgetarg = false;
			cfetchpushstore = 0;
			id_curr_com = -1;
		}
		//иначе если у нас с(++) функция
		else if (tvalfunc->pdata->type == t_cfunc)
		{
			for (int i = 0; i < countarg; i++)
			{
				tvalue = execute.get((execute.count_obj - ((countarg - i) - 1)) - 1);
				stackarg.push(tvalue);
			}
			tcfunc = (gc->get_c_func(tvalfunc));
			stack_pop(execute, countarg + 1);

			//записываем в стек вызовов текущий вызов и сохранияем текущее состояние
			tmpcd = callstack.get(callstack.count_obj);
			tmpcd->valf = tvalfunc;
			if ((long)arg == 1)
				tmpcd->stack_size = execute.count_obj;
			else
				tmpcd->stack_size = -1;

			error = (tcfunc)(s4gm);

			//если указано что нужно проконтролировать количество возвращаемых значений
			//оно должно быть одним
			if (tmpcd->stack_size != -1)
			{
				//если количество возвращенных значений больше 1
				if (tmpcd->stack_size + 1 < execute.count_obj)
				{
					stack_pop(execute, execute.count_obj - (tmpcd->stack_size + 1));	//убираем все лишние
				}
				//иначе если ничего не вернули
				else if (tmpcd->stack_size >= execute.count_obj)
					execute.push(gc->get_val_null());	//добавляем null значение
			}

			--callstack.count_obj;
			sr.free_last_unfree();
			stackarg.count_obj = 0;
		}
		//иначе у нас не функция
		else
		{
			char tmpfile[S4G_MAX_LEN_STR_IN_FILE];
			char tmpstr[S4G_MAX_LEN_TYPE_NAME];
			
			if (curr_comm)
			{
				s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid - 1);
				strcpy(tmpfile, arr_lex->ArrFiles[tmplexs->fileid].c_str());
				sprintf(tmpstr, "%d", tmplexs->numstr);
			}
			else
			{
				strcpy(tmpfile, "#user");
				sprintf(tmpstr, "#user");
			}

			error = -1;
			char strtype[S4G_MAX_LEN_TYPE_NAME];
			s4g_get_str_type(gc->get_type(tvalfunc), strtype);
			sprintf(this->strerror, "[%s]:%s - called value '%s' is not function, this is '%s'", tmpfile, tmpstr, tvalfunc->name, strtype);
		}
}

inline void s4g_vm::com_new_table()
{
	tmpval = gc->cr_val_table_null(S4G_GC_TYPE_VAR_FREE);
	execute.push(tmpval);
}

inline void s4g_vm::com_preincr()
{
	tmpval = execute.get(execute.count_obj - 1);

	if (tmpval->pdata->type == t_int)
	{
		if (tmpval->pdata->typedata == S4G_GC_TYPE_DATA_PRIVATE)
		{
			++(tmpval->pdata->data.i);
		}
		else
		{
			--(tmpval->pdata->ref);
			tmpval->pdata = gc->cr_dara_int((tmpval->pdata->data.i) + 1, S4G_GC_TYPE_DATA_PRIVATE);
			tmpval->pdata->ref = 1;
		}
	}
	else if (tmpval->pdata->type == t_uint)
	{
		if (tmpval->pdata->typedata == S4G_GC_TYPE_DATA_PRIVATE)
		{
			++(tmpval->pdata->data.ui);
		}
		else
		{
			--(tmpval->pdata->ref);
			tmpval->pdata = gc->cr_dara_uint((tmpval->pdata->data.ui) + 1, S4G_GC_TYPE_DATA_PRIVATE);
			tmpval->pdata->ref = 1;
		}
	}
	else if (tmpval->pdata->type == t_float)
	{
		if (tmpval->pdata->typedata == S4G_GC_TYPE_DATA_PRIVATE)
		{
			++(tmpval->pdata->data.f);
		}
		else
		{
			--(tmpval->pdata->ref);
			tmpval->pdata = gc->cr_dara_float((tmpval->pdata->data.f) + 1.0, S4G_GC_TYPE_DATA_PRIVATE);
			tmpval->pdata->ref = 1;
		}
	}
	else
	{
		S4G_VM_OP_ARIF_INCR_DECR_ERR(tmpval);
	}
}

inline void s4g_vm::com_postincr()
{
	tvalue = execute.get(execute.count_obj - 1);

	if (tvalue->pdata->type == t_int)
	{
		if (tvalue->pdata->typedata == S4G_GC_TYPE_DATA_PRIVATE)
		{
			tvalue2 = gc->cr_val2(tvalue, S4G_GC_TYPE_VAR_FREE, S4G_GC_TYPE_DATA_FREE, true);
			execute.push_r(tvalue2);
			++(tvalue->pdata->data.i);
		}
		else
		{
			--(tvalue->pdata->ref);

			tvalue2 = gc->cr_val2(tvalue);
			--(tvalue2->pdata->ref);
			execute.push_r(tvalue2);

			tvalue->pdata = gc->cr_dara_int((tvalue->pdata->data.i) + 1, S4G_GC_TYPE_DATA_PRIVATE);
			tvalue->pdata->ref = 1;
		}
	}
	else if (tvalue->pdata->type == t_uint)
	{
		if (tvalue->pdata->typedata == S4G_GC_TYPE_DATA_PRIVATE)
		{
			tvalue2 = gc->cr_val2(tvalue, S4G_GC_TYPE_VAR_FREE, S4G_GC_TYPE_DATA_FREE, true);
			execute.push_r(tvalue2);
			++(tvalue->pdata->data.ui);
		}
		else
		{
			--(tvalue->pdata->ref);

			tvalue2 = gc->cr_val2(tvalue);
			--(tvalue2->pdata->ref);
			execute.push_r(tvalue2);

			tvalue->pdata = gc->cr_dara_uint((tvalue->pdata->data.ui) + 1, S4G_GC_TYPE_DATA_PRIVATE);
			tvalue->pdata->ref = 1;
		}
	}
	else if (tvalue->pdata->type == t_float)
	{
		if (tvalue->pdata->typedata == S4G_GC_TYPE_DATA_PRIVATE)
		{
			tvalue2 = gc->cr_val2(tvalue, S4G_GC_TYPE_VAR_FREE, S4G_GC_TYPE_DATA_FREE, true);
			execute.push_r(tvalue2);
			++(tvalue->pdata->data.f);
		}
		else
		{
			--(tvalue->pdata->ref);

			tvalue2 = gc->cr_val2(tvalue);
			--(tvalue2->pdata->ref);
			execute.push_r(tvalue2);

			tvalue->pdata = gc->cr_dara_float((tvalue->pdata->data.f) + 1.0f, S4G_GC_TYPE_DATA_PRIVATE);
			tvalue->pdata->ref = 1;
		}
	}
	else
	{
		S4G_VM_OP_ARIF_INCR_DECR_ERR(tmpval);
	}
}

inline void s4g_vm::com_predecr()
{
	tmpval = execute.get(execute.count_obj - 1);

	if (tmpval->pdata->type == t_int)
	{
		if (tmpval->pdata->typedata == S4G_GC_TYPE_DATA_PRIVATE)
		{
			--(tmpval->pdata->data.i);
		}
		else
		{
			--(tmpval->pdata->ref);
			tmpval->pdata = gc->cr_dara_int((tmpval->pdata->data.i) - 1, S4G_GC_TYPE_DATA_PRIVATE);
			tmpval->pdata->ref = 1;
		}
	}
	else if (tmpval->pdata->type == t_uint)
	{
		if (tmpval->pdata->typedata == S4G_GC_TYPE_DATA_PRIVATE)
		{
			--(tmpval->pdata->data.ui);
		}
		else
		{
			--(tmpval->pdata->ref);
			tmpval->pdata = gc->cr_dara_uint((tmpval->pdata->data.ui) - 1, S4G_GC_TYPE_DATA_PRIVATE);
			tmpval->pdata->ref = 1;
		}
	}
	else if (tmpval->pdata->type == t_float)
	{
		if (tmpval->pdata->typedata == S4G_GC_TYPE_DATA_PRIVATE)
		{
			--(tmpval->pdata->data.f);
		}
		else
		{
			--(tmpval->pdata->ref);
			tmpval->pdata = gc->cr_dara_float((tmpval->pdata->data.f) - 1.0, S4G_GC_TYPE_DATA_PRIVATE);
			tmpval->pdata->ref = 1;
		}
	}
	else
	{
		S4G_VM_OP_ARIF_INCR_DECR_ERR(tmpval);
	}
}

inline void s4g_vm::com_postdecr()
{
	tvalue = execute.get(execute.count_obj - 1);

	if (tvalue->pdata->type == t_int)
	{
		if (tvalue->pdata->typedata == S4G_GC_TYPE_DATA_PRIVATE)
		{
			tvalue2 = gc->cr_val2(tvalue, S4G_GC_TYPE_VAR_FREE, S4G_GC_TYPE_DATA_FREE, true);
			execute.push_r(tvalue2);
			--(tvalue->pdata->data.i);
		}
		else
		{
			--(tvalue->pdata->ref);

			tvalue2 = gc->cr_val2(tvalue);
			--(tvalue2->pdata->ref);
			execute.push_r(tvalue2);

			tvalue->pdata = gc->cr_dara_int((tvalue->pdata->data.i) - 1, S4G_GC_TYPE_DATA_PRIVATE);
			tvalue->pdata->ref = 1;
		}
	}
	else if (tvalue->pdata->type == t_uint)
	{
		if (tvalue->pdata->typedata == S4G_GC_TYPE_DATA_PRIVATE)
		{
			tvalue2 = gc->cr_val2(tvalue, S4G_GC_TYPE_VAR_FREE, S4G_GC_TYPE_DATA_FREE, true);
			execute.push_r(tvalue2);
			--(tvalue->pdata->data.ui);
		}
		else
		{
			--(tvalue->pdata->ref);

			tvalue2 = gc->cr_val2(tvalue);
			--(tvalue2->pdata->ref);
			execute.push_r(tvalue2);

			tvalue->pdata = gc->cr_dara_uint((tvalue->pdata->data.ui) - 1, S4G_GC_TYPE_DATA_PRIVATE);
			tvalue->pdata->ref = 1;
		}
	}
	else if (tvalue->pdata->type == t_float)
	{
		if (tvalue->pdata->typedata == S4G_GC_TYPE_DATA_PRIVATE)
		{
			tvalue2 = gc->cr_val2(tvalue, S4G_GC_TYPE_VAR_FREE, S4G_GC_TYPE_DATA_FREE, true);
			execute.push_r(tvalue2);
			--(tvalue->pdata->data.f);
		}
		else
		{
			--(tvalue->pdata->ref);

			tvalue2 = gc->cr_val2(tvalue);
			--(tvalue2->pdata->ref);
			execute.push_r(tvalue2);

			tvalue->pdata = gc->cr_dara_float((tvalue->pdata->data.f) - 1.0f, S4G_GC_TYPE_DATA_PRIVATE);
			tvalue->pdata->ref = 1;
		}
	}
	else
	{
		S4G_VM_OP_ARIF_INCR_DECR_ERR(tmpval);
	}
}

inline void s4g_vm::com_add()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	//execute.pop(2);
	stack_pop(execute,2);

	if (tvalue->pdata->type == t_nnull)
	{
		if (tvalue2->pdata->type == t_int || tvalue2->pdata->type == t_uint || tvalue2->pdata->type == t_float)
			execute.push(tvalue2);
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
		}
	}
	else
	{
		if (tvalue->pdata->type == t_int)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push(gc->cr_val_int(tvalue->pdata->data.i + tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push(gc->cr_val_int(tvalue->pdata->data.i + tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push(gc->cr_val_int(tvalue->pdata->data.i + tvalue2->pdata->data.f));
			else if (tvalue2->pdata->type == t_string)
			{
				s4g_int num2;
				sscanf(gc->get_str(tvalue2), "%i", &num2);
				execute.push(gc->cr_val_int(tvalue->pdata->data.i + num2));
			}
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_uint)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push(gc->cr_val_uint(tvalue->pdata->data.ui + tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push(gc->cr_val_uint(tvalue->pdata->data.ui + tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push(gc->cr_val_uint(tvalue->pdata->data.ui + tvalue2->pdata->data.f));
			else if (tvalue2->pdata->type == t_string)
			{
				s4g_uint num2;
				sscanf(gc->get_str(tvalue2), "%u", &num2);
				execute.push(gc->cr_val_uint(tvalue->pdata->data.ui + num2));
			}
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_float)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push(gc->cr_val_float(tvalue->pdata->data.f + tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push(gc->cr_val_float(tvalue->pdata->data.f + tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push(gc->cr_val_float(tvalue->pdata->data.f + tvalue2->pdata->data.f));
			else if (tvalue2->pdata->type == t_string)
			{
				s4g_float num2;
				sscanf(gc->get_str(tvalue2), "%f", &num2);
				execute.push(gc->cr_val_float(tvalue->pdata->data.f + num2));
			}
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_string)
		{
			String str = gc->get_str(tvalue);

			if (tvalue2->pdata->type == t_int)
			{
				execute.push(gc->cr_val_str((str + tvalue2->pdata->data.i).c_str()));
			}
			else if (tvalue2->pdata->type == t_uint)
				execute.push(gc->cr_val_str((str + tvalue2->pdata->data.ui).c_str()));
			else if (tvalue2->pdata->type == t_float)
				execute.push(gc->cr_val_str((str + tvalue2->pdata->data.f).c_str()));
			else if (tvalue2->pdata->type == t_string)
				execute.push(gc->cr_val_str((str + gc->get_str(tvalue2)).c_str()));
			else if (tvalue2->pdata->type == t_bool)
				execute.push(gc->cr_val_str((str + tvalue2->pdata->data.b).c_str()));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE1(tvalue);
		}
	}
}

inline void s4g_vm::com_sub()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	//execute.pop(2);
	stack_pop(execute, 2);

	if (tvalue->pdata->type == t_nnull)
	{
		if (tvalue2->pdata->type == t_int)
			execute.push(gc->cr_val_int(-tvalue2->pdata->data.i));
		else if (tvalue2->pdata->type == t_float)
			execute.push(gc->cr_val_float(-tvalue2->pdata->data.f));
		else if (tvalue2->pdata->type == t_uint)
		{
			S4G_VM_OP_ARIF_ERROR_UNSUN_UNRES(tvalue2);
		}
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
		}
	}
	else
	{
		if (tvalue->pdata->type == t_int)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push(gc->cr_val_int(tvalue->pdata->data.i - tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push(gc->cr_val_int(tvalue->pdata->data.i - tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push(gc->cr_val_int(tvalue->pdata->data.i - tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_uint)
		{
			if (tvalue2->pdata->type == t_int)
			{
				if (tvalue2->pdata->data.ui > tvalue->pdata->data.ui)
					execute.push(gc->cr_val_int(tvalue->pdata->data.ui - tvalue2->pdata->data.ui));
				else
					execute.push(gc->cr_val_uint(tvalue->pdata->data.ui - tvalue2->pdata->data.ui));
			}
			else if (tvalue2->pdata->type == t_uint)
			{
				if (tvalue2->pdata->data.ui > tvalue->pdata->data.ui)
					execute.push(gc->cr_val_int(tvalue->pdata->data.ui - tvalue2->pdata->data.ui));
				else
					execute.push(gc->cr_val_uint(tvalue->pdata->data.ui - tvalue2->pdata->data.ui));
			}
			else if (tvalue2->pdata->type == t_float)
			{
				if (tvalue2->pdata->data.ui > tvalue->pdata->data.ui)
					execute.push(gc->cr_val_int(tvalue->pdata->data.ui - tvalue2->pdata->data.ui));
				else
					execute.push(gc->cr_val_uint(tvalue->pdata->data.ui - tvalue2->pdata->data.ui));
			}
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_float)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push(gc->cr_val_float(tvalue->pdata->data.f - tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push(gc->cr_val_float(tvalue->pdata->data.f - tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push(gc->cr_val_float(tvalue->pdata->data.f - tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE1(tvalue);
		}
	}
}

inline void s4g_vm::com_mul()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	//execute.pop(2);
	stack_pop(execute, 2);

		if (tvalue->pdata->type == t_int)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push(gc->cr_val_int(tvalue->pdata->data.i * tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push(gc->cr_val_int(tvalue->pdata->data.i * tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push(gc->cr_val_int(tvalue->pdata->data.i * tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_uint)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push(gc->cr_val_uint(tvalue->pdata->data.ui * tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push(gc->cr_val_uint(tvalue->pdata->data.ui * tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push(gc->cr_val_uint(tvalue->pdata->data.ui * tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_float)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push(gc->cr_val_float(tvalue->pdata->data.f * tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push(gc->cr_val_float(tvalue->pdata->data.f * tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push(gc->cr_val_float(tvalue->pdata->data.f * tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE1(tvalue);
		}
}

inline void s4g_vm::com_div()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	//execute.pop(2);
	stack_pop(execute, 2);

	if (tvalue->pdata->type == t_int)
	{
		if (tvalue2->pdata->type == t_int)
			execute.push(gc->cr_val_int(tvalue->pdata->data.i / tvalue2->pdata->data.i));
		else if (tvalue2->pdata->type == t_uint)
			execute.push(gc->cr_val_int(tvalue->pdata->data.i / tvalue2->pdata->data.ui));
		else if (tvalue2->pdata->type == t_float)
			execute.push(gc->cr_val_int(tvalue->pdata->data.i / tvalue2->pdata->data.f));
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
		}
	}

	else if (tvalue->pdata->type == t_uint)
	{
		if (tvalue2->pdata->type == t_int)
			execute.push(gc->cr_val_uint(tvalue->pdata->data.ui / tvalue2->pdata->data.i));
		else if (tvalue2->pdata->type == t_uint)
			execute.push(gc->cr_val_uint(tvalue->pdata->data.ui / tvalue2->pdata->data.ui));
		else if (tvalue2->pdata->type == t_float)
			execute.push(gc->cr_val_uint(tvalue->pdata->data.ui / tvalue2->pdata->data.f));
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
		}
	}

	else if (tvalue->pdata->type == t_float)
	{
		if (tvalue2->pdata->type == t_int)
			execute.push(gc->cr_val_float(tvalue->pdata->data.f / tvalue2->pdata->data.i));
		else if (tvalue2->pdata->type == t_uint)
			execute.push(gc->cr_val_float(tvalue->pdata->data.f / tvalue2->pdata->data.ui));
		else if (tvalue2->pdata->type == t_float)
			execute.push(gc->cr_val_float(tvalue->pdata->data.f / tvalue2->pdata->data.f));
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
		}
	}
	else
	{
		S4G_VM_OP_ARIF_ERROR_TYPE1(tvalue);
	}
}

inline void s4g_vm::com_halt()
{
	//если есть предыдущее состояние
	if (callstack.count_obj > 0)
	{
		//возвращаем его
		com_retprev();
	}
	else
		runexe = false;	//останавливаем выполнение
}

inline void s4g_vm::com_push()
{
	if (cfetchget == 1)
		--cfetchget;

	execute.push(arg);
}

inline void s4g_vm::com_pop()
{
	//execute.pop(1);
	stack_pop(execute, 1);
}

inline void s4g_vm::com_retprev()
{
	tmpcd = callstack.get(callstack.count_obj - 1);
	//возвращаем предыдущее состояние машины, до момента вызова скриптовой функции
	curr_comm = tmpcd->coms;
	curr_vars = tmpcd->vars;
	cfetchget = tmpcd->cfetchget;
	cfetchgetarg = tmpcd->cfetchgetarg;
	cfetchpushstore = tmpcd->cfetchpushstore;
	id_curr_com = tmpcd->id_curr_com;

	gc->del_top_context(true);
	//убираем контексты функции
	if (tmpcd->idexternctx != -1)
	{
		gc->remove_context(tmpcd->idexternctx);
		gc->del_top_context(false);
	}

	gc->activate_prev(tmpcd->lastidctx);

	//если указано что нужно проконтролировать количество возвращаемых значений
	//оно должно быть одним
	if (tmpcd->stack_size != -1)
	{
		//если количество возвращенных значений больше 1
		if (tmpcd->stack_size + 1 < execute.count_obj)
		{
			stack_pop(execute, execute.count_obj - (tmpcd->stack_size + 1));	//убираем все лишние
		}
		//иначе если ничего не вернули
		else if (tmpcd->stack_size >= execute.count_obj)
			execute.push(gc->get_val_null());	//добавляем null значение
	}

	//callstack.pop(1);	//удаляем предыдущее состояние ибо оно стало текущим
	stack_pop(callstack, 1);
	sr.free_last_unfree();

	if (!curr_comm)
		runexe = false;
}

inline void s4g_vm::com_jz()
{
	tmpval = execute.get(execute.count_obj - 1);

	stack_pop(execute, 1);
	jmp = false;
	switch (tmpval->pdata->type)
	{
	case t_null:
		jmp = true;
		break;
	case t_int:
		jmp = tmpval->pdata->data.i == 0;// ->get_int(tmpval) == 0;
		break;
	case t_uint:
		jmp = tmpval->pdata->data.ui == 0;//gc->get_uint(tmpval) == 0;
		break;
	case t_float:
		jmp = tmpval->pdata->data.f == 0.0f;//gc->get_float(tmpval) == 0.0f;
		break;
	case t_bool:
		jmp = !tmpval->pdata->data.b;//!gc->get_bool(tmpval);
		break;
	case t_pdata:
		jmp = tmpval->pdata->data.p == NULL;//gc->get_pdata(tmpval) == NULL;
		break;
	case t_string:
		jmp = (*(String*)(tmpval->pdata->data.p))[0] == 0;//gc->get_str(tmpval)[0] == 0;
		break;
	default:
		{
			s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
			error = -1;
			char strtype[12];
			s4g_get_str_type(tmpval->pdata->type, strtype);
			sprintf(this->strerror, "[%s]:%d - '%s' expected boolable but got '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str, strtype);
		}
	}
	if(jmp)
	{
		id_curr_com += (long)arg;
	}
}

inline void s4g_vm::com_jnz()
{
	tmpval = execute.get(execute.count_obj - 1);

	stack_pop(execute, 1);
	jmp = false;
	switch (tmpval->pdata->type)
	{
	case t_null:
		jmp = true;
		break;
	case t_int:
		jmp = tmpval->pdata->data.i == 0;//gc->get_int(tmpval) == 0;
		break;
	case t_uint:
		jmp = tmpval->pdata->data.ui == 0;//gc->get_uint(tmpval) == 0;
		break;
	case t_float:
		jmp = tmpval->pdata->data.f == 0.0f;//gc->get_float(tmpval) == 0.0f;
		break;
	case t_bool:
		jmp = !tmpval->pdata->data.b;//!gc->get_bool(tmpval);
		break;
	case t_pdata:
		jmp = tmpval->pdata->data.p == NULL;//gc->get_pdata(tmpval) == NULL;
		break;
	case t_string:
		jmp = (*(String*)(tmpval->pdata->data.p))[0] == 0;//gc->get_str(tmpval)[0] == 0;
		break;
	default:
		{
			s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com - 1).lexid);
			error = -1;
			char strtype[12];
			s4g_get_str_type(ttype, strtype);
			sprintf(this->strerror, "[%s]:%d - '%s' expected boolable but got '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str, strtype);
		}
	}
	if(!jmp)
	{
		id_curr_com += (long)arg;
	}
}

inline void s4g_vm::com_jmp()
{
	id_curr_com += (long)arg;
}

inline void s4g_vm::com_mod()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	//execute.pop(2);
	stack_pop(execute, 2);

		if (tvalue->pdata->type == t_int)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push(gc->cr_val_int(tvalue->pdata->data.i % tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push(gc->cr_val_int(tvalue->pdata->data.i % tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push(gc->cr_val_int(tvalue->pdata->data.i % (s4g_int)tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}
		else if (tvalue->pdata->type == t_uint)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push(gc->cr_val_uint(tvalue->pdata->data.ui % tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push(gc->cr_val_uint(tvalue->pdata->data.ui % tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push(gc->cr_val_uint(tvalue->pdata->data.ui % (s4g_int)tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE1(tvalue);
		}
	}

inline void s4g_vm::com_log_and()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	//execute.pop(2);
	stack_pop(execute, 1);

	switch (tvalue->pdata->type)
	{
	case t_null:
		execute.push_r(gc->get_bool(false));
		return;
	case t_int:
		if (tvalue->pdata->data.i == 0)
		{
			execute.push_r(gc->get_bool(false));
			return;
		}
		break;
	case t_uint:
		if (tvalue->pdata->data.ui == 0)
		{
			execute.push_r(gc->get_bool(false));
			return;
		}
		break;
	case t_float:
		if (tvalue->pdata->data.f == 0.0f)
		{
			execute.push_r(gc->get_bool(false));
			return;
		}
		break;
	case t_bool:
		if (!tvalue->pdata->data.b)
		{
			execute.push_r(gc->get_bool(false));
			return;
		}
		break;
	case t_string:
		if ((*(String*)(tvalue->pdata->data.p))[0] == 0)
		{
			execute.push_r(gc->get_bool(false));
			return;
		}
		break;
	}
	switch (tvalue2->pdata->type)
	{
	case t_null:
		execute.push(gc->get_bool(false));
		return;
	case t_int:
		if (tvalue2->pdata->data.i == 0)
		{
			execute.push_r(gc->get_bool(false));
			return;
		}
		break;
	case t_uint:
		if (tvalue2->pdata->data.ui == 0)
		{
			execute.push_r(gc->get_bool(false));
			return;
		}
		break;
	case t_float:
		if (tvalue2->pdata->data.f == 0.0f)
		{
			execute.push_r(gc->get_bool(false));
			return;
		}
		break;
	case t_bool:
		if (!tvalue2->pdata->data.b)
		{
			execute.push_r(gc->get_bool(false));
			return;
		}
		break;
	case t_string:
		if ((*(String*)(tvalue2->pdata->data.p))[0] == 0)
		{
			execute.push_r(gc->get_bool(false));
			return;
		}
		break;
	}
	execute.push_r(gc->get_bool(true));
}

inline void s4g_vm::com_log_or()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	//execute.pop(2);
	stack_pop(execute, 1);

	switch (tvalue->pdata->type)
	{
	case t_int:
		if (tvalue->pdata->data.i != 0)
		{
			execute.push_r(gc->get_bool(true));
			return;
		}
		break;
	case t_uint:
		if (tvalue->pdata->data.ui != 0)
		{
			execute.push_r(gc->get_bool(true));
			return;
		}
		break;
	case t_float:
		if (tvalue->pdata->data.f != 0.0f)
		{
			execute.push_r(gc->get_bool(true));
			return;
		}
		break;
	case t_bool:
		if (tvalue->pdata->data.b)
		{
			execute.push_r(gc->get_bool(true));
			return;
		}
		break;
	case t_string:
		if ((*(String*)(tvalue->pdata->data.p))[0] != 0)
		{
			execute.push_r(gc->get_bool(true));
			return;
		}
		break;
	}
	switch (tvalue2->pdata->type)
	{
	case t_int:
		if (tvalue2->pdata->data.i != 0)
		{
			execute.push_r(gc->get_bool(true));
			return;
		}
		break;
	case t_uint:
		if (tvalue2->pdata->data.ui != 0)
		{
			execute.push_r(gc->get_bool(true));
			return;
		}
		break;
	case t_float:
		if (tvalue2->pdata->data.f != 0.0f)
		{
			execute.push_r(gc->get_bool(true));
			return;
		}
		break;
	case t_bool:
		if (tvalue2->pdata->data.b)
		{
			execute.push_r(gc->get_bool(true));
			return;
		}
		break;
	case t_string:
		if ((*(String*)(tvalue->pdata->data.p))[0] != 0)
		{
			execute.push_r(gc->get_bool(true));
			return;
		}
		break;
	}
	execute.push_r(gc->get_bool(false));
}

inline void s4g_vm::com_log_eq()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	//execute.pop(2);
	stack_pop(execute, 1);

	if((tvalue->pdata->type != t_null && tvalue2->pdata->type == t_null) || (tvalue->pdata->type == t_null && tvalue2->pdata->type != t_null))
	{
		execute.push_r(gc->get_bool(false));
		return;
	}

		if (tvalue->pdata->type == t_int)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i == tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i == tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(num1 == tvalue2->pdata->data.f));
			else if(tvalue2->pdata->type == t_bool)
				execute.push_r(gc->get_bool(!tvalue2->pdata->data.b == num1));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_uint)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui == tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui == tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(num1 == tvalue2->pdata->data.f));
			else if(tvalue2->pdata->type == t_bool)
				execute.push_r(gc->get_bool(!tvalue2->pdata->data.b == num1));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_float)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f == tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f == tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(num1 == tvalue2->pdata->data.f));
			else if(tvalue2->pdata->type == t_bool)
				execute.push_r(gc->get_bool(!tvalue2->pdata->data.b == num1));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}
		else if(tvalue->pdata->type == t_bool)
		{
			s4g_bool num1 = tvalue->pdata->data.b;

			if(tvalue2->pdata->type == t_bool)
				execute.push_r(gc->get_bool(!num1 == !tvalue2->pdata->data.b));
			//else if(num1)
			//	execute.push_r(gc->get_bool(false));
			else if(tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(!num1 == !tvalue2->pdata->data.i));
			else if(tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(!num1 == !tvalue2->pdata->data.ui));
			else if(tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(!num1 == !tvalue2->pdata->data.f));
		else
		{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE1(tvalue);
		}
	}

inline void s4g_vm::com_log_neq()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	//execute.pop(2);
	stack_pop(execute, 1);

		if (tvalue->pdata->type == t_int)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i != tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i != tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(num1 != tvalue2->pdata->data.f));
			else if(tvalue2->pdata->type == t_bool)
				execute.push_r(gc->get_bool(!tvalue2->pdata->data.b == !num1));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_uint)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui != tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui != tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(num1 != tvalue2->pdata->data.f));
			else if(tvalue2->pdata->type == t_bool)
				execute.push_r(gc->get_bool(!tvalue2->pdata->data.b == !num1));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_float)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f != tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f != tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(num1 != tvalue2->pdata->data.f));
			else if(tvalue2->pdata->type == t_bool)
				execute.push_r(gc->get_bool(!tvalue2->pdata->data.b == !num1));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}
		else if(tvalue->pdata->type == t_bool)
		{
			s4g_bool num1 = tvalue->pdata->data.b;

			if(tvalue2->pdata->type == t_bool)
				execute.push_r(gc->get_bool(!num1 != !tvalue2->pdata->data.b));
			//else if(num1)
			//	execute.push_r(gc->get_bool(true));
			else if(tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(!num1 != !tvalue2->pdata->data.i));
			else if(tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(!num1 != !tvalue2->pdata->data.ui));
			else if(tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(!num1 != !tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE1(tvalue);
		}
	}

inline void s4g_vm::com_log_ge()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	//execute.pop(2);
	stack_pop(execute, 1);

		if (tvalue->pdata->type == t_int)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i >= tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i >= tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i >= tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_uint)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui >= tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui >= tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui >= tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_float)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f >= tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f >= tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f >= tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE1(tvalue);
		}
	}

inline void s4g_vm::com_log_le()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	//execute.pop(2);
	stack_pop(execute, 1);

		if (tvalue->pdata->type == t_int)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i <= tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i <= tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i <= tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_uint)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui <= tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui <= tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui <= tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_float)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f <= tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f <= tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f <= tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE1(tvalue);
		}
	}

inline void s4g_vm::com_log_gt()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	//execute.pop(2);
	stack_pop(execute, 1);

		if (tvalue->pdata->type == t_int)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i > tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i > tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i > tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_uint)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui > tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui > tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui > tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_float)
		{
			s4g_float num1 = tvalue->pdata->data.f;

			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f > tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f > tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f > tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE1(tvalue);
		}
	}

inline void s4g_vm::com_log_lt()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	//execute.pop(2);
	stack_pop(execute, 1);

		if (tvalue->pdata->type == t_int)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i < tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i < tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(tvalue->pdata->data.i < tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_uint)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui < tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui < tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(tvalue->pdata->data.ui < tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}

		else if (tvalue->pdata->type == t_float)
		{
			if (tvalue2->pdata->type == t_int)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f < tvalue2->pdata->data.i));
			else if (tvalue2->pdata->type == t_uint)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f < tvalue2->pdata->data.ui));
			else if (tvalue2->pdata->type == t_float)
				execute.push_r(gc->get_bool(tvalue->pdata->data.f < tvalue2->pdata->data.f));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2(tvalue2);
			}
		}
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE1(tvalue);
		}
}

inline void s4g_vm::com_log_not()
{
	tvalue = execute.get(execute.count_obj - 1);

	stack_pop(execute, 1);

	switch(tvalue->pdata->type)
	{
	case t_null:
		execute.push_r(gc->get_bool(true));
		return;
	case t_int:
		execute.push_r(gc->get_bool(tvalue->pdata->data.i == 0));
		return;
	case t_uint:
		execute.push_r(gc->get_bool(tvalue->pdata->data.ui == 0));
		return;
	case t_float:
		execute.push_r(gc->get_bool(tvalue->pdata->data.f == 0.0f));
		return;
	case t_bool:
		execute.push_r(gc->get_bool(!tvalue->pdata->data.b));
		return;
	case t_string:
		execute.push_r(gc->get_bool((*(String*)(tvalue->pdata->data.p))[0] == 0));
		return;
	}
	execute.push_r(gc->get_bool(false));
}

inline void s4g_vm::com_bit_not()
{
	tvalue = execute.get(execute.count_obj - 1);

	stack_pop(execute, 1);

	switch(tvalue->pdata->type)
	{
	case t_int:
		execute.push_r(gc->cr_val_int(~tvalue->pdata->data.i));
		return;
	case t_uint:
		execute.push_r(gc->cr_val_uint(~tvalue->pdata->data.ui));
		return;
	case t_float:
		execute.push_r(gc->cr_val_int(~((s4g_int)tvalue->pdata->data.f)));
		return;
	}
	S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue)
	}

inline void s4g_vm::com_bit_xor()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	stack_pop(execute, 1);

	switch(tvalue->pdata->type)
	{
	case t_int:
		switch(tvalue2->pdata->type)
		{
		case t_int:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.i ^ tvalue2->pdata->data.i));
			return;
		case t_uint:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.i ^ tvalue2->pdata->data.ui));
			return;
		case t_float:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.i ^ ((s4g_int)tvalue2->pdata->data.f)));
			return;
		default:
			S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue2)
		}
		return;
	case t_uint:
		switch(tvalue2->pdata->type)
		{
		case t_int:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.ui ^ tvalue2->pdata->data.i));
			return;
		case t_uint:
			execute.push_r(gc->cr_val_uint(tvalue->pdata->data.ui ^ tvalue2->pdata->data.ui));
			return;
		case t_float:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.ui ^ ((s4g_int)tvalue2->pdata->data.f)));
			return;
		default:
			S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue2)
		}
		return;
	case t_float:
		switch(tvalue2->pdata->type)
		{
		case t_int:
			execute.push_r(gc->cr_val_int(((s4g_int)tvalue->pdata->data.f) ^ tvalue2->pdata->data.i));
			return;
		case t_uint:
			execute.push_r(gc->cr_val_int(((s4g_int)tvalue->pdata->data.f) ^ tvalue2->pdata->data.ui));
			return;
		case t_float:
			execute.push_r(gc->cr_val_int(((s4g_int)tvalue->pdata->data.f) ^ ((s4g_int)tvalue2->pdata->data.f)));
			return;
		default:
			S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue2)
		}
		return;
	}
	S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue)
}

inline void s4g_vm::com_bit_and()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	stack_pop(execute, 1);

	switch(tvalue->pdata->type)
	{
	case t_int:
		switch(tvalue2->pdata->type)
		{
		case t_int:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.i & tvalue2->pdata->data.i));
			return;
		case t_uint:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.i & tvalue2->pdata->data.ui));
			return;
		case t_float:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.i & ((s4g_int)tvalue2->pdata->data.f)));
			return;
		default:
			S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue2)
		}
		return;
	case t_uint:
		switch(tvalue2->pdata->type)
		{
		case t_int:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.ui & tvalue2->pdata->data.i));
			return;
		case t_uint:
			execute.push_r(gc->cr_val_uint(tvalue->pdata->data.ui & tvalue2->pdata->data.ui));
			return;
		case t_float:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.ui & ((s4g_int)tvalue2->pdata->data.f)));
			return;
		default:
			S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue2)
		}
		return;
	case t_float:
		switch(tvalue2->pdata->type)
		{
		case t_int:
			execute.push_r(gc->cr_val_int(((s4g_int)tvalue->pdata->data.f) & tvalue2->pdata->data.i));
			return;
		case t_uint:
			execute.push_r(gc->cr_val_int(((s4g_int)tvalue->pdata->data.f) & tvalue2->pdata->data.ui));
			return;
		case t_float:
			execute.push_r(gc->cr_val_int(((s4g_int)tvalue->pdata->data.f) & ((s4g_int)tvalue2->pdata->data.f)));
			return;
		default:
			S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue2)
		}
		return;
	}
	S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue)
}

inline void s4g_vm::com_bit_or()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	stack_pop(execute, 1);

	switch(tvalue->pdata->type)
	{
	case t_int:
		switch(tvalue2->pdata->type)
		{
		case t_int:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.i | tvalue2->pdata->data.i));
			return;
		case t_uint:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.i | tvalue2->pdata->data.ui));
			return;
		case t_float:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.i | ((s4g_int)tvalue2->pdata->data.f)));
			return;
		default:
			S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue2)
		}
		return;
	case t_uint:
		switch(tvalue2->pdata->type)
		{
		case t_int:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.ui | tvalue2->pdata->data.i));
			return;
		case t_uint:
			execute.push_r(gc->cr_val_uint(tvalue->pdata->data.ui | tvalue2->pdata->data.ui));
			return;
		case t_float:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.ui | ((s4g_int)tvalue2->pdata->data.f)));
			return;
		default:
			S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue2)
		}
		return;
	case t_float:
		switch(tvalue2->pdata->type)
		{
		case t_int:
			execute.push_r(gc->cr_val_int(((s4g_int)tvalue->pdata->data.f) | tvalue2->pdata->data.i));
			return;
		case t_uint:
			execute.push_r(gc->cr_val_int(((s4g_int)tvalue->pdata->data.f) | tvalue2->pdata->data.ui));
			return;
		case t_float:
			execute.push_r(gc->cr_val_int(((s4g_int)tvalue->pdata->data.f) | ((s4g_int)tvalue2->pdata->data.f)));
			return;
		default:
			S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue2)
		}
		return;
	}
	S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue)
}

inline void s4g_vm::com_bit_shiftr()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	stack_pop(execute, 1);

	switch(tvalue->pdata->type)
	{
	case t_int:
		switch(tvalue2->pdata->type)
		{
		case t_int:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.i >> tvalue2->pdata->data.i));
			return;
		case t_uint:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.i >> tvalue2->pdata->data.ui));
			return;
		case t_float:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.i >> ((s4g_int)tvalue2->pdata->data.f)));
			return;
		default:
			S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue2)
		}
		return;
	case t_uint:
		switch(tvalue2->pdata->type)
		{
		case t_int:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.ui >> tvalue2->pdata->data.i));
			return;
		case t_uint:
			execute.push_r(gc->cr_val_uint(tvalue->pdata->data.ui >> tvalue2->pdata->data.ui));
			return;
		case t_float:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.ui >> ((s4g_int)tvalue2->pdata->data.f)));
			return;
		default:
			S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue2)
		}
		return;
	case t_float:
		switch(tvalue2->pdata->type)
		{
		case t_int:
			execute.push_r(gc->cr_val_int(((s4g_int)tvalue->pdata->data.f) >> tvalue2->pdata->data.i));
			return;
		case t_uint:
			execute.push_r(gc->cr_val_int(((s4g_int)tvalue->pdata->data.f) >> tvalue2->pdata->data.ui));
			return;
		case t_float:
			execute.push_r(gc->cr_val_int(((s4g_int)tvalue->pdata->data.f) >> ((s4g_int)tvalue2->pdata->data.f)));
			return;
		default:
			S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue2)
		}
		return;
	}
	S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue)
}

inline void s4g_vm::com_bit_shiftl()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	stack_pop(execute, 1);

	switch(tvalue->pdata->type)
	{
	case t_int:
		switch(tvalue2->pdata->type)
		{
		case t_int:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.i << tvalue2->pdata->data.i));
			return;
		case t_uint:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.i << tvalue2->pdata->data.ui));
			return;
		case t_float:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.i << ((s4g_int)tvalue2->pdata->data.f)));
			return;
		default:
			S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue2)
		}
		return;
	case t_uint:
		switch(tvalue2->pdata->type)
		{
		case t_int:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.ui << tvalue2->pdata->data.i));
			return;
		case t_uint:
			execute.push_r(gc->cr_val_uint(tvalue->pdata->data.ui << tvalue2->pdata->data.ui));
			return;
		case t_float:
			execute.push_r(gc->cr_val_int(tvalue->pdata->data.ui << ((s4g_int)tvalue2->pdata->data.f)));
			return;
		default:
			S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue2)
		}
		return;
	case t_float:
		switch(tvalue2->pdata->type)
		{
		case t_int:
			execute.push_r(gc->cr_val_int(((s4g_int)tvalue->pdata->data.f) << tvalue2->pdata->data.i));
			return;
		case t_uint:
			execute.push_r(gc->cr_val_int(((s4g_int)tvalue->pdata->data.f) << tvalue2->pdata->data.ui));
			return;
		case t_float:
			execute.push_r(gc->cr_val_int(((s4g_int)tvalue->pdata->data.f) << ((s4g_int)tvalue2->pdata->data.f)));
			return;
		default:
			S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue2)
		}
		return;
	}
	S4G_VM_OP_ARIF_INCR_DECR_ERR(tvalue)
}

inline void s4g_vm::com_log_eqt()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	//execute.pop(2);
	stack_pop(execute, 1);

	if(tvalue->pdata->type != tvalue2->pdata->type)
	{
		execute.push_r(gc->get_bool(false));
		return;
	}

	switch(tvalue->pdata->type)
	{
	case t_int:
	case t_uint:
	case t_float:
		execute.push_r(gc->get_bool(tvalue->pdata->data.ui == tvalue2->pdata->data.ui));
		return;
	case t_bool:
		execute.push_r(gc->get_bool(!tvalue->pdata->data.b == !tvalue2->pdata->data.b));
		return;
	case t_string:
		execute.push_r(gc->get_bool(*(String*)tvalue->pdata->data.p == *(String*)tvalue2->pdata->data.p));
		return;
	}

	S4G_VM_OP_ARIF_ERROR_TYPE1(tvalue);
}

inline void s4g_vm::com_log_neqt()
{
	tvalue = execute.get(execute.count_obj - 2);
	tvalue2 = execute.get(execute.count_obj - 1);

	//execute.pop(2);
	stack_pop(execute, 1);

	if(tvalue->pdata->type != tvalue2->pdata->type)
	{
		execute.push_r(gc->get_bool(true));
		return;
	}

	switch(tvalue->pdata->type)
	{
	case t_int:
	case t_uint:
	case t_float:
		execute.push_r(gc->get_bool(tvalue->pdata->data.ui != tvalue2->pdata->data.ui));
		return;
	case t_bool:
		execute.push_r(gc->get_bool(!tvalue->pdata->data.b != !tvalue2->pdata->data.b));
		return;
	case t_string:
		execute.push_r(gc->get_bool(*(String*)tvalue->pdata->data.p != *(String*)tvalue2->pdata->data.p));
		return;
	}

	S4G_VM_OP_ARIF_ERROR_TYPE1(tvalue);
}


#define GEN_OP_STUB(op) inline void s4g_vm::com_ ## op (){}
//GEN_OP_STUB(log_neqt)
//GEN_OP_STUB(log_eqt)
#undef GEN_OP_STUB
///////

int s4g_vm::run(s4g_stack<s4g_command>* commands, s4g_table* vars)
{
	curr_comm = commands;
	id_curr_com = 0;
	curr_vars = vars;
	op = mc_halt;
	arg = 0;
	cfetchget = 3;
	cfetchgetarg = false;
	runexe = true;
	val_end = -1;
	currCom = 0;
		while (runexe && id_curr_com < curr_comm->count())
		{
			currCom = &(curr_comm->get(id_curr_com));
			op = currCom->command;
			arg = currCom->arg;
			
			(this->*(arropf[op]))();
			if (error < 0)
				return -1;
			++id_curr_com;
			++cfetchpushstore;
			++cfetchget;
			oldop = op;
		}

	//обнуляем текущую таблицу для записи и стек с командами
	curr_vars = 0;
	curr_comm = 0;
	id_curr_com = -1;
	cfetchget = 3;
	cfetchgetarg = false;
	cfetchpushstore = 0;

	if (sr.count() > 0)
	{
		com_retprev();
	}
	
		/*if (gvars->is_exists_s("ttable") != -1)
		{
			s4g_value* tvar1 = gvars->gets("ttable");
			s4g_type type1 = gc->get_type(tvar1);
			
			if (gc->get_type(tvar1) == t_table)
			{
				s4g_table* ttable = gc->get_table(tvar1);
				s4g_value* t1 = ttable->gets("1");
				//long num = gc->get_int(tvar1);
				int qwert = 0;
			}
		}*/
	
	return error;
}

const char* s4g_vm::stack_trace()
{
	strstacktrace = "";
	char tmpplace[S4G_MAX_CALL+2];
	sprintf(tmpplace, "");
	char tmpfile[S4G_MAX_LEN_STR_IN_FILE];
	char tmpstr[S4G_MAX_LEN_TYPE_NAME];
		for (int i = 0; i < callstack.count_obj; i++)
		{
			if (callstack[i]->coms)
			{
				s4g_lexeme* tmplexs = this->arr_lex->get(callstack[i]->coms->get(callstack[i]->id_curr_com).lexid - 1);
				strcpy(tmpfile, arr_lex->ArrFiles[tmplexs->fileid].c_str());
				sprintf(tmpstr, "%d", tmplexs->numstr);
			}
			else
			{
				strcpy(tmpfile, "#user");
				sprintf(tmpstr, "#user");
			}

			strstacktrace += tmpplace;
			strstacktrace += callstack[i]->valf->name;
			strstacktrace += "[";
			strstacktrace += tmpfile;
			strstacktrace += ":";
			strstacktrace += tmpstr;
			strstacktrace += "]\n";
			//sprintf(str + strlen(str), "%s%s\n", tmpplace, callstack[i]->namef);
			sprintf(tmpplace + strlen(tmpplace), " ");
		}

	return strstacktrace.c_str();
}

const char* s4g_vm::get_curr_file(char* str)
{
		if (!curr_comm && id_curr_com <= -1)
		{
			if (str)
				sprintf(str, "#endexe");
			return "#endexe";
		}

		if (curr_comm)
		{
			s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid - 1);
			if (str)
				strcpy(str, arr_lex->ArrFiles[tmplexs->fileid].c_str());

			return arr_lex->ArrFiles[tmplexs->fileid].c_str();
		}
		else
		{
			if (str)
				strcpy(str, "#user");

			return "#user";
		}
}

long s4g_vm::get_curr_str(char* str)
{
		if (!curr_comm && id_curr_com <= -1)
		{
			if (str)
				sprintf(str, "#endexe");
			return -1;
		}

		if (curr_comm)
		{
			s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid - 1);
			if (str)
				sprintf(str, "%d", tmplexs->numstr);
			return tmplexs->numstr;
		}
		else
		{
			if (str)
				sprintf(str, "#user");
			return -1;
		}
}

const char* s4g_vm::get_curr_func(char* str)
{
	if (callstack.count_obj > 0)
	{
		if (str)
			strcpy(str, callstack[-1]->valf->name);
		return callstack[-1]->valf->name;
	}
	else
	{
		if (str)
			strcpy(str, "#nonefunc");
		return "#nonefunc";
	}
}