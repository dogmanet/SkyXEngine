
#include <s4g\s4g_compiler.h>

void s4g_compiler::gen(s4g_vm_command comm, s4g_value* val, long _lexid)
{
	comms->push(s4g_command(comm, val, _lexid));
	//printf("%s\n",comm.c_str());
};

int s4g_compiler::compile(s4g_node* node,Stack<s4g_command> * commands)
{
	comms = commands;
	return compile2(node);
}

int s4g_compiler::compile2(s4g_node* node)
{
		if(!node)
			return 0;

		if(node->type == _begin)
		{
			compile2(node->op1);
			gen(mc_halt, 0, node->lexid);
			printf("halt\n");
		}
		else if(node->type == _empty)
		{
			compile2(node->op1);
			compile2(node->op2);
		}
		else if(node->type == _expr)
		{
			compile2(node->op1);
			gen(mc_pop, 0, node->lexid);
			printf("pop\n");
			compile2(node->op2);	//empty
		}
		else if(node->type == _set)
		{
			long countvar = 0;		//
			s4g_node* tmpnode = node->op1;
			while (tmpnode)
			{
				countvar++;
				//compile2(tmpnode);
				tmpnode = tmpnode->op2;
			}

			long countval = 0;		//
			tmpnode = node->op2;
			while (tmpnode)
			{
				countval++;
				//compile2(tmpnode);
				tmpnode = tmpnode->op2;
			}

			/*if (countval == 1 && node->op1 && node->op1->op2 && node->op1->op2->type == _call)
			{
				countval = 2;
			}*/

			if (countvar > 1 || node->op2->type == _call)
			{
				gen(mc_end, 0, node->lexid);
				printf("--end\n");
				tmpnode = node->op2;
				while (tmpnode)
				{
					compile2(tmpnode);
					tmpnode = tmpnode->op2;
				}

				tmpnode = node->op1;
				while (tmpnode)
				{
					compile2(tmpnode);
					tmpnode = tmpnode->op2;
				}

				gen(mc_mstore, gc->cr_val_int(countvar/*, -2*/), node->lexid);
				printf("mstore\n");
			}
			else
			{
				compile2(node->op1);
				compile2(node->op2);
				gen(mc_store, 0, node->lexid);
				printf("store\n");
			}
		}
		else if(node->type == _sett)
		{
			if (node->op1->type == _crvar)
			{
				gen(mc_fetch_get_cr, node->op1->value, node->lexid);
				printf("fetch_get_cr\n");
			}
			else
			{
				gen(mc_fetch_get, node->op1->value, node->lexid);
				printf("fetch_get\n");
			}

			//compile(node->op1);
			compile2(node->op2);
			gen(mc_store, 0, node->lexid);
			printf("store\n");
		}
		else if(node->type == _create_table)
		{
			gen(mc_new_table, node->value, node->lexid);
			printf("create_new_table\n");
			compile2(node->op1);
			compile2(node->op2);
		}
		else if(node->type == _add_in_table)
		{
			//gen(mc_push,node->op1->value);
			//printf("push\n");
			//gen(mc_push,node->op2->value);
			
			//gen(mc_pop);
			//printf("pop\n");
			compile2(node->op1);
			gen(mc_add_in_table,0, node->lexid);
			printf("add_in_table\n");
			gen(mc_pop, 0, node->lexid);
			printf("pop\n");
			compile2(node->op2);
			//compile(node->op1->op1);
		}
		
		else if (node->type == _null)
		{
			gen(mc_push, node->value, node->lexid);
			printf("push\n");
			compile2(node->op3);
		}
		else if (node->type == _int)
		{
			gen(mc_push, node->value, node->lexid);
			printf("push\n");
			compile2(node->op3);
		}
		else if (node->type == _uint)
		{
			gen(mc_push, node->value, node->lexid);
			printf("push\n");
			compile2(node->op3);
		}
		else if (node->type == _float)
		{
			gen(mc_push, node->value, node->lexid);
			printf("push\n");
			compile2(node->op3);
		}
		else if (node->type == _bool)
		{
			gen(mc_push, node->value, node->lexid);
			printf("push\n");
			compile2(node->op3);
		}
		else if(node->type == _numnull)
		{
			gen(mc_push, node->value, node->lexid);
			printf("push\n");
			compile2(node->op3);
		}
		//
		else if(node->type == _string)
		{
			gen(mc_push, node->value, node->lexid);
			printf("push\n");
			compile2(node->op3);
		}
		
		else if(node->type == _get)
		{
			bool iscr = false;
				if (node->op1->type == _crvar)
				{
					iscr = true;
					node->op1->type = _var;
				}
				else if (node->op1->type == _string_cr)
				{
					iscr = true;
					node->op1->type = _string;
				}
				else if (node->op1->type == _int_cr)
				{
					iscr = true;
					node->op1->type = _int;
				}

				compile2(node->op1);

				if (iscr)
				{
					gen(mc_fetch_get_cr, 0, node->lexid);
					printf("fetch_get_cr\n");
				}
				else
				{
					gen(mc_fetch_get, 0, node->lexid);
					printf("fetch_get\n");
				}
			compile2(node->op3);
		}
		else if (node->type == _get_cr)
		{
			compile2(node->op1);
			gen(mc_fetch_get_cr, 0, node->lexid);
			printf("fetch_get_cr\n");
			compile2(node->op3);
		}
		else if(node->type == _var)
		{
			gen(mc_fetch, node->value, node->lexid);
			printf("fetch\n");
			compile2(node->op1);
			compile2(node->op3);
		}
		else if (node->type == _crvar)
		{
			gen(mc_fetch_cr, node->value, node->lexid);
			printf("fetch_cr\n");
			compile2(node->op1);
			compile2(node->op3);
		}
		else if(node->type == _add)
		{
			gen(mc_add, 0, node->lexid);
			printf("add\n");
			compile2(node->op3);
		}
		else if(node->type == _sub)
		{
			gen(mc_sub, 0, node->lexid);
			printf("sub\n");
			compile2(node->op3);
		}
		else if(node->type == _mul)
		{
			gen(mc_mul, 0, node->lexid);
			printf("mul\n");
			compile2(node->op3);
		}
		else if(node->type == _div)
		{
			gen(mc_div, 0, node->lexid);
			printf("div\n");
			compile2(node->op3);
		}
		else if(node->type == _function)
		{
			s4g_s_function* sf = new s4g_s_function();
			s4g_node* tmpnode = node;
				while(tmpnode->op1 && tmpnode->op1->value)
				{
					//countarg++;
						sf->args.push(gc->get_str(tmpnode->op1->value));
						tmpnode = tmpnode->op1;
						if (tmpnode && tmpnode->op1 && tmpnode->op1->type == _marg)
						{
							sf->ismarg = true;
							break;
						}	
				}
			tmpnode = node->op3;
				if (tmpnode)
				{
					/*s4g_value* tmpvaltt = gc->cr_val_table_null(-2);
					sf->externs = gc->get_table(tmpvaltt);*/
					while (tmpnode)
					{
						const char* str = gc->get_str(tmpnode->value);
						sf->externs_strs.push(str);
						/*s4g_value* tmpval = gc->cr_val_null(-2);
						sf->externs->add_val_s(str, tmpval, -2);*/
						tmpnode = tmpnode->op1;
					}
				}
			Stack<s4g_command>* tmpcomms = comms;
			printf("---\n");
			compile(node->op2,&(sf->commands));
			s4g_vm_command ccoomm = sf->commands.get(-1).command;
			if (ccoomm != mc_halt)
				sf->commands.push(s4g_command(mc_halt, 0, -5));
			printf("---\n");
			comms = tmpcomms;

			/*if (sf->commands.count() == 0)
				sf->commands.push(s4g_command(mc_halt, 0, -3));*/
			
			gen(mc_push, gc->cr_val_s_func(sf/*, -3*/), node->lexid);
			printf("push\n");
		}
		else if(node->type == _call)
		{
			gen(mc_precall, /*gc->cr_val_int(countarg, -2),*/0, node->lexid);
			printf("precall\n");
			compile2(node->op1);	//ложим на вершину стека функцию
			long countarg = 0;		//
			s4g_node* tmpnode = node->op3;
				while(tmpnode->op1)
				{
					countarg++;
					compile2(tmpnode->op1);
					tmpnode = tmpnode->op2;
				}
				gen(mc_call, /*gc->cr_val_int(countarg, -2),*/0, node->lexid);
			printf("call\n");
		}
		else if(node->type == _return)
		{
			s4g_node* tmpnode = node;
				while(tmpnode)
				{
					//countarg++;
					compile2(tmpnode->op1);
					tmpnode = tmpnode->op2;
				}
			gen(mc_halt, 0, node->lexid);
			printf("halt\n");
		}
}