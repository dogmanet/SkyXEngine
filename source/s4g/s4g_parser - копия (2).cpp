
#pragma once
#include <s4g\s4g_parser.h>

int s4g_parse_str_is_exists(const char* str)
{
	return 1;
}

void s4g_output_error(const char* format,...)
{
	va_list va;
	char buf[4096];
	va_start(va, format);
	vsprintf_s(buf, 4096, format, va);
	va_end(va);
	MessageBox(0,buf,0,MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
	exit(0);
}

/////////

inline int s4g_is_syms_arif(const char* sstr,char* dstr)
{
	long count_key_syms = sizeof(s4g_key_syms_arif)/S4G_MAX_LEN_SYM;

		for(int i=0;i<count_key_syms;i++)
		{
			int tmplen = strlen(s4g_key_syms_arif[i]);
			int is_syms = 0;
				if(strlen(sstr) >= tmplen)
				{
					
						for(int k=0;k<tmplen;k++)
						{
								if(sstr[k] == s4g_key_syms_arif[i][k])
									is_syms++;
						}

						if(is_syms == tmplen)
						{
							strcpy(dstr,s4g_key_syms_arif[i]);
							return i;
						}
				} 
		}
	return -1;
}

inline int s4g_is_syms_arif_assign(const char* sstr,char* dstr)
{
	long count_key_syms = sizeof(s4g_key_syms_arif_assign)/S4G_MAX_LEN_SYM;

		for(int i=0;i<count_key_syms;i++)
		{
			int tmplen = strlen(s4g_key_syms_arif_assign[i]);
			int is_syms = 0;
				if(strlen(sstr) >= tmplen)
				{
					
						for(int k=0;k<tmplen;k++)
						{
								if(sstr[k] == s4g_key_syms_arif_assign[i][k])
									is_syms++;
						}

						if(is_syms == tmplen)
						{
							strcpy(dstr,s4g_key_syms_arif_assign[i]);
							return i;
						}
				} 
		}
	return -1;
}

inline int s4g_is_syms_logic(const char* sstr,char* dstr)
{
	long count_key_syms_logic = sizeof(s4g_key_syms_logic)/S4G_MAX_LEN_SYM;

		for(int i=0;i<count_key_syms_logic;i++)
		{
			int tmplen = strlen(s4g_key_syms_logic[i]);
			int is_syms_logic = 0;
				if(strlen(sstr) >= tmplen)
				{
					
						for(int k=0;k<tmplen;k++)
						{
								if(sstr[k] == s4g_key_syms_logic[i][k])
									is_syms_logic++;
						}

						if(is_syms_logic == tmplen)
						{
							strcpy(dstr,s4g_key_syms_logic[i]);
							return i;
						}
				} 
		}
	return -1;
}

inline int s4g_is_delimiter(const char* sstr,char* dstr)
{
	long count_key_syms_del = sizeof(s4g_key_syms_del)/S4G_MAX_LEN_SYM;

		for(int i=0;i<count_key_syms_del;i++)
		{
			int tmplen = strlen(s4g_key_syms_del[i]);
			int is_syms_del = 0;
				if(strlen(sstr) >= tmplen)
				{
					
						for(int k=0;k<tmplen;k++)
						{
								if(sstr[k] == s4g_key_syms_del[i][k])
									is_syms_del++;
						}

						if(is_syms_del == tmplen)
						{
							strcpy(dstr,s4g_key_syms_del[i]);
							return i;
						}
				} 
		}
	return -1;
}

inline int s4g_is_assign(const char* sstr,char* dstr)
{
	long count_key_syms_assign = sizeof(s4g_key_syms_assign)/S4G_MAX_LEN_SYM;

		for(int i=0;i<count_key_syms_assign;i++)
		{
			int tmplen = strlen(s4g_key_syms_assign[i]);
			int is_syms_assign = 0;
				if(strlen(sstr) >= tmplen)
				{
					
						for(int k=0;k<tmplen;k++)
						{
								if(sstr[k] == s4g_key_syms_assign[i][k])
									is_syms_assign++;
						}

						if(is_syms_assign == tmplen)
						{
							strcpy(dstr,s4g_key_syms_assign[i]);
							return i;
						}
				} 
		}
	return -1;
}

inline int s4g_is_table_create(const char* sstr,char* dstr)
{
	long count_key_syms_table_create = sizeof(s4g_key_syms_table_create)/S4G_MAX_LEN_SYM;

		for(int i=0;i<count_key_syms_table_create;i++)
		{
			int tmplen = strlen(s4g_key_syms_table_create[i]);
			int is_syms_table_create = 0;
				if(strlen(sstr) >= tmplen)
				{
					
						for(int k=0;k<tmplen;k++)
						{
								if(sstr[k] == s4g_key_syms_table_create[i][k])
									is_syms_table_create++;
						}

						if(is_syms_table_create == tmplen)
						{
							strcpy(dstr,s4g_key_syms_table_create[i]);
							return i;
						}
				} 
		}
	return -1;
}

inline int s4g_is_table_elem(const char* sstr,char* dstr)
{
	long count_key_syms_table_op = sizeof(s4g_key_syms_table_op)/S4G_MAX_LEN_SYM;

		for(int i=0;i<count_key_syms_table_op;i++)
		{
			int tmplen = strlen(s4g_key_syms_table_op[i]);
			int is_syms_table_op = 0;
				if(strlen(sstr) >= tmplen)
				{
					
						for(int k=0;k<tmplen;k++)
						{
								if(sstr[k] == s4g_key_syms_table_op[i][k])
									is_syms_table_op++;
						}

						if(is_syms_table_op == tmplen)
						{
							strcpy(dstr,s4g_key_syms_table_op[i]);
							return i;
						}
				} 
		}
	return -1;
}

inline int s4g_is_syms_group(const char* sstr,char* dstr)
{
	long count_key_syms_group = sizeof(s4g_key_syms_group)/S4G_MAX_LEN_SYM;

		for(int i=0;i<count_key_syms_group;i++)
		{
			int tmplen = strlen(s4g_key_syms_group[i]);
			int is_syms_group = 0;
				if(strlen(sstr) >= tmplen)
				{
					
						for(int k=0;k<tmplen;k++)
						{
								if(sstr[k] == s4g_key_syms_group[i][k])
									is_syms_group++;
						}

						if(is_syms_group == tmplen)
						{
							strcpy(dstr,s4g_key_syms_group[i]);
							return i;
						}
				} 
		}
	return -1;
}

inline bool s4g_is_comment_os(const char* sstr)
{
	int tmplen = strlen(s4g_key_syms_comment_os[0]);
	int is_comment = 0;
		if(strlen(sstr) >= tmplen)
		{
			
				for(int i=0;i<tmplen;i++)
				{
						if(sstr[i] == s4g_key_syms_comment_os[0][i])
							is_comment++;
				}
		}
	return (is_comment == tmplen);
}

inline bool s4g_is_comment_ms_b(const char* sstr)
{
	int tmplen = strlen(s4g_key_syms_comment_ms[0]);
	int is_comment = 0;
		if(strlen(sstr) >= tmplen)
		{
			
				for(int i=0;i<tmplen;i++)
				{
						if(sstr[i] == s4g_key_syms_comment_ms[0][i])
							is_comment++;
				}
		}
	return (is_comment == tmplen);
}

inline bool s4g_is_comment_ms_e(const char* sstr)
{
	int tmplen = strlen(s4g_key_syms_comment_ms[1]);
	int is_comment = 0;
		if(strlen(sstr) >= tmplen)
		{
			
				for(int i=0;i<tmplen;i++)
				{
						if(sstr[i] == s4g_key_syms_comment_ms[1][i])
							is_comment++;
				}
		}
	return (is_comment == tmplen);
}

inline bool s4g_is_null(const char* sstr)
{
	int tmplen = strlen(s4g_key_words[0]);
	int is_null = 0;
		if(strlen(sstr) >= tmplen)
		{
			
				for(int i=0;i<tmplen;i++)
				{
						if(sstr[i] == s4g_key_words[0][i])
							is_null++;
				}
		}
	return (is_null == tmplen);
}

inline int s4g_is_boolean(const char* sstr,char* dstr)
{
	int truestrlen = strlen(s4g_key_bool[0]);
	int falsestrlen = strlen(s4g_key_bool[1]);
	int is_bool = 0;
		if(strlen(sstr) >= truestrlen)
		{
				for(int i=0;i<truestrlen;i++)
				{
						if(sstr[i] == s4g_key_bool[0][i])
							is_bool++;
				}
		}

		if(is_bool == truestrlen)
		{
			strcpy(dstr,s4g_key_bool[0]);
			return 0;
		}

	is_bool = 0;

		if(strlen(sstr) >= falsestrlen)
		{
				for(int i=0;i<falsestrlen;i++)
				{
						if(sstr[i] == s4g_key_bool[1][i])
							is_bool++;
				}
		}

		if(is_bool == falsestrlen)
		{
			strcpy(dstr,s4g_key_bool[1]);
			return 1;
		}
	return -1;
}

inline bool s4g_is_char_str(const char sym)
{
	return (((sym >= 65 && sym <= 90) || (sym >= 97 && sym <= 122) || sym == 95) ? true : false);
}

inline bool s4g_is_char_num_or_point(const char sym)
{
	return ((s4g_is_char_num(sym) || s4g_is_char_point(sym)) ? true : false);
}

inline bool s4g_is_char_num(const char sym)
{
	return ((sym >= 48 && sym <= 57) ? true : false);
}

inline bool s4g_is_char_point(const char sym)
{
	return (sym == 46 ? true : false);
}

inline bool s4g_is_char_arif(const char sym)
{
	return ((sym == 43 || sym == 45 || sym == 42 || sym == 47) ? true : false);
}

inline bool s4g_is_char_arif_pm(const char sym)
{
	return ((sym == 43 || sym == 45) ? true : false);
}

inline void s4g_scan_string(const char* sstr,char* dstr)
{
	int k=0;
		for(int i=0;i<strlen(sstr);i++)
		{
				if(s4g_is_char_str(sstr[i]) || s4g_is_char_num(sstr[i]))
				{
					dstr[k] = sstr[i];
					k++;
				}
				else
				{
					dstr[k] = 0;
					break;
				}
		}
}

inline void s4g_scan_list_string(const char* sstr,char dstr[64][64])
{
	int k=0;
	int t=0;
	//bool islastdel = false;
		for(int i=0;i<strlen(sstr);i++)
		{
				if(s4g_is_char_str(sstr[i]) || s4g_is_char_num(sstr[i]))
				{
					dstr[t][k] = sstr[i];
					k++;
				}
				else if(sstr[i] == ',')
				{
					dstr[t][k] = 0;
					k=0;
					t++;
					//islastdel = true;
				}
				else if(sstr[i] == ' ' || sstr[i] == '\t')
				{

				}
				else
				{
					dstr[t][k] = 0;
					break;
				}
		}
}

inline void s4g_scan_litstring(const char* sstr,char* dstr)
{
	int k=0;
		for(int i=1;i<strlen(sstr);i++)
		{
				if(sstr[i] != '"' || (i > 0 && sstr[i-1] == '\\'))
				{
						if(sstr[i] != '\\')
						{
							dstr[k] = sstr[i];
							k++;
						}
				}
				/*else if(i > 0 && sstr[i-1] == '\\' && sstr[i] == '"')
				{
					dstr[k] = sstr[i];
					dstr[k+1] = sstr[i+1];
					i++;
					k++;
				}*/
				else
				{
					dstr[k] = 0;
					break;
				}
		}
}

inline int s4g_scan_num(const char* sstr,char* dstr)
{
	int k=0;
	int count_tmp_point = 0;
		for(int i=0;i<strlen(sstr);i++)
		{
				if(s4g_is_char_num_or_point(sstr[i]) || (i==0 && s4g_is_char_arif_pm(sstr[i])))
				{
						if(s4g_is_char_point(sstr[i]))
							count_tmp_point++;

						if(count_tmp_point > 1)
							return -1;
					dstr[k] = sstr[i];
					k++;
				}
				else
				{
					break;
				}
		}
	dstr[k] = 0;
	return 0;
}

inline int s4g_is_key_word(const char* str)
{	
	long count_key_words = sizeof(s4g_key_words)/S4G_MAX_LEN_KEY_WORD_DEF;
		for(int i=0;i<count_key_words;i++)
		{
				if(strcmp(s4g_key_words[i],str) == 0)
					return i;
		}
	return -1;
}

inline int s4g_is_key_word_pp(const char* str)
{
	long count_key_words_pp = sizeof(s4g_key_preproc)/S4G_MAX_LEN_KEY_WORD_DEF;
		for(int i=0;i<count_key_words_pp;i++)
		{
				if(strcmp(s4g_key_preproc[i],str) == 0)
					return i;
		}
	return -1;
}

////////////////
s4g_arr_lex::s4g_arr_lex()
{
	curr_num = -1;
}
void s4g_arr_lex::read(const char* file,String AllFile)
{
	long lenfile = AllFile.length();	//размер всего файла с кодом
	long numcursym = 0;	//текущий номер символа
	long numcurstr = 1;	//текущий номер строки
	char tmpword[64];
	int typecomment = 0; //текущий тип комментария, 0 - нет, 1 - однострочный, 2 - многострочный
	int tmpid = -1;
		while(numcursym <= lenfile)
		{
			memset(tmpword,0,64);
			char tmpc = AllFile[numcursym];
				//если текущий сивол новая строка
				if(tmpc == '\n')
				{
					numcurstr++;
					numcursym++;
						if(typecomment == 1)
							typecomment = 0;
				}
				//иначе если у нас возник однострочный комментарий и до этого не было комментариев
				else if(typecomment == 0 && s4g_is_comment_os(AllFile.c_str()+numcursym))
				{
					typecomment = 1;
					numcursym += strlen(s4g_key_syms_comment_os[0]);
				}
				//если у нас нет комментария и мы на начале многострочного комментария
				else if(typecomment == 0 && s4g_is_comment_ms_b(AllFile.c_str()+numcursym))
				{
					typecomment = 2;
					numcursym += strlen(s4g_key_syms_comment_ms[0]);
				}
				//если у нас многострочный комментарий и мы на конце многострочного
				else if(typecomment == 2 && s4g_is_comment_ms_e(AllFile.c_str()+numcursym))
				{
					typecomment = 0;
					numcursym += strlen(s4g_key_syms_comment_ms[1]);
				}
				//если мы не в комментарии и текущий символ не пробел и не табуляция
				else if(typecomment == 0 && !(tmpc == ' ' || tmpc == '\t'))
				{
						//если у нас команда для препроцессора
						if(tmpc == '#')
						{
							s4g_scan_string(AllFile.c_str()+numcursym+1,tmpword);
								if((tmpid = s4g_is_key_word_pp(tmpword)) != -1)
								{
									ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_prep,tmpid));
									numcursym += strlen(tmpword)+1;
								}
								else
								{
									s4g_output_error("file [%s]:%d unexpected word in preprocessor '%s'",file,numcurstr,tmpword);
								}
						}
						//если текущий символ пользовательская строка
						else if(tmpc == '"')
						{
							s4g_scan_litstring(AllFile.c_str()+numcursym,tmpword);
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_string,-1));
							numcursym += strlen(tmpword)+2;
						}
						else if((tmpid = s4g_is_boolean(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_bool,tmpid));
							numcursym += strlen(tmpword);
						}
						else if(s4g_is_null(AllFile.c_str()+numcursym))
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_null,-1));
							numcursym += strlen(s4g_key_words[0]);
						}
						//иначе если текущий символ является буквой
						else if(s4g_is_char_str(tmpc))
						{
							s4g_scan_string(AllFile.c_str()+numcursym,tmpword);
								if((tmpid = s4g_is_key_word(tmpword)) != -1)
									ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_key,tmpid));
								else
									ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_user,-1));
							numcursym += strlen(tmpword);
						}
						//иначе если текущий символ является числом
						else if(s4g_is_char_num(tmpc))
						{
							s4g_scan_num(AllFile.c_str()+numcursym,tmpword);
								//если есть точка в строке значит это float
								if(strstr(tmpword,"."))
									ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_float,-1));
								else
									ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_int,-1));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к логическим 
						else if((tmpid = s4g_is_syms_logic(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_logic,tmpid));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к арифметическим операциям с приравниванием
						else if((tmpid = s4g_is_syms_arif_assign(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_arif_assign,tmpid));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к арифметическим
						else if((tmpid = s4g_is_syms_arif(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_arif,tmpid));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к разделителям
						else if((tmpid = s4g_is_delimiter(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_delimiter,tmpid));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к присваивателям
						else if((tmpid = s4g_is_assign(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_assign,tmpid));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к созданию таблицы
						else if((tmpid = s4g_is_table_create(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_table_create,tmpid));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к обращению к элементу таблицы
						else if((tmpid = s4g_is_table_elem(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_table_elem,tmpid));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к группировке/вызову функции
						else if((tmpid = s4g_is_syms_group(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_group,tmpid));
							numcursym += strlen(tmpword);
						}
						else
						{
							s4g_output_error("file [%s]:%d unexpected symbol '%c'",file,numcurstr,tmpc);
						}
				}
				else
					numcursym++;
		}
}

DWORD s4g_arr_lex::add(const char* _str,long _numstr,int _type,int _id)
{
	ArrLexs.push_back(s4g_lexeme(_str,_numstr,_type,_id));
	return ArrLexs.size() - 1;
}

s4g_lexeme* s4g_arr_lex::get(DWORD num)
{
	if(ArrLexs.size() > num)
		return &(ArrLexs[num]);
}

DWORD s4g_arr_lex::get_curr_num()
{
	return curr_num;
}

void s4g_arr_lex::set_curr_num(DWORD num)
{
		if(num >= 0 && num < ArrLexs.size())
			curr_num = num;
}

s4g_lexeme* s4g_arr_lex::get_curr()
{
		if(curr_num >= 0 && curr_num < ArrLexs.size())
			return &(ArrLexs[curr_num]);
}

s4g_lexeme* s4g_arr_lex::get_next()
{
	curr_num++;
		if(curr_num >= 0 && curr_num < ArrLexs.size())
			return &(ArrLexs[curr_num]);
}
///////////////

int s4g_parse_file(const char* file)
{
	FILE* ffile;

		if(!(ffile = fopen(file, "rt")))
		{
			return -1;
		}
	String AllFile,str,str2,str3;
	char tmpvar[64];
	char tmpoperator[64];
	char tmpval[64];

	long tmppos1,tmppos2,tmppos3;

	char text[S4G_MAX_LEN_STR_IN_FILE];

		while(!feof(ffile))
		{
			text[0] = 0;
			fgets(text, 1024, ffile);
			AllFile += text;
		}

	fclose(ffile);

	s4g_arr_lex* arr_lex = new s4g_arr_lex();
	arr_lex->read(file,AllFile);

	//размеры словарей
	/*long count_key_words = sizeof(s4g_key_words)/S4G_MAX_LEN_KEY_WORD_DEF;
	long count_key_preproc = sizeof(s4g_key_preproc)/S4G_MAX_LEN_KEY_WORD_DEF;
	long count_key_syms_del = sizeof(s4g_key_syms_del)/S4G_MAX_LEN_SYM;
	long count_key_syms_logic = sizeof(s4g_key_syms_logic)/S4G_MAX_LEN_SYM;
	long count_key_syms_assign = sizeof(s4g_key_syms_assign)/S4G_MAX_LEN_SYM;
	long count_key_syms_arif = sizeof(s4g_key_syms_arif)/S4G_MAX_LEN_SYM;
	long count_key_syms_arif_assign = sizeof(s4g_key_syms_arif_assign)/S4G_MAX_LEN_SYM;
	long count_key_syms_comment_os = sizeof(s4g_key_syms_comment_os)/S4G_MAX_LEN_SYM;
	long count_key_syms_comment_ms = sizeof(s4g_key_syms_comment_ms)/S4G_MAX_LEN_SYM;*/

	Array<s4g_lexeme> ArrLexs;	//массив лексем
	
	long lenfile = AllFile.length();	//размер всего файла с кодом
	long numcursym = 0;	//текущий номер символа
	long numcurstr = 1;	//текущий номер строки
	char tmpword[64];
	int typecomment = 0; //текущий тип комментария, 0 - нет, 1 - однострочный, 2 - многострочный
	int tmpid = -1;
		while(numcursym <= lenfile)
		{
			memset(tmpword,0,64);
			char tmpc = AllFile[numcursym];
				//если текущий сивол новая строка
				if(tmpc == '\n')
				{
					numcurstr++;
					numcursym++;
						if(typecomment == 1)
							typecomment = 0;
				}
				//иначе если у нас возник однострочный комментарий и до этого не было комментариев
				else if(typecomment == 0 && s4g_is_comment_os(AllFile.c_str()+numcursym))
				{
					typecomment = 1;
					numcursym += strlen(s4g_key_syms_comment_os[0]);
				}
				//если у нас нет комментария и мы на начале многострочного комментария
				else if(typecomment == 0 && s4g_is_comment_ms_b(AllFile.c_str()+numcursym))
				{
					typecomment = 2;
					numcursym += strlen(s4g_key_syms_comment_ms[0]);
				}
				//если у нас многострочный комментарий и мы на конце многострочного
				else if(typecomment == 2 && s4g_is_comment_ms_e(AllFile.c_str()+numcursym))
				{
					typecomment = 0;
					numcursym += strlen(s4g_key_syms_comment_ms[1]);
				}
				//если мы не в комментарии и текущий символ не пробел и не табуляция
				else if(typecomment == 0 && !(tmpc == ' ' || tmpc == '\t'))
				{
						//если у нас команда для препроцессора
						if(tmpc == '#')
						{
							s4g_scan_string(AllFile.c_str()+numcursym+1,tmpword);
								if((tmpid = s4g_is_key_word_pp(tmpword)) != -1)
								{
									ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_prep,tmpid));
									numcursym += strlen(tmpword)+1;
								}
								else
								{
									s4g_output_error("file [%s]:%d unexpected word in preprocessor '%s'",file,numcurstr,tmpword);
								}
						}
						//если текущий символ пользовательская строка
						else if(tmpc == '"')
						{
							s4g_scan_litstring(AllFile.c_str()+numcursym,tmpword);
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_string,-1));
							numcursym += strlen(tmpword)+2;
						}
						else if((tmpid = s4g_is_boolean(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_bool,tmpid));
							numcursym += strlen(tmpword);
						}
						else if(s4g_is_null(AllFile.c_str()+numcursym))
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_null,-1));
							numcursym += strlen(s4g_key_words[0]);
						}
						//иначе если текущий символ является буквой
						else if(s4g_is_char_str(tmpc))
						{
							s4g_scan_string(AllFile.c_str()+numcursym,tmpword);
								if((tmpid = s4g_is_key_word(tmpword)) != -1)
									ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_key,tmpid));
								else
									ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_user,-1));
							numcursym += strlen(tmpword);
						}
						//иначе если текущий символ является числом
						else if(s4g_is_char_num(tmpc))
						{
							s4g_scan_num(AllFile.c_str()+numcursym,tmpword);
								//если есть точка в строке значит это float
								if(strstr(tmpword,"."))
									ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_float,-1));
								else
									ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::word_int,-1));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к логическим 
						else if((tmpid = s4g_is_syms_logic(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_logic,tmpid));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к арифметическим операциям с приравниванием
						else if((tmpid = s4g_is_syms_arif_assign(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_arif_assign,tmpid));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к арифметическим
						else if((tmpid = s4g_is_syms_arif(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_arif,tmpid));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к разделителям
						else if((tmpid = s4g_is_delimiter(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_delimiter,tmpid));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к присваивателям
						else if((tmpid = s4g_is_assign(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_assign,tmpid));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к созданию таблицы
						else if((tmpid = s4g_is_table_create(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_table_create,tmpid));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к обращению к элементу таблицы
						else if((tmpid = s4g_is_table_elem(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_table_elem,tmpid));
							numcursym += strlen(tmpword);
						}
						//если текущая лексема относится к группировке/вызову функции
						else if((tmpid = s4g_is_syms_group(AllFile.c_str()+numcursym,tmpword)) != -1)
						{
							ArrLexs.push_back(s4g_lexeme(tmpword,numcurstr,s4g_lexeme_type::sym_group,tmpid));
							numcursym += strlen(tmpword);
						}
						else
						{
							s4g_output_error("file [%s]:%d unexpected symbol '%c'",file,numcurstr,tmpc);
						}
				}
				else
					numcursym++;
		}
		
		
	int qwer = 0;
}

int s4g_parse_str(const char* str);