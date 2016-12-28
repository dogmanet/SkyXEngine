
#include "s4g_vm.h"

inline void s4g_vm::com_fetch()
{
	bool is_cr = (op == mc_fetch_cr);
	cfetchpushstore = (cfetchpushstore == 0);

	const char* str = gc->get_str(arg);
		if (strcmp(str, S4G_GLOBAL_NM) == 0)
		{
			execute.push(vgvars);
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

					execute.push(tmpval);
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

					tmpval = gc->cr_val_null(str);
					curr_vars->add_val_s(str, tmpval);
					execute.push(tmpval);
				}
		}

	cfetchget = 0;
}

inline void s4g_vm::com_fetch_get()
{
	bool  is_cr = (op == mc_fetch_get_cr);
	
	int counttop = 1;
	s4g_value* tval = 0;
	if (arg)
	{
		counttop = 0;
		tval = arg;
	}
	s4g_table* ttable = 0;
		if(execute.count_obj >= 1+counttop)
		{
			s4g_type ttype = gc->get_type(execute.get((execute.count_obj - counttop) - 1));
				if(ttype == t_table)
					ttable = gc->get_table(execute.get((execute.count_obj - counttop) - 1));
				else
				{
					char tmpfile[S4G_MAX_LEN_STR_IN_FILE];
					char tmpstr[S4G_MAX_LEN_TYPE_NAME];

					s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid - 1);
					strcpy(tmpfile, arr_lex->ArrFiles[tmplexs->fileid].c_str());
					sprintf(tmpstr, "%d", tmplexs->numstr);

					error = -1;
					char strtype[S4G_MAX_LEN_TYPE_NAME];
					s4g_get_str_type(gc->get_type(tval), strtype);
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
			sprintf(this->strerror, "[%s]:%s - address to the table '%s', but the stack is empty", tmpfile, tmpstr, tval->name);
			return;
		}
			
	

		if (!arg)
		{
			tval = execute.get(execute.count_obj-1);
			//execute.pop(1);
			stack_pop(execute, 1);
		}

		//если предыдущей командой было либо fetch либо fetch_get
		//то значит мы ложили на стек таблицу, а теперь берем значение из этой таблицы и саму таблицу выталкиваем из стека
		//ложа на вершину значение из таблицы
		if (cfetchget == 1 || (oldop == mc_push && cfetchget == 2))
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

					execute.push(tmpval);
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
					
					tmpval = gc->cr_val_null(str);
					ttable->add_val_s(str, tmpval);
					execute.push(tmpval);
				}
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
	s4g_value* tvalue = execute.get(execute.count_obj - 1);
	s4g_value* tvalue2 = execute.get(execute.count_obj - 2);
	
	if (gc->get_type(tvalue) == t_sfunc)
	{
		s4g_s_function* sf = gc->get_s_func(tvalue);
		if (sf->externs_strs.count() > 0 && !sf->externs)
		{
			s4g_value* newsfval = gc->cr_val_s_func(tvalue2->name);
			s4g_s_function* newsf = gc->get_s_func(newsfval);
			newsf->args = sf->args;
			newsf->commands = sf->commands;
			newsf->externs_strs = sf->externs_strs;
			newsf->ismarg = sf->ismarg;
			newsf->externs_val = gc->cr_val_table_null();
			newsf->externs_val->typedata = 1;
			newsf->externs = gc->get_table(newsf->externs_val);
			gc->set_td_data(newsf->externs_val,1);

			long tmpid = -1;
			for (int i = 0; i <= newsf->externs_strs.count(); i++)
			{
				const char* str = newsf->externs_strs.get(i).c_str();
				if ((tmpid = curr_vars->is_exists_s(str)) != -1)
				{
					s4g_value* tmpval = gc->cr_val_null(str);
					//tmpval->isdelete = false;
					gc->c_val(tmpval, curr_vars->getn(tmpid),false);
					//tmpval->typedata = 1;
					//gc->set_td_data(tmpval,1);

					newsf->externs->add_val_s(str, tmpval);
				}
			}
			tvalue = newsfval;
		}
	}

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
	val_end = execute.count_obj-1;
}

inline void s4g_vm::com_mstore()
{
	long keyval = val_end;
	if (keyval == -1)
		keyval = 0;

	long countvar = gc->get_int(arg);
	long beginkeyvar = (execute.count_obj - (countvar - 1)) - 1;
	if (beginkeyvar - keyval > countvar)
		beginkeyvar = keyval + countvar;

	while (true)
	{
		if (keyval < beginkeyvar)
		{
			
			s4g_value* s1 = execute.get((execute.count_obj - (countvar - 1)) - 1);
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
	s4g_table* tt = gc->get_table(execute.get(execute.count_obj - 2));
	s4g_value* val = (execute.get(execute.count_obj - 1));
	tt->add_val(val);
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

	s4g_value* tvalfunc = execute.get(startpos);//в стеке лежит переменная ссылающаяся на функцию
	s4g_type ttype =  gc->get_type(tvalfunc);
		//если тип скриптовая функция
		if (ttype == t_sfunc)
		{
			s4g_s_function* csfunc = gc->get_s_func(execute.get(startpos));

			//если функция не содержит кода, либо из кода там только halt
			if (csfunc->commands.count() <= 1)
			{
				//закачиваем работу с этой функцией
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
				idexternctx = gc->add_context(csfunc->externs);	//то устанавливаем контекст
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
						for (int i = 0; i<tmpargs; ++i)
						{
							//tval2 = gc->cr_val_null();
							tval = execute.get((execute.count_obj - ((countarg - i) - 1))-1);
							//gc->c_val(tval2, tval, false);
							//const char* tmpstr = csfunc->args.get(i + 1).c_str();
							new_ctx->add_val_s(csfunc->args.get(i).c_str(), tval);
						}

						//если есть еще аргументы и у нас следущий аргмент это мультиаргумент
						if (countarg > tmpargs && csfunc->ismarg)
						{
								for (int i = tmpargs; i<countarg; ++i)
								{
									//tval2 = gc->cr_val_null();
									tval = execute.get((execute.count_obj - ((countarg - i) - 1) - 1));
									//gc->c_val(tval2, tval,false);
									csfunc->margtable->add_val(tval);
								}
							
							new_ctx->add_val_s(S4G_MARG, csfunc->marg_val);
						}
				}
			//execute.pop(countarg+1); // выталкиваем из стека все что относилось к функции
			stack_pop(execute, countarg + 1);

			//записываем в стек вызовов текущий вызов и сохранияем текущее состояние
			s4g_call_data* tmpcd = callstack.get(callstack.count_obj);
			tmpcd->coms = curr_comm;
			tmpcd->vars = curr_vars;
			tmpcd->cfetchget = cfetchget;
			tmpcd->cfetchgetarg = cfetchgetarg;
			tmpcd->cfetchpushstore = cfetchpushstore;
			tmpcd->idexternctx = idexternctx;
			tmpcd->idnewctx = idnewctx;
			tmpcd->lastidctx = lastidctx;
			tmpcd->id_curr_com = id_curr_com;
			strcpy(tmpcd->namef, tvalfunc->name);

			//устанавилваем новое окружение и новые конмады
			curr_vars = new_ctx;
			curr_comm = &(csfunc->commands);

			cfetchget = 3;
			cfetchgetarg = false;
			cfetchpushstore = 0;
			id_curr_com = -1;
		}
		//иначе если у нас с(++) функция
		else if (ttype == t_cfunc)
		{
			//CurrCountArg = countarg;
			for (int i = 0; i < countarg; i++)
			{
				s4g_value* tval = execute.get((execute.count_obj - ((countarg - i) - 1)) - 1);
				stackarg.push(tval);
			}
			s4g_c_function tcfunc = (gc->get_c_func(tvalfunc));
			stack_pop(execute, countarg + 1);

			//записываем в стек вызовов текущий вызов и сохранияем текущее состояние
			s4g_call_data* tmpcd = callstack.get(callstack.count_obj);
			tmpcd->coms = curr_comm;
			tmpcd->vars = curr_vars;
			tmpcd->cfetchget = cfetchget;
			tmpcd->cfetchgetarg = cfetchgetarg;
			tmpcd->cfetchpushstore = cfetchpushstore;
			tmpcd->idexternctx = -1;
			tmpcd->idnewctx = -1;
			tmpcd->lastidctx = -1;
			tmpcd->id_curr_com = -1;
			strcpy(tmpcd->namef, tvalfunc->name);

			error = (tcfunc)(s4gm);

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
	s4g_value* ttable = gc->cr_val_table_null();
	execute.push(ttable);
}

inline void s4g_vm::com_add()
{
	s4g_value* s1 = execute.get(execute.count_obj - 2);
	s4g_value* s2 = execute.get(execute.count_obj - 1);

	s4g_type ttype1 = gc->get_type(s1);
	s4g_type ttype2 = gc->get_type(s2);

	//execute.pop(2);
	stack_pop(execute,2);

	if (ttype1 == t_nnull)
	{
		if (ttype2 == t_int || ttype2 == t_uint || ttype2 == t_float)
			execute.push(s2);
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE2;
		}
	}
	else
	{
		if (ttype1 == t_int)
		{
			s4g_int num1 = gc->get_int(s1);

			if (ttype2 == t_int)
				execute.push(gc->cr_val_int(num1 + gc->get_int(s2)));
			else if (ttype2 == t_uint)
				execute.push(gc->cr_val_int(num1 + gc->get_uint(s2)));
			else if (ttype2 == t_float)
				execute.push(gc->cr_val_int(num1 + gc->get_float(s2)));
			else if (ttype2 == t_string)
			{
				s4g_int num2;
				sscanf(gc->get_str(s2), "%i", &num2);
				execute.push(gc->cr_val_int(num1 + num2));
			}
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2;
			}
		}

		else if (ttype1 == t_uint)
		{
			s4g_uint num1 = gc->get_uint(s1);

			if (ttype2 == t_int)
				execute.push(gc->cr_val_uint(num1 + gc->get_int(s2)));
			else if (ttype2 == t_uint)
				execute.push(gc->cr_val_uint(num1 + gc->get_uint(s2)));
			else if (ttype2 == t_float)
				execute.push(gc->cr_val_uint(num1 + gc->get_float(s2)));
			else if (ttype2 == t_string)
			{
				s4g_uint num2;
				sscanf(gc->get_str(s2), "%u", &num2);
				execute.push(gc->cr_val_uint(num1 + num2));
			}
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2;
			}
		}

		else if (ttype1 == t_float)
		{
			s4g_float num1 = gc->get_float(s1);

			if (ttype2 == t_int)
				execute.push(gc->cr_val_float(num1 + gc->get_int(s2)));
			else if (ttype2 == t_uint)
				execute.push(gc->cr_val_float(num1 + gc->get_uint(s2)));
			else if (ttype2 == t_float)
				execute.push(gc->cr_val_float(num1 + gc->get_float(s2)));
			else if (ttype2 == t_string)
			{
				s4g_float num2;
				sscanf(gc->get_str(s2), "%f", &num2);
				execute.push(gc->cr_val_float(num1 + num2));
			}
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2;
			}
		}

		else if (ttype1 == t_string)
		{
			String str = gc->get_str(s1);

			if (ttype2 == t_int)
			{
				execute.push(gc->cr_val_str((str + gc->get_int(s2)).c_str()));
			}
			else if (ttype2 == t_uint)
				execute.push(gc->cr_val_str((str + gc->get_uint(s2)).c_str()));
			else if (ttype2 == t_float)
				execute.push(gc->cr_val_str((str + gc->get_float(s2)).c_str()));
			else if (ttype2 == t_string)
				execute.push(gc->cr_val_str((str + gc->get_str(s2)).c_str()));
			else if (ttype2 == t_bool)
				execute.push(gc->cr_val_str((str + gc->get_bool(s2)).c_str()));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2;
			}
		}
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE1;
		}
	}
}

inline void s4g_vm::com_sub()
{
	s4g_value* s1 = execute.get(execute.count_obj - 2);
	s4g_value* s2 = execute.get(execute.count_obj - 1);

	s4g_type ttype1 = gc->get_type(s1);
	s4g_type ttype2 = gc->get_type(s2);

	//execute.pop(2);
	stack_pop(execute, 2);

	if (ttype1 == t_nnull)
	{
		if (ttype2 == t_int)
			execute.push(gc->cr_val_int(-gc->get_int(s2)));
		else if (ttype2 == t_float)
			execute.push(gc->cr_val_float(-gc->get_float(s2)));
		else if (ttype2 == t_uint)
		{
			S4G_VM_OP_ARIF_ERROR_UNSUN_UNRES;
		}
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE2;
		}
	}
	else
	{
		if (ttype1 == t_int)
		{
			s4g_int num1 = gc->get_int(s1);

			if (ttype2 == t_int)
				execute.push(gc->cr_val_int(num1 - gc->get_int(s2)));
			else if (ttype2 == t_uint)
				execute.push(gc->cr_val_int(num1 - gc->get_uint(s2)));
			else if (ttype2 == t_float)
				execute.push(gc->cr_val_int(num1 - gc->get_float(s2)));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2;
			}
		}

		if (ttype1 == t_uint)
		{
			s4g_uint num1 = gc->get_uint(s1);

			if (ttype2 == t_int)
			{
				s4g_int num2 = gc->get_int(s2);
				if (num2 > num1)
					execute.push(gc->cr_val_int(num1 - num2));
				else
					execute.push(gc->cr_val_uint(num1 - num2));
			}
			else if (ttype2 == t_uint)
			{
				s4g_uint num2 = gc->get_uint(s2);
				if (num2 > num1)
					execute.push(gc->cr_val_int(num1 - num2));
				else
					execute.push(gc->cr_val_uint(num1 - num2));
			}
			else if (ttype2 == t_float)
			{
				s4g_float num2 = gc->get_float(s2);
				if (num2 > num1)
					execute.push(gc->cr_val_int(num1 - num2));
				else
					execute.push(gc->cr_val_uint(num1 - num2));
			}
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2;
			}
		}

		if (ttype1 == t_float)
		{
			s4g_float num1 = gc->get_float(s1);

			if (ttype2 == t_int)
				execute.push(gc->cr_val_float(num1 - gc->get_int(s2)));
			else if (ttype2 == t_uint)
				execute.push(gc->cr_val_float(num1 - gc->get_uint(s2)));
			else if (ttype2 == t_float)
				execute.push(gc->cr_val_float(num1 - gc->get_float(s2)));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2;
			}
		}
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE1;
		}
	}
}

inline void s4g_vm::com_mul()
{
	s4g_value* s1 = execute.get(execute.count_obj - 2);
	s4g_value* s2 = execute.get(execute.count_obj - 1);

	s4g_type ttype1 = gc->get_type(s1);
	s4g_type ttype2 = gc->get_type(s2);

	//execute.pop(2);
	stack_pop(execute, 2);

		if (ttype1 == t_int)
		{
			s4g_int num1 = gc->get_int(s1);

			if (ttype2 == t_int)
				execute.push(gc->cr_val_int(num1 * gc->get_int(s2)));
			else if (ttype2 == t_uint)
				execute.push(gc->cr_val_int(num1 * gc->get_uint(s2)));
			else if (ttype2 == t_float)
				execute.push(gc->cr_val_int(num1 * gc->get_float(s2)));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2;
			}
		}

		if (ttype1 == t_uint)
		{
			s4g_uint num1 = gc->get_uint(s1);

			if (ttype2 == t_int)
				execute.push(gc->cr_val_uint(num1 * gc->get_int(s2)));
			else if (ttype2 == t_uint)
				execute.push(gc->cr_val_uint(num1 * gc->get_uint(s2)));
			else if (ttype2 == t_float)
				execute.push(gc->cr_val_uint(num1 * gc->get_float(s2)));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2;
			}
		}

		if (ttype1 == t_float)
		{
			s4g_float num1 = gc->get_float(s1);

			if (ttype2 == t_int)
				execute.push(gc->cr_val_float(num1 * gc->get_int(s2)));
			else if (ttype2 == t_uint)
				execute.push(gc->cr_val_float(num1 * gc->get_uint(s2)));
			else if (ttype2 == t_float)
				execute.push(gc->cr_val_float(num1 * gc->get_float(s2)));
			else
			{
				S4G_VM_OP_ARIF_ERROR_TYPE2;
			}
		}
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE1;
		}
}

inline void s4g_vm::com_div()
{
	s4g_value* s1 = execute.get(execute.count_obj - 2);
	s4g_value* s2 = execute.get(execute.count_obj - 1);

	s4g_type ttype1 = gc->get_type(s1);
	s4g_type ttype2 = gc->get_type(s2);

	//execute.pop(2);
	stack_pop(execute, 2);

	if (ttype1 == t_int)
	{
		s4g_int num1 = gc->get_int(s1);

		if (ttype2 == t_int)
			execute.push(gc->cr_val_int(num1 / gc->get_int(s2)));
		else if (ttype2 == t_uint)
			execute.push(gc->cr_val_int(num1 / gc->get_uint(s2)));
		else if (ttype2 == t_float)
			execute.push(gc->cr_val_int(num1 / gc->get_float(s2)));
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE2;
		}
	}

	if (ttype1 == t_uint)
	{
		s4g_uint num1 = gc->get_uint(s1);

		if (ttype2 == t_int)
			execute.push(gc->cr_val_uint(num1 / gc->get_int(s2)));
		else if (ttype2 == t_uint)
			execute.push(gc->cr_val_uint(num1 / gc->get_uint(s2)));
		else if (ttype2 == t_float)
			execute.push(gc->cr_val_uint(num1 / gc->get_float(s2)));
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE2;
		}
	}

	if (ttype1 == t_float)
	{
		s4g_float num1 = gc->get_float(s1);

		if (ttype2 == t_int)
			execute.push(gc->cr_val_float(num1 / gc->get_int(s2)));
		else if (ttype2 == t_uint)
			execute.push(gc->cr_val_float(num1 / gc->get_uint(s2)));
		else if (ttype2 == t_float)
			execute.push(gc->cr_val_float(num1 / gc->get_float(s2)));
		else
		{
			S4G_VM_OP_ARIF_ERROR_TYPE2;
		}
	}
	else
	{
		S4G_VM_OP_ARIF_ERROR_TYPE1;
	}
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
	s4g_call_data* tmpcd = callstack.get(callstack.count_obj - 1);
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
}

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

		while (runexe && id_curr_com <= curr_comm->count())
		{
			op = curr_comm->get(id_curr_com).command;
			arg = curr_comm->get(id_curr_com).arg;
			
			(this->*(arropf[op]))();
			if (error < 0)
				return -1;
			id_curr_com++;
			cfetchpushstore++;
			cfetchget++;
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

		/*if (gvars->is_exists_s("a") != -1)
		{
			s4g_value* tvar1 = gvars->gets("a");
			s4g_type type1 = gc->get_type(tvar1);
			if (gc->get_type(tvar1) == t_int)
			{
				long num = gc->get_int(tvar1);
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
			strstacktrace += callstack[i]->namef;
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
	if (callstack.count_obj > 0 && callstack[-1]->coms)
	{
		if (str)
			strcpy(str, callstack[-1]->namef);
		return callstack[-1]->namef;
	}
	else
	{
		if (str)
			strcpy(str, "#nonefunc");
		return "#nonefunc";
	}
}