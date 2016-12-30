
#include "s4g_compiler.h"

inline void s4g_compiler::gen(s4g_vm_command comm, s4g_value* val, long _lexid)
{
	comms->push(s4g_command(comm, val, _lexid));
};

int s4g_compiler::compile(s4g_node* node, s4g_stack<s4g_command> * commands)
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

			long countval = 0;	
			tmpnode = node->op2;
			while (tmpnode)
			{
				countval++;
				//compile2(tmpnode);
				tmpnode = tmpnode->op2;
			}

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

				//compile2(node->op1);

				if (iscr)
				{
					gen(mc_fetch_get_cr, node->op1->value, node->lexid);
					printf("fetch_get_cr\n");
				}
				else
				{
					gen(mc_fetch_get, node->op1->value, node->lexid);
					printf("fetch_get\n");
				}
			compile2(node->op2);
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
						sf->args.push(gc->get_str(tmpnode->op1->value));
						tmpnode = tmpnode->op1;
						if (tmpnode && tmpnode->op1 && tmpnode->op1->type == _marg)
						{
							sf->ismarg = true;
							sf->marg_val = gc->cr_val_table_null();
							sf->marg_val->isdelete = false;
							sf->marg_val->typedata = 1;
							sf->marg_val->pdata->typedata = 1;
							sf->margtable = gc->get_table(sf->marg_val);
							break;
						}	
				}
			tmpnode = node->op3;
				if (tmpnode)
				{
					while (tmpnode)
					{
						const char* str = gc->get_str(tmpnode->value);
						sf->externs_strs.push(str);
						
						tmpnode = tmpnode->op1;
					}
				}
			s4g_stack<s4g_command>* tmpcomms = comms;
			printf("---\n");
			compile(node->op2,&(sf->commands));
			if (sf->commands.count_obj > 0)
			{
				s4g_vm_command ccoomm = sf->commands.get(-1).command;
				if (ccoomm != mc_halt)
					sf->commands.push(s4g_command(mc_halt, 0, -5));
			}
			printf("---\n");
			comms = tmpcomms;
			
			gen(mc_push, gc->cr_val_s_func(sf/*, -3*/), node->lexid);
			printf("push\n");
		}
		else if(node->type == _call)
		{
			gen(mc_precall, 0, node->lexid);
			printf("precall\n");
			compile2(node->op1);	//ложим на вершину стека функцию
			long countarg = 0;		//
			s4g_node* tmpnode = node->op2;
				while(tmpnode->op1)
				{
					countarg++;
					compile2(tmpnode->op1);
					tmpnode = tmpnode->op2;
				}
				gen(mc_call, 0, node->lexid);
			printf("call\n");
			compile2(node->op3);
		}
		else if(node->type == _return)
		{
			s4g_node* tmpnode = node;
				while(tmpnode)
				{
					compile2(tmpnode->op1);
					tmpnode = tmpnode->op2;
				}
			gen(mc_halt, 0, node->lexid);
			printf("halt\n");
		}
		else if(node->type == _if)
		{
			compile2(node->op1);
			int startpos = comms->count();
			gen(mc_jz, 0, node->lexid);
			printf("jz\n");
			//insert jz to op3
			compile2(node->op2);
			int endpos = comms->count();
			comms[0][startpos].arg = (s4g_value*)(endpos - startpos - (node->op3 ? 0 : 1));
			//insert jmp to end
			if(node->op3)
			{
				startpos = comms->count();
				gen(mc_jmp, 0, node->lexid);
				printf("jmp\n");
				compile2(node->op3);
				endpos = comms->count();
				comms[0][startpos].arg = (s4g_value*)(endpos - startpos - 1);
			}
		}
		else if(node->type == _while)
		{
			cyctls_bak.push(cyctls);
			cyctls.clear();

			int startpos = comms->count();
			compile2(node->op1);
			int jzpos = comms->count();
			gen(mc_jz, 0, node->lexid);
			printf("jz\n");
			compile2(node->op2);

			gen(mc_jmp, (s4g_value*)(startpos - comms->count() - 1), node->lexid);
			printf("jmp\n");
			comms[0][jzpos].arg = (s4g_value*)(comms->count() - jzpos - 1);

			for(int i = 0, l = cyctls.size(); i < l; ++i)
			{
				int addr = 0;
				switch(cyctls[i].type)
				{
				case _cyctl::BREAK:
					addr = comms->count() - cyctls[i].addr - 1;
					break;
				case _cyctl::CONTINUE:
					addr = startpos - cyctls[i].addr - 1;
					break;
				}
				comms[0][cyctls[i].addr].arg = (s4g_value*)addr;
			}
			cyctls = cyctls_bak.get(cyctls_bak.count_obj - 1);
			cyctls_bak.pop(1);
		}
		else if(node->type == _for)
		{
			cyctls_bak.push(cyctls);
			cyctls.clear();

			int startpos = comms->count();
			compile2(node->op1);
			int jzpos = comms->count();
			gen(mc_jz, 0, node->lexid);
			printf("jz\n");
			compile2(node->op2);
			int steppos = comms->count();
			compile2(node->op3);
			gen(mc_jmp, (s4g_value*)(startpos - comms->count() - 1), node->lexid);
			printf("jmp\n");
			comms[0][jzpos].arg = (s4g_value*)(comms->count() - jzpos - 1);

			for(int i = 0, l = cyctls.size(); i < l; ++i)
			{
				int addr = 0;
				switch(cyctls[i].type)
				{
				case _cyctl::BREAK:
					addr = comms->count() - cyctls[i].addr - 1;
					break;
				case _cyctl::CONTINUE:
					addr = steppos - cyctls[i].addr - 1;
					break;
				}
				comms[0][cyctls[i].addr].arg = (s4g_value*)addr;
			}
			cyctls = cyctls_bak.get(cyctls_bak.count_obj - 1);
			cyctls_bak.pop(1);
			//cond
			//jz: end
			//{
			//body
			//step
			//jmp: cond
			//}
			//end
		}
		else if(node->type == _break)
		{
			gen(mc_jmp, 0, node->lexid);
			printf("jmp\n");
			cyctls.push_back({comms->count() - 1, _cyctl::BREAK});
			compile2(node->op1);
		}
		else if(node->type == _continue)
		{
			gen(mc_jmp, 0, node->lexid);
			printf("jmp\n");
			cyctls.push_back({comms->count() - 1, _cyctl::CONTINUE});
			compile2(node->op1);
		}
#define GEN_OP(op) \
		else if(node->type == _ ## op)\
		{\
			gen(mc_ ## op, 0, node->lexid);\
			printf(#op "\n");\
			compile2(node->op3);\
		}

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
}