
extern "C"
{

#define S4G_MAX_LEN_KEY_WORD_DEF 9
#define S4G_MAX_LEN_SYM 3

const char s4g_key_words[][S4G_MAX_LEN_KEY_WORD_DEF] = {
"null",
"function",
"var",
"global",
"return",
"if",
"and",
"or",
"then",
"elseif",
"else",
"for",
"while",
"do",
"end"
};

const char s4g_key_bool[][S4G_MAX_LEN_KEY_WORD_DEF] = {
"true",
"false"
};

const char s4g_key_preproc[][S4G_MAX_LEN_KEY_WORD_DEF] = {
"include",
"define",
"undef"
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

enum s4g_lexeme_type
{
	word_user,	//пользовательское слово
	word_null,	//пустой тип null
	word_string,//строка
	word_float,	//число с плавающей запятой
	word_int,	//целое число
	word_bool,	//логичесткое значение
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
	s4g_lexeme(const char* _str,long _numstr,int _type,int _id){strcpy(str,_str);numstr = _numstr,type = _type;id = _id;}
	char str[64];	//строковое представление лексемы
	long numstr;	//номер строки на которой находится лексема
	int type;		//тип лексемы
	int id;			//порядковый номер лексемы из массива слов к которому она относится
};

enum s4g_type_
{
	_if,_var,_add,_return,
};

struct s4g_value
{
	int type;
	void* value;
};

struct s4g_node
{
	int type;
	s4g_value* value;
	s4g_node* op1;
	s4g_node* op2;
	s4g_node* op3;
};


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


void s4g_output_error(const char* format,...);

class s4g_arr_lex
{
public:
	s4g_arr_lex();
	void read(const char* file,String AllFile);
	DWORD add(const char* _str,long _numstr,int _type,int _id);
	s4g_lexeme* get(DWORD num);
	DWORD get_curr_num();
	void set_curr_num(DWORD num);
	s4g_lexeme* get_curr();
	s4g_lexeme* get_next();

protected:
	Array<s4g_lexeme> ArrLexs;
	DWORD curr_num;
};

int s4g_parse_file(const char* file);
int s4g_parse_str(const char* str);
int s4g_gen_tree(s4g_node* tree);

}