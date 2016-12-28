
#ifndef s4g_h
#define s4g_h

extern "C"
{
#include "string.cpp"
#define S4G_VERSION "0.0"
	
#if defined(S4G_BUILD_DLL)
	#if defined(S4G_EXE)
	#define S4G_API __declspec (dllimport)
	#elif defined(S4G_DLL)
	#define S4G_API  __declspec (dllexport)
	#endif
#else
#define S4G_API extern
#endif

//все типы данных которые могут быть использованы в скриптах
enum s4g_type
{
	t_none = -1,
	t_null = 0,
	t_table = 1,
	t_string = 2,
	t_float = 3,
	t_int = 4,
	t_uint = 5,
	t_bool = 6,
	t_pdata = 7, 
	t_cfunc = 8,	
	t_sfunc = 9,
	t_nnull = 10,	//цифра 0, вставляется в выражения с унарным минусом
};

struct s4g_main;
struct s4g_value;
struct s4g_table;

#define S4G_MAX_LEN_TYPE_NAME 64
#define S4G_MAX_LEN_VAR_NAME 64
#define S4G_MAX_LEN_STR_IN_FILE 1024

#define S4G_MAX_CALL 1000	//максимальное количество вызовов (рекурсивных и вложенных)
#define S4G_GLOBAL_NM "_g"	//обращение в скрипте к глобальному пространству имен
#define S4G_MARG "args"
#define S4G_RESERVE_STACK_EXE 1000
#define S4G_ADD_MEM_CONTEXTS 16

#define S4G_RESERVE_VALUE 10000
#define S4G_RESERVE_VALUE_MEM 10000

#define S4G_RESERVE_DATA 10000
#define S4G_RESERVE_DATA_MEM 10000

#define S4G_RESERVE_CONTEXTS 100
#define S4G_RESERVE_CONTEXTS_MEM 100

#define S4G_RESERVE_STRING_MEM 1000
#define S4G_RESERVE_SFUNC_MEM 1000
#define S4G_RESERVE_TABLE_MEM 1000

#define S4G_NOTICE 0
#define S4G_WARNING 1
#define S4G_ERROR 2

#define s4g_breakpoint _asm { int 3 }

//настраиваемые типы
#define s4g_int long
#define s4g_uint unsigned long
#define s4g_float float
#define s4g_bool short int
#define s4g_pdata void*

//тип си(++) функции
//если функция возвращает значение меньше нуля то значит произошла ошибка и машина остановит выполнение кода
//все остальные значения (>0) не имеют смысла
typedef int(*s4g_c_function) (s4g_main* vm);

//тип функции вывода информации и обработки ошибок
typedef void(*s4g_report_func) (s4g_main* s4gm, int level, const char* name_ss, const char* format, ...);

//строковое представление типов
const char s4g_str_type[][S4G_MAX_LEN_TYPE_NAME] = {
"none",
"null",
"table",
"string",
"float",
"int",
"uint",
"bool",
"pointer_data",
"cfunc",
"sfunc",
"nnull"
};

/////

//возвращает строковое представление типа tt в str_type
S4G_API const char* s4g_get_str_type(s4g_type tt, char* str_type = 0);

S4G_API s4g_main* s4g_init(const char* name);//инициализируем скриптовую систему
S4G_API void s4g_kill(s4g_main* s4gm);//завершаем работу скриптовой системы
S4G_API int s4g_load_file(s4g_main* s4gm, const char* file);//загрузить скрипт из файла
S4G_API int s4g_load_str(s4g_main* s4gm, const char* str);//загрузить скрипт из строки

S4G_API void s4g_set_rf(s4g_report_func rf);
S4G_API void s4g_gen_msg(s4g_main* s4gm, int level, const char* format, ...);
//вызываем сборку мусора, возвращает количество занимаемой памяти
S4G_API int s4g_call_gc(s4g_main* s4gm);

//функции для вставки на вершину стека значения
S4G_API void s4g_spush_table_null(s4g_main* s4gm, int count_elem);
S4G_API void s4g_spush_c_func(s4g_main* s4gm, s4g_c_function func);
S4G_API void s4g_spush_int(s4g_main* s4gm, s4g_int num);
S4G_API void s4g_spush_uint(s4g_main* s4gm, s4g_uint num);
S4G_API void s4g_spush_float(s4g_main* s4gm, s4g_float num);
S4G_API void s4g_spush_str(s4g_main* s4gm, const char* str);
S4G_API void s4g_spush_bool(s4g_main* s4gm, s4g_bool bf);
S4G_API void s4g_spush_pdata(s4g_main* s4gm, s4g_pdata pdata);
S4G_API void s4g_spush_null(s4g_main* s4gm);
S4G_API void s4g_spush_precall(s4g_main* s4gm);
S4G_API void s4g_spush_value(s4g_main* s4gm, s4g_value* val);

//функции сохранения в пространства имен
//после сохранения функция выталкивает сохраненное значение с вершины стека
S4G_API void s4g_sstore_g(s4g_main* s4gm, const char* name); //в глобальном пространстве, name имя которое будет присвоено значению на вершине стека
S4G_API void s4g_sstore(s4g_main* s4gm, int index, const char* name);// index в стеке должна быть таблица, name имя переменной, а значение берется с вершины стека
//table(stack[index])[name] = stack[-1];

//получить значения
//после получения, функция ложит на вершину стека полученное значение
S4G_API void s4g_sget_g(s4g_main* s4gm, const char* name);	//из глобального, name - имя переменной значение которой берем
S4G_API void s4g_sget(s4g_main* s4gm, int index, const char* name);// index в стеке должна быть таблица, name имя переменной
//stack.push(table(stack[index])[name]);

//является ли значение переменной в стеке по номеру index типом? 0 нет, 1 да
S4G_API int s4g_sis_int(s4g_main* s4gm, int index);
S4G_API int s4g_sis_uint(s4g_main* s4gm, int index);
S4G_API int s4g_sis_float(s4g_main* s4gm, int index);
S4G_API int s4g_sis_str(s4g_main* s4gm, int index);
S4G_API int s4g_sis_bool(s4g_main* s4gm, int index);
S4G_API int s4g_sis_c_func(s4g_main* s4gm, int index);
S4G_API int s4g_sis_s_func(s4g_main* s4gm, int index);
S4G_API int s4g_sis_table(s4g_main* s4gm, int index);
S4G_API int s4g_sis_pdata(s4g_main* s4gm, int index);
S4G_API int s4g_sis_null(s4g_main* s4gm, int index);

//возвращает тип значения переменной по номеру в стеке index
S4G_API s4g_type s4g_sget_type(s4g_main* s4gm, int index);

//возвращает приведенное к определнному типу значнеие перменной по номеру index в стеке
S4G_API s4g_int s4g_sget_int(s4g_main* s4gm, int index);
S4G_API s4g_uint s4g_sget_uint(s4g_main* s4gm, int index);
S4G_API s4g_float s4g_sget_float(s4g_main* s4gm, int index);
S4G_API s4g_bool s4g_sget_bool(s4g_main* s4gm, int index);
S4G_API const char* s4g_sget_str(s4g_main* s4gm, int index);
S4G_API s4g_c_function s4g_sget_cfunc(s4g_main* s4gm, int index);
S4G_API s4g_pdata s4g_sget_pdata(s4g_main* s4gm, int index);

S4G_API void s4g_spop(s4g_main* s4gm, int count);//выталкивает из стека count значений
S4G_API int s4g_sgettop(s4g_main* s4gm);	//количество элементов в стеке, и есесно номер вершины стека

//вызов функции, narg - количество аргументов
//сначала в стек ложится сама вызываемая функция, затем аргументы если есть и только потом эта функция
//если narg == 0 и s4g_gettop - narg в стеке не функция то ошибка
S4G_API void s4g_call(s4g_main* s4gm, bool call_func = false);

S4G_API int s4g_cfcount_arg(s4g_main* s4gm);	//количество аргументов которые были переданы функции
S4G_API s4g_value* s4g_cfget_arg(s4g_main* s4gm, int narg);
//преобразование аргументац функции к типам и их возвращение
//если narg == 1 то значит функция вернет первый аргумент и так далее
S4G_API s4g_int s4g_cfget_int(s4g_main* s4gm, int narg);
S4G_API s4g_uint s4g_cfget_uint(s4g_main* s4gm, int narg);
S4G_API s4g_float s4g_cfget_float(s4g_main* s4gm, int narg);
S4G_API s4g_bool s4g_cfget_bool(s4g_main* s4gm, int narg);
S4G_API const char* s4g_cfget_str(s4g_main* s4gm, int narg);
S4G_API s4g_c_function s4g_cfget_cfunc(s4g_main* s4gm, int narg);
S4G_API s4g_pdata s4g_cfget_pdata(s4g_main* s4gm, int narg);

//является ли значение аргумента функции по номеру narg (нумерация с 1) типом? 0 нет, 1 да
S4G_API int s4g_cfis_null(s4g_main* s4gm, int narg);
S4G_API int s4g_cfis_int(s4g_main* s4gm, int narg);
S4G_API int s4g_cfis_uint(s4g_main* s4gm, int narg);
S4G_API int s4g_cfis_float(s4g_main* s4gm, int narg);
S4G_API int s4g_cfis_bool(s4g_main* s4gm, int narg);
S4G_API int s4g_cfis_str(s4g_main* s4gm, int narg);
S4G_API int s4g_cfis_table(s4g_main* s4gm, int narg);
S4G_API int s4g_cfis_cfunc(s4g_main* s4gm, int narg);
S4G_API int s4g_cfis_pdata(s4g_main* s4gm, int narg);

S4G_API s4g_type s4g_cfget_type(s4g_main* s4gm, int narg);
S4G_API const char* s4g_cfget_str_type(s4g_main* s4gm, int narg, char* str = 0);

//добавить контекст который будет всегда доступен
//необходимо добалять сразу после инициализации скриптововй системы
//если требуется чтобы новый контекст существовал наравне с глобальным пространством
//index - позиция в стеке таблицы которая будет контекстом
S4G_API void s4g_ctx_add(s4g_main* s4gm, int index);

S4G_API const char* s4g_value_name(s4g_value* value);
S4G_API long s4g_table_size(s4g_table* ttable);
S4G_API s4g_value* s4g_table_get(s4g_table* ttable, long key);
//возвращает строку с текстом результата трассировки стека вызовов
S4G_API const char* s4g_stack_trace(s4g_main* s4gm);

S4G_API const char* s4g_dbg_get_curr_file(s4g_main* s4gm, char* str = 0);
S4G_API long s4g_dbg_get_curr_str(s4g_main* s4gm, char* str = 0);
S4G_API const char* s4g_dbg_get_curr_func(s4g_main* s4gm, char* str = 0);

}
#endif