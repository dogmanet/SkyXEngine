
#pragma once
#include <s4g\s4g_parser.h>

#include <s4g\s4g_vm.cpp>
#include <s4g\s4g_compiler.cpp>
#include <s4g\s4g_lexer.cpp>


///////////////

int testcfunc(s4g_vm* vm)
{
	
	return 1;
}

s4g_node* s4g_builder_syntax_tree::s4g_gen_statement()
{
	s4g_lexeme* tmplexs = arr_lex->get_next();

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
					tmplexs = arr_lex->get_next();
					s4g_node* tmpnode = s4g_get_ret_vals();	//считываем возвращаемые выражения
					tmplexs = arr_lex->get_prev();	//делаем шаг назад в лексемах
					tmpnode->op2->op1 = s4g_gen_statement();	//и считываем то что после return
					return tmpnode;
				}
				//объявление функции /function
				else if(tmplexs->id == 1)
				{
					s4g_lexeme* sfunclex = tmplexs;
					s4g_node* name_func = 0;
					tmplexs = arr_lex->get_next();
					bst_condition_eof(tmplexs);

						//если тип лексемы пользовательское слово
						if(tmplexs->type == word_user)
						{
							name_func = s4g_get_term();
						}
						else //error
						{
							status = -1;
							sprintf(this->error,"[%s]:%d - unresolved word for name function [%s]",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr,tmplexs->str);
							return 0;
						}
					bst_condition_error(this);
					tmplexs = arr_lex->get_next();
					bst_condition_eof(tmplexs);
						//если лексема это открывающая скобка
						if(tmplexs->type == sym_group && tmplexs->id == 0)
							tmplexs = arr_lex->get_next();
						else //error
						{
							status = -1;
							sprintf(this->error,"[%s]:%d - unresolved word for name function [%s]",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr,tmplexs->str);
							return 0;
						}

					s4g_node* head_func = s4g_get_function_def_head();
					bst_condition_error(this);
					arr_lex->get_prev();
					this->overend--;
					s4g_node* body_func = s4g_gen_statement();
					bst_condition_error(this);
					tmplexs = arr_lex->get_curr();
					bst_condition_eof(tmplexs);
						//если текущая лексема не означает завершение операции end
						if(!(arr_lex->get_curr_num() < arr_lex->get_count() && tmplexs->type == word_key && tmplexs->id == 2)) //error
						{
							status = -1;
							sprintf(this->error,"[%s]:%d - not found end for funcion",this->arr_lex->ArrFiles[sfunclex->fileid],sfunclex->numstr);
							return 0;
						}
					s4g_node* nfunc = new s4g_node(_function,0,head_func,body_func);
					return new s4g_node(_expr,0,new s4g_node(_set,0,name_func,nfunc),new s4g_node(_empty,0,s4g_gen_statement()));
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
					return new s4g_node(_empty,0,s4g_gen_statement());
				}
		}
		//иначе у считываем выражение
		else
		{
			//arr_lex->get_next();
			node = new s4g_node(_expr,0,s4g_get_expr());
			tmplexs = arr_lex->get_curr();
			bst_condition_eof(tmplexs);
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
					node->op2 = new s4g_node(_empty,0,s4g_gen_statement());
				}
		}
	
	return node;
}

s4g_node* s4g_builder_syntax_tree::s4g_get_term()
{
	s4g_lexeme* tmplexs = arr_lex->get_curr();
	bst_condition_eof(tmplexs);
		//если тип лексемы пользовательское слово
		if(tmplexs->type == word_user)
		{
			//создаем тип нода - переменная со значением
			s4g_node* node = new s4g_node(_var, cr_val(t_string, tmplexs->str));
			s4g_node* tmpnode = node;

			//начинаем цикл проверки - а не обращение ли это к элемента таблицы
				while(1)
				{
					tmplexs = arr_lex->get_next();
					bst_condition_eof(tmplexs);
						//если лексема относится к считыванию из таблицы .
						if(tmplexs->type == sym_table_elem && tmplexs->id == 2)
						{
							tmplexs = arr_lex->get_next();
							bst_condition_eof(tmplexs);
								if(tmplexs->type == word_user)
								{
									tmpnode->op1 = new s4g_node(_get, 0, new s4g_node(_string, cr_val(t_string, tmplexs->str)), new s4g_node(_empty));
									tmpnode = tmpnode->op1->op2;
								}
								else
									break;
						}
						//иначе если лексема относится к считыванию из таблицы [
						else if(tmplexs->type == sym_table_elem && tmplexs->id == 0)
						{
							tmplexs = arr_lex->get_next();
							bst_condition_eof(tmplexs);
							tmpnode->op1 = new s4g_node(_get,0,s4g_get_expr(),new s4g_node(_empty));
							tmpnode = tmpnode->op1->op2;

							tmplexs = arr_lex->get_curr();
							bst_condition_eof(tmplexs);
								//если следующий символ за выражением не закрывающий доступ к таблице ]
								if(!(tmplexs->type == sym_table_elem && tmplexs->id == 1))
								{
									status = -1;
									sprintf(this->error,"[%s]:%d - none end for access in table",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
									return 0;
								}
						}
						//иначе у нас нет обращения к элементам таблицы
						else 
						{
							//откатываем к предыдущей лексеме и останаливаем цикл
							arr_lex->get_prev();
							break;
						}
				}
			return node;
		}
		//если тип лексемы пустое значение
		else if(tmplexs->type == word_null)
			return new s4g_node(_null);
		//если тип лексемы строка
		else if(tmplexs->type == word_string)
			return new s4g_node(_string, cr_val(t_string, tmplexs->str));
		//если тип лексемы число с плавающей запятой
		else if(tmplexs->type == word_float)
			return new s4g_node(_float, cr_val(t_float, tmplexs->str));
		//если тип лексемы целочисленное значение
		else if(tmplexs->type == word_int)
			return new s4g_node(_int, cr_val(t_int, tmplexs->str));
		/*else if(tmplexs->type == word_bool)
			return new s4g_node(word_bool,new s4g_value(word_bool,tmplexs->str));*/
		else
			return 0;
}

s4g_node* s4g_builder_syntax_tree::s4g_get_op()
{
	s4g_lexeme* tmplexs = arr_lex->get_prev();
	Stack<s4g_node*> stack_var;
	Stack<s4g_node*> stack_op;
	s4g_node* node = 0;
	s4g_node* tmpnode = 0;
	s4g_node* tmpnode2 = 0;
	String strt;
	int how_type_next = -2;	//-1 - sym_arif, 0 - sym_group (, 1 - sym_group ), 2 - числа
		while(1)
		{
			tmplexs = arr_lex->get_next();
			bst_condition_eof(tmplexs);
				//если тип текущей лексемы арифметический либо это скобки
				if(tmplexs->type == sym_arif || tmplexs->type == sym_group)
				{
					//определяем тип будущего нода
					int tmptypeop = -1;
						if(tmplexs->type == sym_arif)
						{
								//если предыдущий тип был арифметической операцией
								if(how_type_next == -1)	//error
								{
									status = -1;
									sprintf(this->error,"[%s]:%d - недопустимая последовательсть арифметических операций друг за другом",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
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
											stack_var.push(new s4g_node(_numnull, cr_val(t_nn)));
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
											stack_var.push(new s4g_node(_numnull, cr_val(t_nn)));
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
											sprintf(this->error, "[%s]:%d - недопустимое начало выражения [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
											return 0;
										}
										//а предудщий символ арифметическая операция
										if(how_type_next == -1)	//error
										{
											status = -1;
											sprintf(this->error,"[%s]:%d - недопустимое окончание выражения",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
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
													status = -1;
													sprintf(this->error,"[%s]:%d - лишняя закрывающая скобка",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
													return 0;
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
													sprintf(this->error,"[%s]:%d - лишняя закрывающая скобка",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
													return 0;
												}
											//вызов функции дает запрос на поиск
											arr_lex->get_prev();
											break;
										}
								}
								//если в стеке чтото есть либо операция открываюая скобка
								else if(stack_op.count() > 0 || tmptypeop == _group_b)
								{
										if(tmptypeop == _group_b)
										{
											stack_op.push(new s4g_node((s4g_type_op)tmptypeop));
										}
										else if(s4g_get_priority((s4g_type_op)tmptypeop) <= s4g_get_priority(stack_op.get(stack_op.count())->type))
										{
											//выталкиваем
											s4g_node* qwert = stack_op.get(stack_op.count());
											stack_var.push(qwert/*stack_op.get(stack_op.count())*/);
											stack_op.pop(1);
											stack_op.push(new s4g_node((s4g_type_op)tmptypeop));
										}
										else
										{
											//помещаем
											stack_op.push(new s4g_node((s4g_type_op)tmptypeop));
										}
								}
								//иначе стек пуст и у нас какая-то операция
								else
									stack_op.push(new s4g_node((s4g_type_op)tmptypeop));
						}
						//иначе у нас неопределенная операция
						else
						{
							arr_lex->get_prev();
							break;
						}
				}
				//иначе если удалось считать терм
				else if((tmpnode = s4g_get_term()) != 0)
				{
						//если предыдующий тип был термом то значит у нас идут 2 терма подряд
						if(how_type_next == 2)	//error
						{
							status = -1;
							sprintf(this->error,"[%s]:%d - недопустимая последовательность термов друг за другом",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
							return 0;
						}
					tmplexs = arr_lex->get_next();
					bst_condition_eof(tmplexs);
						//если терм это переменная и следующий символ это открывающая скобка
						if(tmpnode->type == _var && tmplexs->type == sym_group && tmplexs->id == 0)
						{
							//то у нас вызов функции
							tmplexs = arr_lex->get_next();
							bst_condition_eof(tmplexs);
							overge--;
							s4g_node* args = s4g_get_arg_call_func();
							bst_condition_error(this);
							tmplexs = arr_lex->get_curr();
							stack_var.push(new s4g_node(_call,0,tmpnode,args));
						}
						else
						{
							stack_var.push(tmpnode);
							tmplexs = arr_lex->get_prev();
						}
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
			tmplexs = arr_lex->get_prev();
			status = -1;
			sprintf(this->error,"[%s]:%d - недопустимое окончание выражения",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
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
							sprintf(this->error,"[%s]:%d - нет закрывающей скобки",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
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
	s4g_lexeme* tmplexs = arr_lex->get_curr();
	s4g_node* node = new s4g_node(_arg);
	s4g_node* tmpnode = node;
	s4g_node* tmpnode2 = 0;
	Array<String> arrarg;
		
		while(1)
		{
			tmplexs = arr_lex->get_curr();
				
				//если тип лексемы закрывающая скобка
				if(tmplexs->type == sym_group && tmplexs->id == 1)
				{
					arr_lex->get_next();
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
									tmpnode->value = cr_val(t_string, tmplexs->str);
								}
								else
								{
									tmpnode->op1 = new s4g_node(_arg, cr_val(t_string, tmplexs->str));
									tmpnode = tmpnode->op1;
								}

							arrarg.push_back(tmplexs->str);
							tmplexs = arr_lex->get_next();

								if((tmplexs->type == sym_delimiter && tmplexs->id == 1))
								{
									arr_lex->get_next();
								}
								else if(!(tmplexs->type == sym_group && tmplexs->id == 1))
								{
									this->status = -1;
									sprintf(this->error,"[%s]:%d - нет разделителя аргументов",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr);
									return 0;
								}
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
	s4g_lexeme* tmplexs = arr_lex->get_curr();
	s4g_node* node = new s4g_node(_create_table);
	s4g_node* tmpnode = node;
	int type_last = -1;
		while(1)
		{
			tmplexs = arr_lex->get_curr();
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
					arr_lex->get_next();
					break;
				}
				//если не запятая
				else if(!(tmplexs->type == sym_delimiter && tmplexs->id == 1))
				{
					s4g_node* tmpnode2 = s4g_get_expr();
					bst_condition_error(this);
						if(tmpnode2->type != _set)
						{
							tmpnode->op1 = new s4g_node(_add_in_table,0,tmpnode2,new s4g_node(_empty));
							tmpnode = tmpnode->op1->op2;
						}
						else
						{
							tmpnode2->type = _sett;
							tmpnode->op1 = tmpnode2;
							tmpnode->op2 = new s4g_node(_empty);
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
					arr_lex->get_next();
					type_last = 0;
				}
		}
	return node;
}

s4g_node* s4g_builder_syntax_tree::s4g_get_ret_vals()
{
	s4g_lexeme* tmplexs = arr_lex->get_curr();
	s4g_node* node = new s4g_node(_return);
	s4g_node* tmpnode = node;
	s4g_node* tmpnode2 = 0;
		while(1)
		{
			tmplexs = arr_lex->get_curr();
				if(!(tmplexs->type == sym_delimiter && tmplexs->id == 1) && 
					!(tmplexs->type == sym_delimiter && tmplexs->id == 0) &&
					!(tmplexs->type == word_key && tmplexs->id == 2))
				{
					tmpnode2 = s4g_get_expr();
					tmpnode->op1 = tmpnode2;//new s4g_node(_expr,0,tmpnode2,new s4g_node(_empty));
					tmpnode->op2 = new s4g_node(_empty);
					tmpnode = tmpnode->op2;
					tmplexs = arr_lex->get_curr();
						if(tmplexs->type == sym_delimiter && tmplexs->id == 0)
						{
							arr_lex->get_next();
							break;
						}
					arr_lex->get_next();
				}
				else if((tmplexs->type == sym_delimiter && tmplexs->id == 1))
				{
					arr_lex->get_next();
				}
				else 
					break;
		}
	return node;
}

s4g_node* s4g_builder_syntax_tree::s4g_get_arg_call_func()
{
	s4g_lexeme* tmplexs = arr_lex->get_curr();
	s4g_node* node = new s4g_node(_arg);
	s4g_node* tmpnode = node;
	s4g_node* tmpnode2 = 0;
	int type_last = -1;
		while(1)
		{
			tmplexs = arr_lex->get_curr();
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
					//tmplexs = arr_lex->get_next();
					break;
				}
				else if(!(tmplexs->type == sym_delimiter && tmplexs->id == 1))
				{
					tmpnode2 = s4g_get_expr();
					tmpnode->op1 = tmpnode2;//new s4g_node(_expr,0,tmpnode2,new s4g_node(_empty));
					tmpnode->op2 = new s4g_node(_empty);
					tmpnode = tmpnode->op2;
					arr_lex->get_next();
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
					arr_lex->get_next();
				}

		}
	return node;
}

s4g_node* s4g_builder_syntax_tree::s4g_get_expr()
{
	s4g_lexeme* tmplexs = arr_lex->get_curr();
		//если текущий токен - создание таблицы
		if(tmplexs->type == sym_table_create && tmplexs->id == 0)
		{
			arr_lex->get_next();
			s4g_node* tmpnode = s4g_get_table();
			arr_lex->get_prev();
			//tmpnode->op3 = s4g_get_expr();
			return tmpnode;
		}
	s4g_node* node = s4g_get_op();
	tmplexs = arr_lex->get_curr();
		if(node == 0)
			return 0;
		//если тип текущего нода переменная и следующая лексема присвоение
		if(node->type == _var && tmplexs->type == sym_assign)
		{
			tmplexs = arr_lex->get_next();
				//если символ создания таблицы
				if(tmplexs->type == sym_table_create && tmplexs->id == 0)
				{
					arr_lex->get_next();
					//создаем нод и получаем содержимое таблицы
					return new s4g_node(_set,0,node,s4g_get_table());
				}
				//если символ ключевое слово "функция"
				else if(tmplexs->type == word_key && tmplexs->id == 1)
				{
					s4g_lexeme* sfunclex = tmplexs;
					s4g_node* name_func = node;

					tmplexs = arr_lex->get_next();
					bst_condition_eof(tmplexs);
						//если лексема это открывающая скобка
						if(tmplexs->type == sym_group && tmplexs->id == 0)
							tmplexs = arr_lex->get_next();
						else //error
						{
							status = -1;
							sprintf(this->error,"[%s]:%d - expected [%s] but got [%s]",this->arr_lex->ArrFiles[tmplexs->fileid],tmplexs->numstr,s4g_key_syms_group[0],tmplexs->str);
							return 0;
						}

					s4g_node* head_func = s4g_get_function_def_head();
					bst_condition_error(this);
					arr_lex->get_prev();
					this->overend--;
					s4g_node* body_func = s4g_gen_statement();
					bst_condition_error(this);
					tmplexs = arr_lex->get_curr();
					bst_condition_eof(tmplexs);
						//если текущая лексема не означает завершение операции end
						if(!(arr_lex->get_curr_num() < arr_lex->get_count() && tmplexs->type == word_key && tmplexs->id == 2)) //error
						{
							status = -1;
							sprintf(this->error,"[%s]:%d - not found end for funcion",this->arr_lex->ArrFiles[sfunclex->fileid],sfunclex->numstr);
							return 0;
						}
					arr_lex->get_next();
					s4g_node* nfunc = new s4g_node(_function,0,head_func,body_func);
					return new s4g_node(_set,0,name_func,nfunc);
				}
			return new s4g_node(_set,0,node,s4g_get_expr());
		}
	return node;
}

s4g_node* s4g_builder_syntax_tree::s4g_gen_tree()
{
	status = 0;error[0]=0;
	s4g_node* tmpnode = 0;

	tmpnode = new s4g_node(s4g_type_op::_begin,0,s4g_gen_statement());

	return tmpnode;
}