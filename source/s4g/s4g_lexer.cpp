

int s4g_parse_str_is_exists(const char* str)
{
	return 1;
}

void s4g_output_error(const char* format, ...)
{
	va_list va;
	char buf[4096];
	va_start(va, format);
	vsprintf_s(buf, 4096, format, va);
	va_end(va);
	MessageBox(0, buf, 0, MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL);
	exit(0);
}

/////////

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
		else if (typecomment == 0 && !(tmpc == ' ' || tmpc == '\t'))
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
				tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_null, -1, curr_id_file);
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
				numcursym++;
				tmpc = str[numcursym];
				if (s4g_is_char_str(tmpc))
				{
					s4g_scan_string(str + numcursym, tmpword);
					if ((tmpid = s4g_is_key_word(tmpword)) == -1)
						tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_user_cr, -1, curr_id_file);
					else
					{
						sprintf(strerror, "[%s]:%d - !!!!!!!!!!!!!!!!!!!!", ArrFiles[curr_id_file], numcurstr, tmpc);
						return 0;
					}
					numcursym += strlen(tmpword);
					break;
				}
				else if (s4g_is_char_num(tmpc))
				{
					s4g_scan_num(str + numcursym, tmpword);
					numcursym += strlen(tmpword);
					//если есть точка в строке значит это float
					if (strstr(tmpword, "."))
					{
						sprintf(strerror, "[%s]:%d - !!!!!!!!!!!!!!!!!!!!", ArrFiles[curr_id_file], numcurstr, tmpc);
						return 0;
					}
					else
					{
						int slen = strlen(tmpword) - 1;
						if (tmpword[slen] == 'u')
							tmplex = LexPool.Alloc(tmpword, numcurstr, s4g_lexeme_type::word_uint_cr, -1, curr_id_file);
						else if (tmpword[slen] == 'f')
						{
							sprintf(strerror, "[%s]:%d - !!!!!!!!!!!!!!!!!!!!", ArrFiles[curr_id_file], numcurstr, tmpc);
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
					sprintf(strerror, "[%s]:%d - !!!!!!!!!!!!!!!!!!!!", ArrFiles[curr_id_file], numcurstr, tmpc);
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
			else if (tmpc == 0)
			{
				numcursym++;
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

	if (isfile)
	{
		FILE* ffile;

		if (!(ffile = fopen(file_str, "rt")))
		{
			return -1;
		}
		
		int len = strlen(file_str);
		while (len >= 0)
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

		ArrFiles.push_back(file_str);
		char text[S4G_MAX_LEN_STR_IN_FILE];

		while (!feof(ffile))
		{
			text[0] = 0;
			fgets(text, 1024, ffile);
			AllFile += text;
		}

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
				//если это инклюд (подключение файла)
				if(tmplex->id == S4GLPP_INCLUDE)
				{
					s4g_lexeme* tmplex2 = r_get_lexeme(AllFile.c_str() + numcursym, &numcursym, &numcurstr);
					if (tmplex2->type == word_string)
					{
						char newpath[1024];
						sprintf(newpath, "%s%s", pathforfile, tmplex2->str);
						int  tmp_curr_id_file = curr_id_file;
						if (read(newpath) == -1)
							return -1;
						curr_id_file = tmp_curr_id_file;
					}
				}
				else if(tmplex->id == S4GLPP_LINE)
				{
					s4g_lexeme* tmplex2 = r_get_lexeme(AllFile.c_str() + numcursym, &numcursym, &numcurstr);
					if(tmplex2->type != word_int)
					{
						sprintf(strerror, "[%s]:%d - #line expected a line number, found [%s]", ArrFiles[tmplex2->fileid], tmplex2->numstr, tmplex2->str);
						return -1;
					}
					sscanf(tmplex2->str, "%d", &numcurstr);
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
				//если это дефайн
				else if(tmplex->id == S4GLPP_DEFINE)
				{
					//считываем имя
					s4g_lexeme* tmpnamedef = r_get_lexeme(AllFile.c_str() + numcursym, &numcursym, &numcurstr);
					//если имя дефайна пользовательское слово а не ключевое
					if (tmpnamedef->type == word_user)
					{
						s4g_define* tmpdef = (s4g_define*)alloca(sizeof(s4g_define));
						sprintf(tmpdef->name, "%s", tmpnamedef->str);
						long tmpoldnsym = numcursym;
						s4g_lexeme* tmplexg = r_get_lexeme(AllFile.c_str() + numcursym, &numcursym, &numcurstr);
						//если у нас не конец файла
						if (tmplexg)
						{
							long oldnstr = numcurstr;

							//если у нас открывающая скобка
							if (tmplexg->type == sym_group && tmplexg->id == 0 && numcursym - tmpoldnsym == strlen(s4g_key_syms_group[0]))
							{
								//удаляем за ненадобностью лексему скобки
								LexPool.Delete(tmplexg);
								tmplexg = NULL;
								while (1)
								{
									s4g_lexeme* tmplex2 = r_get_lexeme(AllFile.c_str() + numcursym, &numcursym, &numcurstr);
									//ели мы все еще находимся в зоне дефайна
									if (oldnstr == numcurstr)
									{
										//если считали лексему и она является пользовательским словом
										if (tmplex2 && tmplex2->type == word_user)
										{
											//проверяем на уникальность имя нового аргумента
											bool is_arg_unic = true;
											for (int i = 0; i<tmpdef->arg.size(); i++)
											{
												if (tmpdef->arg[i] == tmplex2->str)
												{
													is_arg_unic = false;
													break;
												}
											}

											//если аргумент не уникальный выдаем ошибку
											if (!is_arg_unic)
											{
												sprintf(strerror, "[%s]:%d - none unic argument [%s] in define", ArrFiles[tmplex2->fileid], tmplex2->numstr, tmplex2->str);
												return -1;
											}

											tmpdef->arg.push_back(tmplex2->str); //записываем имя аргумента
											//считываем следующую лексему
											s4g_lexeme* tmplex3 = r_get_lexeme(AllFile.c_str() + numcursym, &numcursym, &numcurstr);

											//если лексему не удалось считать то это конец файла
											if (!tmplex3)
											{
												//внезапный конец файла, а мы еще не закончили считывание аргументов
												sprintf(strerror, "[%s]:%d - unexpected end of file in define", ArrFiles[tmplex2->fileid], tmplex2->numstr);
												return -1;
											}

											//если эта лексема закрывающая скобка
											if (tmplex3->type == sym_group && tmplex3->id == 1)
											{
												//удаляем ее и прерываем цикл
												LexPool.Delete(tmplex3);
												tmplex3 = NULL;
												break;
											}
											//если эта лексема запятая
											else if (tmplex3->type == sym_delimiter && tmplex3->id == 1)
											{
												LexPool.Delete(tmplex3);
												tmplex3 = NULL;
											}
											//иначе это другая лексема, а это ошибка
											else
											{

												sprintf(strerror, "[%s]:%d - unresolved lexeme [%s] in define, ojidal delimiter or dest zakriv skobky", ArrFiles[tmplex3->fileid], tmplex3->numstr, tmplex3->str);
												return -1;
											}
										}
										//иначе ошибка
										else // error
										{
											if (!tmplex2)
											{
												//внезапный конец файла, а мы еще не закончили считывание аргументов
												sprintf(strerror, "[%s]:%d - vnezapno end of file in define", ArrFiles[tmplex2->fileid], tmplex2->numstr);
												return -1;
											}
											else if (tmplex2 && tmplex2->type != word_user)
											{
												//тип лексемы-аргумента не пользовательское слово
												sprintf(strerror, "[%s]:%d - unresolved word [%s] in define", ArrFiles[tmplex2->fileid], tmplex2->numstr, tmplex2->str);
												return -1;
											}
										}
									}
									//иначе мы еще не закрыли скобку и перешли на другую строку - ошибка
									else
									{
										sprintf(strerror, "[%s]:%d - unresolved perehod in new string for define argument", ArrFiles[tmplex2->fileid], tmplex2->numstr);
										return -1;
									}
								}
							}
							//иначе мы считали лексему кода дефайна
							else
							{
								//удаляем эту лексему
								LexPool.Delete(tmplexg);
								tmplexg = NULL;
								//переводим назад данные на считывание предыд
								numcursym = tmpoldnsym;
							}

							//записыаем лексемы в дефайн
							while (1)
							{
								long oldnsym = numcursym;
								s4g_lexeme* tmplex2 = r_get_lexeme(AllFile.c_str() + numcursym, &numcursym, &numcurstr);
								//если мы еще читаем лексемы относящиеся к дефайну
								if (oldnstr == numcurstr)
								{
									if (tmplex2)
									{
										//если у дефайна нет аргументов
										if (tmpdef->arg.size() > 0)
										{
											bool tmp_l_is_w = false;	//является ли лексема аргументом дефайна
											for (int i = 0; i<tmpdef->arg.size(); i++)
											{
												//если лексема дефайна является аргументом дефайна
												if (tmpdef->arg[i] == tmplex2->str)
												{
													//то записываем с указанием к какому аргументу относится лексема
													tmpdef->lexs.push_back(s4g_def_lex_arg(tmplex2, i));
													tmp_l_is_w = true;
													break;
												}
											}

											//если лексема дефайна не является аргументом дефайна
											if (!tmp_l_is_w)
												tmpdef->lexs.push_back(s4g_def_lex_arg(tmplex2, -1));
										}
										//если нет аргументов
										else //то просто записываем
											tmpdef->lexs.push_back(s4g_def_lex_arg(tmplex2, -1));
									}
									else
										break;
								}
								//иначе мы читаем лексемы кода
								else
								{
									//удаляем лексему
									LexPool.Delete(tmplex2);
									tmplex2 = NULL;
									//и переводим назад значения, на предыдущю лексему
									numcurstr = oldnstr;
									numcursym = oldnsym;
									break;
								}
							}
						}
						memcpy(ArrDefines[tmpdef->name], tmpdef, sizeof(s4g_define));
					}
					
					else //error
					{
						sprintf(strerror, "[%s]:%d - unresolved use key word in define [%s]", ArrFiles[tmpnamedef->fileid], tmpnamedef->numstr, tmpnamedef->str);
						return -1;
					}
					int wqert = 0;
				}
				else if(tmplex->id == S4GLPP_UNDEF)
				{
					s4g_lexeme* tmpnamedef = r_get_lexeme(AllFile.c_str() + numcursym, &numcursym, &numcurstr);
						if (ArrDefines.IsExists(tmpnamedef->str))
						{
							ArrDefines.Del(tmpnamedef->str);

							LexPool.Delete(tmpnamedef);
							tmpnamedef = NULL;
						}
				}
				//иначе если лексема не имеет идентификатора то это подстановка пользовательского дефайна
				else if(tmplex->id == S4GLPP_USER)
				{
					//если дефайн существует
					if (ArrDefines.IsExists(tmplex->str))
					{
						s4g_define* tmpdef = ArrDefines[tmplex->str];
						//если дефайн не функция
						if (tmpdef->arg.size() == 0)
						{
							//просто вставляем все лексемы дефайна
							for (int i = 0; i<tmpdef->lexs.size(); i++)
							{
								//заменяя id файла на id текущей лексемы и номер стркои на текущую лексему
								s4g_lexeme* tmpll = tmpdef->lexs[i].lexeme;
								tmpll->fileid = tmplex->fileid;
								tmpll->numstr = tmplex->numstr;
								ArrLexs.push_back(tmpll);
							}
						}
						//иначе дефайн с аргументами
						else
						{
							s4g_lexeme* tmplex3 = r_get_lexeme(AllFile.c_str() + numcursym, &numcursym, &numcurstr);
							//если при подстановке дефайна следует открывающая скобка
							if (tmplex3->type == sym_group && tmplex3->id == 0)
							{
								//все в норме, считываем аргументы
								Array<Array<s4g_lexeme*>> tmplexss;
								int tmphowarg = 0;
								while (1)
								{
									s4g_lexeme* tmplex2 = r_get_lexeme(AllFile.c_str() + numcursym, &numcursym, &numcurstr);

									//если не удалось считать лексему то скорее всего конец файла
									if (!tmplex2) //error
									{
										sprintf(strerror, "[%s]:%d - неожиданный конец файла при считываниии аргументов дефайна [%s]", ArrFiles[tmplex->fileid], tmplex->numstr, tmplex->str);
										return -1;
									}

									//если лексема это разделитель аргументов
									if (tmplex2->type == sym_delimiter && tmplex2->id == 1)
									{
										//если мы хоть что-то считали в аргумент
										if (tmplexss[tmphowarg].size() > 0)
											tmphowarg++;	//подтверждаем считывание аргумента
										//иначе мы ничего не считали а получили разделитель аргументов
										else //error
										{
											sprintf(strerror, "[%s]:%d - ничего не считано в [%d] аргумент дефайна [%s]", ArrFiles[tmplex->fileid], tmplex->numstr, tmphowarg, tmplex->str);
											return -1;
										}

										//если количество считанных аргументов больше либо равно количеству аргументов
										if (tmphowarg >= tmpdef->arg.size())	//error
										{
											sprintf(strerror, "[%s]:%d - больше аргументов в дефайне [%s] либо лишняя запятая", ArrFiles[tmplex->fileid], tmplex->numstr, tmphowarg, tmplex->str);
											return -1;
										}
									}
									//иначе если считаная лексема это закрывающая скобка
									else if (tmplex2->type == sym_group && tmplex2->id == 1)
									{
										//подтвеждаем считывание аргумента и останалвиваем цикл считывания
										tmphowarg++;
										break;
									}
									//иначе лексема является составляющей аргумента
									else
									{
										//добавляем лексему к аргументу
										tmplexss[tmphowarg].push_back(tmplex2);
									}
								}

								//если количество аргументов дефайна не равно колиеству считанных лексем
								if (tmphowarg != tmpdef->arg.size())	//error
								{
									sprintf(strerror, "[%s]:%d - в дефайне [%s] всего аргументов [%d], а считано [%d]", ArrFiles[tmplex->fileid], tmplex->numstr, tmphowarg, tmplex->str, tmpdef->arg.size(), tmphowarg);
									return -1;
								}
								//иначе все в норме
								else
								{
									//подставляем лексемы из дефайна
									for (int i = 0; i<tmpdef->lexs.size(); i++)
									{
										//получаем номер аргумента
										int numarg = tmpdef->lexs[i].num_arg;
										//если лексема не является аргументом
										if (numarg == -1)
											ArrLexs.push_back(tmpdef->lexs[i].lexeme);	//просто подставляем
										//иначе лексема является аргументом
										else
										{
											//заменяем лексему дефайна на лексемы аргумента
											for (int k = 0; k<tmplexss[tmpdef->lexs[i].num_arg].size(); k++)
											{
												s4g_lexeme* ttl = tmplexss[tmpdef->lexs[i].num_arg][k];
												ArrLexs.push_back(ttl);
											}
										}
									}
								}
							}
							//при подстановке дефайна не следует открывающая скобка, выдаем ошибку
							else // error
							{
								sprintf(strerror, "[%s]:%d - in define expected otkr skob but get [%s]", ArrFiles[tmplex->fileid], tmplex->numstr, tmplex3->str);
								return -1;
							}
						}
					}
					//иначе дефайна нет, подставить нечего, выдаем ошибку
					else
					{
						sprintf(strerror, "[%s]:%d - define [%s] is not exists", ArrFiles[tmplex->fileid], tmplex->numstr, tmplex->str);
						return -1;
					}
				}
			}
			//иначе считанная лексема это код
			else
				ArrLexs.push_back(tmplex);
		}
		else
			break;
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