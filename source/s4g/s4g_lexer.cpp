
#include "s4g_preprocessor.h"


inline int s4g_is_syms_arif(const char* sstr, char* dstr)
{
	long count_key_syms = sizeof(s4g_key_syms_arif) / S4G_MAX_LEN_SYM;

	for (int i = 0; i<count_key_syms; i++)
	{
		int tmplen = strlen(s4g_key_syms_arif[i]);
		int is_syms = 0;
		if (strlen(sstr) >= tmplen)
		{

			for (int k = 0; k<tmplen; k++)
			{
				if (sstr[k] == s4g_key_syms_arif[i][k])
					is_syms++;
			}

			if (is_syms == tmplen)
			{
				strcpy(dstr, s4g_key_syms_arif[i]);
				return i;
			}
		}
	}
	return -1;
}

inline int s4g_is_syms_arif_assign(const char* sstr, char* dstr)
{
	long count_key_syms = sizeof(s4g_key_syms_arif_assign) / S4G_MAX_LEN_SYM;

	for (int i = 0; i<count_key_syms; i++)
	{
		int tmplen = strlen(s4g_key_syms_arif_assign[i]);
		int is_syms = 0;
		if (strlen(sstr) >= tmplen)
		{

			for (int k = 0; k<tmplen; k++)
			{
				if (sstr[k] == s4g_key_syms_arif_assign[i][k])
					is_syms++;
			}

			if (is_syms == tmplen)
			{
				strcpy(dstr, s4g_key_syms_arif_assign[i]);
				return i;
			}
		}
	}
	return -1;
}

inline int s4g_is_syms_logic(const char* sstr, char* dstr)
{
	long count_key_syms_logic = sizeof(s4g_key_syms_logic) / S4G_MAX_LEN_SYM;

	for (int i = 0; i<count_key_syms_logic; i++)
	{
		int tmplen = strlen(s4g_key_syms_logic[i]);
		int is_syms_logic = 0;
		if (strlen(sstr) >= tmplen)
		{

			for (int k = 0; k<tmplen; k++)
			{
				if (sstr[k] == s4g_key_syms_logic[i][k])
					is_syms_logic++;
			}

			if (is_syms_logic == tmplen)
			{
				strcpy(dstr, s4g_key_syms_logic[i]);
				return i;
			}
		}
	}
	return -1;
}

inline int s4g_is_delimiter(const char* sstr, char* dstr)
{
	long count_key_syms_del = sizeof(s4g_key_syms_del) / S4G_MAX_LEN_SYM;

	for (int i = 0; i<count_key_syms_del; i++)
	{
		int tmplen = strlen(s4g_key_syms_del[i]);
		int is_syms_del = 0;
		if (strlen(sstr) >= tmplen)
		{

			for (int k = 0; k<tmplen; k++)
			{
				if (sstr[k] == s4g_key_syms_del[i][k])
					is_syms_del++;
			}

			if (is_syms_del == tmplen)
			{
				strcpy(dstr, s4g_key_syms_del[i]);
				return i;
			}
		}
	}
	return -1;
}

inline int s4g_is_assign(const char* sstr, char* dstr)
{
	long count_key_syms_assign = sizeof(s4g_key_syms_assign) / S4G_MAX_LEN_SYM;

	for (int i = 0; i<count_key_syms_assign; i++)
	{
		int tmplen = strlen(s4g_key_syms_assign[i]);
		int is_syms_assign = 0;
		if (strlen(sstr) >= tmplen)
		{

			for (int k = 0; k<tmplen; k++)
			{
				if (sstr[k] == s4g_key_syms_assign[i][k])
					is_syms_assign++;
			}

			if (is_syms_assign == tmplen)
			{
				strcpy(dstr, s4g_key_syms_assign[i]);
				return i;
			}
		}
	}
	return -1;
}

inline int s4g_is_table_create(const char* sstr, char* dstr)
{
	long count_key_syms_table_create = sizeof(s4g_key_syms_table_create) / S4G_MAX_LEN_SYM;

	for (int i = 0; i<count_key_syms_table_create; i++)
	{
		int tmplen = strlen(s4g_key_syms_table_create[i]);
		int is_syms_table_create = 0;
		if (strlen(sstr) >= tmplen)
		{

			for (int k = 0; k<tmplen; k++)
			{
				if (sstr[k] == s4g_key_syms_table_create[i][k])
					is_syms_table_create++;
			}

			if (is_syms_table_create == tmplen)
			{
				strcpy(dstr, s4g_key_syms_table_create[i]);
				return i;
			}
		}
	}
	return -1;
}

inline int s4g_is_table_elem(const char* sstr, char* dstr)
{
	long count_key_syms_table_op = sizeof(s4g_key_syms_table_op) / S4G_MAX_LEN_SYM;

	for (int i = 0; i<count_key_syms_table_op; i++)
	{
		int tmplen = strlen(s4g_key_syms_table_op[i]);
		int is_syms_table_op = 0;
		if (strlen(sstr) >= tmplen)
		{

			for (int k = 0; k<tmplen; k++)
			{
				if (sstr[k] == s4g_key_syms_table_op[i][k])
					is_syms_table_op++;
			}

			if (is_syms_table_op == tmplen)
			{
				strcpy(dstr, s4g_key_syms_table_op[i]);
				return i;
			}
		}
	}
	return -1;
}

inline int s4g_is_syms_group(const char* sstr, char* dstr)
{
	long count_key_syms_group = sizeof(s4g_key_syms_group) / S4G_MAX_LEN_SYM;

	for (int i = 0; i<count_key_syms_group; i++)
	{
		int tmplen = strlen(s4g_key_syms_group[i]);
		int is_syms_group = 0;
		if (strlen(sstr) >= tmplen)
		{

			for (int k = 0; k<tmplen; k++)
			{
				if (sstr[k] == s4g_key_syms_group[i][k])
					is_syms_group++;
			}

			if (is_syms_group == tmplen)
			{
				strcpy(dstr, s4g_key_syms_group[i]);
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
	if (strlen(sstr) >= tmplen)
	{

		for (int i = 0; i<tmplen; i++)
		{
			if (sstr[i] == s4g_key_syms_comment_os[0][i])
				is_comment++;
		}
	}
	return (is_comment == tmplen);
}

inline bool s4g_is_comment_ms_b(const char* sstr)
{
	int tmplen = strlen(s4g_key_syms_comment_ms[0]);
	int is_comment = 0;
	if (strlen(sstr) >= tmplen)
	{

		for (int i = 0; i<tmplen; i++)
		{
			if (sstr[i] == s4g_key_syms_comment_ms[0][i])
				is_comment++;
		}
	}
	return (is_comment == tmplen);
}

inline bool s4g_is_comment_ms_e(const char* sstr)
{
	int tmplen = strlen(s4g_key_syms_comment_ms[1]);
	int is_comment = 0;
	if (strlen(sstr) >= tmplen)
	{

		for (int i = 0; i<tmplen; i++)
		{
			if (sstr[i] == s4g_key_syms_comment_ms[1][i])
				is_comment++;
		}
	}
	return (is_comment == tmplen);
}

inline bool s4g_is_null(const char* sstr)
{
	int tmplen = strlen(s4g_key_words[0]);
	int is_null = 0;
	if (strlen(sstr) >= tmplen)
	{

		for (int i = 0; i<tmplen; i++)
		{
			if (sstr[i] == s4g_key_words[0][i])
				is_null++;
		}
	}
	return (is_null == tmplen);
}

inline int s4g_is_boolean(const char* sstr, char* dstr)
{
	int truestrlen = strlen(s4g_key_bool[0]);
	int falsestrlen = strlen(s4g_key_bool[1]);
	int is_bool = 0;
	if (strlen(sstr) >= truestrlen)
	{
		for (int i = 0; i<truestrlen; i++)
		{
			if (sstr[i] == s4g_key_bool[0][i])
				is_bool++;
		}
	}

	if (is_bool == truestrlen)
	{
		strcpy(dstr, s4g_key_bool[0]);
		return 0;
	}

	is_bool = 0;

	if (strlen(sstr) >= falsestrlen)
	{
		for (int i = 0; i<falsestrlen; i++)
		{
			if (sstr[i] == s4g_key_bool[1][i])
				is_bool++;
		}
	}

	if (is_bool == falsestrlen)
	{
		strcpy(dstr, s4g_key_bool[1]);
		return 1;
	}
	return -1;
}

inline bool s4g_is_char_str(const char sym)
{
	return (sym >= 'a' && sym <= 'z') || (sym >= 'A' && sym <= 'Z') || sym == '_';
}

inline bool s4g_is_char_num_or_point(const char sym)
{
	return s4g_is_char_num(sym) || s4g_is_char_point(sym);
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

inline void s4g_scan_string(const char* sstr, char* dstr)
{
	int k = 0;
	for (int i = 0; i<strlen(sstr); i++)
	{
		if (s4g_is_char_str(sstr[i]) || s4g_is_char_num(sstr[i]))
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
/*
inline int s4g_is_marg(const char* sstr, char* dstr)
{
	long count_amarg = sizeof(s4g_key_syms_marg) / S4G_MAX_LEN_KEY_WORD_DEF;
	for (int i = 0; i<count_amarg; i++)
	{
		if (strcmp(s4g_key_syms_marg[i], sstr) == 0)
		{
			strcpy(dstr, s4g_key_syms_marg[i]);
			return i;
		}
	}
	return -1;
}*/


inline void s4g_scan_litstring(const char* cur, char* dstr, int & len)
{
	++cur;
	while(*cur && *cur != '"')
	{
		if(*cur == '\\')
		{
			++cur; ++len;
			switch(*cur)
			{
			case L'"':
			case L'\\':
			case L'/':
				*dstr++ = *cur;
				break;
			case L'b':
				*dstr++ = L'\b';
				break;
			case L'f':
				*dstr++ = L'\f';
				break;
			case L'n':
				*dstr++ = L'\n';
				break;
			case L'r':
				*dstr++ = L'\r';
				break;
			case L't':
				*dstr++ = L'\t';
				break;
			}
		}
		else
		{
			*dstr++ = *cur;
		}
		++cur; ++len;
	}
	*dstr = 0;
}

inline int s4g_scan_num(const char* sstr, char* dstr)
{
	int k = 0;
	int count_tmp_point = 0;
	int count_read_sym = 0;
	for (int i = 0; i<strlen(sstr); i++)
	{
		if (s4g_is_char_num_or_point(sstr[i]) || (i == 0 && s4g_is_char_arif_pm(sstr[i])) ||
			(count_read_sym == 0 && (sstr[i] == 'u' || sstr[i] == 'f')))
		{
			if (s4g_is_char_point(sstr[i]))
				count_tmp_point++;

			if (count_tmp_point > 1)
				return -1;

			if (sstr[i] == 'u' || sstr[i] == 'f')
				count_read_sym++;
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
	long count_key_words = sizeof(s4g_key_words) / S4G_MAX_LEN_KEY_WORD_DEF;
	for (int i = 0; i<count_key_words; i++)
	{
		if (strcmp(s4g_key_words[i], str) == 0)
			return i;
	}
	return -1;
}

inline int s4g_is_key_word_pp(const char* str)
{
	long count_key_words_pp = sizeof(s4g_key_preproc) / S4G_MAX_LEN_KEY_WORD_DEF;
	for (int i = 0; i<count_key_words_pp; i++)
	{
		if (strcmp(s4g_key_preproc[i], str) == 0)
			return i;
	}
	return -1;
}

////////////////
s4g_arr_lex::s4g_arr_lex()
{
	curr_num = -1;
	strerror[0] = 0;
}

s4g_lexeme* s4g_arr_lex::r_get_lexeme(const char* str, long* curr_pos, long* curr_num_str)
{
	s4g_lexeme* tmplex = 0;
	long numcursym = 0;	//текущий номер символа
	long numcurstr = *curr_num_str;	//текущий номер строки
	char tmpword[S4G_MAX_LEN_VAR_NAME];
	int typecomment = 0; //текущий тип комментария, 0 - нет, 1 - однострочный, 2 - многострочный
	int tmpid = -1;
	while (true)
	{
		tmpword[0] = 0;
		//memset(tmpword, 0, S4G_MAX_LEN_VAR_NAME);
		char tmpc = str[numcursym];

		if (tmpc == 0)
		{
			break;
		}

		//если текущий сивол новая строка
		if (tmpc == '\n')
		{
			numcurstr++;
			numcursym++;
			if (typecomment == 1)
				typecomment = 0;
		}
		//иначе если у нас возник однострочный комментарий и до этого не было комментариев
		else if (typecomment == 0 && s4g_is_comment_os(str + numcursym))
		{
			typecomment = 1;
			numcursym += strlen(s4g_key_syms_comment_os[0]);
		}
		//если у нас нет комментария и мы на начале многострочного комментария
		else if (typecomment == 0 && s4g_is_comment_ms_b(str + numcursym))
		{
			typecomment = 2;
			numcursym += strlen(s4g_key_syms_comment_ms[0]);
		}
		//если у нас многострочный комментарий и мы на конце многострочного
		else if (typecomment == 2 && s4g_is_comment_ms_e(str + numcursym))
		{
			typecomment = 0;
			numcursym += strlen(s4g_key_syms_comment_ms[1]);
		}
		//если мы не в комментарии и текущий символ не пробел и не табуляция
		else if (typecomment == 0 && !isspace(tmpc))
		{
			//если у нас команда для препроцессора
			if (tmpc == '#')
			{
				s4g_scan_string(str + numcursym + 1, tmpword);
				tmpid = s4g_is_key_word_pp(tmpword);
				//if((tmpid = s4g_is_key_word_pp(tmpword)) != -1)
				//{
				tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_prep, tmpid, curr_id_file);
				numcursym += strlen(tmpword) + 1;
				break;
				//}
				//else
				//{
				//s4g_output_error("file [%s]:%d unexpected word in preprocessor '%s'",file,numcurstr,tmpword);
				//}
			}
			//если текущий символ пользовательская строка
			else if (tmpc == '"')
			{
				int len = 0;
				s4g_scan_litstring(str + numcursym, tmpword, len);
				tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_string, -1, curr_id_file);
				numcursym += len + 2;
				break;
			}
			else if ((tmpid = s4g_is_boolean(str + numcursym, tmpword)) != -1)
			{
				tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_bool, tmpid, curr_id_file);
				numcursym += strlen(tmpword);
				break;
			}
			else if (s4g_is_null(str + numcursym))
			{
				tmplex = LexPool.Alloc("null", numcurstr, s4g_lexeme_type::word_null, -1, curr_id_file);
				numcursym += strlen(s4g_key_words[0]);
				break;
			}
			else if (str[numcursym] == '.' && str[numcursym+1] == '.' && str[numcursym+2] == '.')
			{
				tmplex = LexPool.Alloc("...", numcurstr, s4g_lexeme_type::marg, -1, curr_id_file);
				numcursym += 3;
				break;
			}
			else if (tmpc == '$')
			{
				s4g_lexeme* tmplexs = 0;
				if (ArrLexs.size() > 0)
					tmplexs = ArrLexs[ArrLexs.size() - 1];

				numcursym++;
				tmpc = str[numcursym];
				if (s4g_is_char_str(tmpc))
				{
					s4g_scan_string(str + numcursym, tmpword);
					if ((tmpid = s4g_is_key_word(tmpword)) == -1)
						tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_user_cr, -1, curr_id_file);
					else
					{
						sprintf(strerror, "[%s]:%d - unresolved using of key word '%s' for create a new element", ArrFiles[curr_id_file], numcurstr, tmpword);
						return 0;
					}
					numcursym += strlen(tmpword);
					break;
				}
				else if (tmplexs && tmplexs->type == sym_table_elem)
				{
					if (s4g_is_char_num(tmpc))
					{
						s4g_scan_num(str + numcursym, tmpword);
						numcursym += strlen(tmpword);
						//если есть точка в строке значит это float
						if (strstr(tmpword, "."))
						{
							sprintf(strerror, "[%s]:%d - '%s' is float type this is unresolved for create a new element", ArrFiles[curr_id_file], numcurstr, tmpword);
							return 0;
						}
						else
						{
							int slen = strlen(tmpword) - 1;
							if (tmpword[slen] == 'u')
								tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_uint_cr, -1, curr_id_file);
							else if (tmpword[slen] == 'f')
							{
								sprintf(strerror, "[%s]:%d - '%s' is float type this is unresolved for create a new element", ArrFiles[curr_id_file], numcurstr, tmpword);
								return 0;
							}
							else
								tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_int_cr, -1, curr_id_file);
						}

						break;
					}
					else if (tmpc == '"')
					{
						int len = 0;
						s4g_scan_litstring(str + numcursym, tmpword, len);
						tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_string_cr, -1, curr_id_file);
						numcursym += len + 2;
						break;
					}
					else
					{
						sprintf(strerror, "[%s]:%d - '%s' unresolved type for create a new element in table", ArrFiles[curr_id_file], numcurstr, tmpword);
						return 0;
					}
				}
				else
				{
					sprintf(strerror, "[%s]:%d - '$' unresolved notation to this lexeme", ArrFiles[curr_id_file], numcurstr);
					return 0;
				}
			}
			//иначе если текущий символ является буквой
			else if (s4g_is_char_str(tmpc))
			{
				s4g_scan_string(str + numcursym, tmpword);
				if ((tmpid = s4g_is_key_word(tmpword)) != -1)
					tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_key, tmpid, curr_id_file);
				else
					tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_user, -1, curr_id_file);
				numcursym += strlen(tmpword);
				break;
			}
			//иначе если текущий символ является числом
			else if (s4g_is_char_num(tmpc))
			{
				s4g_scan_num(str + numcursym, tmpword);
				numcursym += strlen(tmpword);
				//если есть точка в строке значит это float
				if (strstr(tmpword, "."))
					tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_float, -1, curr_id_file);
				else
				{
					int slen = strlen(tmpword) - 1;
					if (tmpword[slen] == 'u')
						tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_uint, -1, curr_id_file);
					else if (tmpword[slen] == 'f')
						tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_float, -1, curr_id_file);
					else
						tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_int, -1, curr_id_file);
				}

				break;
			}
			//если текущая лексема относится к логическим 
			else if ((tmpid = s4g_is_syms_logic(str + numcursym, tmpword)) != -1)
			{
				tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::sym_logic, tmpid, curr_id_file);
				numcursym += strlen(tmpword);
				break;
			}
			//если текущая лексема относится к арифметическим операциям с приравниванием
			else if ((tmpid = s4g_is_syms_arif_assign(str + numcursym, tmpword)) != -1)
			{
				tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::sym_arif_assign, tmpid, curr_id_file);
				numcursym += strlen(tmpword);
				break;
			}
			//если текущая лексема относится к арифметическим
			else if ((tmpid = s4g_is_syms_arif(str + numcursym, tmpword)) != -1)
			{
				tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::sym_arif, tmpid, curr_id_file);
				numcursym += strlen(tmpword);
				break;
			}
			//если текущая лексема относится к разделителям
			else if ((tmpid = s4g_is_delimiter(str + numcursym, tmpword)) != -1)
			{
				tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::sym_delimiter, tmpid, curr_id_file);
				numcursym += strlen(tmpword);
				break;
			}
			//если текущая лексема относится к присваивателям
			else if ((tmpid = s4g_is_assign(str + numcursym, tmpword)) != -1)
			{
				tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::sym_assign, tmpid, curr_id_file);
				numcursym += strlen(tmpword);
				break;
			}
			//если текущая лексема относится к созданию таблицы
			else if ((tmpid = s4g_is_table_create(str + numcursym, tmpword)) != -1)
			{
				tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::sym_table_create, tmpid, curr_id_file);
				numcursym += strlen(tmpword);
				break;
			}
			//если текущая лексема относится к обращению к элементу таблицы
			else if ((tmpid = s4g_is_table_elem(str + numcursym, tmpword)) != -1)
			{
				tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::sym_table_elem, tmpid, curr_id_file);
				numcursym += strlen(tmpword);
				break;
			}
			//если текущая лексема относится к группировке/вызову функции
			else if ((tmpid = s4g_is_syms_group(str + numcursym, tmpword)) != -1)
			{
				tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::sym_group, tmpid, curr_id_file);
				numcursym += strlen(tmpword);
				break;
			}
			else
			{
				sprintf(strerror, "[%s]:%d - unresolved symbol [%c]", ArrFiles[curr_id_file], numcurstr, tmpc);
				return 0;
			}
		}
		else
			numcursym++;
	}

	//if(curr_pos)
	*curr_pos += numcursym;

	//if(curr_num_str)
	*curr_num_str = numcurstr;
	return tmplex;
}

int s4g_arr_lex::read(const char* file_str, bool isfile)
{
	char pathforfile[1024];
	pathforfile[0] = 0;
	String AllFile;

	s4g_preprocessor preproc;

	if (isfile)
	{
		FILE* ffile;

		if(!(ffile = fopen(file_str, "rb")))
		{
			return -1;
		}
		
		int len = strlen(file_str);
		while(len >= 0)
		{
			len--;
			if (file_str[len] == '\\' || file_str[len] == '/')
			{
				//
				memcpy(pathforfile, file_str, len);
				pathforfile[len] = 0;
				break;
			}
		}

		if(pathforfile[0])
		{
			preproc.AddIncPath(pathforfile);
		}

		ArrFiles.push_back(file_str);
		char text[S4G_MAX_LEN_STR_IN_FILE];

		int size;
		fseek(ffile, 0, SEEK_END);
		size = ftell(ffile);
		fseek(ffile, 0, SEEK_SET);
		AllFile.Reserve(size + 2);
		fread((void*)(AllFile.c_str()), 1, size, ffile);
		AllFile[size] = '\n';
		AllFile[size + 1] = 0;

		fclose(ffile);
	}
	else
	{
		AllFile = file_str;
		char user_str[256];
		sprintf(user_str, "user str #%d", ArrFiles.size());
		ArrFiles.push_back(user_str);
	}

	curr_id_file = ArrFiles.size() - 1;

	AllFile = preproc.Process(AllFile.c_str(), file_str);
	s4gm->create_var = preproc.isDefined(S4G_CREATE_VAR);
	if(preproc.IsError())
	{
		sprintf(strerror, "%s", preproc.GetError());
		return -1;
	}

	long numcursym = 0;
	long numcurstr = 1;
	while (1)
	{
		if (AllFile.length() <= numcursym)
			break;
		s4g_lexeme* tmplex = r_get_lexeme(AllFile.c_str() + numcursym, &numcursym, &numcurstr);
		if (tmplex)
		{
			//если тип лексемы относится к препроцессору
			if (tmplex->type == word_prep)
			{
				if(tmplex->id == S4GLPP_LINE)
				{
					s4g_lexeme* tmplex2 = r_get_lexeme(AllFile.c_str() + numcursym, &numcursym, &numcurstr);
					if(tmplex2->type != word_int)
					{
						sprintf(strerror, "[%s]:%d - #line expected a line number, found [%s]", ArrFiles[tmplex2->fileid], tmplex2->numstr, tmplex2->str);
						return -1;
					}
					sscanf(tmplex2->str, "%d", &numcurstr);
					--numcurstr;
					LexPool.Delete(tmplex2);

					tmplex2 = r_get_lexeme(AllFile.c_str() + numcursym, &numcursym, &numcurstr);
					if(tmplex2->type != word_string)
					{
						sprintf(strerror, "[%s]:%d - #line expected a string containing the filename, found [%s]", ArrFiles[tmplex2->fileid], tmplex2->numstr, tmplex2->str);
						return -1;
					}
					int idx = -1;
					for(int ci = 0, cl = ArrFiles.size(); ci < cl; ++ci)
					{
						if(!strcmp(ArrFiles[ci].c_str(), tmplex2->str))
						{
							idx = ci;
							break;
						}
					}
					if(idx < 0)
					{
						idx = ArrFiles.size();
						ArrFiles.push_back(tmplex2->str);
					}
					curr_id_file = idx;
					LexPool.Delete(tmplex2);
				}
			}
			//иначе считанная лексема это код
			else
				ArrLexs.push_back(tmplex);
		}
		else
		{
			if (strerror[0] == 0)
				break;
			else
				return -1;
		}
	}

	/*ffile = fopen((String(file)+".tmp").c_str(), "wt");

	for(long i=0;i<ArrLexs.size();i++)
	{
	fprintf(ffile,"%s : %d\n",ArrLexs[i]->str,ArrLexs[i]->type);
	}

	fclose(ffile);*/
	return 0;
}

s4g_lexeme* s4g_arr_lex::get(DWORD num)
{
	if (ArrLexs.size() > num)
		return (ArrLexs[num]);
}

DWORD s4g_arr_lex::get_curr_num()
{
	return curr_num;
}

DWORD s4g_arr_lex::get_count()
{
	return ArrLexs.size();
}

void s4g_arr_lex::set_curr_num(DWORD num)
{
	if (num >= 0 && num < ArrLexs.size())
		curr_num = num;
}

s4g_lexeme* s4g_arr_lex::get_curr()
{
	if (curr_num >= 0 && curr_num < ArrLexs.size())
		return (ArrLexs[curr_num]);
	else
		return 0;
}

s4g_lexeme* s4g_arr_lex::get_next()
{
	//curr_num++;
	if (curr_num + 1 >= 0 && curr_num + 1 < ArrLexs.size())
	{
		curr_num++;
		return (ArrLexs[curr_num]);
	}
	else
	{
		curr_num = ArrLexs.size();
		return 0;
	}
}

s4g_lexeme* s4g_arr_lex::get_prev()
{
	//curr_num--;
	if (curr_num - 1 >= 0 && curr_num - 1 < ArrLexs.size())
	{
		curr_num--;
		return (ArrLexs[curr_num]);
	}
	else
	{
		curr_num = 0;
		return 0;
	}
}