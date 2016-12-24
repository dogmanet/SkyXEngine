
#include <s4g\s4g_vm.h>

inline void s4g_vm::com_fetch()
{
	bool is_cr = (op == mc_fetch_cr);
	cfetchpushstore = (cfetchpushstore == 0);

	const char* str = gc->get_str(arg);
		if (strcmp(str, S4G_GLOBAL_NM) == 0)
		{
			execute.push(vgvars);
			//stack_push(execute, vgvars);
		}
		else
		{
					s4g_value* tmpval = 0;
					long idctx = -1;
						if ((idctx = gc->ctx_is_exists_s(str, &tmpval)) != -1)
						{
							/*if (is_cr)
							{
								error = -1;
								s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
								sprintf(this->strerror, "[%s]:%d - value '%s' is exists", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
								return;
							}*/
							//s4g_value* tmpval = curr_vars->gets(str);
							//s4g_type tt1 = gc->get_type(tmpval);
							execute.push(tmpval);
							//stack_push(execute, tmpval);
						}
						else
						{
							/*if (!is_cr)
							{
								error = -1;
								s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
								sprintf(this->strerror, "[%s]:%d - value '%s' is not exists", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
								return;
							}*/


							tmpval = gc->cr_val_null();
							curr_vars->add_val_s(str, tmpval);
							//s4g_value* tmpval2 = curr_vars->gets(str);
							execute.push(tmpval);
							//stack_push(execute, tmpval);
						}
				}
		/*}
		else
		{
			error = -1;
			s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
			sprintf(this->strerror, "[%s]:%d - error of vm, fetch without arg", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
			return;
		}*/
	//return 0;

	cfetchget = 1;
}

inline void s4g_vm::com_fetch_get()
{
	bool  is_cr = (op == mc_fetch_get_cr);
	
	int counttop = 1;
		if (arg)
			counttop = 0;
	s4g_table* ttable = 0;
		if(execute.count() >= 1+counttop)
		{
			s4g_type ttype = gc->get_type(execute.get(execute.count()-counttop));
				if(ttype == t_table)
					ttable = gc->get_table(execute.get(execute.count() - counttop));
				else
				{
					error = -1;
					s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
					char strtype[12];
					s4g_get_str_type(ttype, strtype);
					sprintf(this->strerror, "[%s]:%d - value '%s' expected table but got %s", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str, strtype);
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
			error = -1;
			s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
			sprintf(this->strerror, "[%s]:%d - address to the table '%s', but the stack is empty", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
			return;
			//ttable = curr_vars;
		}
			
	s4g_value* tval = 0;

		if (arg)
			tval = arg;
		else
		{
			tval = execute.get(execute.count());
			//execute.pop(1);
			stack_pop(execute, 1);
		}

		//если предыдущей командой было либо fetch либо fetch_get
		//то значит мы ложили на стек таблицу, а теперь берем значение из этой таблицы и саму таблицу выталкиваем из стека
		//ложа на вершину значение из таблицы
		if(cfetchget == 1)
			stack_pop(execute, 1);
	s4g_value* tmpval = 0;
	s4g_type ttype = gc->get_type(tval);
		if (ttype == t_string || ttype == t_int || ttype == t_uint)
		{
			const char* str=0;
			char str2[S4G_MAX_LEN_VAR_NAME];
			if (ttype == t_string)
				str = gc->get_str(tval);
			else if (ttype == t_int)
				str = ltoa(gc->get_int(tval), str2,10);
			
				if (ttable->is_exists_s2(str, &tmpval) != -1)
				{
					/*if (is_cr)
					{
						error = -1;
						s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
						sprintf(this->strerror, "[%s]:%d - key number '%s' is exists in table", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return;
					}*/
					//s4g_value* tmpval = ttable->gets(str);
					execute.push(tmpval);
					//stack_push(execute, tmpval);
				}
				else
				{
					/*if (!is_cr)
					{
						error = -1;
						s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
						sprintf(this->strerror, "[%s]:%d - key number '%s' is not exists in table", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return;
					}*/
					
					tmpval = gc->cr_val_null(/*tval->nlexid*/);
					ttable->add_val_s(str, tmpval/*, tval->nlexid*/);
					//ttable->add_null_s(str, tval->nlexid);
					//s4g_value* tval2 = ttable->gets(str);
					execute.push(tmpval);
					//stack_push(execute, tmpval);
				}
		}
		else
		{
			error = -1;
			s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
			char strtype[12];
			s4g_get_str_type(gc->get_type(tval), strtype);
			sprintf(this->strerror, "[%s]:%d - data type '%s' is unresolved address in table", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str, strtype);
			//return error;
		}
	//return 0;

	cfetchget = 1;
	cfetchgetarg = (arg == 0);
}

inline void s4g_vm::com_store()
{
	s4g_value* tvalue = execute.get(execute.count());
	s4g_value* tvalue2 = execute.get(execute.count()-1);
	
	if (gc->get_type(tvalue) == t_sfunc)
	{
		s4g_s_function* sf = gc->get_s_func(tvalue);
		if (sf->externs_strs.count() > 0 && !sf->externs)
		{
			s4g_value* newsfval = gc->cr_val_s_func();
			s4g_s_function* newsf = gc->get_s_func(newsfval);
			newsf->args = sf->args;
			newsf->commands = sf->commands;
			if (sf->externs_strs.size() == 0)
				int qwert = 0;
			newsf->externs_strs = sf->externs_strs;
			newsf->ismarg = sf->ismarg;
			newsf->main_extern = ++sf->main_extern;
			newsf->externs_val = gc->cr_val_table_null();// new s4g_table();
			newsf->externs_val->typedata = 1;
			newsf->externs = gc->get_table(newsf->externs_val);
			gc->set_td_data(newsf->externs_val,1);

			long tmpid = -1;
			for (int i = 1; i <= newsf->externs_strs.count(); i++)
			{
				const char* str = newsf->externs_strs.get(i).c_str();
				if ((tmpid = curr_vars->is_exists_s(str)) != -1)
				{
					s4g_value* tmpval = gc->cr_val_null();
					//tmpval->isdelete = false;
					gc->c_val(tmpval, curr_vars->getn(tmpid+1),false);
					//tmpval->typedata = 1;
					//gc->set_td_data(tmpval,1);

					newsf->externs->add_val_s(str, tmpval);
				}
			}
			tvalue = newsfval;
		}
		else if (sf->externs_strs.count() > 0 && sf->externs)
		{
			int qwert = 0;
		}
	}

	//s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
	/*char strtype[12];
	s4g_get_str_type(gc->get_type(tval), strtype);
	sprintf(this->strerror, "[%s]:%d - data type '%s' is unresolved address in table", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str, strtype);
	*/

	gc->c_val(tvalue2, tvalue);
	
	if (oldop == mc_push && cfetchpushstore == 1)
	{
		//execute.pop(1);
		stack_pop(execute, 1);
	}
	else
	{
		//execute.pop(2);
		stack_pop(execute, 2);
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
	val_end = execute.count();
}

inline void s4g_vm::com_mstore()
{
	long keyval = val_end;
	if (keyval == 0)
		keyval = 1;

	long countvar = gc->get_int(arg);
	long beginkeyvar = execute.count() - (countvar - 1);
	if (beginkeyvar - keyval > countvar)
		beginkeyvar = keyval + countvar;

	while (true)
	{
		if (keyval < beginkeyvar)
		{
			
			s4g_value* s1 = execute.get(execute.count() - (countvar - 1));
			s4g_value* s2 = execute.get(keyval);

			gc->c_val(s1,s2);
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
	s4g_table* tt = gc->get_table(execute.get(execute.count() - 1));
	s4g_value* val = (execute.get(execute.count()));
	tt->add_val(val);
}

inline void s4g_vm::com_precall()
{
	sr.setn_first_free(execute.count());
}

inline void s4g_vm::com_call()
{
	//если количество вызовов не превысело установленный лимит
	if (S4G_MAX_CALL <= callstack.count())
	{
		//иначе выдаем ошибку о превышении нормы
		error = -1;
		s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
		sprintf(this->strerror, "[%s]:%d - stack overflow, limit = %d", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, S4G_MAX_CALL);
		return;
	}

	int startpos = sr.getv_last_unfree()+1;
	long countarg = execute.count_obj - startpos;	//получаем количество аргументов

	s4g_value* tvalfunc = execute.get(startpos);//в стеке лежит переменная ссылающаяся на функцию
	s4g_type ttype =  gc->get_type(tvalfunc);
		//если тип скриптовая функция
		if (ttype == t_sfunc)
		{
			s4g_s_function* csfunc = gc->get_s_func(execute.get(execute.count_obj - countarg));
			if (csfunc->commands.count() == 0)
			{
				sr.free_last_unfree();
				//execute.pop(countarg + 1); // выталкиваем из стека все что относилось к функции
				stack_pop(execute, countarg + 1);
				return;
			}
			long lastidctx = gc->deactivate_prev();	//деактивируем все активные возможные предыдущие контексты

			long idexternctx = -1;
			//если у нас есть подставляемые значения из другого констекста
			if (csfunc->externs)
			{
				idexternctx = gc->add_context(csfunc->externs);	//то устанавливаем контекст*/
			}

			s4g_table* new_ctx = 0;
			long idnewctx = gc->add_new_context(&new_ctx);	//создаем новый контекст
			
			
				//если аргументы есть
				if(countarg > 0)
				{
					s4g_value* tval = 0;
					s4g_value* tval2 = 0;

					long tmpargs = countarg;
					if (countarg > csfunc->args.count_obj)
						tmpargs = csfunc->args.count_obj;

						//записываем на сколько хватает аргументов
						for (int i = 0; i<tmpargs; i++)
						{
							tval2 = gc->cr_val_null();
							tval = execute.get(execute.count_obj - ((countarg - i) - 1));
							gc->c_val(tval2, tval, false);
							//const char* tmpstr = csfunc->args.get(i + 1).c_str();
							new_ctx->add_val_s(csfunc->args.get(i + 1).c_str(), tval2);
						}

						//если есть еще аргументы и у нас следущий аргмент это мультиаргумент
						/*if (countarg > tmpargs && csfunc->ismarg)
						{
							s4g_table* tablemarg = new s4g_table();
								for (int i = tmpargs; i<countarg; i++)
								{
									//tval2 = gc->cr_val_null();
									tval = execute.get(execute.count_obj - ((countarg - i) - 1));
									//gc->c_val(tval2, tval,false);
									tablemarg->add_val(tval);
								}
								s4g_value* tmarg = gc->cr_val_table(tablemarg);
							new_ctx->add_val_s(S4G_MARG, tmarg);
						}*/
				}
			//execute.pop(countarg+1); // выталкиваем из стека все что относилось к функции
			stack_pop(execute, countarg + 1);
			if (curr_comm)
			{
				//записываем в стек вызовов текущий вызов и сохранияем текущее состояние
				s4g_command commm = curr_comm->get(id_curr_com);
				s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid - 1);
				//callstack.push(s4g_call_data(curr_comm, curr_vars, cfetchget, cfetchgetarg, cfetchpushstore, id_curr_com, lastidctx, idnewctx, idexternctx, tmplexs->str));
				s4g_call_data* tmpcd = callstack.get((callstack.count_obj != 0 ? callstack.count_obj : 1));
				tmpcd->coms = curr_comm;
				tmpcd->vars = curr_vars;
				tmpcd->cfetchget = cfetchget;
				tmpcd->cfetchgetarg = cfetchgetarg;
				tmpcd->cfetchpushstore = cfetchpushstore;
				tmpcd->idexternctx = idexternctx;
				tmpcd->idnewctx = idnewctx;
				tmpcd->lastidctx = lastidctx;
				tmpcd->id_curr_com = id_curr_com;
				strcpy(tmpcd->namef, tmplexs->str);
			}
			else
			{
				s4g_call_data* tmpcd = callstack.get((callstack.count_obj != 0 ? callstack.count_obj : 1));
				tmpcd->coms = 0;
				tmpcd->vars = 0;
				tmpcd->cfetchget = false;
				tmpcd->cfetchgetarg = false;
				tmpcd->cfetchpushstore = 0;
				tmpcd->idexternctx = idexternctx;
				tmpcd->idnewctx = idnewctx;
				tmpcd->lastidctx = lastidctx;
				tmpcd->id_curr_com = 0;
			}

			//устанавилваем новое окружение и новые конмады
			curr_vars = new_ctx;
			curr_comm = &(csfunc->commands);

			cfetchget = false;
			cfetchgetarg = false;
			cfetchpushstore = 0;
			id_curr_com = 0;
		}
		//иначе если у нас с(++) функция
		else if (ttype == t_cfunc)
		{
			CurrCountArg = countarg;
			s4g_c_function tcfunc = (gc->get_c_func(tvalfunc));
			(tcfunc)(s4gm);
			CurrCountArg = -1;
		}
		//иначе у нас не функция
		else
		{
			//выдаем ошибку
			s4g_command commm = curr_comm->get(id_curr_com - 1);
			s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com - 1).lexid - 1);
			error = -1;
			char strtype[12];
			s4g_get_str_type(gc->get_type(tvalfunc), strtype);
			sprintf(this->strerror, "[%s]:%d - called value '%s' is not function, this is '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str, strtype);
			//return error;
		}
	//return 0;
}

inline void s4g_vm::com_new_table(/*s4g_value* val*/)
{
	s4g_value* ttable = gc->cr_val_table_null();
	execute.push(ttable);
}

inline void s4g_vm::com_add()
{
	s4g_value* s1 = execute.get(execute.count() - 1);
	s4g_value* s2 = execute.get(execute.count());
	execute.pop(2);
	if (gc->get_type(s1) == t_nnull)
	{
		if (gc->get_type(s2) == t_int)
		{
			long num2 = gc->get_int(s2);
			execute.push(gc->cr_val_int(num2/*, s2->nlexid*/));
		}
	}
	else
	{
		long num1 = gc->get_int(s1);
		long num2 = gc->get_int(s2);
		execute.push(gc->cr_val_int(num1 + num2/*, s1->nlexid*/));
	}
	//return 0;
}

inline void s4g_vm::com_sub()
{
	s4g_value* s1 = execute.get(execute.count() - 1);
	s4g_value* s2 = execute.get(execute.count());
	execute.pop(2);
	if (gc->get_type(s1) == t_nnull)
	{
		if (gc->get_type(s2) == t_int)
		{
			long num2 = gc->get_int(s2);
			execute.push(gc->cr_val_int(-num2/*, s2->nlexid*/));
		}
	}
	else
	{
		long num1 = gc->get_int(s1);
		long num2 = gc->get_int(s2);
		execute.push(gc->cr_val_int(num1 - num2/*, s1->nlexid*/));
	}
	//return 0;
}

inline void s4g_vm::com_mul()
{
	s4g_value* s1 = execute.get(execute.count() - 1);
	s4g_value* s2 = execute.get(execute.count());
	long num1 = gc->get_int(execute.get(execute.count() - 1));
	long num2 = gc->get_int(execute.get(execute.count() - 0));
	execute.pop(2);
	execute.push(gc->cr_val_int(num1 * num2/*, s1->nlexid*/));
	//return 0;
}

inline void s4g_vm::com_div()
{
	s4g_value* s1 = execute.get(execute.count() - 1);
	s4g_value* s2 = execute.get(execute.count());
	long num1 = gc->get_int(execute.get(execute.count() - 1));
	long num2 = gc->get_int(execute.get(execute.count() - 0));
	execute.pop(2);
	execute.push(gc->cr_val_int(num1 / num2/*, s1->nlexid*/));
	//return 0;
}

inline void s4g_vm::com_halt()
{
	//если есть предыдущее состояние
	if (callstack.count() > 0)
	{
		//возвращаем его
		com_retprev();
	}
	else
		runexe = false;	//останавливаем выполнение
	//return 0;
}

inline void s4g_vm::com_push()
{
	if (cfetchget == 1)
		--cfetchget;
	/*if ((oldop == mc_fetch || oldop == mc_fetch_cr) && cfetchpushstore == 0)
		cfetchpushstore++;*/
	/*else if (cfetchpushstore == 1)
		cfetchpushstore++;*/
	execute.push(arg);
	//stack_push(execute, arg);
	//return 0;
}

inline void s4g_vm::com_pop()
{
	//execute.pop(1);
	stack_pop(execute, 1);
	//return 0;
}

inline void s4g_vm::com_retprev()
{
	int curr_count_cs = callstack.count();
	s4g_call_data* tmpcd = callstack.get(curr_count_cs);
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

	callstack.pop(1);	//удаляем предыдущее состояние ибо оно стало текущим
	//stack_pop(execute, 1);
	sr.free_last_unfree();

	if (!curr_comm)
		runexe = false;
	//return 0;
}

///////

int s4g_vm::run(s4g_stack<s4g_command>* commands, s4g_table* vars)
{
	curr_comm = commands;
	id_curr_com = 1;
	curr_vars = vars;
	op = mc_halt;
	arg = 0;
	//long pc = 1;
	cfetchget = false;
	cfetchgetarg = false;
	runexe = true;
	val_end = -1;
	precall = -1;
		while (runexe && id_curr_com <= curr_comm->count())
		{
			op = curr_comm->get(id_curr_com).command;
			arg = curr_comm->get(id_curr_com).arg;
			
			/*if ((this->*(arropf[op]))() != 0)
				return -1;*/
			(this->*(arropf[op]))();
			if (error != 0)
				return -1;
			id_curr_com++;
			cfetchpushstore++;
			cfetchget++;
			oldop = op;
		}
	curr_vars = 0;
	curr_comm = 0;

	if (sr.count() > 0)
	{
		com_retprev();
	}
	//s4g_table* ttype = gc->get_table(vars->gets("ttable"));
		/*if (gvars->is_exists_s("ttable"))
		{
			s4g_table* ttype = gc->get_table(gvars->gets("ttable"));
			s4g_value* tvar0 = ttype->gets("a");
			s4g_type type0 = gc->get_type(tvar0);
			if (gc->get_type(tvar0) == t_int)
			{
				long num = gc->get_int(tvar0);
				int qwert = 0;
			}
		}*/

		/*if (gvars->is_exists_s("q"))
		{
			s4g_value* tvar1 = gvars->gets("q");
			s4g_type type1 = gc->get_type(tvar1);
			if (gc->get_type(tvar1) == t_int)
			{
				long num = gc->get_int(tvar1);
				int qwert = 0;
			}
		}*/

		/*if (gvars->is_exists_s("q2"))
		{
			s4g_value* tvar1 = gvars->gets("q2");
			s4g_type type1 = gc->get_type(tvar1);
			if (gc->get_type(tvar1) == t_int)
			{
				long num = gc->get_int(tvar1);
				int qwert = 0;
			}
		}*/

		/*if (gvars->is_exists_s("b"))
		{
			s4g_value* tvar1 = gvars->gets("b");
			if (gc->get_type(tvar1) == t_int)
			{
				long num = gc->get_int(tvar1);
				int qwert = 0;
			}
		}

		if (gvars->is_exists_s("c"))
		{
			s4g_value* tvar2 = gvars->gets("c");
			if (gc->get_type(tvar2) == t_int)
			{
				long num = gc->get_int(tvar2);
				int qwert = 0;
			}
		}*/
	
	return error;
}