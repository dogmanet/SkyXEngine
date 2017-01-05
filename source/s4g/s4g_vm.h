
#ifndef s4g_vm_h
#define s4g_vm_h

enum s4g_vm_command
{
	mc_halt = 0,	//остановить текущее выполнение
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
	mc_preincr,
	mc_predecr,
	mc_postincr,
	mc_postdecr,
	mc_new_table,	//создать таблицу и положить на вершину стека
	mc_add_in_table,//добавить в таблицу котоаря лежит на вершине
	mc_append_table,//добавить новый элемент в конец таблицы и положить его на вершину
	mc_precall,		//сообщение о том что вскоре будет вызвана функция, записать в первый свободный регистр размер стека
	mc_call,		//вызов функции

	mc_block_new,
	mc_block_del,

	//переход относительный
	mc_jz,
	mc_jnz,
	mc_jmp,

	//переход абсолютный
	//mc_jza,
	//mc_jnza,
	//mc_jmpa,

	mc_mod,
	mc_log_and,
	mc_log_or,
	mc_bit_xor,
	mc_bit_not,
	mc_bit_shiftr,
	mc_bit_shiftl,
	mc_log_neqt,
	mc_log_eq,
	mc_log_neq,
	mc_log_ge,
	mc_log_le,
	mc_log_gt,
	mc_log_lt,
	mc_log_not,
	mc_log_eqt,
	mc_bit_and,
	mc_bit_or,

	mc_last
};

#define S4G_VM_OP_ARIF_ERROR_TYPE1(tval1) \
	s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com).lexid - 1);\
	error = -1;\
	char strtype[12];\
	s4g_get_str_type(tval1->pdata->type, strtype); \
	sprintf(this->strerror, "[%s]:%d - attempt to perform arithmetic on arg #1 '%s' (a '%s' value)", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str, strtype);\
	return;

#define S4G_VM_OP_ARIF_ERROR_TYPE2(tval2) \
	s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com-1).lexid); \
	error = -1; \
	char strtype[12]; \
	s4g_get_str_type(tval2->pdata->type, strtype); \
	sprintf(this->strerror, "[%s]:%d - attempt to perform arithmetic on arg #2 '%s' (a '%s' value)", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str, strtype); \
	return;

#define S4G_VM_OP_ARIF_ERROR_UNSUN_UNRES(tval2) \
	s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com - 1).lexid); \
	error = -1; \
	char strtype[12]; \
	s4g_get_str_type(tval2->pdata->type, strtype); \
	sprintf(this->strerror, "[%s]:%d - '%s' unary symbol is unresolved to '%s' type", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str, strtype); \
	return;

#define S4G_VM_OP_ARIF_INCR_DECR_ERR(tval) \
	s4g_lexeme* tmplexs = this->arr_lex->get(curr_comm->get(id_curr_com - 1).lexid); \
	error = -1; \
	char strtype[12]; \
	s4g_get_str_type(tval->pdata->type, strtype); \
	sprintf(this->strerror, "[%s]:%d - '%s' expected number type, but got '%s' type", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str, strtype); \
	return;


#include <s4g\s4g_compiler.h>

//структура описывающая сохраненное предыдущее состояние при вызове функции и содержащее имя вызыванной функции
struct s4g_call_data
{
	s4g_call_data()
	{
		coms = 0;vars = 0;cfetchget = 3;cfetchgetarg = false;cfetchpushstore = 0;
		id_curr_com = lastidctx = idnewctx = idexternctx = -1; valf = 0; stack_size = -1;//namef[0] = 0;
	}

	s4g_call_data(s4g_stack<s4g_command>* _coms, s4g_table* _vars, int _cfetchget, bool _cfetchgetarg, int _cfetchpushstore, 
				long _id_curr_com, long _lastidctx, long _idnewctx, long _idexternctx, long _stack_size, s4g_value* _valf)
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
		stack_size = _stack_size;
		_valf = valf;
		/*if (_namef)
			strcpy(namef,_namef);*/
	}

	~s4g_call_data(){}

	s4g_stack<s4g_command>* coms;	//команды выполнения
	s4g_table* vars;				//таблица с переменными (окружением)
	int cfetchget;				
	bool cfetchgetarg;	
	int cfetchpushstore;
	long id_curr_com;
	long lastidctx;
	long idnewctx;
	long idexternctx;
	long stack_size;
	s4g_value* valf;
	//char namef[S4G_MAX_LEN_VAR_NAME];	// имя функции которая вызвалась и спровоцировала сохранение текущего состяния
};

class s4g_vm
{
public:
	
	s4g_vm(s4g_gc* _gc);
	~s4g_vm();

	int run(s4g_stack<s4g_command>* commands, s4g_table* vars);

	//функции для исполнения байт кода
	inline void com_fetch();
	inline void com_fetch_get();
	inline void com_store();
	inline void com_end();
	inline void com_mstore();
	inline void com_add_in_table();
	inline void com_append_table();
	inline void com_precall();
	inline void com_call();
	inline void com_new_table();

	inline void com_retprev();	//возвращаем предыдущее состояние машины, в случае когда функция отработала все свои команды

	inline void com_add();
	inline void com_sub();
	inline void com_mul();
	inline void com_div();

	inline void com_halt();
	inline void com_push();
	inline void com_pop();

	inline void com_jz();
	inline void com_jnz();
	inline void com_jmp();

#define GEN_OP(op) inline void com_ ## op();
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

	//inline void com_jza();
	//inline void com_jnza();
	//inline void com_jmpa();
	//

	inline void com_block_new();
	inline void com_block_del();
	inline void com_preincr();
	inline void com_postincr();

	inline void com_predecr();
	inline void com_postdecr();

	typedef void(s4g_vm::*opfunc) ();
	opfunc arropf[mc_last];

	const char* get_curr_file(char* str=0);
	long get_curr_str(char* str = 0);
	const char* get_curr_func(char* str = 0);

	String strstacktrace;
	const char* stack_trace();

	s4g_stack<s4g_command>* curr_comm;
	long id_curr_com;

	
	s4g_table* gvars;	//глобальное пространство имен _g
	s4g_value* vgvars;	//переменная хранящая в себе глобальное пространство имен
	s4g_table* curr_vars;	//текущее установленное пространство имен, есл выполняется функция то пространство имен функции
	s4g_stack<s4g_value*, S4G_RESERVE_STACK_EXE> execute;	//стек выполнения команд

	int error;	//есть ли ошибка? -1 есть, остальное все в норме
	char strerror[1024];	//строка ошибки

	s4g_arr_lex* arr_lex;
	s4g_gc* gc;
	s4g_main* s4gm;

	s4g_vm_command oldop;
	s4g_vm_command op;
	s4g_value* arg;
	long val_end;		//
	bool runexe;		//можно ли выполнять код (внутренее использование)
	s4g_stack<s4g_value*, 64> stackarg;
	
	int cfetchget;		//была ли предыдущая команда либо fetch либо fetch_get
	bool cfetchgetarg;	//была ли предыдущая команда fetch_get с аргументом или без
	int cfetchpushstore;//следовала ли комбинаци fetch push store, если да то значит = 3, нужно для com_store

	s4g_stack_register<int, S4G_MAX_CALL+1> sr;//стек регистров, хранит размер стека исполнения на момент добавления значения в регистр
	s4g_stack<s4g_call_data*> callstack;	//стэк вызовов с сохраненным предыдущим состоянием

	s4g_stack<s4g_call_data*> blockstack;	//стэк вызовов с сохраненным предыдущим состоянием

	//рабочие данные
	const char* str;
	char str2[S4G_MAX_LEN_VAR_NAME];
	s4g_value* tvalfunc;
	s4g_value* tmpval;
	s4g_value* tvalue;
	s4g_value* tvalue2;
	s4g_type ttype;
	long idctx;
	s4g_s_function* csfunc;
	s4g_c_function tcfunc;
	s4g_call_data* tmpcd;
	bool is_cr;
	s4g_table* ttable;
	bool jmp;
	s4g_command * currCom;
};

#endif