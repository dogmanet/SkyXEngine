
#include <s4g\s4g_compiler.h>

void s4g_compiler::gen(s4g_vm_command comm,s4g_value* val)
{
	comms->push(s4g_command(comm,val));
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
			gen(mc_halt);
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
			gen(mc_pop);
			printf("pop\n");
			compile2(node->op2);	//empty
		}
		else if(node->type == _set)
		{
			compile2(node->op1);
			compile2(node->op2);
			gen(mc_store/*,node->op1->value*/);
			printf("store\n");
			//compile(node->op1->op1);	//когда записываем переменную в таблицу
					/*compile(node->op2);
					gen(mc_store,node->op1->value);
					printf("store\n");
					compile(node->op1->op1);	//когда записываем переменную в таблицу*/
		}
		else if(node->type == _sett)
		{
			gen(mc_fetch_get,node->op1->value);
			printf("fetch_get\n");
			//compile(node->op1);
			compile2(node->op2);
			gen(mc_store);
			printf("store\n");
		}
		else if(node->type == _create_table)
		{
			gen(mc_new_table);
			printf("create_new_table\n");
			compile2(node->op1);
		}
		else if(node->type == _add_in_table)
		{
			//gen(mc_push,node->op1->value);
			//printf("push\n");
			//gen(mc_push,node->op2->value);
			
			//gen(mc_pop);
			//printf("pop\n");
			compile2(node->op1);
			gen(mc_add_in_table);
			printf("add_in_table\n");
			gen(mc_pop);
			printf("pop\n");
			compile2(node->op2);
			//compile(node->op1->op1);
		}
		else if(node->type == _int)
		{
			gen(mc_push,node->value);
			printf("push\n");
			compile2(node->op3);
		}
		else if(node->type == _numnull)
		{
			gen(mc_push,node->value);
			printf("push\n");
			compile2(node->op3);
		}
		//
		else if(node->type == _string)
		{
			gen(mc_push,node->value);
			printf("push\n");
			compile2(node->op3);
		}
		else if(node->type == _get)
		{
			compile2(node->op1);
			gen(mc_fetch_get);
			printf("fetch_get\n");
			compile2(node->op3);
		}
		else if(node->type == _var)
		{
			gen(mc_fetch,node->value);
			printf("fetch\n");
				/*if(node->op1 && node->op1->type == _var)
				{
					compile(node->op1);
					gen(mc_pop);
					printf("pop\n");
				}
				else*/
					compile2(node->op1);
					compile2(node->op3);
		}
		else if(node->type == _add)
		{
			gen(mc_add);
			printf("add\n");
			//compile(node->op1);
			compile2(node->op3);
		}
		else if(node->type == _sub)
		{
			gen(mc_sub);
			printf("sub\n");
			//compile(node->op1);
			compile2(node->op3);
		}
		else if(node->type == _mul)
		{
			gen(mc_mul);
			printf("mul\n");
			//compile(node->op1);
			compile2(node->op3);
		}
		else if(node->type == _div)
		{
			gen(mc_div);
			printf("div\n");
			//compile(node->op1);
			compile2(node->op3);
		}
		else if(node->type == _function)
		{
			s4g_s_function* sf = new s4g_s_function();
			s4g_node* tmpnode = node;
				while(tmpnode->op1 && tmpnode->op1->value)
				{
					//countarg++;
					sf->args.push(tmpnode->op1->value->get_string());
					tmpnode = tmpnode->op1;
				}
			Stack<s4g_command>* tmpcomms = comms;
			printf("---\n");
			compile(node->op2,&(sf->commands));
			printf("---\n");
			comms = tmpcomms;
			s4g_value* tval = new s4g_value();
			tval->set_s_function(sf);
			gen(mc_push,tval);
			printf("push\n");
			//compile2(node->op3);
		}
		else if(node->type == _call)
		{
			compile2(node->op1);	//ложим на вершину стека функцию
			long countarg = 0;		//
			s4g_node* tmpnode = node->op2;
				while(tmpnode->op1)
				{
					countarg++;
					compile2(tmpnode->op1);
					tmpnode = tmpnode->op2;
				}
			gen(mc_call,new s4g_value(countarg));
			printf("call\n");
			compile2(node->op3);
		}
		else if(node->type == _return)
		{
			s4g_node* tmpnode = node;
				while(tmpnode->op1)
				{
					//countarg++;
					compile2(tmpnode->op1);
					tmpnode = tmpnode->op2;
				}
			//compile2(node->op1);
			gen(mc_halt);
		}
}