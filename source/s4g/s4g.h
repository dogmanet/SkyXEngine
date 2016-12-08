
#define S4G_MAX_LEN_VAR_NAME 64
#define S4G_MAX_LEN_STR_IN_FILE 1024

#define mem_delete(data) delete data;data = 0;
#define mem_delete_a(data) delete[] data;data = 0;

#include <s4g\array.h>
#include <s4g\stack.h>
#include <s4g\aatable.h>

#define S4G_MAX_CALL 200	//максимальное количество вызовов (рекурсивны и вложенных)
#define S4G_GLOBAL_NM "_g"	//обращение в скрипте к глобальному пространству имен
#define S4G_MARG "args"

//настраиваемые типы
#define s4g_int long
#define s4g_uint unsigned long
#define s4g_float float
#define s4g_bool short int

extern class s4g_main;
extern class s4g_vm;
//тип си(++) функции
typedef int(*s4g_c_function) (s4g_main* vm);

//все типы данных которые могут быть использованы в скриптах
enum s4g_type
{
	t_null = 0,
	t_table = 1,
	t_string = 2,
	t_float = 3,
	t_int = 4,
	t_uint = 5,
	t_bool = 6,
	t_user_data = 7, //unsupported
	t_cfunc = 8,	//unsupported
	t_sfunc = 9,
	t_nn = 10,
};

extern class s4g_arr_lex;
extern class s4g_preprocessor;
extern struct s4g_node;
extern struct s4g_builder_syntax_tree;
extern class s4g_compiler;

extern class s4g_table;
extern class s4g_value;
extern struct s4g_command;
extern struct s4g_s_function;
extern struct s4g_context;
extern class s4g_gc;


//основа взаимоествия
struct s4g_main
{
	s4g_main();
	s4g_arr_lex* arr_lex;	//лексический анализатор
	s4g_node* gnode;		//построенное аст
	s4g_builder_syntax_tree* bst;	//строитель аст
	Stack<s4g_command>* commands;	//массив с байт кодом
	s4g_compiler* compiler;			//компилятор
	s4g_vm* vmachine;				//виртуальная машина
	s4g_gc* gc;						//сборщик мусора
	char strerror[1024];			//сообщение об ошибке
};

//представление данных
struct s4g_data
{
	s4g_data();
	~s4g_data();
	void* data;	//указатель на хранимые данные
	int ref;	//количество ссылающихся переменных на эти данные
	s4g_type type;	//тип хранимый в data
	int typedata;	//тип данных для сборщика мусора, 0 - удалять если надо, 1 - нельзя удалять
};

//переменная
struct s4g_value
{
	s4g_value(long lexid = -1);
	~s4g_value();
	
	bool isdelete;	//можно ли удалять переменную
	int typedata;	//тип данных для сборщика мусора, 0 - удалять если надо, 1 - нельзя удалять
	long nlexid;	//идентификатор лексемы которая сгенерировала создание переменной
	long iddata;	//идентификатор s4g_data из массива управляемого сборщиком мусора
	long idvar;		//идентификатор этой переменной в массиве управляемым сборщиком мусора
};

//таблица
class s4g_table
{
public:
	s4g_table();
	~s4g_table()
	{
		for (int i = 0; i < arr_value.GetSize(); i++)
		{
			arr_value[i]->isdelete = true;
		}
	}

	inline s4g_value* getn(DWORD key);		//получить переменную по порядковому номеру
	inline s4g_value* gets(const char* str);	//получить переменную по строке
	inline bool is_exists_s(const char* str);	//существует ли переменная с данным именем
	inline bool is_exists_n(DWORD key);	//существует ли переменная по указанному ключу

	inline void add_val_s(const char* name, s4g_value* val, long lexid);	//добавить переменную и присовить ей имя в текущей таблице
	inline void add_val_n(long num, s4g_value* val, long lexid);			//добавить переменную по ключу
	inline void add_val(s4g_value* val, long lexid);						//добавить переменную в конец таблицы

	inline long size();						//размер таблицы в элементах
	inline const char* get_name_id(long id);//получить имя по id

protected:
	AATable<s4g_value> arr_value;
};

//сборщик мусора
class s4g_gc
{
public:
	s4g_gc();

	//создание переменных
	inline s4g_value* cr_val_null(long lexid);
	inline s4g_value* cr_val_table_null(long lexid);
	inline s4g_value* cr_val_int(s4g_int num, long lexid);
	inline s4g_value* cr_val_uint(s4g_uint num, long lexid);
	inline s4g_value* cr_val_float(s4g_float num, long lexid);
	inline s4g_value* cr_val_bool(s4g_bool bf, long lexid);
	inline s4g_value* cr_val_str(const char* str, long lexid);
	inline s4g_value* cr_val_table(s4g_table* tt, long lexid);
	inline s4g_value* cr_val_s_func(s4g_s_function* func, long lexid);
	inline s4g_value* cr_val_c_func(s4g_c_function func, long lexid);
	inline s4g_value* cr_val(int _type, const char* _val, long lexid);	//создать переменную из _val с типом _type
	inline s4g_value* cr_val_nn(long lexid);	//создать цифру 0 (для случаев кода: -123 будет 0-123)

	inline void c_val(s4g_value* dest, s4g_value* src, bool incr = true);	//присвоить dest данные из src
	inline void set_td_data(s4g_value* val, int td);	//присвоить данным на которые ссылается перменные тип для сборщика

	//получить значения из пременных
	inline s4g_int get_int(s4g_value* val);
	inline s4g_uint get_uint(s4g_value* val);
	inline s4g_float get_float(s4g_value* val);
	inline const char* get_str(s4g_value* val);
	inline s4g_table* get_table(s4g_value* val);
	inline s4g_s_function* get_s_func(s4g_value* val);
	inline s4g_c_function get_c_func(s4g_value* val);
	inline s4g_bool get_bool(s4g_value* val);

	inline s4g_type get_type(s4g_value* val);	//получить тип переменной

	//работа с контекстами
	inline long add_new_context(s4g_table** tt);	//создать и добавить новый контекст, возвращает id контекста, а в tt записывает указатель на таблицу
	inline long add_context(s4g_table* tt);			//добавить контекст основанный на таблице, возвращает id добавленного контекста
	inline void activate_prev(long lastidctx);		//активирует предыдущие контексты до lastidctx (это значение было получено при вызове deactivate_prev)
	inline long deactivate_prev();					//деактивировать все предыдущие контексты, возвращает номер контекста который деактивирован последним
	inline void del_context(long id);				//пометить контекст как ненужный и при сборке мусора снести все с него
	inline void remove_context(long id);			//пометить контекст как не нужный и при сборке удалить только контекст но не его данные
	inline void set_ctx_for_del(s4g_context* ctx);	//добавить контекст для удаления

	//существует ли переменная с именем str во всех предыдущих контекстах
	//если да то возвращает id контекста в котором найдено, в val записывает указатель на переменную
	inline long ctx_is_exists_s(const char* str, s4g_value** val);	
	
	//запустить сборку мусора
	void clear();

	int typedata;	//тип создаваемых данных, 0 - если больше не нужны то удалть, 1 - не удалять
	//парсер и компилер создают данные которые не удаляются, машина в большинстве случаев создает данные подлежащие удалению

protected:
	inline long GetNewIDVal();	//получить свободный ключ для записи переменной, если нет свободных мест то вернуть -1
	inline long GetNewIDData();	//получить свободный ключ для записи значения, если нет свободных мест то вернуть -1
	Array<s4g_value*> arrvar;	//массив переменных
	Array<s4g_data*> arrdata;	//массив данных
	Array<s4g_context*> oldarrcontexts;	//массив контекстов которые уже свое отработали и нуждаются в удалении
	Array<s4g_context*> arrcurrcontexts;//массив подключенных в данный момент контекстов
};

//тип функция
struct s4g_s_function
{
	s4g_s_function(){ externs = 0; ismarg = false; }
	~s4g_s_function(){ mem_delete(externs); }
	s4g_table* externs;	//подстановка переменных и предыдщуего контекста
	Stack<String> externs_strs;	//имена переменных для подстановки из предыдущего контекста
	Stack<String> args;	//имена аргументов
	Stack<s4g_command> commands; //опкод
	bool ismarg; //принимает ли функция перенное количество аргументов?
};

//контекст содержащий в себе все переменные текущего исполнения
struct s4g_context
{
	s4g_context(){ table = 0; valid = false; }
	~s4g_context() {}
	s4g_table* table;	//таблица с переменными
	bool valid;			//рабочий ли контекст в данный момент?
	//это нужно в случе когда вызывается функция, вызываемая блокирует все предыдущие контексты, после отработки разблокирует
};

//////

s4g_main* s4g_init();

void s4g_push_c_func(s4g_main* sm,s4g_c_function func);
void s4g_store_g(s4g_main* sm, const char* name, s4g_c_function func);

int s4g_load_file(s4g_main* s4gm,const char* file);
//int s4g_call(s4g_machine* sm,int narg,int result);