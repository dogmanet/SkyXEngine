
#pragma once

enum s4g_vm_command
{
	mc_halt = -1,	//остановить текущее выполнение
	mc_fetch,		//положить на вершину стека значение переменной
	mc_fetch_cr,	//создать и положить на вершину стека значение переменной
	mc_fetch_get,	//получить значение из таблицы, если предыдущим вызовом был mc_fetch
	mc_fetch_get_cr,//создать значение в таблице, если предыдущим вызовом был mc_fetch
	mc_store,		//сохранить в переменной (-1 по отношению к вершине стека) значение с вершины стека
	mc_end,			//для мультисохранения, разделяет левую часть сохранения от правой
	mc_mstore,		//мультиприсваивание
	mc_push,		//положить на вершину стека
	mc_pop,			//удалить значение с вершины стека
	mc_add,			//-1 + 0
	mc_sub,			//-1 - 0
	mc_mul,			//-1 * 0
	mc_div,			//-1 / 0
	mc_new_table,	//создать таблицу и положить на вершину стека
	mc_add_in_table,//добавить в таблицу котоаря лежит на вершине
	mc_call,		//вызов функции
};

#include <s4g\s4g_compiler.h>

//структура описывающая сохраненное предыдущее состояние при вызове функции и содержащее имя вызыванной функции
struct s4g_call_data
{
	s4g_call_data()
	{
		coms = 0;
		vars = 0;
		cfetchget = cfetchgetarg = false;
		cfetchpushstore = 0;
		id_curr_com = lastidctx = idnewctx = idexternctx = -1;
		namef = "";
	}

	s4g_call_data(
		Stack<s4g_command>* _coms, 
		s4g_table* _vars, 
		bool _cfetchget, 
		bool _cfetchgetarg, 
		int _cfetchpushstore, 
		long _id_curr_com, 
		long _lastidctx, 
		long _idnewctx,
		long _idexternctx,
		const char* _namef)
	{
		coms = _coms;
		vars = _vars;
		cfetchget = _cfetchget;
		cfetchgetarg = _cfetchgetarg;
		cfetchpushstore = _cfetchgetarg;
		id_curr_com = _id_curr_com;
		lastidctx = _lastidctx;
		idnewctx = _idnewctx;
		idexternctx = _idexternctx;
		namef = _namef;
	}

	Stack<s4g_command>* coms;	//команды выполнения
	s4g_table* vars;			//таблица с переменными (окружение)
	bool cfetchget;				
	bool cfetchgetarg;	
	int cfetchpushstore;
	long id_curr_com;
	long lastidctx;
	long idnewctx;
	long idexternctx;

	String namef;	// имя функции которая вызвалась и спровоцировала сохранение текущего состяния
};

class s4g_vm
{
public:
	s4g_vm(s4g_gc* _gc)
	{ 
		gc = _gc;  
		gc->add_new_context(&gvars); 
		vgvars = gc->cr_val_table(gvars, -2); 
		strerror[0] = 0; error = 0;
		cfetchpushstore = 0;
		curr_vars = 0;
	}

	int run(Stack<s4g_command>* commands,s4g_table* vars);

	//функции для исполнения байт кода
	inline int com_fetch(s4g_value* val, bool is_cr);
	inline int com_fetch_get(s4g_value* val, bool is_cr);
	inline int com_store();
	inline int com_mstore(s4g_value* arg, int val_end);
	inline int com_add_in_table();
	inline int com_call(s4g_value* val);
	inline int com_new_table(s4g_value* val);

	inline int com_retprev(long* pc);	//возвращаем предыдущее состояние машины, в случае когда функция отработала все свои команды

	inline int com_add();
	inline int com_sub();
	inline int com_mul();
	inline int com_div();
	//

	Stack<s4g_command>* curr_comm;
	long id_curr_com;

	s4g_table* gvars;	//глобальное пространство имен _g
	s4g_value* vgvars;	//переменная хранящая в себе глобальнео пространство имен
	s4g_table* curr_vars;	//текущее установленное пространство имен, есл выполняется функция то пространство имен функции
	Stack<s4g_value*> execute;	//стек выполнения команд

	int error;	//есть ли ошибка? -1 есть, остальное все в норме
	char strerror[1024];	//строка ошибки

	s4g_arr_lex* arr_lex;
	s4g_gc* gc;
	s4g_main* s4gm;
	
	bool cfetchget;		//была ли предыдущая команда либо fetch либо fetch_get
	bool cfetchgetarg;	//была ли предыдущая команда fetch_get с аргументом или без
	int cfetchpushstore;//следовалаи комбинаци fetch push store, если да то значит = 3, нужно для com_store

	Stack<s4g_call_data> callstack;	//стэк вызовов с сохраненным предыдущим состоянием
};
