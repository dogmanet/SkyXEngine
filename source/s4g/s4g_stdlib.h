
#ifndef s4g_stdlib_h
#define s4g_stdlib_h

#define S4G_STDLIB_COND_ARG(s4gm,countarg,expectedca)\
if (countarg < expectedca)\
{\
	s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%d function '%s' expected %d arguments, but got %d", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), s4g_dbg_get_curr_func(s4gm), expectedca, countarg); \
	return -1; \
}

int s4g_std_format_str(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	String fstr;
	char tmpstr[64];
	const char* strfmt = s4g_cfget_str(s4gm, 1);
	int curr_arg = 2;
	fstr = strfmt;
	const char* tmpfind = strfmt;
	while (tmpfind = strstr(tmpfind, "%"))
	{
		if (curr_arg > countarg)
		{
			s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%s format string expect more arguments than it was transferred", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm));
			return -1;
		}

		if (tmpfind[1] == 's')
		{
			if (s4g_cfis_str(s4gm, curr_arg))
			{
				const char* argn = s4g_cfget_str(s4gm, curr_arg);
				fstr.Replace("%s", argn, tmpfind - strfmt);
			}
			else
			{
				s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%s format string expected arg #%d string, but got '%s'", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), curr_arg, s4g_cfget_str_type(s4gm, curr_arg));
				return -1;
			}
		}
		else if (tmpfind[1] == 'i')
		{
			if (s4g_cfis_int(s4gm, curr_arg))
			{
				s4g_int argn = s4g_cfget_int(s4gm, curr_arg);
				tmpstr[0] = 0;
				sprintf(tmpstr, "%i", argn);
				fstr.Replace("%i", tmpstr, tmpfind - strfmt);
			}
			else
			{
				s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%s format string expected arg #%d int, but got '%s'", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), curr_arg, s4g_cfget_str_type(s4gm, curr_arg));
				return -1;
			}
		}
		else if (tmpfind[1] == 'u')
		{
			if (s4g_cfis_uint(s4gm, curr_arg))
			{
				s4g_uint argn = s4g_cfget_uint(s4gm, curr_arg);
				tmpstr[0] = 0;
				sprintf(tmpstr, "%u", argn);
				fstr.Replace("%u", tmpstr, tmpfind - strfmt);
			}
			else
			{
				s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%s format string expected arg #%d uint, but got '%s'", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), curr_arg, s4g_cfget_str_type(s4gm, curr_arg));
				return -1;
			}
		}
		else if (tmpfind[1] == 'b')
		{
			if (s4g_cfis_bool(s4gm, curr_arg))
			{
				s4g_bool argn = s4g_cfget_bool(s4gm, curr_arg);
				tmpstr[0] = 0;
				sprintf(tmpstr, "%d", argn);
				fstr.Replace("%b", tmpstr, tmpfind - strfmt);
			}
			else
			{
				s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%s format string expected arg #%d bool, but got '%s'", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), curr_arg, s4g_cfget_str_type(s4gm, curr_arg));
				return -1;
			}
		}
		else if (tmpfind[1] == 'f')
		{
			if (s4g_cfis_float(s4gm, curr_arg))
			{
				s4g_float argn = s4g_cfget_float(s4gm, curr_arg);
				tmpstr[0] = 0;
				sprintf(tmpstr, "%f", argn);
				fstr.Replace("%f", tmpstr, tmpfind - strfmt);
			}
			else
			{
				s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%s format string expected arg #%d float, but got '%s'", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), curr_arg, s4g_cfget_str_type(s4gm, curr_arg));
				return -1;
			}
		}
		++curr_arg;
		++tmpfind;
	}

	s4g_spush_str(s4gm, fstr.c_str());

	return 1;
}

int s4g_std_print(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

		if (s4g_cfis_str(s4gm, 1))
			printf(s4g_cfget_str(s4gm, 1));
		else
		{
			s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%d function '%s' expected arg #1 string, but got '%s'", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), s4g_dbg_get_curr_func(s4gm), s4g_cfget_str_type(s4gm, 1));
			return -1;
		}

	return 0;
}

int s4g_std_assert(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 2);

		if (s4g_cfis_bool(s4gm, 1) || s4g_cfis_null(s4gm, 1))
		{
			if (!s4g_cfget_bool(s4gm, 1))
			{
				if (s4g_cfis_str(s4gm, 2))
				{
					s4g_gen_msg(s4gm, S4G_ERROR, s4g_cfget_str(s4gm, 2));
					return -1;
				}
				else
				{
					s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%d function '%s' expected arg #2 string, but got '%s'", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), s4g_dbg_get_curr_func(s4gm), s4g_cfget_str_type(s4gm, 2));
					return -1;
				}
			}
		}
		else
		{
			s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%d function '%s' expected arg #1 bool, but got '%s'", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), s4g_dbg_get_curr_func(s4gm), s4g_cfget_str_type(s4gm, 1));
			return -1;
		}

	return 0;
}

int s4g_std_strlen(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	if (s4g_cfis_str(s4gm, 1))
		s4g_spush_int(s4gm,strlen(s4g_cfget_str(s4gm, 1)));
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%d function '%s' expected arg #1 string, but got '%s'", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), s4g_dbg_get_curr_func(s4gm), s4g_cfget_str_type(s4gm, 1));
		return -1;
	}
	return 1;
}

int s4g_std_tablen(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	if (s4g_cfis_table(s4gm, 1))
	{
		s4g_table* tt = (s4g_table*)s4g_cfget_pdata(s4gm, 1);
		s4g_spush_int(s4gm, s4g_table_size(tt));
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%d function '%s' expected arg #1 string, but got '%s'", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), s4g_dbg_get_curr_func(s4gm), s4g_cfget_str_type(s4gm, 1));
		return -1;
	}

	return 1;
}

int s4g_std_call_gc(s4g_main* s4gm)
{
	s4g_call_gc(s4gm);
	return 0;
}

int s4g_std_unpack(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);
	int countpush = 0;
	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	for (int i = 0; i < countarg; ++i)
	{
		if (s4g_cfis_table(s4gm, 1))
		{
			s4g_table* tt = (s4g_table*)s4g_cfget_pdata(s4gm, i + 1);
			for (int k = 0; k < s4g_table_size(tt); ++k)
			{
				s4g_spush_value(s4gm, s4g_table_get(tt,k));
				++countpush;
			}
		}
		else
		{
			s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%d function '%s' expected arg #%d string, but got '%s'", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), s4g_dbg_get_curr_func(s4gm), i, s4g_cfget_str_type(s4gm, 1));
			return -1;
		}
	}
	return countpush;
}

///////



int s4g_std_toint(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_type ttype = s4g_cfget_type(s4gm, 1);
	
	if (ttype == t_int)
	{
		s4g_spush_value(s4gm, s4g_cfget_arg(s4gm, 1));
	}
	else if (ttype == t_uint)
	{
		s4g_uint tval = s4g_cfget_uint(s4gm, 1);
		s4g_spush_int(s4gm, tval);
	}
	else if (ttype == t_float)
	{
		s4g_float tval = s4g_cfget_float(s4gm, 1);
		s4g_spush_int(s4gm, tval);
	}
	else if (ttype == t_bool)
	{
		s4g_bool tval = s4g_cfget_bool(s4gm, 1);
		s4g_spush_int(s4gm, tval);
	}
	else if (ttype == t_string)
	{
		const char* tval = s4g_cfget_str(s4gm, 1);
		s4g_int num;
		sscanf(tval, "%d", &num);
		s4g_spush_int(s4gm, num);
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%d function '%s' expected arg #1 valid type, but got %s", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), s4g_dbg_get_curr_func(s4gm), s4g_cfget_str_type(s4gm,1));
		return -1;
	}
	return 1;
}

int s4g_std_touint(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_type ttype = s4g_cfget_type(s4gm, 1);

	if (ttype == t_int)
	{
		s4g_int tval = s4g_cfget_int(s4gm, 1);
		s4g_spush_uint(s4gm, tval);
	}
	else if (ttype == t_uint)
	{
		s4g_spush_value(s4gm, s4g_cfget_arg(s4gm, 1));
	}
	else if (ttype == t_float)
	{
		s4g_float tval = s4g_cfget_float(s4gm, 1);
		s4g_spush_uint(s4gm, tval);
	}
	else if (ttype == t_bool)
	{
		s4g_bool tval = s4g_cfget_bool(s4gm, 1);
		s4g_spush_uint(s4gm, tval);
	}
	else if (ttype == t_string)
	{
		const char* tval = s4g_cfget_str(s4gm, 1);
		s4g_uint num;
		sscanf(tval, "%u", &num);
		s4g_spush_uint(s4gm, num);
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%d function '%s' expected arg #1 valid type, but got %s", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), s4g_dbg_get_curr_func(s4gm), s4g_cfget_str_type(s4gm, 1));
		return -1;
	}
	return 1;
}

int s4g_std_tofloat(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_type ttype = s4g_cfget_type(s4gm, 1);

	if (ttype == t_int)
	{
		s4g_int tval = s4g_cfget_int(s4gm, 1);
		s4g_spush_float(s4gm, tval);
	}
	else if (ttype == t_uint)
	{
		s4g_uint tval = s4g_cfget_uint(s4gm, 1);
		s4g_spush_float(s4gm, tval);
	}
	else if (ttype == t_float)
	{
		s4g_spush_value(s4gm, s4g_cfget_arg(s4gm, 1));
	}
	else if (ttype == t_bool)
	{
		s4g_bool tval = s4g_cfget_bool(s4gm, 1);
		s4g_spush_float(s4gm, tval);
	}
	else if (ttype == t_string)
	{
		const char* tval = s4g_cfget_str(s4gm, 1);
		s4g_float num;
		sscanf(tval, "%f", &num);
		s4g_spush_float(s4gm, num);
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%d function '%s' expected arg #1 valid type, but got %s", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), s4g_dbg_get_curr_func(s4gm), s4g_cfget_str_type(s4gm, 1));
		return -1;
	}
	return 1;
}

int s4g_std_tobool(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_type ttype = s4g_cfget_type(s4gm, 1);

	if (ttype == t_int)
	{
		s4g_int tval = s4g_cfget_int(s4gm, 1);
		s4g_spush_bool(s4gm, tval);
	}
	else if (ttype == t_uint)
	{
		s4g_uint tval = s4g_cfget_uint(s4gm, 1);
		s4g_spush_bool(s4gm, tval);
	}
	else if (ttype == t_float)
	{
		s4g_float tval = s4g_cfget_float(s4gm, 1);
		s4g_spush_bool(s4gm, tval);
	}
	else if (ttype == t_bool)
	{
		s4g_spush_value(s4gm, s4g_cfget_arg(s4gm, 1));
	}
	else if (ttype == t_string)
	{
		const char* tval = s4g_cfget_str(s4gm, 1);
		bool bf = false;
		if (strcmp("true", tval) == 0 || tval[0] != 0 && tval[0] != '0')
			bf = true;

		s4g_spush_bool(s4gm, bf);
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%d function '%s' expected arg #1 valid type, but got %s", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), s4g_dbg_get_curr_func(s4gm), s4g_cfget_str_type(s4gm, 1));
		return -1;
	}
	return 1;
}

int s4g_std_tostring(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_type ttype = s4g_cfget_type(s4gm, 1);
	char tmpstr[S4G_MAX_LEN_VAR_NAME];

	if (ttype == t_int)
	{
		s4g_int tval = s4g_cfget_int(s4gm, 1);
		sprintf(tmpstr, "%i", tmpstr);
		s4g_spush_str(s4gm, tmpstr);
	}
	else if (ttype == t_uint)
	{
		s4g_uint tval = s4g_cfget_uint(s4gm, 1);
		sprintf(tmpstr, "%u", tmpstr);
		s4g_spush_str(s4gm, tmpstr);
	}
	else if (ttype == t_float)
	{
		s4g_float tval = s4g_cfget_float(s4gm, 1);
		sprintf(tmpstr, "%f", tmpstr);
		s4g_spush_str(s4gm, tmpstr);
	}
	else if (ttype == t_bool)
	{
		s4g_bool tval = s4g_cfget_float(s4gm, 1);
		sprintf(tmpstr, "%d", tmpstr);
		s4g_spush_str(s4gm, tmpstr);
	}
	else if (ttype == t_string)
	{
		s4g_spush_value(s4gm, s4g_cfget_arg(s4gm, 1));
	}
	else if (ttype == t_null)
	{
		s4g_spush_str(s4gm, "null");
	}
	else if (ttype == t_table)
	{
		sprintf(tmpstr, "table:%d", s4g_cfget_pdata(s4gm,1));
		s4g_spush_str(s4gm, tmpstr);
	}
	else if (ttype == t_cfunc)
	{
		sprintf(tmpstr, "cfunc:%d", s4g_cfget_pdata(s4gm, 1));
		s4g_spush_str(s4gm, tmpstr);
	}
	else if (ttype == t_pdata)
	{
		sprintf(tmpstr, "pdata:%d", s4g_cfget_pdata(s4gm, 1));
		s4g_spush_str(s4gm, tmpstr);
	}
	else if (ttype == t_sfunc)
	{
		s4g_value* tval = s4g_cfget_arg(s4gm,1);
		sprintf(tmpstr, "sfunc:%s", s4g_value_name(tval));
		s4g_spush_str(s4gm, tmpstr);
	}
	else
	{
		s4g_gen_msg(s4gm, S4G_ERROR, "[%s]:%d function '%s' expected arg #1 valid type, but got %s", s4g_dbg_get_curr_file(s4gm), s4g_dbg_get_curr_str(s4gm), s4g_dbg_get_curr_func(s4gm), s4g_cfget_str_type(s4gm, 1));
		return -1;
	}
	return 1;
}

///////

int s4g_std_type(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_spush_int(s4gm, s4g_cfget_type(s4gm,1));
	
	return 1;
}

int s4g_std_str_type(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_spush_str(s4gm, s4g_cfget_str_type(s4gm, 1));

	return 1;
}

///////

int s4g_std_isnull(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_spush_bool(s4gm, (s4g_cfget_type(s4gm,1) == t_null));

	return 1;
}

int s4g_std_isnum(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_type ttype = s4g_cfget_type(s4gm, 1);
	s4g_spush_bool(s4gm, (ttype == t_int || ttype == t_uint || ttype == t_float));

	return 1;
}

int s4g_std_isint(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_spush_bool(s4gm, (s4g_cfget_type(s4gm, 1) == t_int));

	return 1;
}

int s4g_std_isuint(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_spush_bool(s4gm, (s4g_cfget_type(s4gm, 1) == t_uint));

	return 1;
}

int s4g_std_isfloat(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_spush_bool(s4gm, (s4g_cfget_type(s4gm, 1) == t_float));

	return 1;
}

int s4g_std_isbool(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_spush_bool(s4gm, (s4g_cfget_type(s4gm, 1) == t_bool));

	return 1;
}

int s4g_std_isstring(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_spush_bool(s4gm, (s4g_cfget_type(s4gm, 1) == t_string));

	return 1;
}

int s4g_std_istable(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_spush_bool(s4gm, (s4g_cfget_type(s4gm, 1) == t_table));

	return 1;
}

int s4g_std_ispdata(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_spush_bool(s4gm, (s4g_cfget_type(s4gm, 1) == t_pdata));

	return 1;
}

int s4g_std_iscfunc(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_spush_bool(s4gm, (s4g_cfget_type(s4gm, 1) == t_cfunc));

	return 1;
}

int s4g_std_issfunc(s4g_main* s4gm)
{
	int countarg = s4g_cfcount_arg(s4gm);

	S4G_STDLIB_COND_ARG(s4gm, countarg, 1);

	s4g_spush_bool(s4gm, (s4g_cfget_type(s4gm, 1) == t_sfunc));

	return 1;
}

///////


void s4g_export_stdlib(s4g_main* s4gm)
{
	s4g_spush_table_null(s4gm,16);
	int index = s4g_sgettop(s4gm)-1;
	s4g_spush_c_func(s4gm, s4g_std_format_str);
	s4g_sstore(s4gm, index, "format_str");

	s4g_spush_c_func(s4gm, s4g_std_print);
	s4g_sstore(s4gm, index, "print");

	s4g_spush_c_func(s4gm, s4g_std_assert);
	s4g_sstore(s4gm, index, "assert");

	s4g_spush_c_func(s4gm, s4g_std_strlen);
	s4g_sstore(s4gm, index, "strlen");

	s4g_spush_c_func(s4gm, s4g_std_tablen);
	s4g_sstore(s4gm, index, "tablen");

	s4g_spush_c_func(s4gm, s4g_std_call_gc);
	s4g_sstore(s4gm, index, "call_gc");

	s4g_spush_c_func(s4gm, s4g_std_unpack);
	s4g_sstore(s4gm, index, "unpack");

	s4g_spush_c_func(s4gm, s4g_std_toint);
	s4g_sstore(s4gm, index, "toint");

	s4g_spush_c_func(s4gm, s4g_std_touint);
	s4g_sstore(s4gm, index, "touint");

	s4g_spush_c_func(s4gm, s4g_std_tofloat);
	s4g_sstore(s4gm, index, "tofloat");

	s4g_spush_c_func(s4gm, s4g_std_tobool);
	s4g_sstore(s4gm, index, "tobool");

	s4g_spush_c_func(s4gm, s4g_std_tostring);
	s4g_sstore(s4gm, index, "tostring");


	s4g_spush_c_func(s4gm, s4g_std_type);
	s4g_sstore(s4gm, index, "type");

	s4g_spush_c_func(s4gm, s4g_std_str_type);
	s4g_sstore(s4gm, index, "str_type");


	s4g_spush_c_func(s4gm, s4g_std_isnull);
	s4g_sstore(s4gm, index, "isnull");

	s4g_spush_c_func(s4gm, s4g_std_isnum);
	s4g_sstore(s4gm, index, "isnum");

	s4g_spush_c_func(s4gm, s4g_std_isint);
	s4g_sstore(s4gm, index, "isint");

	s4g_spush_c_func(s4gm, s4g_std_isuint);
	s4g_sstore(s4gm, index, "isuint");

	s4g_spush_c_func(s4gm, s4g_std_isfloat);
	s4g_sstore(s4gm, index, "isfloat");

	s4g_spush_c_func(s4gm, s4g_std_isbool);
	s4g_sstore(s4gm, index, "isbool");

	s4g_spush_c_func(s4gm, s4g_std_isstring);
	s4g_sstore(s4gm, index, "isstring");

	s4g_spush_c_func(s4gm, s4g_std_istable);
	s4g_sstore(s4gm, index, "istable");

	s4g_spush_c_func(s4gm, s4g_std_ispdata);
	s4g_sstore(s4gm, index, "ispdata");

	s4g_spush_c_func(s4gm, s4g_std_iscfunc);
	s4g_sstore(s4gm, index, "iscfunc");

	s4g_spush_c_func(s4gm, s4g_std_issfunc);
	s4g_sstore(s4gm, index, "issfunc");


	s4g_spush_int(s4gm, t_null);
	s4g_sstore(s4gm, index, "t_null");

	s4g_spush_int(s4gm, t_table);
	s4g_sstore(s4gm, index, "t_table");

	s4g_spush_int(s4gm, t_string);
	s4g_sstore(s4gm, index, "t_string");

	s4g_spush_int(s4gm, t_float);
	s4g_sstore(s4gm, index, "t_float");

	s4g_spush_int(s4gm, t_int);
	s4g_sstore(s4gm, index, "t_int");

	s4g_spush_int(s4gm, t_uint);
	s4g_sstore(s4gm, index, "t_uint");

	s4g_spush_int(s4gm, t_bool);
	s4g_sstore(s4gm, index, "t_bool");

	s4g_spush_int(s4gm, t_pdata);
	s4g_sstore(s4gm, index, "t_pdata");

	s4g_spush_int(s4gm, t_cfunc);
	s4g_sstore(s4gm, index, "t_cfunc");

	s4g_spush_int(s4gm, t_sfunc);
	s4g_sstore(s4gm, index, "t_sfunc");

	s4g_spush_str(s4gm, "s4g 0.0");
	s4g_sstore(s4gm, index, "_version");


	s4g_ctx_add(s4gm, index);
	s4g_spop(s4gm, 1);
}

#endif