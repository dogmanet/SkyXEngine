
#ifndef s4g_h
#define s4g_h

extern "C"
{

#include "string.cpp"
#include <limits.h>

#define S4G_VERSION "0.9"
#define S4G_COPYRIGHT "Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017"
#define S4G_SITE "s4g.su"

//чтобы использовать скрипты прямо в приложении ничего дополнительно объявлять не надо
//для экспорта в dll необходимо объявить S4G_BUILD_DLL и S4G_DLL
//для импорта из dll необходимо объявить S4G_BUILD_DLL и S4G_EXE
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
	t_null = 0,		//пустое значение
	t_table = 1,	//таблица (ассоциативный массив)
	t_string = 2,	//строковое значение (пользовательская строка)
	t_float = 3,	//число с плавающей запятой
	t_int = 4,		//знаковое целое десятичное число
	t_uint = 5,		//знаковое целое десятичное число
	t_bool = 6,		//логическое значение
	t_pdata = 7,	//указатель на пользовательские данные
	t_cfunc = 8,	//с(++) функция
	t_sfunc = 9,	//скриптовая функция
	t_nnull = 10,	//цифра 0, вставляется в выражения с унарным минусом
};

struct s4g_main;	//основа взаимодействия между с(++) и скриптами
struct s4g_value;	//переменная-значение
struct s4g_table;	//таблица

#define S4G_CREATE_VAR "S4G_CREATE_VAR"

//const
#define S4G_MAX_LEN_TYPE_NAME 64	//максимальная длина имени типа
#define S4G_MAX_LEN_VAR_NAME 64		//максимальная длина имени переменной
#define S4G_MAX_LEN_STR_IN_FILE 1024//максимальная длина загружаемого файла

#define S4G_MAX_CALL 1000	//максимальное количество вызовов (рекурсивных и вложенных)
#define S4G_MAX_ENCLOSURE 100	//максимальное количество вложенных блоков
#define S4G_GLOBAL_NM "_g"	//обращение в скрипте к глобальному пространству имен
#define S4G_MARG "args"		//таблица для обращения к аргументам в случае если функция принимает неопределенное количество аргументов

//резер элементов - непосредственный резерв для стека хранящего определенные элементы
//резерв памяти элементов - резерв выделяемой памяти для создания новых элементов
//(для всех резервов): на это же количество будет увеличиваться дополнительный резерв в случе превышения предыдущего резерва

#define S4G_RESERVE_STACK_EXE 1000	//резерв элементов для стека исполнения

#define S4G_RESERVE_VALUE 10000		//резерв элементов для переменных
#define S4G_RESERVE_VALUE_MEM 10000	//резерв памяти элементов для переменных

#define S4G_RESERVE_DATA 10000		//резерв элементов для данных
#define S4G_RESERVE_DATA_MEM 10000	//резерв памяти элементов для данных

#define S4G_RESERVE_CONTEXTS 100	//резерв элементов для контекстов
#define S4G_ADD_MEM_CONTEXTS 16		//количество создаваемых контекстов на каждый случай нехватки
#define S4G_RESERVE_CONTEXTS_MEM 100//резерв памяти элементов для контекстов

#define S4G_RESERVE_STRING_MEM 1000	//резерв памяти элементов для строк

#define S4G_RESERVE_SFUNC_MEM 1000	//резерв памяти элементов для скриптовых функций

#define S4G_RESERVE_TABLE_MEM 1000	//резерв памяти элементов для таблиц
#define S4G_RESERVE_TABLE_ELEM 16	//на сколько элементов будет резервироваться место в таблицах

//уровни сообщений
#define S4G_NOTICE 0	//уведомление
#define S4G_WARNING 1	//предупреждение
#define S4G_ERROR 2		//ошибка

//принудительная точка остановки при любой сборке проекта (для отладки)
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

//стандартная функция выдачи сообщений (заменяема посредством api)
void s4g_report(s4g_main* s4gm, int level, const char* name_ss, const char* format, ...)
{
#if defined(_WINDOWS)
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);
#endif
	char buf[4096];
	va_list va;
	va_start(va, format);
	vsprintf_s(buf, sizeof(buf), format, va);
	va_end(va);

	if (level == 0)
		fprintf(stdout, "s4g %s notice: %s\n", name_ss, buf);
	else if (level == 1)
		fprintf(stdout, "s4g %s warning: %s\n", name_ss, buf);
	else
	{
		fprintf(stdout, "s4g %s error: %s\n", name_ss, buf);
		Sleep(50000);
		exit(1);
	}
}

s4g_report_func s4g_rf = s4g_report;

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

#define S4G_NM_GLOBAL INT_MIN	//глобальное пространство имен
#define S4G_NM_SYS INT_MIN+1		//языковое/системное простраство имен
//глобальнео простарство имен доступно из локальных контекстов только через _g (как объявлено в S4G_GLOBAL_NM), в глобальном все пишется напрямую в него
//языковое/системное простраство имен доступно напрямую при любом контексте, поэтому в этоя зыковое простарство можно экспортировать все что необходимо для прямого использования внутри скриптов

//возвращает строковое представление типа tt в str_type
S4G_API const char* s4g_get_str_type(s4g_type tt, char* str_type = 0);

S4G_API s4g_main* s4g_init(const char* name);//инициализируем скриптовую систему
S4G_API void s4g_kill(s4g_main* s4gm);//завершаем работу скриптовой системы
S4G_API int s4g_load_file(s4g_main* s4gm, const char* file);//загрузить скрипт из файла
S4G_API int s4g_load_str(s4g_main* s4gm, const char* str);//загрузить скрипт из строки

S4G_API void s4g_set_rf(s4g_report_func rf);	//установить новую функцию выдачи сообщений
S4G_API void s4g_gen_msg(s4g_main* s4gm, int level, const char* format, ...);	//генерировать сообщение

//вызываем сборку мусора, возвращает количество занимаемой памяти
S4G_API int s4g_call_gc(s4g_main* s4gm);

/*
С(++) => stack
*/

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

/*
stack => script
*/

//функция сохранения в пространство имен
//после сохранения функция выталкивает сохраненное значение с вершины стека
S4G_API void s4g_sstore(s4g_main* s4gm, int index, const char* name);
//index в стеке должна быть таблица, либо S4G_NM_GLOBAL если идет сохранеие в глобальное пространство, либо S4G_NM_SYS если в языковое/системное
//name имя переменной, а значение берется с вершины стека
//table(stack[index])[name] = stack[-1];

//получить значения
//после получения, функция ложит на вершину стека полученное значение
S4G_API void s4g_sget(s4g_main* s4gm, int index, const char* name);
//index в стеке должна быть таблица, либо S4G_NM_GLOBAL если берется из глобального пространства, либо S4G_NM_SYS если из языкового/системного
//name имя переменной, а значение берется с вершины стека
//stack.push(table(stack[index])[name]);

/*
stack => C(++)
*/

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

S4G_API void s4g_spop(s4g_main* s4gm, int count);	//выталкивает из стека count значений
S4G_API int s4g_sgettop(s4g_main* s4gm);			//количество элементов в стеке, и есесно номер вершины стека


//вызов функции/исполнения скрипта
//сначала в стек ложится сама вызываемая функция, затем аргументы если есть и только потом s4g_call
S4G_API void s4g_call(s4g_main* s4gm, bool call_func = false);

/*
script => C(++) function
*/

S4G_API int s4g_cfcount_arg(s4g_main* s4gm);				//количество аргументов которые были переданы функции
S4G_API s4g_value* s4g_cfget_arg(s4g_main* s4gm, int narg);	//возвращает s4g_value значения аргумента

//преобразование аргумента функции к типам и их возвращение
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

S4G_API s4g_type s4g_cfget_type(s4g_main* s4gm, int narg);	//возвращает тип аргумента
S4G_API const char* s4g_cfget_str_type(s4g_main* s4gm, int narg, char* str = 0);//возвращает строковое в предствление типа аргумента


//DEBUG

S4G_API const char* s4g_value_name(s4g_value* value);			//возвращает имя переменной
S4G_API long s4g_table_size(s4g_table* ttable);					//возвращает размер таблицы
S4G_API s4g_value* s4g_table_get(s4g_table* ttable, long key);	//возвращает s4g_value* которая располагается в таблице по ключу key, если есть, иначе 0

S4G_API const char* s4g_stack_trace(s4g_main* s4gm);	//возвращает строку с текстом результата трассировки стека вызовов

S4G_API const char* s4g_dbg_get_curr_file(s4g_main* s4gm, char* str = 0);	//возвращает путь до файла скрипта который выполняется в данный момент (если str то записывает в него)
S4G_API long s4g_dbg_get_curr_str(s4g_main* s4gm, char* str = 0);			//возвращает номер строки файла скрипта который выполняется в данный момент (если str то записывает в него)
S4G_API const char* s4g_dbg_get_curr_func(s4g_main* s4gm, char* str = 0);	//возвращает функцию которая выполняется в данный момент (если str то записывает в него)


//Pre Process

S4G_API bool s4g_pp_is_def(s4g_main* s4gm, const char* name_def);
S4G_API void s4g_pp_def(s4g_main* s4gm, const char* def_str);
S4G_API void s4g_pp_undef(s4g_main* s4gm, const char* name_def);
S4G_API void s4g_pp_add_inc_path(s4g_main* s4gm, const char* path);
}
#endif