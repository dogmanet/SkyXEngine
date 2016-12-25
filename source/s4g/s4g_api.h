
#ifndef s4g_api_h
#define s4g_api_h

//возвращает строковое представление типа tt в str_type
extern void s4g_get_str_type(s4g_type tt, char* str_type);

s4g_main* s4g_init(const char* name);//инициализируем скриптовую систему
void s4g_kill(s4g_main* s4gm);//завершаем работу скриптовой системы
int s4g_load_file(s4g_main* s4gm, const char* file);//загрузить скрипт из файла
int s4g_load_str(s4g_main* s4gm, const char* str);//загрузить скрипт из строки

void s4g_set_rf(s4g_report_func rf);

//вызываем сборку мусора, mls - количество млсек которое будет работать сборщик, если 0 то соберет весь мусор
//возвращает количество занимаемой памяти
int s4g_call_gc(s4g_main* s4gm, DWORD mls = 0);

//функции для вставки на вершину стека значения
void s4g_spush_table_null(s4g_main* s4gm, int count_elem);
void s4g_spush_c_func(s4g_main* s4gm, s4g_c_function func);
void s4g_spush_int(s4g_main* s4gm, s4g_int num);
void s4g_spush_uint(s4g_main* s4gm, s4g_uint num);
void s4g_spush_float(s4g_main* s4gm, s4g_float num);
void s4g_spush_str(s4g_main* s4gm, const char* str);
void s4g_spush_bool(s4g_main* s4gm, s4g_bool bf);
void s4g_spush_pdata(s4g_main* s4gm, s4g_pdata pdata);
void s4g_spush_null(s4g_main* s4gm);
void s4g_spush_precall(s4g_main* s4gm);

//функции сохранения в пространства имен
//после сохранения функция выталкивает сохраненное значение с вершины стека
void s4g_sstore_g(s4g_main* s4gm, const char* name); //в глобальном пространстве, name имя которое будет присвоено значению на вершине стека
void s4g_sstore(s4g_main* s4gm, int index, const char* name);// index в стеке должна быть таблица, name имя переменной, а значение берется с вершины стека
//table(stack[index])[name] = stack[-1];

//получить значения
//после получения, функция ложит на вершину стека полученное значение
void s4g_sget_g(s4g_main* s4gm, const char* name);	//из глобального, name - имя переменной значение которой берем
void s4g_sget(s4g_main* s4gm, int index, const char* name);// index в стеке должна быть таблица, name имя переменной
//stack.push(table(stack[index])[name]);

//является ли значение переменной в стеке по номеру index типом? 0 нет, 1 да
int s4g_sis_int(s4g_main* s4gm, int index);
int s4g_sis_uint(s4g_main* s4gm, int index);
int s4g_sis_float(s4g_main* s4gm, int index);
int s4g_sis_str(s4g_main* s4gm, int index);
int s4g_sis_bool(s4g_main* s4gm, int index);
int s4g_sis_c_func(s4g_main* s4gm, int index);
int s4g_sis_s_func(s4g_main* s4gm, int index);
int s4g_sis_table(s4g_main* s4gm, int index);
int s4g_sis_pdata(s4g_main* s4gm, int index);
int s4g_sis_null(s4g_main* s4gm, int index);

//возвращает тип значения переменной по номеру в стеке index
s4g_type s4g_sget_type(s4g_main* s4gm, int index);

//возвращает приведенное к определнному типу значнеие перменной по номеру index в стеке
s4g_int s4g_sget_int(s4g_main* s4gm, int index);
s4g_uint s4g_sget_uint(s4g_main* s4gm, int index);
s4g_float s4g_sget_float(s4g_main* s4gm, int index);
s4g_bool s4g_sget_bool(s4g_main* s4gm, int index);
const char* s4g_sget_str(s4g_main* s4gm, int index);
s4g_c_function s4g_sget_cfunc(s4g_main* s4gm, int index);
s4g_pdata s4g_sget_pdata(s4g_main* s4gm, int index);

void s4g_spop(s4g_main* s4gm, int count);//выталкивает из стека count значений
int s4g_sgettop(s4g_main* s4gm);	//количество элементов в стеке, и есесно номер вершины стека

//вызов функции, narg - количество аргументов
//сначала в стек ложится сама вызываемая функция, затем аргументы если есть и только потом эта функция
//если narg == 0 и s4g_gettop - narg в стеке не функция то ошибка
void s4g_call(s4g_main* s4gm, bool call_func = false);

int s4g_cfcount_arg(s4g_main* s4gm);	//количество аргументов которые были переданы функции

//преобразование аргументац функции к типам и их возвращение
//если narg == 1 то значит функция вернет первый аргумент и так далее
s4g_int s4g_cfget_int(s4g_main* s4gm, int narg);
s4g_uint s4g_cfget_uint(s4g_main* s4gm, int narg);
s4g_float s4g_cfget_float(s4g_main* s4gm, int narg);
s4g_bool s4g_cfget_bool(s4g_main* s4gm, int narg);
const char* s4g_cfget_str(s4g_main* s4gm, int narg);
s4g_c_function s4g_cfget_cfunc(s4g_main* s4gm, int narg);

int s4g_cfis_int(s4g_main* s4gm, int narg);
int s4g_cfis_uint(s4g_main* s4gm, int narg);
int s4g_cfis_float(s4g_main* s4gm, int narg);
int s4g_cfis_bool(s4g_main* s4gm, int narg);
int s4g_cfis_str(s4g_main* s4gm, int narg);
int s4g_cfis_cfunc(s4g_main* s4gm, int narg);

#endif