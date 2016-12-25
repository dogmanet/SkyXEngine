
#ifndef s4g_conf_h
#define s4g_conf_h

#define S4G_MAX_LEN_TYPE_NAME 64
#define S4G_MAX_LEN_VAR_NAME 64
#define S4G_MAX_LEN_STR_IN_FILE 1024

#define mem_delete(data) delete data;data = 0;
#define mem_delete_a(data) delete[] data;data = 0;

#define S4G_MAX_CALL 200	//максимальное количество вызовов (рекурсивных и вложенных)
#define S4G_MAX_USER_STACK 200	//размер пользовательского стека
#define S4G_GLOBAL_NM "_g"	//обращение в скрипте к глобальному пространству имен
#define S4G_MARG "args"
#define S4G_RESERVE_STACK_EXE 10000
#define S4G_ADD_MEM_CONTEXTS 16

#define S4G_RESERVE_VALUE 10000
#define S4G_RESERVE_VALUE_MEM 10000

#define S4G_RESERVE_DATA 100000
#define S4G_RESERVE_DATA_MEM 100000

#define S4G_RESERVE_CONTEXTS 100
#define S4G_RESERVE_CONTEXTS_MEM 100

#define S4G_RESERVE_INT_MEM 10000
#define S4G_RESERVE_UINT_MEM 10000
#define S4G_RESERVE_FLOAT_MEM 10000
#define S4G_RESERVE_BOOL_MEM 10000
#define S4G_RESERVE_STRING_MEM 10000
#define S4G_RESERVE_SFUNC_MEM 10000
#define S4G_RESERVE_TABLE_MEM 10000

#define S4G_NOTICE 0
#define S4G_WARNING 1
#define S4G_ERROR 2

#define s4g_breakpoint _asm { int 3 }

//тип функции вывода информации и обработки ошибок
typedef void(*s4g_report_func) (int level, const char* name_ss, const char* format, ...);

//настраиваемые типы
#define s4g_int long
#define s4g_uint unsigned long
#define s4g_float float
#define s4g_bool short int
#define s4g_pdata void*

struct s4g_main;
//тип си(++) функции
typedef int(*s4g_c_function) (s4g_main* vm);

#endif