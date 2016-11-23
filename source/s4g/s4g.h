
#define S4G_MAX_LEN_VAR_NAME 64
#define S4G_MAX_LEN_STR_IN_FILE 1024

#include <s4g\array.h>
#include <s4g\stack.h>
#include <s4g\aatable.h>

#define mem_delete(data) delete data;data = 0;
#define mem_delete_a(data) delete[] data;data = 0;

enum s4g_type
{
	t_null = 0,
	t_table = 1,
	t_string = 2,
	t_float = 3,
	t_int = 4,
	t_uint = 5,	//unsupported
	t_bool = 6,
	t_user_data = 7, //unsupported
	t_cfunc = 8,	//unsupported
	t_sfunc = 9,
	t_nn = 10,
};

enum s4g_error
{
	e_file_not_found = -1,
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

class s4g_vm;
typedef int (*s4g_c_function) (s4g_vm* vm);

struct s4g_main
{
	s4g_main();
	s4g_arr_lex* arr_lex;
	s4g_node* gnode;
	s4g_builder_syntax_tree* bst;
	Stack<s4g_command>* commands;
	s4g_compiler* compiler;
	s4g_vm* vmachine;
	char strerror[1024];
};

//значение
class s4g_value
{
public:
	s4g_value();
	~s4g_value();

	inline s4g_type get_type();

	inline void cr_value(s4g_value* val);
	inline void set_value(s4g_value* val);

	inline void init_long(long num);
	inline void init_float(double num);
	inline void init_str(const char* str);
	inline void init_bool(int bf);
	inline void init_table(s4g_table* tt);

	inline void init_s_func(s4g_s_function* func);
	inline void init_c_func(s4g_c_function func);

	inline void cr_table();
	inline void cr_null();
	inline void cr_nn();

	inline long get_long();
	inline double get_float();
	inline String & get_str();
	inline s4g_table* get_table();
	inline s4g_s_function* get_s_func();
	inline s4g_c_function get_c_func();
	inline int get_bool();
	
protected:
	friend s4g_value;
	friend s4g_table;
	s4g_type type;
	void* data;
};

inline s4g_value* cr_val_null();
inline s4g_value* cr_val_table_null();
inline s4g_value* cr_val_long(long num);
inline s4g_value* cr_val_float(float num);
inline s4g_value* cr_val_bool(bool bf);
inline s4g_value* cr_val_str(const char* str);
inline s4g_value* cr_val_table(s4g_table* tt);
inline s4g_value* cr_val_s_func(s4g_s_function* func);
inline s4g_value* cr_val_c_func(s4g_c_function func);
inline s4g_value* cr_val(int _type, const char* _val = 0);

//таблица
class s4g_table
{
public:
	s4g_table();

	inline void set_val_s(const char* name, s4g_value* val);
	inline void add_val_s(const char* name, s4g_value* val);

	inline void set_val_n(long num, s4g_value* val);
	inline void add_val_n(long num, s4g_value* val);

	inline void set_val(s4g_value* val);
	inline void add_val(s4g_value* val);

	inline void add_s(const char* name, long val);
	inline void add_s(const char* name, double val);
	inline void add_s(const char* name, const char* val);
	inline void add_s(const char* name, bool val);
	inline void add_s(const char* name, s4g_s_function* val);
	inline void add_s(const char* name, s4g_c_function val);
	inline void add_s(const char* name, s4g_table* val);
	inline void add_s(const char* name);
	inline void add_t_s(const char* name);
	
	inline void add_n(long num, long val);
	inline void add_n(long num, double val);
	inline void add_n(long num, const char* val);
	inline void add_n(long num, bool val);
	inline void add_n(long num, s4g_s_function* val);
	inline void add_n(long num, s4g_c_function val);
	inline void add_n(long num, s4g_table* val);
	inline void add_n(long num);
	inline void add_t_n(long num);

	void add(long val);
	void add(double val);
	void add(const char* val);
	void add(bool val);
	void add(s4g_s_function* val);
	void add(s4g_c_function val);
	void add(s4g_table* val);
	void add();
	void add_t();

	
	s4g_value* getn(DWORD key);
	s4g_value* gets(const char* str);
	bool is_exists_s(const char* str);
	bool is_exists_n(DWORD key);

protected:
	AATable<s4g_value> arr_value;
};

//тип функция
struct s4g_s_function
{
	s4g_s_function(){}
	s4g_table vars;	//переменные функции
	Stack<String> args;	//имена аргументов
	Stack<s4g_command> commands; //опкод
};

s4g_main* s4g_init();

void s4g_push_c_func(s4g_vm* vm,s4g_c_function func);
void s4g_store(s4g_vm* vm,const char* name);

int s4g_load_file(s4g_main* s4gm,const char* file);
//int s4g_call(s4g_machine* sm,int narg,int result);