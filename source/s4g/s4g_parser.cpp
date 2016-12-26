
#include <s4g/s4g_parser.h>

#include <s4g/s4g_vm.cpp>
#include <s4g/s4g_compiler.cpp>
#include <s4g/s4g_lexer.cpp>
#include <s4g/s4g_preprocessor.cpp>


///////////////

s4g_node* s4g_builder_syntax_tree::s4g_gen_statement()
{
	 s4g_lexeme* lex_get_curr0(tmplexs);
	
		//если лексема не считана значит скорее всего лексемы исчерпаны
		if(tmplexs == 0)
		{
			return 0;
		}
	s4g_node* node=0;

		//если тип лексемы это ключевое слово языка
		if(tmplexs->type == word_key)
		{
				//возвращаение /return
				if(tmplexs->id == 3)
				{
					bool oldisender = isender;
					isender = false;
					lex_get_next0(tmplexs);
					listread = false;
					s4g_node* tmpnode = s4g_get_ret_vals();	//считываем возвращаемые выражения
					bst_cond_er(this);
					listread = true;
					isender = oldisender;
					tmpnode->op3 = s4g_gen_statement();	//и считываем то что после return
					bst_cond_er(this);
					return tmpnode;
				}
				//объявление функции /function
				else if(tmplexs->id == 1)
				{
					long funclexid = curr_lexid;
					bool oldisender = isender;
					isender = false;
					s4g_lexeme* sfunclex = tmplexs;
					s4g_node* name_func = 0;
					lex_get_next0(tmplexs);

						//если тип лексемы пользовательское слово
						if(tmplexs->type == word_user_cr)
						{
							status = -1;
							sprintf(this->error, "[%s]:%d - function provides for the creation of a new variable, it's too much [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, s4g_key_syms_create[0]);
							return 0;
						}
						else if (tmplexs->type == word_user)
						{
							readcall = false;
							name_func = s4g_get_term();
							name_func->type = _crvar;
							readcall = true;
						}
						else //error
						{
							status = -1;
							sprintf(this->error,"[%s]:%d - unresolved word for name function [%s]",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr,tmplexs->str);
							return 0;
						}

					lex_get_curr0(tmplexs);
					bst_cond_er(this);
						//если лексема это открывающая скобка
						if(tmplexs->type == sym_group && tmplexs->id == 0)
							lex_get_next(tmplexs)
						else //error
						{
							status = -1;
							sprintf(this->error,"[%s]:%d - unresolved word for name function [%s]",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr,tmplexs->str);
							return 0;
						}

					s4g_node* head_func = s4g_get_function_def_head();
					bst_cond_er(this);
					this->overend--;
					
					s4g_node* body_func = s4g_gen_statement();
					
					bst_cond_er(this);
					lex_get_curr0(tmplexs);
						//если текущая лексема не означает завершение операции end
						if(!(arr_lex->get_curr_num() < arr_lex->get_count() && tmplexs->type == word_key && tmplexs->id == 2)) //error
						{
							status = -1;
							sprintf(this->error,"[%s]:%d - not found end for funcion",this->arr_lex->ArrFiles[sfunclex->fileid],sfunclex->numstr);
							return 0;
						}
					lex_get_next(tmplexs);
					s4g_node* nfunc = new s4g_node(_function, funclexid, 0, head_func, body_func);
					isender = oldisender;
					return new s4g_node(_expr, funclexid, 0, new s4g_node(_set, funclexid, 0, name_func, nfunc), new s4g_node(_empty, curr_lexid, 0, s4g_gen_statement()));
				}
				//конец /end
				else if(tmplexs->id == 2)
				{
					this->overend++;
						//если мы не искали энд но нашли значит он здесь лишний
						if(this->overend > 0)
						{
							status = -1;
							sprintf(this->error,"[%s]:%d - not found operation for end",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
						}
					return 0;
				}
		}
		//разделитель
		else if(tmplexs->type == sym_delimiter)
		{
				//разделитель операций
				if(tmplexs->id == 0)
				{
					lex_get_next0(tmplexs);
						if (tmplexs)
							return new s4g_node(_empty, curr_lexid, 0, s4g_gen_statement());
				}
		}
		//иначе у считываем выражение
		else
		{
			bool oldisender = isender;
			isender = false;
			node = new s4g_node(_expr, curr_lexid, 0, s4g_get_expr());
			lex_get_curr0(tmplexs);
			bst_cond_er(this);
			isender = oldisender;
				//если после выражения нет символа окончания операции ;
				if(!(tmplexs->type == sym_delimiter && tmplexs->id == 0))	//error
				{
						if(status == 0)
						{
							status = -1;
							sprintf(this->error,"[%s]:%d - none end for expression",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
						}
					return 0;
				}
				else
				{
					lex_get_next0(tmplexs);
					if (tmplexs)
					{
						node->op2 = new s4g_node(_empty, curr_lexid, 0, s4g_gen_statement());
						bst_cond_er(this);
					}
				}
		}
	
	return node;
}

s4g_node* s4g_builder_syntax_tree::s4g_get_term()
{
	s4g_lexeme* lex_get_curr0(tmplexs);
	bst_cond_eof(tmplexs);
		//если тип лексемы пользовательское слово
		if(tmplexs->type == word_user)
		{
			//создаем тип нода - переменная со значением
			s4g_node* node = new s4g_node(_var, curr_lexid, gc->cr_val(t_string, tmplexs->str/*, curr_lexid*/));
			s4g_node* tmpnode = node;
			bool isnext = true;
			//начинаем цикл проверки - а не обращение ли это к элемента таблицы
				while(1)
				{
					lex_get_next0(tmplexs);
						//если лексема относится к считыванию из таблицы .
						if(tmplexs->type == sym_table_elem && tmplexs->id == 2)
						{
							lex_get_next0(tmplexs);
								if(tmplexs->type == word_user)
								{
									tmpnode->op1 = new s4g_node(_get, curr_lexid, 0, new s4g_node(_string, curr_lexid, gc->cr_val(t_string, tmplexs->str/*, curr_lexid*/)), new s4g_node(_empty, curr_lexid));
									tmpnode = tmpnode->op1->op2;
								}
								else if (tmplexs->type == word_user_cr)
								{
									tmpnode->op1 = new s4g_node(_get_cr, curr_lexid, 0, new s4g_node(_string, curr_lexid, gc->cr_val(t_string, tmplexs->str/*, curr_lexid*/)), new s4g_node(_empty, curr_lexid));
									tmpnode = tmpnode->op1->op2;
								}
								else
								{
									lex_get_prev0(tmplexs);
									break;
								}
						}
						//иначе если лексема относится к считыванию из таблицы [
						else if(tmplexs->type == sym_table_elem && tmplexs->id == 0)
						{
							lex_get_next0(tmplexs);
							tmpnode->op1 = new s4g_node(_get, curr_lexid, 0, s4g_get_expr(), new s4g_node(_empty, curr_lexid));
							tmpnode = tmpnode->op1->op2;

							lex_get_curr0(tmplexs);
								//если следующий символ за выражением не закрывающий доступ к таблице ]
								if(!(tmplexs->type == sym_table_elem && tmplexs->id == 1))
								{
									status = -1;
									sprintf(this->error,"[%s]:%d - none end for access in table",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
									return 0;
								}
						}
						//если терм это переменная и следующий символ это открывающая скобка
						else if (readcall && tmplexs->type == sym_group && tmplexs->id == 0)
						{
							//то у нас вызов функции
							long funccallidlex = curr_lexid;
							lex_get_next0(tmplexs);
							bst_cond_eof(tmplexs);
							overge--;
							bool oldlistread = listread;
							listread = false;
							s4g_node* args = s4g_get_arg_call_func();
							listread = oldlistread;
							bst_cond_er(this);
							lex_get_curr0(tmplexs);
							//lex_get_prev0(tmplexs);
							return new s4g_node(_call, funccallidlex, 0, node, 0, args);
						}
						//иначе у нас нет обращения к элементам таблицы
						else 
						{
							//откатываем к предыдущей лексеме и останаливаем цикл
							isnext = false;
							lex_get_prev0(tmplexs);
							break;
						}
				}

			lex_get_next0(tmplexs);
			return node;
		}
		else if (tmplexs->type == word_user_cr)
		{
			s4g_node* node = new s4g_node(_crvar, curr_lexid, gc->cr_val(t_string, tmplexs->str/*, curr_lexid*/));
			lex_get_next0(tmplexs);
			return node;
		}
		//если тип лексемы пустое значение
		else if(tmplexs->type == word_null)
		{
			s4g_node* node = new s4g_node(_null, curr_lexid, gc->cr_val_null(/*curr_lexid*/));
			lex_get_next0(tmplexs);
			return node;
		}
		//если тип лексемы строка
		else if(tmplexs->type == word_string)
		{
			s4g_node* node = new s4g_node(_string, curr_lexid, gc->cr_val(t_string, tmplexs->str/*, curr_lexid*/));
			lex_get_next0(tmplexs);
			return node;
		}
		else if (tmplexs->type == word_string_cr)
		{
			s4g_node* node = new s4g_node(_string_cr, curr_lexid, gc->cr_val(t_string, tmplexs->str/*, curr_lexid*/));
			lex_get_next0(tmplexs);
			return node;
		}
		//если тип лексемы число с плавающей запятой
		else if(tmplexs->type == word_float)
		{
			s4g_node* node = new s4g_node(_float, curr_lexid, gc->cr_val(t_float, tmplexs->str/*, curr_lexid*/));
			lex_get_next0(tmplexs);
			return node;
		}
		//если тип лексемы целочисленное значение
		else if(tmplexs->type == word_int)
		{
			s4g_node* node = new s4g_node(_int, curr_lexid, gc->cr_val(t_int, tmplexs->str/*, curr_lexid*/));
			lex_get_next0(tmplexs);
			return node;
		}
		else if (tmplexs->type == word_int_cr)
		{
			s4g_node* node = new s4g_node(_int_cr, curr_lexid, gc->cr_val(t_int, tmplexs->str/*, curr_lexid*/));
			lex_get_next0(tmplexs);
			return node;
		}
		else if (tmplexs->type == word_uint)
		{
			s4g_node* node = new s4g_node(_uint, curr_lexid, gc->cr_val(t_uint, tmplexs->str/*, curr_lexid*/));
			lex_get_next0(tmplexs);
			return node;
		}
		else if (tmplexs->type == word_bool)
		{
			s4g_node* node = new s4g_node(_bool, curr_lexid, gc->cr_val(t_bool, tmplexs->str/*, curr_lexid*/));
			lex_get_next0(tmplexs);
			return node;
		}
		else
			return 0;
}

s4g_node* s4g_builder_syntax_tree::s4g_get_op()
{
	s4g_lexeme* lex_get_curr0(tmplexs);
	s4g_stack<s4g_node*> stack_var;
	s4g_stack<s4g_node*> stack_op;
	s4g_node* node = 0;
	s4g_node* tmpnode = 0;
	s4g_node* tmpnode2 = 0;
	s4g_node* tmpnode3 = 0;
	String strt;
	bool locallistread = true; //если начали считывать арифметические операции то запрещаем считывание листа переменных
	long curr_num_lex = arr_lex->get_curr_num();
	int how_type_next = -2;	//-1 - sym_arif, 0 - sym_group (, 1 - sym_group ), 2 - числа
		while(1)
		{
			lex_get_curr0(tmplexs);
			if (!tmplexs)
			{
				lex_get_prev0(tmplexs);
				break;
			}
			bst_cond_eof(tmplexs);
			curr_num_lex = arr_lex->get_curr_num();
				//если тип текущей лексемы арифметический либо это скобки
				if(tmplexs->type == sym_arif || tmplexs->type == sym_group)
				{
					locallistread = false;
					//определяем тип будущего нода
					int tmptypeop = -1;
						if(tmplexs->type == sym_arif)
						{
								//если предыдущий тип был арифметической операцией
								if(how_type_next == -1)	//error
								{
									status = -1;
									sprintf(this->error,"[%s]:%d - Invalid sequence of arithmetic operations one after the other",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
									return 0;
								}

								//если мы только начали анализировать выражение
								if(how_type_next == -2)
								{
										//и первым арифметическим выражением встречаем не -
										if(tmplexs->id != 1)	//error
										{
											status = -1;
											sprintf(this->error,"[%s]:%d - illegal inderection",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
											return 0;
										}
										//иначе
										else
										{
											//вставляем универсальную цифру 0
											stack_var.push(new s4g_node(_numnull, curr_lexid, gc->cr_val(t_nnull, 0/*, curr_lexid*/)));
										}
								}

								//если предыдущей была открывающая скобка
								if(how_type_next == 0)
								{
										//и следующая арифметическая операция не -
										if(tmplexs->id != 1)	//error
										{
											status = -1;
											sprintf(this->error,"[%s]:%d - illegal inderection",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
											return 0;
										}
										//иначе
										else 
										{
											//вставляем универсальную цифру 0
											stack_var.push(new s4g_node(_numnull, curr_lexid, gc->cr_val(t_nnull, 0/*, curr_lexid*/)));
										}
								}

								if(tmplexs->id == 0)
									tmptypeop = _add;
								else if(tmplexs->id == 1)
									tmptypeop = _sub;
								else if(tmplexs->id == 2)
									tmptypeop = _mul;
								else if(tmplexs->id == 3)
									tmptypeop = _div;

							how_type_next = -1;
						}
						else if(tmplexs->type == sym_group)
						{
								if(tmplexs->id == 0)
								{
									tmptypeop = _group_b;
									how_type_next = 0;
								}
								//если сейчас закрывающая скобка
								else if(tmplexs->id == 1)
								{
										if (how_type_next == -2)
										{
											status = -1;
											sprintf(this->error, "[%s]:%d - unresolved begin for expression [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
											return 0;
										}
										//а предудщий символ арифметическая операция
										if(how_type_next == -1)	//error
										{
											status = -1;
											sprintf(this->error,"[%s]:%d - unresolved end for expression",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
											return 0;
										}
									tmptypeop = _group_e;
									how_type_next = 1;
								}
						}

						//если определили тип будущего нода то начинаем формировать команды для стека
						if(tmptypeop != -1)
						{
								//если тип закрывающая скобка
								if(tmptypeop == _group_e)
								{
										//если в стеке операций чтото есть
										if(stack_op.count() > 0)
										{
												if (stack_op.get(stack_op.count())->type == _group_b)
												{
													status = -1;
													sprintf(this->error, "[%s]:%d - unresolved  null expression ()", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
													return 0;
												}
											bool is_find_gb = false;//найдена ли открывающая скобка
												//циклом выталкиваем из стека операций все вплоть до первой открывающей скобки
												while(stack_op.count() > 0)
												{
														//если последняя операция в стеке открывающая скобка
														if(stack_op.get(stack_op.count())->type == _group_b)
														{
															//выталкиваем ее из стека и завершаем цикл
															stack_op.pop(1);
															is_find_gb = true;
															break;
														}
														else
														{
															s4g_node* qwert = stack_op.get(stack_op.count());
															stack_var.push(qwert);
															stack_op.pop(1);
														}
												}

												//если не была найдена открывающая скобка то значит у нас ошибка
												if(!is_find_gb)	//error
												{
													overge++;
													//если никто не давал запрос на поиск закрывающей скобки то значит она здесь лишняя
														if (overge > 0) //error
														{
															status = -1;
															sprintf(this->error, "[%s]:%d - excess ')'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
															return 0;
														}
													//вызов функции дает запрос на поиск
													//lex_get_prev0(tmplexs);
													break;
												}
										}
										//если стек пуст а у нас закрывающая скобка
										else
										{
											overge++;
												//если никто не давал запрос на поиск закрывающей скобки то значит она здесь лишняя
												if(overge > 0) //error
												{
													status = -1;
													sprintf(this->error,"[%s]:%d - excess ')'",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
													return 0;
												}
											//вызов функции дает запрос на поиск
											//lex_get_prev0(tmplexs);
											break;
										}
								}
								//если в стеке чтото есть либо операция открываюая скобка
								else if(stack_op.count() > 0 || tmptypeop == _group_b)
								{
										if(tmptypeop == _group_b)
										{
											stack_op.push(new s4g_node((s4g_type_op)tmptypeop, curr_lexid));
										}
										else if(s4g_get_priority((s4g_type_op)tmptypeop) <= s4g_get_priority(stack_op.get(stack_op.count())->type))
										{
											//выталкиваем
											s4g_node* qwert = stack_op.get(stack_op.count());
											stack_var.push(qwert/*stack_op.get(stack_op.count())*/);
											stack_op.pop(1);
											stack_op.push(new s4g_node((s4g_type_op)tmptypeop, curr_lexid));
										}
										else
										{
											//помещаем
											stack_op.push(new s4g_node((s4g_type_op)tmptypeop, curr_lexid));
										}
								}
								//иначе стек пуст и у нас какая-то операция
								else
									stack_op.push(new s4g_node((s4g_type_op)tmptypeop, curr_lexid));
						}
						//иначе у нас неопределенная операция
						else
						{
							//lex_get_prev0(tmplexs);
							break;
						}
					lex_get_next(tmplexs);
				}
				//иначе если удалось считать терм
				else if((tmpnode = s4g_get_term()) != 0)
				{
					bst_cond_er(this);
						//если предыдующий тип был термом то значит у нас идут 2 терма подряд
						if(how_type_next == 2)	//error
						{
							status = -1;
							sprintf(this->error,"[%s]:%d - Invalid sequence of terms to each other",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
							return 0;
						}
					
					stack_var.push(tmpnode);
					how_type_next = 2;
				}
				//иначе у нас не операция и не терм
				else
				{
					//прерываем цикл
					break;
				}
		}

		//если последним символом следовала арифметическая операция
		if(how_type_next == -1)	//error
		{
			lex_get_prev0(tmplexs);
			status = -1;
			sprintf(this->error,"[%s]:%d - invalid end for expression",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
			return 0;
		}

		//если стек операций не опустошен
		if(stack_op.count() > 0)
		{
				//циклом выталкиваем все из стека операций 
				//и записываем все эти операции в переменные
				for(int i=0;i<stack_op.count();i++)
				{
					s4g_node* tmtm = stack_op.get(stack_op.count()-i);
					
						//если у нас встретилась открывающая скобка то значит мы ее не закрыли
						if(tmtm->type == _group_b)	//error
						{
							status = -1;
							sprintf(this->error,"[%s]:%d - no ')'",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
							return 0;
						}
					stack_var.push(tmtm);
				}
			
		}
	
		//если количество считанных термов больше нуля то записываем первый считанный в основной нод
		if(stack_var.count() > 0)
		{
			node = stack_var.get(1);
		}

		//если всетаки считали более одного терма то скорее всего у нас какое то выражение
		//подготавливаемся к циклу записи
		if(stack_var.count() > 1)
		{
			s4g_node* tmtm = stack_var.get(2);
			node->op3 = stack_var.get(2);	//заталкиваем третий нод, ибо в первом ноде может быть обращение к элементам таблицы, а во втором аргументы для вызова функции!!!
			tmpnode = node->op3;
		}

		for(int i=1;i<stack_var.count();i++)
		{
			s4g_node* tmtm = stack_var.get(i+1);
			tmpnode->op3 = stack_var.get(i+1);//заталкиваем третий нод, ибо в первом ноде может быть обращение к элементам таблицы, а во втором аргументы для вызова функции!!!
			tmpnode = tmpnode->op3;
		}

	return node;
}	

s4g_node* s4g_builder_syntax_tree::s4g_get_function_def_head()
{
	s4g_lexeme* lex_get_curr0(tmplexs);
	s4g_node* node = new s4g_node(_arg, curr_lexid);
	s4g_node* tmpnode = node;
	s4g_node* tmpnode2 = 0;
	Array<String> arrarg;
		
		while(1)
		{
			lex_get_curr0(tmplexs);
				
				//если тип лексемы закрывающая скобка
				if(tmplexs->type == sym_group && tmplexs->id == 1)
				{
					lex_get_next(tmplexs);
					break;
				}
				//если тип лексемы не закрывающая скобка
				else if(!(tmplexs->type == sym_delimiter && tmplexs->id == 1))
				{
						//если тип лексемы пользвоательское слово
						if(tmplexs->type == word_user)
						{
								//проверяем аргумент на уникальность
								for(int i=0;i<arrarg.size();i++)
								{
									String str = arrarg[i];
										if(arrarg[i] == tmplexs->str)
										{
											this->status = -1;
											sprintf(this->error,"[%s]:%d - non unic argument [%s]",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr,tmplexs->str);
											return 0;
										}
								}

								//если мы еще не ушли внутрь нода
								if(node->value == 0)
								{
									tmpnode->value = gc->cr_val(t_string, tmplexs->str/*, curr_lexid*/);
								}
								else
								{
									tmpnode->op1 = new s4g_node(_arg, curr_lexid, gc->cr_val(t_string, tmplexs->str/*, curr_lexid*/));
									tmpnode = tmpnode->op1;
								}

							arrarg.push_back(tmplexs->str);
							lex_get_next0(tmplexs);

								if((tmplexs->type == sym_delimiter && tmplexs->id == 1))
								{
									lex_get_next(tmplexs);
								}
								else if(!(tmplexs->type == sym_group && tmplexs->id == 1))
								{
									this->status = -1;
									sprintf(this->error,"[%s]:%d - no separate arguments",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
									return 0;
								}
						}
						else if (tmplexs->type == marg)
						{
							tmpnode->op1 = new s4g_node(_marg, curr_lexid);
							lex_get_next0(tmplexs);
							if (!(tmplexs->type == sym_group && tmplexs->id == 1))
							{
								this->status = -1;
								sprintf(this->error, "[%s]:%d - expected ')' but got %s", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
								return 0;
							}
							lex_get_next0(tmplexs);
							break;
						}
						//иначе у нас не пользовательское слово
						else //error
						{
							this->status = -1;
							sprintf(this->error,"[%s]:%d - unresolved type for argument [%s]",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr,tmplexs->str);
							return 0;
						}
				}
				//иначе у нас какойто другой тип
				else //error
				{
					this->status = -1;
					sprintf(this->error,"[%s]:%d - unresolved definition for argument [%s]",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr,tmplexs->str);
					return 0;
				}
		}
	return node;
}

s4g_node* s4g_builder_syntax_tree::s4g_get_table()
{
	s4g_lexeme* lex_get_curr0(tmplexs);
	s4g_node* node = new s4g_node(_create_table, curr_lexid, gc->cr_val_null(/*curr_lexid*/));
	s4g_node* tmpnode = node;
	int type_last = -1;
	listread = false;
		while(1)
		{
			lex_get_curr0(tmplexs);
				if (type_last == 1 && !(tmplexs->type == sym_delimiter && tmplexs->id == 1) && !(tmplexs->type == sym_table_create && tmplexs->id == 1))
				{
					this->status = -1;
					sprintf(this->error, "[%s]:%d - expected delimiter arg but got [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
					return 0;
				}
				//если символ оконцания таблицы }
				if(tmplexs->type == sym_table_create && tmplexs->id == 1)
				{
						if (type_last == 0)
						{
							this->status = -1;
							sprintf(this->error, "[%s]:%d - expected arg but got end definition of the table", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
							return 0;
						}
					lex_get_next(tmplexs);
					break;
				}
				//если не запятая
				else if(!(tmplexs->type == sym_delimiter && tmplexs->id == 1))
				{
					s4g_node* tmpnode2 = s4g_get_expr();
					lex_get_curr0(tmplexs);
					bst_cond_er(this);
						if(tmpnode2->type != _set)
						{
							tmpnode->op1 = new s4g_node(_add_in_table, curr_lexid, 0, tmpnode2, new s4g_node(_empty, curr_lexid));
							tmpnode = tmpnode->op1->op2;
						}
						else
						{
							tmpnode2->type = _sett;
							tmpnode->op1 = tmpnode2;
							tmpnode->op2 = new s4g_node(_empty, curr_lexid);
							tmpnode = tmpnode->op2;
						}
					type_last = 1;
				}
				else if((tmplexs->type == sym_delimiter && tmplexs->id == 1))
				{
						if (type_last == 0)
						{
							this->status = -1;
							sprintf(this->error, "[%s]:%d - expected double delimiter ','", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
							return 0;
						}
					lex_get_next(tmplexs);
					type_last = 0;
				}
		}
	listread = true;
	return node;
}

s4g_node* s4g_builder_syntax_tree::s4g_get_ret_vals()
{
	s4g_lexeme* lex_get_curr0(tmplexs);
	s4g_node* node = new s4g_node(_return);
	s4g_node* tmpnode = node;
	s4g_node* tmpnode2 = 0;
		while(1)
		{
			lex_get_curr0(tmplexs);
				if(!(tmplexs->type == sym_delimiter && tmplexs->id == 1) && 
					!(tmplexs->type == sym_delimiter && tmplexs->id == 0) &&
					!(tmplexs->type == word_key && tmplexs->id == 2))
				{
					tmpnode2 = s4g_get_expr();
					bst_cond_er(this);
					tmpnode->op1 = tmpnode2;//new s4g_node(_expr,0,tmpnode2,new s4g_node(_empty));
					tmpnode->op2 = new s4g_node(_empty, curr_lexid);
					tmpnode = tmpnode->op2;
					lex_get_curr0(tmplexs);
						if(tmplexs->type == sym_delimiter && tmplexs->id == 0)
						{
							break;
						}
						else
							lex_get_next(tmplexs);
				}
				else if((tmplexs->type == sym_delimiter && tmplexs->id == 1))
				{
					lex_get_next(tmplexs);
				}
				else 
					break;
		}
	return node;
}

s4g_node* s4g_builder_syntax_tree::s4g_get_arg_call_func()
{
	s4g_lexeme* lex_get_curr0(tmplexs);
	s4g_node* node = new s4g_node(_arg);
	s4g_node* tmpnode = node;
	s4g_node* tmpnode2 = 0;
	int type_last = -1;

		while(1)
		{
			lex_get_curr0(tmplexs);
				if (type_last == 1 && !(tmplexs->type == sym_delimiter && tmplexs->id == 1) && !(tmplexs->type == sym_group && tmplexs->id == 1))
				{
					this->status = -1;
					sprintf(this->error, "[%s]:%d - expected delimiter arg but got [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
					return 0;
				}
				//если текущий символ закрывающая скобка
				if (tmplexs->type == sym_group && tmplexs->id == 1)
				{
						if (type_last == 0)
						{
							this->status = -1;
							sprintf(this->error, "[%s]:%d - expected arg but got end call of function", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
							return 0;
						}
					lex_get_next0(tmplexs);
					break;
				}
				else if(!(tmplexs->type == sym_delimiter && tmplexs->id == 1))
				{
					tmpnode2 = s4g_get_expr();
					bst_cond_er(this);
					tmpnode->op1 = tmpnode2;//new s4g_node(_expr,0,tmpnode2,new s4g_node(_empty));
					tmpnode->op2 = new s4g_node(_empty, curr_lexid);
					tmpnode = tmpnode->op2;
					type_last = 1;
				}
				else if((tmplexs->type == sym_delimiter && tmplexs->id == 1))
				{
						if (type_last == 0)
						{
							this->status = -1;
							sprintf(this->error, "[%s]:%d - expected double delimiter ','", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
							return 0;
						}
					type_last = 0;
					lex_get_next(tmplexs);
				}

		}
	return node;
}

s4g_node* s4g_builder_syntax_tree::s4g_get_expr()
{
	s4g_lexeme* lex_get_curr0(tmplexs);

		//если текущий токен - создание таблицы
		if(tmplexs->type == sym_table_create && tmplexs->id == 0)
		{
			lex_get_next(tmplexs);
			s4g_node* tmpnode = s4g_get_table();
			bst_cond_er(this);
			return tmpnode;
		}
	s4g_node* node = s4g_get_op();
	s4g_node* tmpnode = 0;
	s4g_node* tmpnode2 = 0;
	lex_get_curr0(tmplexs);

	if (node == 0)
		return 0;

		if (listread && tmplexs && tmplexs->type == sym_delimiter && tmplexs->id == 1)
		{
			lex_get_next0(tmplexs);
			bst_cond_eof(tmplexs);

			tmpnode = node;
			listread = false;
				while ((tmpnode2 = s4g_get_op()) != 0)
				{
					tmpnode->op2 = tmpnode2;
					tmpnode = tmpnode->op2;

					lex_get_curr0(tmplexs);

						if (tmplexs && (tmplexs->type == sym_delimiter && tmplexs->id == 1))
							lex_get_next(tmplexs)
						else if (!tmplexs || !(tmplexs->type == sym_delimiter && tmplexs->id == 1))
						{
							listread = true;
						}
				}
			bst_cond_er(this);
			listread = true;
		}
		
		
		//если тип текущего нода переменная и следующая лексема присвоение
		if ((node->type == _var || node->type == _crvar) && tmplexs->type == sym_assign)
		{
			lex_get_next0(tmplexs);
				//если символ создания таблицы
				if(tmplexs->type == sym_table_create && tmplexs->id == 0)
				{
					lex_get_next(tmplexs);
					//создаем нод и получаем содержимое таблицы
					return new s4g_node(_set, curr_lexid, 0, node, s4g_get_table());
				}
				//если символ ключевое слово "функция"
				else if(tmplexs->type == word_key && tmplexs->id == 1)
				{
					long cfunclexid = curr_lexid;
					s4g_lexeme* sfunclex = tmplexs;
					s4g_node* name_func = node;

					lex_get_next0(tmplexs);
					bst_cond_eof(tmplexs);
						//если лексема это открывающая скобка
						if(tmplexs->type == sym_group && tmplexs->id == 0)
							lex_get_next(tmplexs)
						else //error
						{
							status = -1;
							sprintf(this->error,"[%s]:%d - expected [%s] but got [%s]",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr,s4g_key_syms_group[0],tmplexs->str);
							return 0;
						}

					s4g_node* head_func = s4g_get_function_def_head();
					bst_cond_er(this);
					lex_get_curr0(tmplexs);
					s4g_node* extern_data = 0;
					s4g_node* tmped = 0;
					if (tmplexs->type == word_key && tmplexs->id == 4)
					{
						int type_last = -1;
						while (true)
						{
							lex_get_next0(tmplexs);
							s4g_node* tmpop = 0;
							if (type_last == 1 && !(tmplexs->type == sym_delimiter && tmplexs->id == 1) && !(tmplexs->type == sym_delimiter && tmplexs->id == 0))
							{
								this->status = -1;
								sprintf(this->error, "[%s]:%d - expected delimiter arg but got [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
								return 0;
							}
							else if ((type_last == 0 || type_last == -1) && tmplexs->type == word_user)
							{
								tmpop = new s4g_node(_var, curr_lexid, gc->cr_val(t_string, tmplexs->str/*, curr_lexid*/));
								type_last = 1;
								if (extern_data)
								{
									tmped->op1 = tmpop;
									tmped = tmped->op1;
								}
								else
								{
									extern_data = tmpop;
									tmped = extern_data;
								}
							}
							else if (type_last == 1 && tmplexs->type == sym_delimiter && tmplexs->id == 1)
							{
								type_last = 0;
							}
							else if (tmplexs->type == sym_delimiter && tmplexs->id == 0)
							{
								lex_get_next0(tmplexs);
								break;
							}
							//extern_data
						}
					}

					this->overend--;
					s4g_node* body_func = s4g_gen_statement();
					bst_cond_er(this);
					lex_get_curr0(tmplexs);
					bst_cond_eof(tmplexs);
						//если текущая лексема не означает завершение операции end
						if(!(arr_lex->get_curr_num() < arr_lex->get_count() && tmplexs->type == word_key && tmplexs->id == 2)) //error
						{
							status = -1;
							sprintf(this->error,"[%s]:%d - not found end for funcion",this->arr_lex->ArrFiles[sfunclex->fileid],sfunclex->numstr);
							return 0;
						}
					lex_get_next0(tmplexs);
					s4g_node* nfunc = new s4g_node(_function, cfunclexid, 0, head_func, body_func, extern_data);
					return new s4g_node(_set, cfunclexid-1,0, name_func, nfunc);
				}
				return new s4g_node(_set, curr_lexid, 0, node, s4g_get_expr());
		}
	return node;
}

s4g_node* s4g_builder_syntax_tree::s4g_gen_tree()
{
	status = 0;error[0]=0;
	s4g_node* tmpnode = 0;
	arr_lex->set_curr_num(0);
	tmpnode = new s4g_node(s4g_type_op::_begin,-1,0,s4g_gen_statement());

	return tmpnode;
}