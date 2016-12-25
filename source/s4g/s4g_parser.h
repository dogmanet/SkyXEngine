
#ifndef s4g_parser_h
#define s4g_parser_h

#define S4G_MAX_LEN_KEY_WORD_DEF 9
#define S4G_MAX_LEN_SYM 4

const char s4g_key_words[][S4G_MAX_LEN_KEY_WORD_DEF] = {
"null",		//0
"function",	//1
"end",		//2
"return",	//3
"extern",	//4
"if",		//
"and",		//
"or",		//
"then",		//
"elseif",	//
"else",		//
"for",		//
"while",	//
"do",		//

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

const char s4g_key_setcompiler[][S4G_MAX_LEN_KEY_WORD_DEF] = {
"ginf",
"crd"
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

const char s4g_key_syms_create[][S4G_MAX_LEN_SYM] = {
"$"
};

const char s4g_key_syms_marg[][S4G_MAX_LEN_SYM] = {
"..."
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

//массив с приоритетами операций
const int s4g_prioritet_op[] = {
10,// ()
20,// + -
30,// * /
};

//перечисление типов создаваемых лексем
enum s4g_lexeme_type
{
	word_user,		//пользовательское слово
	word_user_cr,	//создаваемое пользовательское слово
	word_null,		//пустой тип null
	word_string,	//строка
	word_string_cr,	//создающая строка
	word_float,		//число с плавающей запятой
	word_int,		//целое знаковое число
	word_uint,		//целое беззнаковое число
	word_int_cr,	//создающее целое знаковое число
	word_uint_cr,	//создающее целое беззнаковое число
	word_bool,		//логическое значение
	word_key,		//ключевое слово языка
	word_prep,		//слово препроцессора
	marg,			//переменное количество аргументов
	sym_delimiter,	//символ разделителя
	sym_logic,		//символ логики
	sym_assign,		//символ присвоения
	sym_arif,		//арифметический символ
	sym_arif_assign,//арифметический символ и присвоение
	sym_group,		//символ группировки ()
	sym_table_create,	//символ создания таблицы
	sym_table_elem,		//символ обращения к элементу таблицы
};

//лексема
struct s4g_lexeme
{
	s4g_lexeme(){}
	s4g_lexeme(const char* _str, long _numstr, int _type, int _id, int _idfile){ strcpy(str, _str); numstr = _numstr, type = _type; id = _id; fileid = _idfile; }
	char str[S4G_MAX_LEN_TYPE_NAME];	//строковое представление лексемы
	long numstr;	//номер строки на которой находится лексема
	int type;		//тип лексемы
	int id;			//порядковый номер лексемы из массива слов к которому она относится
	int fileid;		//порядковый номер файла
};

//типы нодов
enum s4g_type_op
{
	_begin,	//начало программы
	_empty,	//путой нод, соединящее звено нодов
	
	_expr,	//выражение
	_var,	//переменная
	_crvar,	//создаваемая переменная

	//ноды содержащие в себе значения с типами
	_null,_float, _int, _int_cr, _uint, _uint_cr, _bool,  _string, _string_cr, 
	_function,
	_numnull, //цифра 0 для выражений типа -123 = 0-123

	_arg, _marg, //нод содержит аргументы
	_set,	//присваивание
	_sett,	//присваивание в таблице
	_get,	//получение переменной
	_get_cr,//создание и получение переменной 
	_call,	//вызов функции
	_add,//+
	_sub,//-
	_mul,//*
	_div,///
	_group_b,//(
	_group_e,//)
	_return,		//возвращение значений
	_create_table,	//создание таблицы
	_add_in_table,	//добавление в таблицу
};

//вычисление приоритетов операции
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

//нод, ключевой объект в абстрактном синтаксическом дереве
struct s4g_node
{
	s4g_node()
	{
		type = _begin; value = 0; op1 = op2 = op3 = 0; lexid = -1;
	}
	s4g_node(s4g_type_op _type, long _lexid = -1, s4g_value* _value = 0, s4g_node* _op1 = 0, s4g_node* _op2 = 0, s4g_node* _op3 = 0){ type = _type; lexid = _lexid; value = _value; op1 = _op1; op2 = _op2; op3 = _op3; }
	s4g_type_op type;	//тип нода
	long lexid;
	s4g_value* value;	//значение если надо

	//содержимое нода или продолжение
	s4g_node* op1;	
	s4g_node* op2;
	s4g_node* op3;
};
//type == _expr:
	//op1 - выражение, 
	//op2 то что следует за выражением для целостности

//type == _call:
	//op1 - _var
	//op2 - значит у нас перечисление того что будем присваивать
	//op3 - _arg
		//op1 - аргумент
		//op2 - _empty
			//op1 - _ аргумент
				//op2 - _empty
					//...

//type == _function:
	//op1 - _arg первый аргумент:
		//value
		//op1 - следующий _arg
			//...
	//op2 - тело функции

//type == _set:
	//op1 - чему присваиваем
	//op2 - то что присваиваем

//type == _var
	//op1 - _get если существует то значит идет обращение к элементу таблицы
		//op1 - _string/_number/_crvar
		//op2 - empty
			//op1 - _get
				//...
	//op2 - _var/_crvar - если существует нод то значит у нас идет перечисление через запятую переменных которым или которые будем присваивать
	//op3 - _var/value/operation - если существует нод значит у нас идут какие то арифметические операции или сравнения
		//...

//type == _create_table
	//либо op1 - _sett
		//тоже самое что и _set (_sett указыает что манипулируем в таблице)
	//либо op1 - _add_in_table
		//op1 - то что добавляем в таблицу
		//op2 - _empty
			//op1 - _add_in_table
			//...
	//op2 - _empty
		//op1 - _sett
		//op2 - _empty
			//...


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
//inline int s4g_is_marg(const char* sstr, char* dstr);		//символом группировки/вызова функции

//является ли текущий сивол ...
inline bool s4g_is_char_str(const char sym);				//строкой
inline bool s4g_is_char_num_or_point(const char sym);		//числом либо точкой
inline bool s4g_is_char_num(const char sym);				//числом
inline bool s4g_is_char_point(const char sym);				//точкой
inline bool s4g_is_char_arif(const char sym);				//арифметическим символом + - * /
inline bool s4g_is_char_arif_pm(const char sym);			//арифметическим символом + - 

//считывание из строки ...
inline void s4g_scan_string(const char* sstr,char* dstr);				//слова состоящего из букв и цифр
inline void s4g_scan_litstring(const char* sstr,char* dstr);			//пользовательской строки начинающейся и заканчивающейся "
inline int s4g_scan_num(const char* sstr,char* dstr);					//числа

inline int s4g_is_key_word(const char* str);	//является ли слово ключевым
inline int s4g_is_key_word_pp(const char* str);	//является ли слово ключевым из препроцессора

//аргумент препроцессора
struct s4g_def_lex_arg
{
	s4g_def_lex_arg(){}
	s4g_def_lex_arg(s4g_lexeme* _l,int _a){lexeme = _l;num_arg = _a;}
	s4g_lexeme* lexeme;
	int num_arg;
};

//команда препроцессора, может быть как константой так и функцией
struct s4g_define
{
	char name[64];
	Array<String> arg;
	Array<s4g_def_lex_arg> lexs;
};

//лексический анализатор
class s4g_arr_lex
{
public:
	s4g_arr_lex();
	int read(const char* file_str,bool isfile=true);
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
	long curr_num;
	MemAlloc<s4g_lexeme> LexPool;
};

#define bst_iferr(cond,text,...) if(cond) {this->status = -1; sprintf(this->error, text, ...); return 0;)
#define bst_cond_er(bst) if(bst->status != 0) return 0;
#define bst_cond_eof(lexeme) if(!lexeme && !isender) {\
status = 1;\
tmplexs = arr_lex->get_prev();\
sprintf(error,"[%s]:%d - unexpected end of file",arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);\
return 0; \
}

#define curr_lexid arr_lex->get_curr_num()

#define lex_get_curr(tmplex) { tmplex = arr_lex->get_curr(); bst_cond_eof(tmplex);}
#define lex_get_next(tmplex) { tmplex = arr_lex->get_next(); bst_cond_eof(tmplex);}
#define lex_get_prev(tmplex) { tmplex = arr_lex->get_prev(); bst_cond_eof(tmplex);}

#define lex_get_curr0(tmplex) tmplex = arr_lex->get_curr(); bst_cond_eof(tmplex);
#define lex_get_next0(tmplex) tmplex = arr_lex->get_next(); bst_cond_eof(tmplex);
#define lex_get_prev0(tmplex) tmplex = arr_lex->get_prev(); bst_cond_eof(tmplex);


//строитель абстрактного синтаксического дерева
struct s4g_builder_syntax_tree
{
s4g_builder_syntax_tree(){ status = 0; error[0] = 0; overend = 0; overge = 0; listread = true; readcall = true; isender = true; }

s4g_node* s4g_gen_tree();	//построить аст и вернуть первый нод

s4g_node* s4g_gen_statement();//считывание главных инструкций
s4g_node* s4g_get_ret_vals();		//считывание возвращаемых значений
s4g_node* s4g_get_arg_call_func();	//считывание аргументов при вызове функции
s4g_node* s4g_get_function_def_head();	//считывание аргументов при создании функции
s4g_node* s4g_get_term();	//считывание термов
s4g_node* s4g_get_op();		//считывание операций
s4g_node* s4g_get_expr();	//считывание выражений
s4g_node* s4g_get_table();	//считывание содержимого создаваемой таблицы

bool isender;	//если список лексем закончился то можно ли завершить построение?
bool listread;	//можем ли мы считывать списки переменны/значений или они уже считываются (то есть false)?
bool readcall;	//можем ли мы считывать вызов функции, или сейчас считывается объявление функции (то есть false)?
int overend;	//если 0 значит все в норме, если более 0 то лишний энд, если меньше 0 то значит мы ищем энды в количестве abs(overend)
int overge;		//инкрементируя мы говорим что считываем вызов функции и если считывание выражения найден лишнюю закрывающую скобку то она принадлежит вызову (тада декрементирует)
int status;		//текущий статус, 0 - все норм, все другое это ошибка
char error[1024];//строка с описанием ошибки

s4g_arr_lex* arr_lex;
s4g_gc* gc;
};

#endif