
#pragma once

enum s4g_vm_command
{
	mc_halt = -1,	//остановить текущее выполнение
	mc_fetch,		//положить на вершину стека значение переменной
	mc_fetch_get,	//получить значение из таблицы, если предыдущим вызовом был mc_fetch
	mc_store,		//сохранить в переменной значение с вершины стека
	mc_push,		//положить на вершину стека
	mc_pop,			//удалить значение с вершины стека
	mc_add,
	mc_sub,
	mc_mul,
	mc_div,
	mc_new_table,	//создать таблицу
	mc_add_in_table,//добавить в таблицу
	mc_call,		//вызов функции
};

#include <s4g\s4g_compiler.h>

class s4g_vm
{
public:
	s4g_vm(){ vgvars = new s4g_value(); vgvars->init_table(&gvars); }

	int run(Stack<s4g_command>* commands,s4g_table* vars);

	//
	inline void com_fetch(s4g_value* val);
	inline void com_fetch_get(s4g_value* val);
	inline void com_store();
	inline void com_add_in_table();
	inline void com_call(s4g_value* val);
	inline void com_new_table();
	//

	s4g_table gvars;	//глобальное пространство имен _g
	s4g_value* vgvars;	//переменная хранящая в себе глобальнео пространство имен
	s4g_table* curr_vars;	//текущее установленное пространство имен, есл выполняется функция то пространство имен функции
	Stack<s4g_value*> execute;	//стек выполнения команд

	bool cfetchget;		//была ли предыдущая команда либо fetch либо fetch_get
	bool cfetchgetarg;	//была ли предыдущая команда fetch_get с аргументом или без
};
