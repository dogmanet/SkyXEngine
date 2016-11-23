
extern "C"
{

#define S4G_MAX_LEN_KEY_WORD_DEF 9
#define S4G_MAX_LEN_SYM 3

const char s4g_key_words[][S4G_MAX_LEN_KEY_WORD_DEF] = {
"null",		//0
"function",	//1
"end",		//2
"return",	//3
"if",		//5
"and",		//6
"or",		//7
"then",		//8
"elseif",	//9
"else",		//10
"for",		//11
"while",	//12
"do",		//13

};

const char s4g_key_bool[][S4G_MAX_LEN_KEY_WORD_DEF] = {
"true",
"false"
};

const char s4g_key_preproc[][S4G_MAX_LEN_KEY_WORD_DEF] = {
"include",
"define",
"undef",
"if",
"ifdef",
"defined",
"elif",
"else",
"end",
};

const char s4g_key_syms_del[][S4G_MAX_LEN_SYM] = {
";",//разделитель операций
","	//разделитель аргументов
};

const char s4g_key_syms_group[][S4G_MAX_LEN_SYM] = {
"(",//начало группировки/вызова функции
")"//конец группировки/вызова функции
};

const char s4g_key_syms_table_create[][S4G_MAX_LEN_SYM] = {
"{","}",//начало конец создания таблицы
};

const char s4g_key_syms_table_op[][S4G_MAX_LEN_SYM] = {
"[","]",//начало конец обращение к элементу таблицы по строке
"."		//обращение к элементу таблицы по имени
};

const char s4g_key_syms_logic[][S4G_MAX_LEN_SYM] = {
"!","==","!=",">","<",">=","<="
};

const char s4g_key_syms_assign[][S4G_MAX_LEN_SYM] = {
"="
};

const char s4g_key_syms_arif[][S4G_MAX_LEN_SYM] = {
"+","-","*","/"
};

const char s4g_key_syms_arif_assign[][S4G_MAX_LEN_SYM] = {
"++","--","+=","-=","*=","/="
};

//однострочный комментарий
const char s4g_key_syms_comment_os[][S4G_MAX_LEN_SYM] = {
"//"
};

//многострочный комментарий
const char s4g_key_syms_comment_ms[][S4G_MAX_LEN_SYM] = {
"/*",	//начало
"*/"	//конец
};

const int s4g_prioritet_op[] = {
10,// ()
20,// + -
30,// * /
};

enum s4g_lexeme_type
{
	word_user,	//пользовательское слово
	word_null,	//пустой тип null
	word_string,//строка
	word_float,	//число с плавающей запятой
	word_int,	//целое число
	word_uint,	//целое число
	word_bool,	//логическое значение
	word_key,	//ключевое слово языка
	word_prep,	//слово препроцессора
	sym_delimiter,	//символ разделителя
	sym_logic,		//символ логики
	sym_assign,		//символ присвоения
	sym_arif,		//арифметический символ
	sym_arif_assign,//арифметический символ и присвоение
	sym_group,
	sym_table_create,	//символ создания таблицы
	sym_table_elem,		//символ обращения к элементу таблицы
};

struct s4g_lexeme
{
	s4g_lexeme(){}
	s4g_lexeme(const char* _str,long _numstr,int _type,int _id,int _idfile){strcpy(str,_str);numstr = _numstr,type = _type;id = _id;fileid = _idfile;}
	char str[64];	//строковое представление лексемы
	long numstr;	//номер строки на которой находится лексема
	int type;		//тип лексемы
	int id;			//порядковый номер лексемы из массива слов к которому она относится
	int fileid;		//порядковый номер файла
};

enum s4g_type_op
{
	_begin,_empty,_null,_expr,
	_float,_int,_uint,_numnull,_string,_function,_arg,_end,
	_if,_var,_set,_sett,_get,_call,
	_add,//+
	_sub,//-
	_mul,//*
	_div,///
	_group_b,//(
	_group_e,//)
	_return,_retval,
	_create_table,
	_add_in_table,
};

int s4g_get_priority(s4g_type_op op)
{
		if(op == _add || op == _sub)
			return s4g_prioritet_op[1];
		else if(op == _mul || op == _div)
			return s4g_prioritet_op[2];
		else if(op == _group_b || op == _group_e)
			return s4g_prioritet_op[0];
		else
			return -1;
}

struct s4g_node
{
	s4g_node()
	{
		type = _begin;value = 0;op1 = op2 = op3 = 0;
	}
	s4g_node(s4g_type_op _type,s4g_value* _value = 0,s4g_node* _op1 = 0,s4g_node* _op2 = 0,s4g_node* _op3 = 0){type = _type;value = _value;op1 = _op1;op2 = _op2;op3 = _op3;}
	s4g_type_op type;
	s4g_value* value;
	s4g_node* op1;
	s4g_node* op2;
	s4g_node* op3;
};
//если разбирали expression - выражение, то op1 это выражение, а op2 то что следует за выражением для целостности


//описатель символов ( ) [ ] { }
struct s4g_sym_o_desc
{
	s4g_sym_o_desc(){}
	s4g_sym_o_desc(char _cb,char _ce,bool iscf,long _numstrb){cb = _cb;ce = _ce;numstrb = _numstrb;is_call_f=iscf;}
	char cb,ce;		//код символа
	bool is_call_f;
	long numstrb,numstre;	//номер строки на котором он начинается
};

//является ли анализируемая лексема ...
inline int s4g_is_syms_arif(const char* sstr,char* dstr);		//арифметическим сиволом из s4g_key_syms_arif
inline int s4g_is_syms_arif_assign(const char* sstr,char* dstr);//арифметическим сиволом с приравниванием из s4g_key_syms_arif_assign
inline int s4g_is_syms_logic(const char* sstr,char* dstr);		//логическим символом из s4g_key_syms_logic
inline bool s4g_is_comment_os(const char* sstr);				//однострочным комментарием
inline bool s4g_is_comment_ms_b(const char* sstr);				//началом многострочного комментария
inline bool s4g_is_comment_ms_e(const char* sstr);				//концом многострочного комментария
inline int s4g_is_boolean(const char* sstr,char* dstr);			//логическим значеием
inline bool s4g_is_null(const char* sstr);						//пустым значением
inline int s4g_is_delimiter(const char* sstr,char* dstr);		//разделителем
inline int s4g_is_assign(const char* sstr,char* dstr);			//присваиванием
inline int s4g_is_table_create(const char* sstr,char* dstr);	//символом создания таблицы
inline int s4g_is_table_elem(const char* sstr,char* dstr);		//символом обращения к элементу таблицы
inline int s4g_is_syms_group(const char* sstr,char* dstr);		//символом группировки/вызова функции

//является ли текущий сивол ...
inline bool s4g_is_char_str(const char sym);				//строкой
inline bool s4g_is_char_num_or_point(const char sym);		//числом либо точкой
inline bool s4g_is_char_num(const char sym);				//числом
inline bool s4g_is_char_point(const char sym);				//точкой
inline bool s4g_is_char_arif(const char sym);				//арифметическим символом + - * /
inline bool s4g_is_char_arif_pm(const char sym);			//арифметическим символом + - 

//считывание из строки ...
inline void s4g_scan_string(const char* sstr,char* dstr);				//слова состоящего из букв и цифр
inline void s4g_scan_list_string(const char* sstr,char dstr[64][64]);	//
inline void s4g_scan_litstring(const char* sstr,char* dstr);			//пользовательской строки начинающейся и заканчивающейся "
inline int s4g_scan_num(const char* sstr,char* dstr);					//числа


inline int s4g_is_key_word(const char* str);	//является ли слово ключевым
inline int s4g_is_key_word_pp(const char* str);	//является ли слово ключевым из препроцессора

struct s4g_def_lex_arg
{
	s4g_def_lex_arg(){}
	s4g_def_lex_arg(s4g_lexeme* _l,int _a){lexeme = _l;num_arg = _a;}
	s4g_lexeme* lexeme;
	int num_arg;
};

struct s4g_define
{
	char name[64];
	Array<String> arg;
	Array<s4g_def_lex_arg> lexs;
};

class s4g_arr_lex
{
public:
	s4g_arr_lex();
	int read(const char* file);
	int gen_lexs(const char* str,Array<s4g_lexeme*>* arr_lexs,bool is_def);
	s4g_lexeme* get(DWORD num);
	DWORD get_curr_num();
	DWORD get_count();
	void set_curr_num(DWORD num);
	s4g_lexeme* get_curr();
	s4g_lexeme* get_next();
	s4g_lexeme* get_prev();
	s4g_lexeme* r_get_lexeme(const char* str,long* curr_pos,long* curr_num_str);

	char strerror[1024];
//protected:
	Array<s4g_lexeme*> ArrLexs;
	AATable<s4g_define> ArrDefines;
	Array<String> ArrFiles;
	Array<String> ArrPathForFiles;
	int curr_id_file;// = ArrFiles.size()-1;
	DWORD curr_num;
};


#define bst_condition_error(bst) if(bst->status != 0) return 0;
#define bst_condition_eof(lexeme) if(!lexeme) {\
status = 1;\
tmplexs = arr_lex->get_prev();\
sprintf(error,"[%s]:%d - неожиданный канец файла ",arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);\
return 0;\
}

struct s4g_builder_syntax_tree
{
s4g_builder_syntax_tree(){status = 0;error[0]=0;overend = 0;overge = 0;}
s4g_node* s4g_gen_tree();
s4g_node* s4g_gen_statement();
s4g_node* s4g_get_ret_vals();
s4g_node* s4g_get_arg_call_func();
s4g_node* s4g_get_function_def_head();
s4g_node* s4g_get_term();
s4g_node* s4g_get_op();
s4g_node* s4g_get_expr();
s4g_node* s4g_get_table();
int overend;	//если 0 значит все в норме, если более 0 то лишний энд, если меньше 0 то значит мы ищем энды в количестве abs(overend)
int overge;
int status;
char error[1024];
s4g_arr_lex* arr_lex;
};
}