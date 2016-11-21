
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

void s4g_parse_g_e_t(char* str)
{
	long tmppos1,tmppos2,tmppos3;
	tmppos2 = tmppos3 = 0;
	String tmpstr,tmpstr2;
	tmpstr2 = str;
		while((tmppos2 = StrFind(tmpstr2.c_str(),"[",tmppos3)) != -1)
				{
					tmppos1 = 0;
						for(int i=0;i<tmppos2;i++)
						{
								if(tmpstr2.c_str()[tmppos2-i] == ' ' || str[tmppos2-i] == '\t')
								{
									tmppos1 = tmppos2-i;
									break;
								}
						}
					tmpstr += "__g_e_t '";
					tmpstr += StrSubStr(tmpstr2.c_str(),tmppos1,tmppos2-tmppos1);
					tmpstr += "','";
						if((tmppos3 = StrFind(tmpstr2.c_str(),"]",tmppos2)) != -1)
						{
							tmpstr += StrSubStr(tmpstr2.c_str(),tmppos2+1,tmppos3 - (tmppos2+1));
						}
						else
						{
							//error
						}
					tmpstr += "'";

					tmppos3++;
					tmpstr2.Delete(tmppos1,(tmppos3+1)-tmppos1);
					tmpstr2.insert(tmppos1,tmpstr.c_str());
				}
	sprintf(str,"%s",tmpstr2.c_str());
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


	long tmpcurposstr = 0;
	long tmpcurposstrold = 0;
	long tmpsizestrold = 0;

	Array<s4g_sym_o_desc> ArrSymO1;
	Array<s4g_sym_o_desc> ArrSymO2;
	Array<s4g_sym_o_desc> ArrSymO3;
	long tmpnpos=0;
	long tmpcurrstr = 1;
	int tmperror = 0;
	char tmpstr[64];

	Stack<s4g_state_name> tmpstate;
	Stack<int> tmpggstate;
	//char symdelay = 0;

		while(true)
		{
			tmpcurposstrold = tmpcurposstr;
			
			tmpcurposstr = StrGetStr(AllFile.c_str(),text,S4G_MAX_LEN_STR_IN_FILE,tmpcurposstr);
			tmpsizestrold = strlen(text);
			
				for(int i=0;i<strlen(text);i++)
				{
					memset(tmpstr,0,64);
						//если идут буквы то считываем слово
						if(s4g_is_char_str(text[i]))
						{
							s4g_scan_string(text+i,tmpstr);
							i += strlen(tmpstr)-1;
								
								if(strcmp(tmpstr,"local") == 0)
								{
									tmpstate.push(s4g_state_name(s4g_state_parse::sp_local,tmpstr,tmpcurrstr));
									tmpggstate.push(s4g_state_parse::sp_local);
									/*tmpstr[0] = 0;
									i++;
										for(;i<strlen(text);i++)
										{
												if(s4g_is_char_str(text[i]))
												{
													s4g_scan_string(text+i,tmpstr);
													i += strlen(tmpstr);
													char* tmptmp = text+i;
													tmpstate.push(s4g_state_name(s4g_state_parse::sp_var,tmpstr,tmpcurrstr));
													break;
												}
										}

										if(tmpstr[0] == 0)
										{
											//error
											int qwert = 0;
										}*/
								}
								/*else if(strcmp(tmpstr,"global") == 0)
								{
									tmpstate.push(s4g_state_name(s4g_state_parse::sp_gvar,tmpstr,tmpcurrstr));
									tmpggstate.push(s4g_state_parse::sp_gvar);
								}*/
								else if(strcmp(tmpstr,"function") == 0)
								{
									
								}
								else if(strcmp(tmpstr,"return") == 0)
								{
										//если размер стека глобальных состояний не пуст
										if(tmpggstate.count() > 0)
										{
												if(tmpggstate.get(-1) == s4g_state_parse::sp_function)
												{
													tmpstate.push(s4g_state_name(s4g_state_parse::sp_return,"return_b",tmpcurrstr));
													tmpggstate.push(s4g_state_parse::sp_return);
												}
												else
												{
													//error
													int qwert = 0;
												}
										}
										//иначе он пуст, а это значит что мы возвращем значения при выполнении скрипта
										else
										{
											tmpstate.push(s4g_state_name(s4g_state_parse::sp_return,"return_b",tmpcurrstr));
											tmpggstate.push(s4g_state_parse::sp_return);
										}
								}
								else
								{
										if(tmpggstate.count() == 0 || (!(
											tmpggstate.get(-1) == s4g_state_parse::sp_local ||
											tmpggstate.get(-1) == s4g_state_parse::sp_gvar ||
											tmpggstate.get(-1) == s4g_state_parse::sp_table ||
											tmpggstate.get(-1) == s4g_state_parse::sp_function ||
											tmpggstate.get(-1) == s4g_state_parse::sp_return
											) && tmpggstate.get(-1) != s4g_state_parse::sp_gvar)
											)
										{
											tmpggstate.push(s4g_state_parse::sp_gvar);
											tmpstate.push(s4g_state_name(s4g_state_parse::sp_gvar,"gvar",tmpcurrstr));
										}
									//записываем что это переменная
									tmpstate.push(s4g_state_name(s4g_state_parse::sp_var,tmpstr,tmpcurrstr));
								}
							int qwert = 0;
						}
						//если идет цифра или знаки (+ или -) или точка то считываем число
						else if(s4g_is_char_num(text[i]) || s4g_is_char_arif_pm(text[i]) /*|| s4g_is_char_point(text[i])*/)
						{
								if((tmperror = s4g_scan_num(text+i,tmpstr)) != 0)
								{
										if(tmperror == -1)
											s4g_output_error("File: %s : %d - unresolved point in const",file,tmpcurrstr);
								}
							i += strlen(tmpstr)-1;
							
								/*if(!(text[i] == '\0' || text[i] == ' ' || text[i] == '\t' || text[i] == ';' || text[i] == ',' || 
									text[i] == ')' || text[i] == '}' || 
									text[i] == '+' || text[i] == '-' || text[i] == '/' || text[i] == '*'))
								{
									s4g_output_error("File: %s : %d - unresolved %c za const",file,tmpcurrstr,text[i]);
								}
							i -= 1;*/
							tmpstate.push(s4g_state_name(s4g_state_parse::sp_const,tmpstr,tmpcurrstr));
							int qwert = 0;
						}

						else if(text[i] == '=')
						{
								if(tmpstate.get(-1).state == s4g_state_parse::sp_var/* || tmpstate.get(-1).state == s4g_state_parse::sp_table_g*/)
								{
									tmpstate.push(s4g_state_name(s4g_state_parse::sp_op_r,"__r",tmpcurrstr));
								}
								else
								{
									//error
									s4g_output_error("File: %s : %d - unresolved %c",file,tmpcurrstr,text[i]);
								}
						}
						else if(text[i] == '(')
						{
								//если до этого мы считали переменную
								if(tmpstate.get(-1).state == s4g_state_parse::sp_var)
								{
									tmpggstate.push(s4g_state_parse::sp_function);
									tmpstate.push(s4g_state_name(s4g_state_parse::sp_op_arg,"call_f_b",tmpcurrstr));//тогда это имя функции раз уж есть скобка
									ArrSymO1.push_back(s4g_sym_o_desc('(',')',true,tmpcurrstr));
								}
								else
									ArrSymO1.push_back(s4g_sym_o_desc('(',')',false,tmpcurrstr));
						}
						else if(text[i] == ')')
						{
								if(ArrSymO1.size() > 0)
								{
										//если мы считывали аргументы для вызова функции
										if(ArrSymO1[ArrSymO1.size()-1].is_call_f)
										{
											tmpggstate.pop(1);
											tmpstate.push(s4g_state_name(s4g_state_parse::sp_op_arg,"call_f_e",tmpcurrstr));
										}
									ArrSymO1.erase(ArrSymO1.size()-1);
								}
								else
								{
									//error
									int qwer = 0;
								}
						}
						else if(text[i] == '{')
						{
								//если до этого мы считали переменную
								/*if(tmpstate.get(-1).state == s4g_state_parse::sp_op_r)
								{*/
									tmpggstate.push(s4g_state_parse::sp_table);
									tmpstate.push(s4g_state_name(s4g_state_parse::sp_table,"c_table_b",tmpcurrstr));
									ArrSymO2.push_back(s4g_sym_o_desc('{','}',true,tmpcurrstr));
								/*}
								else
								{
									//error
									int wqert = 0;
								}*/
							//ArrSymO2.push_back(s4g_sym_o_desc('{','}',true,tmpcurrstr));
						}
						else if(text[i] == '}')
						{
								if(ArrSymO2.size() > 0)
								{
										//если мы считывали аргументы для вызова функции
										if(ArrSymO2[ArrSymO2.size()-1].is_call_f)
										{
											tmpstate.push(s4g_state_name(s4g_state_parse::sp_table,"c_table_e",tmpcurrstr));
										}
									ArrSymO2.erase(ArrSymO2.size()-1);
									tmpggstate.pop(1);
								}
								else
								{
									//error
									int qwer = 0;
								}
						}

						else if(text[i] == '[')
						{
								//если до этого мы считали переменную
								if(tmpstate.get(-1).state == s4g_state_parse::sp_var)
								{
									tmpstate.push(s4g_state_name(s4g_state_parse::sp_table_gs,"table_gs",tmpcurrstr));//тогда это имя функции раз уж есть скобка
									ArrSymO3.push_back(s4g_sym_o_desc('[',']',true,tmpcurrstr));
								}
								else
								{
									//error
									int wqert = 0;
								}
							//ArrSymO2.push_back(s4g_sym_o_desc('{','}',true,tmpcurrstr));
						}
						else if(text[i] == ']')
						{
								if(ArrSymO3.size() > 0)
								{
									ArrSymO3.erase(ArrSymO3.size()-1);
								}
								else
								{
									//error
									int qwer = 0;
								}
						}

						else if(text[i] == '"')
						{
							s4g_scan_litstring(text+i,tmpstr);
							i += strlen(tmpstr)+1;
							
								if(tmpstate.get(-1).state == s4g_state_parse::sp_table_gs)
									tmpstate.push(s4g_state_name(s4g_state_parse::sp_var,tmpstr,tmpcurrstr));
								else
									tmpstate.push(s4g_state_name(s4g_state_parse::sp_string,tmpstr,tmpcurrstr));
						}

						else if(text[i] == '.')
						{
								if(tmpstate.get(-1).state == s4g_state_parse::sp_var)
								{
									tmpstate.push(s4g_state_name(s4g_state_parse::sp_table_gs,"table_gs",tmpcurrstr));
								}
								else
								{
									///error
									int qwert = 0;
								}
						}

						else if(text[i] == ',')
						{
								if(tmpggstate.count() > 0)
								{
										if(
											!(
											tmpggstate.get(-1) == s4g_state_parse::sp_local ||
											tmpggstate.get(-1) == s4g_state_parse::sp_gvar ||
											tmpggstate.get(-1) == s4g_state_parse::sp_table ||
											tmpggstate.get(-1) == s4g_state_parse::sp_function ||
											tmpggstate.get(-1) == s4g_state_parse::sp_return
											)
											)
										{
											//error
											int qwert = 0;
										}
								}
								else
								{
									//error
									int qwert = 0;
								}
						}


						else if(text[i] == ';')
						{
								if(ArrSymO1.size() > 0)
								{
									//error
									int qwert = 0;
								}

								if(ArrSymO2.size() > 0)
								{
									//error
									int qwert = 0;
								}

								if(ArrSymO3.size() > 0)
								{
									//error
									int qwert = 0;
								}

								if(tmpggstate.count() > 0 && tmpggstate.get(-1) == sp_local)
								{
									tmpstate.push(s4g_state_name(s4g_state_parse::sp_local,"local_end",tmpcurrstr));
									tmpggstate.pop(1);
								}

								if(tmpggstate.count() > 0 && tmpggstate.get(-1) == sp_gvar)
								{
									tmpstate.push(s4g_state_name(s4g_state_parse::sp_gvar,"gvar_end",tmpcurrstr));
									tmpggstate.pop(1);
								}

								if(tmpggstate.count() > 0 && tmpggstate.get(-1) == sp_return)
								{
									tmpstate.push(s4g_state_name(s4g_state_parse::sp_return,"return_end",tmpcurrstr));
									tmpggstate.pop(1);
								}
						}
				}
			
				if(tmpcurposstr == AllFile.length())
					break;
			tmpcurrstr++;
			int qwert = 0;
		}
		

	ffile = fopen((String(file)+".tmp").c_str(), "wt");

		for(long i=0;i<tmpstate.count();i++)
		{
			fprintf(ffile,"%s : %d\n",tmpstate.get(i+1).name,tmpstate.get(i+1).str);
		}

	fclose(ffile);
		
	int qwer = 0;
}

int s4g_parse_str(const char* str);