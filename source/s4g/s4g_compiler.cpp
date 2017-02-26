
#include "s4g_compiler.h"

#ifdef S4G_NPRINT_OPS
#	define def_printf(format,...) 
#else
#	define def_printf(format,...) printf(format,__VA_ARGS__);
#endif

inline void s4g_compiler::gen(s4g_vm_command comm, s4g_value* val, long _lexid, long _second_data)
{
	comms->push(s4g_command(comm, val, _lexid, _second_data));
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
			def_printf("halt\n");
		}
		else if(node->type == _empty)
		{
			compile2(node->op1);
			compile2(node->op2);
		}
		else if(node->type == _expr)
		{
			compile2(node->op1);
			if ((*comms)[-1].command != mc_call)
			{
				gen(mc_pop, 0, node->lexid);
				def_printf("pop\n");
			}
			compile2(node->op2);	//empty
		}
		else if(node->type == _set)
		{
			compile2(node->op1);
			compile2(node->op2);
			gen(mc_store, 0, node->lexid);
			def_printf("store\n");
		}
		else if(node->type == _sett)
		{
				if (node->op1->type == _crvar)
				{
					gen(mc_fetch_get_cr, node->op1->value, node->lexid);
					def_printf("fetch_get_cr\n");
				}
				else
				{
					gen(mc_fetch_get, node->op1->value, node->lexid);
					def_printf("fetch_get\n");
				}

			compile2(node->op2);

			gen(mc_store, 0, node->lexid);
			def_printf("store\n");

			gen(mc_pop, 0, node->lexid);
			def_printf("pop\n");
		}
		else if(node->type == _create_table)
		{
			gen(mc_new_table, node->value, node->lexid);
			def_printf("create_new_table\n");
			compile2(node->op1);
			compile2(node->op2);
		}
		else if(node->type == _add_in_table)
		{
			compile2(node->op1);
			gen(mc_add_in_table,0, node->lexid);
			def_printf("add_in_table\n");
			gen(mc_pop, 0, node->lexid);
			def_printf("pop\n");
			compile2(node->op2);
			//compile(node->op1->op1);
		}
		
		else if (node->type == _null)
		{
			gen(mc_push, node->value, node->lexid);
			def_printf("push\n");
			compile2(node->op3);
		}
		else if (node->type == _int)
		{
			gen(mc_push, node->value, node->lexid);
			def_printf("push\n");
			compile2(node->op3);
		}
		else if (node->type == _uint)
		{
			gen(mc_push, node->value, node->lexid);
			def_printf("push\n");
			compile2(node->op3);
		}
		else if (node->type == _float)
		{
			gen(mc_push, node->value, node->lexid);
			def_printf("push\n");
			compile2(node->op3);
		}
		else if (node->type == _bool)
		{
			gen(mc_push, node->value, node->lexid);
			def_printf("push\n");
			compile2(node->op3);
		}
		else if(node->type == _numnull)
		{
			gen(mc_push, node->value, node->lexid);
			def_printf("push\n");
			compile2(node->op3);
		}
		//
		else if(node->type == _string)
		{
			gen(mc_push, node->value, node->lexid);
			def_printf("push\n");
			compile2(node->op3);
		}
		
		else if (node->type == _get || node->type == _get_preincr || node->type == _get_predecr || node->type == _get_postincr || node->type == _get_postdecr)
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

				bool non_exists_var = false;

				if (node->op1 && (node->op1->type == _var || (node->op1->op1 || node->op1->op2 || node->op1->op3)))
				{
					non_exists_var = true;
					compile2(node->op1);
				}
				
				if (iscr)
				{
					gen(mc_fetch_get_cr, (non_exists_var ? 0 : node->op1->value), node->lexid, (long)node->value);
					def_printf("fetch_get_cr\n");
				}
				else
				{
					gen(mc_fetch_get, (non_exists_var ? 0 : node->op1->value), node->lexid, (long)node->value);
					def_printf("fetch_get\n");
				}

				if (node->type == _get_preincr)
					gen(mc_preincr, 0, node->lexid);
				else if (node->type == _get_postincr)
					gen(mc_postincr, 0, node->lexid);

				if (node->type == _get_predecr)
					gen(mc_predecr, 0, node->lexid);
				else if (node->type == _get_postdecr)
					gen(mc_postdecr, 0, node->lexid);

			compile2(node->op2);
			compile2(node->op3);
		}
		else if (node->type == _get_cr)
		{
			compile2(node->op1);
			gen(mc_fetch_get_cr, 0, node->lexid);
			def_printf("fetch_get_cr\n");
			compile2(node->op3);
		}
		else if(node->type == _var)
		{
			gen(mc_fetch, node->value, node->lexid);
			def_printf("fetch\n");
			compile2(node->op1);
			s4g_node* tmpnode = node->op2;
			while (tmpnode && (tmpnode->type == _crvar || tmpnode->type == _var))
			{
				if (tmpnode->type == _crvar)
				{
					gen(mc_fetch_cr, tmpnode->value, node->lexid);
					def_printf("fetch_cr\n");
				}
				else
				{
					gen(mc_fetch, tmpnode->value, node->lexid);
					def_printf("fetch\n");
				}
				gen(mc_pop, 0, node->lexid);
				def_printf("pop\n");
				tmpnode = tmpnode->op2;
			}
			compile2(node->op3);
		}
		else if (node->type == _crvar)
		{
			gen(mc_fetch_cr, node->value, node->lexid);
			def_printf("fetch_cr\n");
			compile2(node->op1);
			s4g_node* tmpnode = node->op2;
			while (tmpnode && (tmpnode->type == _crvar || tmpnode->type == _var))
			{
				if (tmpnode->type == _crvar)
				{
					gen(mc_fetch_cr, tmpnode->value, node->lexid);
					def_printf("fetch_cr\n");
				}
				else
				{
					gen(mc_fetch, tmpnode->value, node->lexid);
					def_printf("fetch\n");
				}
				gen(mc_pop, 0, node->lexid);
				def_printf("pop\n");
				tmpnode = tmpnode->op2;
			}
			compile2(node->op3);
		}
		else if (node->type == _append_table)
		{
			gen(mc_append_table, node->value, node->lexid);
			def_printf("append_table\n");
		}
		else if (node->type == _var_preincr || node->type == _var_postincr || node->type == _var_predecr || node->type == _var_postdecr)
		{
			gen(mc_fetch, node->value, node->lexid);
			def_printf("fetch\n");
			if (node->type == _var_preincr)
			{
				gen(mc_preincr, 0, node->lexid);
				def_printf("preincr\n");
			}
			else if (node->type == _var_postincr)
			{
				gen(mc_postincr, 0, node->lexid);
				def_printf("postincr\n");
			}
			else if (node->type == _var_predecr)
			{
				gen(mc_predecr, 0, node->lexid);
				def_printf("predecr\n");
			}
			else if (node->type == _var_postdecr)
			{
				gen(mc_postdecr, 0, node->lexid);
				def_printf("postdecr\n");
			}
			compile2(node->op1);
			compile2(node->op3);
		}
		
		else if(node->type == _add)
		{
			gen(mc_add, 0, node->lexid);
			def_printf("add\n");
			compile2(node->op3);
		}
		else if(node->type == _sub)
		{
			gen(mc_sub, 0, node->lexid);
			def_printf("sub\n");
			compile2(node->op3);
		}
		else if(node->type == _mul)
		{
			gen(mc_mul, 0, node->lexid);
			def_printf("mul\n");
			compile2(node->op3);
		}
		else if(node->type == _div)
		{
			gen(mc_div, 0, node->lexid);
			def_printf("div\n");
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
							sf->marg_val = gc->cr_val_table_null(0,S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS);
							
							sf->marg_val->pdata->typedata = S4G_GC_TYPE_DATA_SYS;
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
			def_printf("-------b\n");
			compile(node->op2,&(sf->commands));
			if (sf->commands.count_obj > 0)
			{
				s4g_vm_command ccoomm = sf->commands.get(-1).command;
				if (ccoomm != mc_halt)
					sf->commands.push(s4g_command(mc_halt, 0, -5));
			}
			def_printf("-------e\n");
			comms = tmpcomms;
			
			gen(mc_push, gc->cr_val_s_func(sf, 0, S4G_GC_TYPE_VAR_SYS), node->lexid);
			def_printf("push\n");
		}
		else if(node->type == _call)
		{
			gen(mc_precall, 0, node->lexid);
			def_printf("precall\n");
			compile2(node->op1);	//ложим на вершину стека функцию
			long countarg = 0;		//
			s4g_node* tmpnode = node->op2;// ->op1->op2;
				while(tmpnode->op1)
				{
					countarg++;
					compile2(tmpnode->op1);
					tmpnode = tmpnode->op2;
				}
				gen(mc_call, node->value, node->lexid);
			def_printf("call\n");
			compile2(node->op3);
		}
		else if(node->type == _return)
		{
			compile2(node->op1);

			gen(mc_halt, 0, node->lexid);
			def_printf("halt\n");
		}
		else if(node->type == _if)
		{
			compile2(node->op1);
			int startpos = comms->count();
			gen(mc_jz, 0, node->lexid);
			def_printf("jz\n");
			//insert jz to op3
			compile2(node->op2);
			int endpos = comms->count();
			comms[0][startpos].arg = (s4g_value*)(endpos - startpos - (node->op3 ? 0 : 1));
			//insert jmp to end
			if(node->op3)
			{
				startpos = comms->count();
				gen(mc_jmp, 0, node->lexid);
				def_printf("jmp\n");
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
			def_printf("jz\n");
			compile2(node->op2);

			gen(mc_jmp, (s4g_value*)(startpos - comms->count() - 1), node->lexid);
			def_printf("jmp\n");
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

			/*gen(mc_pop, 0, node->lexid);
			def_printf("pop\n");*/

			int startpos = comms->count();
			compile2(node->op1);
			int jzpos = comms->count();
			gen(mc_jz, 0, node->lexid);
			def_printf("jz\n");
			compile2(node->op2);
			int steppos = comms->count();
			compile2(node->op3);
			//gen(mc_pop, 0, node->lexid);
			//def_printf("pop\n");
			gen(mc_jmp, (s4g_value*)(startpos - comms->count() - 1), node->lexid);
			def_printf("jmp\n");
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
		else if (node->type == _block)
		{
			gen(mc_block_new, 0, node->lexid);
			def_printf("block_new\n");
			compile2(node->op1);
			compile2(node->op2);
			//compile2(node->op3);
			gen(mc_block_del, 0, node->lexid);
			def_printf("block_del\n");
			compile2(node->op3);
			/*gen(mc_jmp, 0, node->lexid);
			def_printf("jmp\n");
			cyctls.push_back({ comms->count() - 1, _cyctl::BREAK });
			compile2(node->op1);*/
		}
		else if(node->type == _break)
		{
			gen(mc_jmp, 0, node->lexid);
			def_printf("jmp\n");
			cyctls.push_back({comms->count() - 1, _cyctl::BREAK});
			compile2(node->op1);
		}
		else if(node->type == _continue)
		{
			gen(mc_jmp, 0, node->lexid);
			def_printf("jmp\n");
			cyctls.push_back({comms->count() - 1, _cyctl::CONTINUE});
			compile2(node->op1);
		}
		else if(node->type == _chain)
		{
			for(int i = 0, l = node->ops.size(); i < l; ++i)
			{
				compile2(node->ops[i]);
			}
		}
#define GEN_OP(op) \
		else if(node->type == _ ## op)\
		{\
			gen(mc_ ## op, 0, node->lexid);\
			def_printf(#op "\n");\
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