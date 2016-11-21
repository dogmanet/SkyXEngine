
#include <s4g\s4g_vm.h>

inline void s4g_vm::com_fetch(s4g_value* val)
{
		if(val)
		{
			const char* str = val->get_string().c_str();
				if(strcmp(str,"_g") == 0)
					execute.push(vgvars);
				else
				{
						if(curr_vars->is_exists_s(str))
						{
							execute.push((curr_vars->gets(str)));
						}
						else
						{
							int qwert = 0;
							curr_vars->add_value(str);
							execute.push((curr_vars->gets(str)));
						}
				}
		}
}

inline void s4g_vm::com_fetch_get(s4g_value* val)
{
	int counttop = 1;
		if(val)
			counttop = 0;
	s4g_table* ttable = 0;
		if(execute.count() >= 1+counttop)
		{
			s4g_type ttype = execute.get(execute.count()-counttop)->get_type();
				if(ttype == t_table)
					ttable = execute.get(execute.count()-counttop)->get_table();
				//если предыдущей командой было либо fetch либо fetch_get
				//то значит мы ложили на стек таблицу, однако это оказалось не так
				else if(cfetchget)	//error
					int qwert = 0;
				else
					ttable = curr_vars;
		}
		else
			ttable = curr_vars;
			
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

		if(tval->get_type() == t_string)
		{
			const char* str = tval->get_string().c_str();
				if(ttable->is_exists_s(str))
				{
					execute.push((ttable->gets(str)));
				}
				else
				{
					ttable->add_value(str);
					s4g_value* tval2 = ttable->gets(str);
					execute.push((ttable->gets(str)));
				}
		}
		else if(tval->get_type() == t_int)
		{
			long num = tval->get_long();
				if(ttable->is_exists_n(num))
				{
					execute.push((ttable->getn(num)));
				}
				else
				{
					int qwert = 0;
					ttable->add_key_null(num);
					s4g_value* tval2 = ttable->getn(num);
					execute.push((ttable->getn(num)));
				}
		}
}

inline void s4g_vm::com_store()
{
	s4g_value* tvalue = execute.get(execute.count());
	s4g_value* tvalue2 = execute.get(execute.count()-1);
	
	memcpy(tvalue2,tvalue,sizeof(s4g_value));
					
	execute.pop(2);
		if(cfetchgetarg)
		{
			execute.pop(1);
			cfetchgetarg = false;
		}
}

inline void s4g_vm::com_add_in_table()
{
	s4g_table* tt = execute.get(execute.count()-1)->get_table();
	s4g_value* val = (execute.get(execute.count()));
	tt->add_value(0,val);
}

inline void s4g_vm::com_call(s4g_value* val)
{
	long countarg = val->get_long();
	s4g_value* tvalfunc = execute.get(execute.count()-countarg);
		if(tvalfunc->get_type() == t_sfunc)
		{
			s4g_s_function* csfunc = execute.get(execute.count()-countarg)->get_s_func();
			s4g_value* tval = 0;
				if(countarg > 0)
				{
						for(int i=0;i<countarg;i++)
						{
							tval = execute.get(execute.count()-((countarg-i)-1));
							csfunc->vars.add_value(csfunc->args.get(i+1).c_str(),tval);
						}
				}
			execute.pop(countarg+1);
			bool tmp_cfetchget = cfetchget;
			bool tmp_cfetchgetarg = cfetchgetarg;
			run(&(csfunc->commands),&(csfunc->vars));
			cfetchget = tmp_cfetchget;
			cfetchgetarg = tmp_cfetchgetarg;
		}
		else if(tvalfunc->get_type() == t_cfunc)
		{
			int qwert = 0;
			s4g_c_function tcfunc = (tvalfunc->get_c_func());
			(tcfunc)(this);
		}
	//!!!урегулировать рекурсию
	int qwert = 0;
}

inline void s4g_vm::com_new_table()
{
	execute.push(new s4g_value(t_table));
}

int s4g_vm::run(Stack<s4g_command>* commands,s4g_table* vars)
{
	curr_vars = vars;
	s4g_vm_command op;
	s4g_value* arg;
	long pc = 1;
	cfetchget = false;
	cfetchgetarg = false;
		while(1)
		{
			op = commands->get(pc).command;
			arg = commands->get(pc).arg;

				if(op == mc_fetch_get)
				{
					com_fetch_get(arg);
					pc += 1;
				}
				else if(op == mc_fetch)
				{
					com_fetch(arg);
					pc += 1;
				}
				else if(op == mc_store)
				{
					com_store();
					pc += 1;
				}
				else if(op == mc_new_table)
				{
					com_new_table();
					pc +=1;
				}
				else if(op == mc_add_in_table)
				{
					com_add_in_table();
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
					s4g_value* s1 = execute.get(execute.count()-1);
					s4g_value* s2 = execute.get(execute.count());
					execute.pop(2);
						if(s1->get_type() == t_nn)
						{
								if(s2->get_type() == t_int)
								{
									long num2 = s2->get_long();
									execute.push(new s4g_value(+num2));
								}
						}
						else
						{
							long num1 = s1->get_long();
							long num2 = s2->get_long();
							execute.push(new s4g_value(num1+num2));
						}
					
					pc++;
				}
				else if(op == mc_sub)
				{
					s4g_value* s1 = execute.get(execute.count()-1);
					s4g_value* s2 = execute.get(execute.count());
					execute.pop(2);
						if(s1->get_type() == t_nn)
						{
								if(s2->get_type() == t_int)
								{
									long num2 = s2->get_long();
									execute.push(new s4g_value(-num2));
								}
						}
						else
						{
							long num1 = s1->get_long();
							long num2 = s2->get_long();
							execute.push(new s4g_value(num1-num2));
						}
					pc++;
				}
				else if(op == mc_mul)
				{
					long num1 = execute.get(execute.count()-1)->get_long();
					long num2 = execute.get(execute.count()-0)->get_long();
					execute.pop(2);
					execute.push(new s4g_value(num1 * num2));
					pc++;
				}
				else if(op == mc_div)
				{
					long num1 = execute.get(execute.count()-1)->get_long();
					long num2 = execute.get(execute.count()-0)->get_long();
					execute.pop(2);
					execute.push(new s4g_value(num1 / num2));
					pc++;
				}

				else if(op == mc_call)
				{
					com_call(arg);
					pc++;
				}
				else if(op == mc_halt)
					break;

				if(op == mc_fetch_get || op == mc_fetch)
					cfetchget = true;
				else if(op != mc_push)
					cfetchget = false;

				if(op == mc_fetch_get && !arg)
					cfetchgetarg = true;
				else if(op == mc_fetch_get)
					cfetchgetarg = false;

				if (pc > commands->count())
				{	
					break;
				}
		}

	s4g_table* ttype = vars->gets("ttable")->get_table();
	s4g_value* tval0 = vars->gets("c");
		if(tval0->get_type() == t_int)
		{
			long num = vars->gets("c")->get_long();
			int qwert = 0;
		}
	/*s4g_value* tval1 = vars.getn(2);
	s4g_value* tval2 = vars.getn(3);*/
	//String str = vars["ttable"]->get_table()->get(1)->get_string();
	//long num = vars.gets("c")->get_long();
	//long num = vars.gets("ttable")->get_table()->getn(1)/*->get_table()->getn(3)->get_table()->gets("wert")*/->get_long();
	//String str = vars.gets("ttable")->get_table()->getn(2)->get_string();
	//s4g_value* tval = vars.gets("ttable")->get_table()->getn(1);
	//long num2 = vars.gets("ttable")->get_table()->gets("qwert")->get_long();
	//String str2 = vars.gets("ttable")->get_table()->getn(4)->get_string();
	/*s4g_table* ttype2 = vars.gets("ttable")->get_table()->getn(1)->get_table();
	long num3 = vars.gets("ttable")->get_table()->getn(1)->get_table()->getn(1)->get_long();
	long str4 = vars.gets("ttable")->get_table()->getn(1)->get_table()->getn(2)->get_long();*/
		/*printf("a = %d\n",vars["a"]->get_long());
		printf("b = %d\n",vars["b"]->get_long());
		printf("c = %d\n",vars["c"]->get_long());*/
	return 0;
}