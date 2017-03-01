
#ifndef s4g_main_h
#define s4g_main_h

#if defined(_WINDOWS)
#define dbg_break _asm { int 3 }
#else
#define dbg_break asm("int $3");
#endif

#define mem_delete(data) delete data;data = 0;
#define mem_delete_a(data) delete[] data;data = 0;

#include "s4g_stack.h"
#include <core/MemAlloc.h>
#include <core/array.h>
#include <core/assotiativearray.h>

#define S4G_GC_TYPE_DATA_FREE 0		//простые публичные данные
#define S4G_GC_TYPE_DATA_PRIVATE 1	//приватные данные, доступны только одной переменной
#define S4G_GC_TYPE_DATA_SYS 2		//системные публичные данные (созданные при парсинге и компилировании)

#define S4G_GC_TYPE_VAR_DEL -1	//переменная подлежит удалению
#define S4G_GC_TYPE_VAR_FREE 0	//простая переменная
#define S4G_GC_TYPE_VAR_SYS 1	//системная перменная

//ключи доступа к основным данным
#define S4G_GC_KEY_NULL 0	//null
#define S4G_GC_KEY_BTRUE 1	//bool true
#define S4G_GC_KEY_BFALSE 2	//bool false
#define S4G_GC_KEY_NNULL 3	//nnull

class s4g_arr_lex;
struct s4g_node;
struct s4g_builder_syntax_tree;
class s4g_compiler;

struct s4g_command;
struct s4g_s_function;
struct s4g_context;
class s4g_gc;
class s4g_vm;

const char* s4g_get_str_type(s4g_type tt, char* str_type);
void s4g_gen_msg(s4g_main* s4gm, int level, const char* format, ...);

#define S4G_COND_TYPE1(s4gm, val,retval) if (!val){s4g_gen_msg(s4gm, S4G_ERROR, "value is not init, api function [%s]",__FUNCTION__);	return retval;}
#define S4G_COND_TYPE2(s4gm, val,retval) if (!val->pdata){s4g_gen_msg(s4gm, S4G_ERROR, "value is has not data, api function [%s]",__FUNCTION__);	return retval;}
#define S4G_COND_TYPE_12(s4gm, val,retval) S4G_COND_TYPE1(s4gm, val,retval) S4G_COND_TYPE2(s4gm, val,retval)

//основа взаимодествия
struct s4g_main
{
	s4g_main(const char* _name);
	~s4g_main();
	s4g_arr_lex* arr_lex;	//лексический анализатор
	s4g_node* gnode;		//построенное аст
	s4g_builder_syntax_tree* bst;	//строитель аст
	s4g_stack<s4g_command>* commands;	//массив с байт кодом
	s4g_compiler* compiler;			//компилятор
	s4g_vm* vmachine;				//виртуальная машина
	s4g_gc* gc;						//сборщик мусора
	char strerror[1024];			//сообщение об ошибке
	char name[S4G_MAX_LEN_VAR_NAME];
	int create_var;
};

//представление данных
struct s4g_data
{
	s4g_data();
	~s4g_data();

	union
	{
		void* p;
		s4g_int i;
		s4g_uint ui;
		s4g_float f;
		s4g_bool b;
	} data;

	int ref;	//количество ссылающихся переменных на эти данные
	s4g_type type;	//тип хранимый в data
	int typedata;	//тип данных для сборщика мусора, 0 - удалять если надо, 1 - нельзя удалять
	long iddata;	//ключ по которому можно обратится к массиву arrdata и найти эту структуру
};

//переменная
struct s4g_value
{
	s4g_value();
	~s4g_value();

	int typedata;	//тип данных для сборщика мусора, 0 - удалять если надо, 1 - нельзя удалять
	//long iddata;	//идентификатор s4g_data из массива управляемого сборщиком мусора
	long idvar;		//идентификатор этой переменной в массиве управляемым сборщиком мусора
	char name[S4G_MAX_LEN_VAR_NAME];
	//только для внутреннего использования!!!
	s4g_data* pdata;
};

//таблица
class s4g_table
{
public:
	s4g_table();
	~s4g_table();

	inline void clear();	//очистить таблицу

	inline s4g_value* getn(long key);			//получить переменную по порядковому номеру
	inline s4g_value* gets(const char* str);	//получить переменную по строке
	inline int is_exists_s(const char* str);	//существует ли переменная с данным именем
	inline bool is_exists_n(long key);			//существует ли переменная по указанному ключу

	inline long is_exists_s2(const char* str, s4g_value** tval);	//существует ли переменная с данным именем
	inline bool is_exists_n2(long key, s4g_value** tval);	//существует ли переменная по указанному ключу

	inline long get_key(const char* name);	//получить ключ по имени

	inline void add_val_s(const char* name, s4g_value* val);	//добавить переменную и присовить ей имя в текущей таблице
	inline void add_val_n(long num, s4g_value* val);			//добавить переменную по ключу
	inline void add_val(s4g_value* val);						//добавить переменную в конец таблицы

	inline long size();						//размер таблицы в элементах
	inline const char* get_name_id(long id);//получить имя по id

	inline void reserve(int count_elem);	//зарезервировать место в таблице под count_elem ключей

	inline s4g_value * cr_if_not_exists(const char* str, s4g_gc * gc);
	inline void remove(const char * str);
	inline void remove(long key);
//protected:
	struct item_name
	{
		mutable char Name[S4G_MAX_LEN_VAR_NAME];
		mutable const char * tmpName;
		__forceinline bool operator==(const item_name & str) const
		{
			return(strcmp(tmpName ? tmpName : Name, str.tmpName ? str.tmpName : str.Name) == 0);
		}

		__forceinline bool operator<(const item_name & str) const
		{
			return(strcmp(tmpName ? tmpName : Name, str.tmpName ? str.tmpName : str.Name) < 0);
		}

		__forceinline void SetName(const char * str)const
		{
			strcpy(Name, str);
			tmpName = NULL;
		}

		__forceinline const char * GetName() const
		{
			return(tmpName ? tmpName : Name);
		}

		__forceinline item_name(const char * str)
		{
			tmpName = str;
			Name[0] = 0;
		}

		__forceinline item_name()
		{
			tmpName = NULL;
			Name[0] = 0;
		}
	};
	struct table_desc
	{
		const item_name * name;
		s4g_value* Value;
	};
	long count_obj;
	Array<table_desc*, S4G_RESERVE_TABLE_ELEM> Arr;
	MemAlloc<table_desc, S4G_RESERVE_TABLE_ELEM> Mem;
	typedef AssotiativeArray<item_name, long> IndexType;
	typedef const IndexType::Node * IndexNode;
	IndexType NameIndex;
};


//сборщик мусора
class s4g_gc
{
public:
	s4g_gc();
	~s4g_gc();

#define def_cr_val_null(tval,tdata,val_name) \
	tval = MemValue.Alloc(); \
	if (val_name)\
		strcpy(tval->name, val_name); \
	else\
		strcpy(tval->name, "#"); \
	tval->idvar = arrvar.count_obj; \
	arrvar.push(tval); \
	tdata = MemData.Alloc(); \
	/*tval->iddata = arrdata.count_obj;*/ \
	tdata->iddata = arrdata.count_obj; \
	tval->pdata = tdata; \
	arrdata.push(tdata);

	//создание переменных
	//name - имя переменной если надо
	inline s4g_value* cr_val_null(const char* name = 0, int td_val = S4G_GC_TYPE_VAR_FREE);
	inline s4g_value* get_val_null();
	inline s4g_value* cr_val_pdata(s4g_pdata pdata, const char* name = 0, int td_val = S4G_GC_TYPE_VAR_FREE);
	inline s4g_value* cr_val_table_null(const char* name = 0, int td_val = S4G_GC_TYPE_VAR_FREE, int td_data = S4G_GC_TYPE_DATA_FREE);
	inline s4g_value* cr_val_int(s4g_int num, const char* name = 0, int td_val = S4G_GC_TYPE_VAR_FREE, int td_data = S4G_GC_TYPE_DATA_FREE);
	inline s4g_value* cr_val_uint(s4g_uint num, const char* name = 0, int td_val = S4G_GC_TYPE_VAR_FREE, int td_data = S4G_GC_TYPE_DATA_FREE);
	inline s4g_value* cr_val_float(s4g_float num, const char* name = 0, int td_val = S4G_GC_TYPE_VAR_FREE, int td_data = S4G_GC_TYPE_DATA_FREE);
	inline s4g_value* cr_val_bool(s4g_bool bf, const char* name = 0, int td_val = S4G_GC_TYPE_VAR_FREE);
	inline s4g_value* cr_val_str(const char* str, const char* name = 0, int td_val = S4G_GC_TYPE_VAR_FREE, int td_data = S4G_GC_TYPE_DATA_FREE);
	inline s4g_value* cr_val_table(s4g_table* tt, const char* name = 0, int td_val = S4G_GC_TYPE_VAR_FREE);
	inline s4g_value* cr_val_s_func(const char* name = 0, int td_val = S4G_GC_TYPE_VAR_FREE, int td_data = S4G_GC_TYPE_DATA_FREE);
	inline s4g_value* cr_val_s_func(s4g_s_function* func, const char* name = 0,int td_val = S4G_GC_TYPE_VAR_FREE);
	inline s4g_value* cr_val_c_func(s4g_c_function func, const char* name = 0,int td_val = S4G_GC_TYPE_VAR_FREE);
	inline s4g_value* cr_val(int _type, const char* _val, const char* name = 0, int td_val = S4G_GC_TYPE_VAR_FREE, int td_data = S4G_GC_TYPE_DATA_FREE);	//создать переменную из _val с типом _type
	inline s4g_value* cr_val_nn();	//создать цифру 0 (для случаев кода: -123 будет 0-123)

	inline s4g_value* get_bool(bool bf);
	inline s4g_value* cr_val2(s4g_value* val, int td_val = S4G_GC_TYPE_VAR_FREE, int td_data = S4G_GC_TYPE_DATA_FREE, bool copy_data = false);

	inline s4g_data* cr_dara(s4g_data* tdata, int td_data = S4G_GC_TYPE_DATA_FREE);
	inline s4g_data* cr_dara_int(s4g_int num, int td_data = S4G_GC_TYPE_DATA_FREE);
	inline s4g_data* cr_dara_uint(s4g_uint num, int td_data = S4G_GC_TYPE_DATA_FREE);
	inline s4g_data* cr_dara_float(s4g_float num, int td_data = S4G_GC_TYPE_DATA_FREE);

	inline void c_val(s4g_value* dest, s4g_value* src, bool incr = true);	//присвоить dest данные из src, incr - увеличивать ли счетчик ссылок на src
	inline void set_td_data(s4g_value* val, int td);	//присвоить данным на которые ссылается перменная тип для сборщика

	//получить значения из пременных
	inline s4g_int get_int(s4g_value* val);
	inline s4g_uint get_uint(s4g_value* val);
	inline s4g_float get_float(s4g_value* val);
	inline const char* get_str(s4g_value* val);
	inline String* get_str2(s4g_value* val);
	inline s4g_table* get_table(s4g_value* val);
	inline s4g_s_function* get_s_func(s4g_value* val);
	inline s4g_c_function get_c_func(s4g_value* val);
	inline s4g_bool get_bool(s4g_value* val);
	inline s4g_pdata get_pdata(s4g_value* val);

	inline s4g_type get_type(s4g_value* val);	//получить тип переменной

	//работа с контекстами
	inline void add_mem_contexts();	//выделить дополнительную память для контекстов
	inline long add_new_context(s4g_table** tt, int status_valid = 1);	//создать и добавить новый контекст, возвращает id контекста, а в tt записывает указатель на таблицу
	inline long add_context(s4g_table* tt, int status_valid = 1);		//добавить контекст основанный на таблице, возвращает id добавленного контекста, используется для замыканий
	inline void activate_prev(long lastidctx);		//активирует предыдущие контексты до lastidctx (это значение было получено при вызове deactivate_prev)
	inline long deactivate_prev();					//деактивировать все предыдущие контексты, возвращает номер контекста который деактивирован последним
	inline void del_top_context(bool clear);		//пометить контекст как ненужный и при сборке мусора снести все с него
	inline void remove_context(long id);			//заменить тавблицу в контексте на контекстную таблицу, ибо если этот вызов произошел то у нас стояла замыкающая таблица
	inline void add_const_context();				//добавить константный(всегда доступный) контекст, используется для бибилиотек языка
	inline bool add_in_const_ctx(const char* name, s4g_value* val);
	inline s4g_value* get_out_const_ctx(const char* name);
	//существует ли переменная с именем str во всех предыдущих контекстах
	//если да то возвращает id контекста в котором найдено, в val записывает указатель на переменную
	//если не найдено то возвращает -1
	inline long ctx_is_exists_s(const char* str, s4g_value** val);

	//запустить сборку мусора
	inline void del_data(s4g_data* tdata);
	void clear();

	inline void begin_of_const_data();	//старт создания константных значений (при загрузке скрипта)
	inline void end_of_const_data();	//окончание создания константных значений (окончание загрузки скрипта)

	inline long get_count_mem_busy();
	inline long get_count_mem_allocated();

protected:
	friend class s4g_main;
	friend class s4g_vm;
	s4g_main* s4gm;

	s4g_stack<s4g_value*, S4G_RESERVE_VALUE> arrvar;//массив переменных
	s4g_stack<s4g_data*, S4G_RESERVE_DATA> arrdata;	//массив данных
	s4g_stack<s4g_context*, S4G_RESERVE_CONTEXTS> arrcurrcontexts;//массив подключенных в данный момент контекстов
	long curr_num_top_ctx = 0;	//текущий ключ верхнего контекста

	//резервация памяти
	MemAlloc<s4g_value, S4G_RESERVE_VALUE_MEM> MemValue;	//для значений
	MemAlloc<s4g_data, S4G_RESERVE_DATA_MEM> MemData;		//для данных
	MemAlloc<s4g_context, S4G_RESERVE_CONTEXTS_MEM> MemCtx;	//для контекстов
	MemAlloc<String, S4G_RESERVE_STRING_MEM> MemString;		//для строк
	MemAlloc<s4g_s_function, S4G_RESERVE_SFUNC_MEM> MemSFunc;//для скриптовых функций
	MemAlloc<s4g_table, S4G_RESERVE_TABLE_MEM> MemTable;	//для таблиц
	long id_const_ctx;
	long count_nd_data;	//последний ключ в таблице с данными + 1 (для того чтобы не анализиировать константные данные при сборке мусора)
	long count_nd_value;//последний ключ в таблице со значениями + 1 (для того чтобы не анализиировать константные значения при сборке мусора)
};

//тип функция
struct s4g_s_function
{
	s4g_s_function(){ externstable = 0; ismarg = false; marg_val = 0; margtable = 0; }
	~s4g_s_function(){}
	s4g_value* externs_val;				//значение для externs
	s4g_table* externstable;			//подстановка переменных и предыдщуего контекста
	s4g_stack<String> externs_strs;		//имена переменных для подстановки из предыдущего контекста
	s4g_stack<String> args;				//имена аргументов
	s4g_stack<s4g_command> commands;	//опкод
	bool ismarg;						//принимает ли функция перенное количество аргументов?
	s4g_value* marg_val;				//значение для margtable
	s4g_table* margtable;				//подстановка переменных и предыдщуего контекста
};

//контекст содержащий в себе все переменные текущего исполнения
struct s4g_context
{
	s4g_context(){ table = 0; valid = false; oldtable = 0; }
	~s4g_context() {}
	s4g_table* table;	//таблица с переменными
	s4g_table* oldtable;//таблица с переменными
	int valid;			//рабочий ли контекст в данный момент? 0 - нет, 1 да, 2 постоянно доступный
	//это нужно в случе когда вызывается функция, вызываемая блокирует все предыдущие контексты, после отработки разблокирует
};

#endif