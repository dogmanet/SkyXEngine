
#include <s4g\s4g_vm.h>

inline int s4g_vm::com_fetch(s4g_value* val, bool is_cr)
{
		if(val)
		{
			const char* str = gc->get_str(val);
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
							if (is_cr)
							{
								error = -1;
								s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
								sprintf(this->strerror, "[%s]:%d - value '%s' is exists", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
								return error;
							}
							//s4g_value* tmpval = curr_vars->gets(str);
							s4g_type tt1 = gc->get_type(tmpval);
							execute.push(tmpval);
						}
						else
						{
							if (!is_cr)
							{
								error = -1;
								s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
								sprintf(this->strerror, "[%s]:%d - value '%s' is not exists", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
								return error;
							}


							tmpval = gc->cr_val_null(val->nlexid);
							curr_vars->add_val_s(str, tmpval, val->nlexid);
							//s4g_value* tmpval2 = curr_vars->gets(str);
							execute.push(tmpval);
						}
				}
		}
		else
		{
			error = -1;
			s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
			sprintf(this->strerror, "[%s]:%d - error of vm, fetch without arg", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
			return error;
		}
	return 0;
}

inline int s4g_vm::com_fetch_get(s4g_value* val, bool is_cr)
{
	int counttop = 1;
		if(val)
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
					return error;
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
			return error;
			//ttable = curr_vars;
		}
			
	s4g_value* tval = 0;

		if(val)
			tval = val;
		else
		{
			tval = execute.get(execute.count());
			execute.pop(1);
		}

		//если предыдущей командой было либо fetch либо fetch_get
		//то значит мы ложили на стек таблицу, а теперь берем значение из этой таблицы и саму таблицу выталкиваем из стека
		//ложа на вершину значение из таблицы
		if(cfetchget)
			execute.pop(1);

		if (gc->get_type(tval) == t_string)
		{
			const char* str = gc->get_str(tval);
				if(ttable->is_exists_s(str))
				{
					if (is_cr)
					{
						error = -1;
						s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
						sprintf(this->strerror, "[%s]:%d - key number '%s' is exists in table", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return error;
					}
					s4g_value* tmpval = ttable->gets(str);
					execute.push(tmpval);
				}
				else
				{
					if (!is_cr)
					{
						error = -1;
						s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
						sprintf(this->strerror, "[%s]:%d - key number '%s' is not exists in table", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return error;
					}
					s4g_value* tmpval = gc->cr_val_null(tval->nlexid);
					ttable->add_val_s(str, tmpval, tval->nlexid);
					//ttable->add_null_s(str, tval->nlexid);
					//s4g_value* tval2 = ttable->gets(str);
					execute.push((ttable->gets(str)));
				}
		}
		else if (gc->get_type(tval) == t_int)
		{
			long num = gc->get_int(tval);
				if(ttable->is_exists_n(num))
				{
					if (is_cr)
					{
						error = -1;
						s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
						sprintf(this->strerror, "[%s]:%d - key number %d is exists in table", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, num);
						return error;
					}
					s4g_value* tmpval = ttable->getn(num);
					execute.push(tmpval);
				}
				else
				{
					if (!is_cr)
					{
						error = -1;
						s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
						sprintf(this->strerror, "[%s]:%d - key number %d is not exists in table", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, num);
						return error;
					}
					s4g_value* tmpval = gc->cr_val_null(tval->nlexid);
					ttable->add_val_n(num, tmpval, tval->nlexid);
					//int qwert = 0;
					//ttable->add_null_n(num, tval->nlexid);
					//s4g_value* tval2 = ttable->getn(num);
					execute.push((ttable->getn(num)));
				}
		}
		else
		{
			error = -1;
			s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
			char strtype[12];
			s4g_get_str_type(gc->get_type(tval), strtype);
			sprintf(this->strerror, "[%s]:%d - data type '%s' is unresolved address in table", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str, strtype);
			return error;
		}
	return 0;
}

s4g_s_function* sf1212 = 0;

inline int s4g_vm::com_store()
{
	s4g_value* tvalue = execute.get(execute.count());
	s4g_value* tvalue2 = execute.get(execute.count()-1);
	
	if (gc->get_type(tvalue) == t_sfunc)
	{
		s4g_s_function* sf = gc->get_s_func(tvalue);
		if (sf->externs_strs.count() > 0 && !sf->externs)
		{
			sf1212 = sf;
			
			sf->externs = new s4g_table();

			for (int i = 1; i <= sf->externs_strs.count(); i++)
			{
				const char* str = sf->externs_strs.get(i).c_str();
				if (curr_vars->is_exists_s(str))
				{
					s4g_value* tmpval = gc->cr_val_null(-2);
					
					gc->c_val(tmpval, curr_vars->gets(str));
					tmpval->typedata = 1;
					gc->set_td_data(tmpval,1);

					sf->externs->add_val_s(str, tmpval, -2);
				}
			}
			int qwert = 0;
		}
		else if (sf->externs_strs.count() > 0 && sf->externs)
		{
			int qwert = 0;
		}
	}

	gc->c_val(tvalue2, tvalue);
	
	if (cfetchpushstore == 2)
		execute.pop(1);
	else
		execute.pop(2);

		if(cfetchgetarg)
		{
			execute.pop(2);
			cfetchgetarg = false;
		}
	return 0;
}

inline int s4g_vm::com_mstore(s4g_value* arg, int val_end)
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

			s4g_type t1 = gc->get_type(s1);
			s4g_type t2 = gc->get_type(s2);

			gc->c_val(s1,s2);
		}
		else
			break;

		keyval++;
		countvar--;
	}


	execute.pop(execute.count() - val_end);
	return 0;
}

inline int s4g_vm::com_add_in_table()
{
	s4g_table* tt = gc->get_table(execute.get(execute.count() - 1));
	s4g_value* val = (execute.get(execute.count()));
	tt->add_val(val, val->nlexid);
	return 0;
}

inline int s4g_vm::com_call(s4g_value* val)
{
	long countarg = gc->get_int(val);	//получаем количество аргументов
	s4g_value* tvalfunc = execute.get(execute.count()-countarg);	//ниже по стеку лежит переменная ссылающаяся на функцию
	s4g_type ttype = gc->get_type(tvalfunc);
		//если тип скриптовая функция
		if (gc->get_type(tvalfunc) == t_sfunc)
		{
			long lastidctx = gc->deactivate_prev();	//деактивируем все активные возможные предыдущие контексты
			s4g_table* new_ctx = 0;
			long idnewctx = gc->add_new_context(&new_ctx);	//создаем новый контекст
			long idexternctx = -1;
			
			s4g_s_function* csfunc = gc->get_s_func(execute.get(execute.count() - countarg));

			//если у нас есть подставляемые значения из другого констекста
			if (csfunc->externs)
				idexternctx = gc->add_context(csfunc->externs);	//то устанавливаем контекст

			s4g_value* tval = 0;
			s4g_value* tval2 = 0;
				//если аргументы есть
				if(countarg > 0)
				{
					long tmpargs = countarg;
					if (countarg > csfunc->args.count())
						tmpargs = csfunc->args.count();

						//записываем на сколько хватает аргументов
						for (int i = 0; i<tmpargs; i++)
						{
							tval2 = gc->cr_val_null(-3);
							tval = execute.get(execute.count()-((countarg-i)-1));
							gc->c_val(tval2, tval, false);
							new_ctx->add_val_s(csfunc->args.get(i + 1).c_str(), tval2, tval2->nlexid);
						}

						//если есть еще аргументы и у нас следущий аргмент это мультиаргумент
						if (countarg > tmpargs && csfunc->ismarg)
						{
							s4g_table* tablemarg = new s4g_table();
								for (int i = tmpargs; i<countarg; i++)
								{
									tval2 = gc->cr_val_null(-3);
									tval = execute.get(execute.count() - ((countarg - i) - 1));
									gc->c_val(tval2, tval,false);
									tablemarg->add_val(tval2, tval2->nlexid);
								}
								s4g_value* tmarg = gc->cr_val_table(tablemarg, -3);
							new_ctx->add_val_s(S4G_MARG, tmarg, -3);
						}
				}
			execute.pop(countarg+1); // выталкиваем из стека все что относилось к функции

			if (curr_comm)
			{
				//записываем в стек вызовов текущий вызов и сохранияем текущее состояние
				s4g_command commm = curr_comm->get(id_curr_com - 1);
				s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com - 1).lexid - 1);
				callstack.push(s4g_call_data(curr_comm, curr_vars, cfetchget, cfetchgetarg, cfetchpushstore, id_curr_com, lastidctx, idnewctx, idexternctx, tmplexs->str));
			}

			//устанавилваем новое окружение и новые конмады
			curr_vars = new_ctx;
			curr_comm = &(csfunc->commands);

			cfetchget = false;
			cfetchgetarg = false;
			cfetchpushstore = 0;
			id_curr_com = 1;
		}
		//иначе если у нас с(++) функция
		else if (gc->get_type(tvalfunc) == t_cfunc)
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
			return error;
		}
	return 0;
}

inline int s4g_vm::com_new_table(s4g_value* val)
{
	long tmpnum = -3;
		if (val)
			tmpnum = val->nlexid;
		s4g_value* ttable = gc->cr_val_table_null(tmpnum);
		execute.push(ttable);
	return 0;
}

inline int s4g_vm::com_add()
{
	s4g_value* s1 = execute.get(execute.count() - 1);
	s4g_value* s2 = execute.get(execute.count());
	execute.pop(2);
	if (gc->get_type(s1) == t_nnull)
	{
		if (gc->get_type(s2) == t_int)
		{
			long num2 = gc->get_int(s2);
			execute.push(gc->cr_val_int(num2, s2->nlexid));
		}
	}
	else
	{
		long num1 = gc->get_int(s1);
		long num2 = gc->get_int(s2);
		execute.push(gc->cr_val_int(num1 + num2, s1->nlexid));
	}
	return 0;
}

inline int s4g_vm::com_sub()
{
	s4g_value* s1 = execute.get(execute.count() - 1);
	s4g_value* s2 = execute.get(execute.count());
	execute.pop(2);
	if (gc->get_type(s1) == t_nnull)
	{
		if (gc->get_type(s2) == t_int)
		{
			long num2 = gc->get_int(s2);
			execute.push(gc->cr_val_int(-num2, s2->nlexid));
		}
	}
	else
	{
		long num1 = gc->get_int(s1);
		long num2 = gc->get_int(s2);
		execute.push(gc->cr_val_int(num1 - num2, s1->nlexid));
	}
	return 0;
}

inline int s4g_vm::com_mul()
{
	s4g_value* s1 = execute.get(execute.count() - 1);
	s4g_value* s2 = execute.get(execute.count());
	long num1 = gc->get_int(execute.get(execute.count() - 1));
	long num2 = gc->get_int(execute.get(execute.count() - 0));
	execute.pop(2);
	execute.push(gc->cr_val_int(num1 * num2, s1->nlexid));
	return 0;
}

inline int s4g_vm::com_div()
{
	s4g_value* s1 = execute.get(execute.count() - 1);
	s4g_value* s2 = execute.get(execute.count());
	long num1 = gc->get_int(execute.get(execute.count() - 1));
	long num2 = gc->get_int(execute.get(execute.count() - 0));
	execute.pop(2);
	execute.push(gc->cr_val_int(num1 / num2, s1->nlexid));
	return 0;
}

inline int s4g_vm::com_retprev(long* pc)
{
	int curr_count_cs = callstack.count();

	//возвращаем предыдущее состояние машины, до момента вызова скриптовой функции
	curr_comm = callstack.get(curr_count_cs).coms;
	curr_vars = callstack.get(curr_count_cs).vars;
	cfetchget = callstack.get(curr_count_cs).cfetchget;
	cfetchgetarg = callstack.get(curr_count_cs).cfetchgetarg;
	cfetchpushstore = callstack.get(curr_count_cs).cfetchpushstore;
	id_curr_com = callstack.get(curr_count_cs).id_curr_com;
	*pc = id_curr_com;

	//убираем контексты функции
	if (callstack.get(curr_count_cs).idexternctx != -1)
		gc->remove_context(callstack.get(curr_count_cs).idexternctx);
	gc->del_context(callstack.get(curr_count_cs).idnewctx);
	gc->activate_prev(callstack.get(curr_count_cs).lastidctx);

	callstack.pop(1);	//удаляем предыдущее состояние ибо оно стало текущим
	return 0;
}

///////

int s4g_vm::run(Stack<s4g_command>* commands,s4g_table* vars)
{
	curr_comm = commands;
	id_curr_com = 0;
	curr_vars = vars;
	s4g_vm_command op;
	s4g_value* arg;
	long pc = 1;
	cfetchget = false;
	cfetchgetarg = false;
	long val_end = 0;
		while (pc <= curr_comm->count())
		{
			op = curr_comm->get(pc).command;
			arg = curr_comm->get(pc).arg;
			id_curr_com = pc;
				if(op == mc_fetch_get)
				{
					if (com_fetch_get(arg, false) != 0)
						return -1;
					pc += 1;
				}
				else if (op == mc_fetch_get_cr)
				{
					if (com_fetch_get(arg, true) != 0)
						return -1;
					pc += 1;
				}
				else if(op == mc_fetch)
				{
					if (com_fetch(arg, false) != 0)
						return -1;
					pc += 1;
				}
				else if (op == mc_fetch_cr)
				{
					if (com_fetch(arg, true) != 0)
						return -1;
					pc += 1;
				}
				else if(op == mc_store)
				{
					if(com_store() != 0)
						return -1;
					pc += 1;
				}
				else if (op == mc_end)
				{
					val_end = execute.count();
					pc += 1;
				}
				else if (op == mc_mstore)
				{
					if (com_mstore(arg,val_end) != 0)
						return -1;
					pc += 1;
				}
				else if(op == mc_new_table)
				{
					if (com_new_table(arg) != 0)
						return -1;
					pc +=1;
				}
				else if(op == mc_add_in_table)
				{
					if (com_add_in_table() != 0)
						return -1;
					pc +=1;
				}
				else if(op == mc_push)
				{
					execute.push(arg);
					pc +=1;
				}
				else if(op == mc_pop)
				{
					execute.pop(1);
					pc++;
				}
				else if(op == mc_add)
				{
					if (com_add() != 0)
						return -1;
					pc++;
				}
				else if(op == mc_sub)
				{
					if (com_sub() != 0)
						return -1;
					pc++;
				}
				else if(op == mc_mul)
				{
					if (com_mul() != 0)
						return -1;
					pc++;
				}
				else if(op == mc_div)
				{
					if (com_div() != 0)
						return -1;
					pc++;
				}
				//если вызываем функцию
				else if(op == mc_call)
				{
					//если количество вызовов не превысело установленный лимит
					if (S4G_MAX_CALL > callstack.count())
					{
						pc++;
						id_curr_com = pc;
						if (com_call(arg) != 0)
							return -1;
						else
							pc = 1;
					}
					else
					{
						//иначе выдаем ошибку о превышении нормы
						error = -1;
						s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid);
						sprintf(this->strerror, "[%s]:%d - stack overflow, limit = %d", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, S4G_MAX_CALL);
						return error;
					}
					
				}
				//если команда остановки выполнения
				else if (op == mc_halt)
				{
					//если есть предыдущее состояние
					if (callstack.count() > 0)
					{
						//возвращаем его
						com_retprev(&pc);
					}
					else
						break;	//останавливаем выполнение
				}

				
				if (op == mc_fetch_get || op == mc_fetch || op == mc_fetch_get_cr || op == mc_fetch_cr)
					cfetchget = true;
				else if(op != mc_push)
					cfetchget = false;

				if ((op == mc_fetch_get || op == mc_fetch_get_cr) && !arg)
					cfetchgetarg = true;
				else if (op == mc_fetch_get || op == mc_fetch_get_cr)
					cfetchgetarg = false;

				if ((op == mc_fetch || op == mc_fetch_cr))
					cfetchpushstore++;
				else if (op == mc_push)
					cfetchpushstore++;
				else
					cfetchpushstore = 0;

				//если все команды выполнились (а такое возможно когда команд нет вообще, ибо останавливается все haltом)
				if (pc > curr_comm->count())
				{	
					//если стек содержит предыдущие состояния
					if (callstack.count() > 0)
					{
						//возвращаем их
						com_retprev(&pc);
					}
					else
						break;	//иначе останавливаем выполнение
				}
		}
	curr_vars = 0;
	curr_comm = 0;
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

		if (gvars->is_exists_s("q"))
		{
			s4g_value* tvar1 = gvars->gets("q");
			s4g_type type1 = gc->get_type(tvar1);
			if (gc->get_type(tvar1) == t_int)
			{
				long num = gc->get_int(tvar1);
				int qwert = 0;
			}
		}

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