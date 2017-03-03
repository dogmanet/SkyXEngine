
#include "s4g_parser.h"

#include "s4g_vm.cpp"
#include "s4g_compiler.cpp"
#include "s4g_lexer.cpp"
#include "s4g_preprocessor.cpp"

//генерация инструкций циклом
s4g_node* s4g_builder_syntax_tree::s4g_gen_statement()
{
	//стартовый нод
	s4g_node* node = NodePool.Alloc(s4g_type_op::_begin, -1, (s4g_value*)0);
	s4g_stack<s4g_statement> arrnode;	//массив инструкций которые надо определить
	arrnode.push(s4g_statement(node, &(node->op1)));
	s4g_node* oldroot = 0;	//предыдущий родитель удаляемой инструкции (на случай веток условий)
	long countinn = 0;		//количество итераций прошедших с момента последнего обновления oldroot
	bool isread = true;
	
	while (isread)
	{
		//если уже второй раз заходим в в цикл после последнего обновления oldroot
		if (countinn > 1)
			oldroot = 0;	//обнуляем предыдущего родителя

		s4g_lexeme* lex_get_curr0(tmplexs);

		//если лексема не считана значит скорее всего лексемы исчерпаны
		if (tmplexs == 0)
			break;

		//если тип лексемы это ключевое слово языка
		if (tmplexs->type == word_key)
		{
			//возвращаение /return
			if (tmplexs->id == S4GLKW_RETURN)
			{
				bool oldisender = isender;
				isender = false;
				lex_get_next0(tmplexs);

				//op1 - возвращаемое выражение
				//считываем выражение
				s4g_node* tmpnode = NodePool.Alloc(_return, curr_lexid, (s4g_value*)0, s4g_get_expr(false));

				//продвигаемся дальше по иерархии
				*(arrnode[arrnode.count_obj - 1].node) = tmpnode;

				lex_get_curr0(tmplexs);
				bst_cond_er(this);
				isender = oldisender;

				//если после выражения нет символа окончания операции ;
				if (!((tmplexs->type == sym_delimiter && tmplexs->id == 0) || (tmpnode->op1 && tmpnode->op1->type == _set && tmpnode->op1->op2 && tmpnode->op1->op2->type == _function)))	//error
				{
					status = -1;
					sprintf(this->error, "[%s]:%d - ';' expected but found '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
					return 0;
				}
				lex_get_next0(tmplexs);
				
				//переходим на продолжение, то есть на то что будет после
				arrnode[arrnode.count_obj - 1].node = &((*arrnode[arrnode.count_obj - 1].node)->op3);
			}
			//объявление функции /function
			else if (tmplexs->id == S4GLKW_FUNCTION)
			{
				long funclexid = curr_lexid;
				bool oldisender = isender;
				isender = false;

				s4g_node* name_func = 0;
				lex_get_next0(tmplexs);

				//если тип лексемы создаваемое пользовательское слово
				if (tmplexs->type == word_user_cr)
				{
					//ошибка, указывать создание не надо
					status = -1;
					sprintf(this->error, "[%s]:%d - function provides for the creation of a new variable, it's too much [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, s4g_key_syms_create[0]);
					return 0;
				}
				//если тип лексемы пользовательское слово
				else if (tmplexs->type == word_user)
				{
					readcall = false;	//запрещаем вызовы функций
					name_func = s4g_get_term();	//считываем имя функции
					name_func->type = _crvar;	//указываем на создание такого имени
					readcall = true;	//разрешаем считывать вызовы функций
				}
				//иначе у нас какой-то другой тип, недопустимый
				else //error
				{
					status = -1;
					sprintf(this->error, "[%s]:%d - unresolved word for name function [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
					return 0;
				}

				lex_get_curr0(tmplexs);
				bst_cond_er(this);

				//если лексема это открывающая скобка
				if (tmplexs->type == sym_group && tmplexs->id == 0)
				{
					lex_get_next(tmplexs);
				}
				//иначе ошибка, ибо нам надо как то аргументы считать
				else //error
				{
					status = -1;
					sprintf(this->error, "[%s]:%d - '(' expected, but got [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
					return 0;
				}

				//считываем заголовок функции (точнее список аргументов)
				s4g_node* head_func = s4g_get_function_def_head();
				
				lex_get_curr0(tmplexs);
				bst_cond_er(this);

				//считываем начало блока
				s4g_begin_read_block();
				
				lex_get_curr0(tmplexs);
				bst_cond_er(this);

				//op1 - имя функции, для доступа
				//op2 - тело функции для исполнения
				s4g_node* nfunc = NodePool.Alloc(_function, funclexid, (s4g_value*)0, head_func);

				s4g_node* nfuncexpr = NodePool.Alloc(_expr, funclexid, (s4g_value*)0, 
										NodePool.Alloc(_set, funclexid, (s4g_value*)0, name_func, nfunc), 
										NodePool.Alloc(_empty, curr_lexid, (s4g_value*)0));

				//проходим дальше
				*arrnode[arrnode.count_obj - 1].node = nfuncexpr;
				arrnode[arrnode.count_obj - 1].node = &((*arrnode[arrnode.count_obj - 1].node)->op2->op1);

				//добавляем новую инструкцию, родитель самая функция
				arrnode.push(s4g_statement(nfunc, &(nfunc->op2)));

				isender = oldisender;
			}
			//условие if
			else if (tmplexs->id == S4GLKW_IF)
			{
				bool oldisender = isender;
				isender = false;
				
				lex_get_next0(tmplexs);
				//если текущая лексема не (
				if (tmplexs->type != sym_group || tmplexs->id != 0)
				{
					//генерим ошибку ибо нам надо как-то сгруппировать условие
					status = -1;
					sprintf(this->error, "[%s]:%d - '(' expected but found '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
					return(0);
				}

				lex_get_next0(tmplexs);
				--overge;
				//считываем условие
				s4g_node* tmpnode = NodePool.Alloc(_if, curr_lexid, (s4g_value*)0, s4g_get_expr(false));
				
				lex_get_curr0(tmplexs);
				bst_cond_er(this);

				isender = oldisender;
				//если после условий не )
				if (tmplexs->type != sym_group || tmplexs->id != 1)
				{
					//генерим ошибку, ибо группировка условий не удалась
					status = -1;
					sprintf(this->error, "[%s]:%d - ')' expected but found '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
					return(0);
				}

				lex_get_next0(tmplexs);
				//читаем начало блока
				s4g_begin_read_block();
				
				lex_get_curr(tmplexs);
				bst_cond_er(this);

				s4g_node* tmpnode2 = NodePool.Alloc(_block, curr_lexid, (s4g_value*)0, tmpnode);

				//если условие выполниться то значит мы сюда пришли из ветки else
				if (arrnode[arrnode.count_obj - 1].root->ud == 10)
				{
					//значит предыдущий родитель это if и его ветка уже отработана,
					//смело можем переназначать родителя
					arrnode[arrnode.count_obj - 2].root = tmpnode2;

					arrnode[arrnode.count_obj - 1].root = tmpnode2;
					*arrnode[arrnode.count_obj - 1].node = tmpnode2;
					arrnode[arrnode.count_obj - 1].node = &(tmpnode2->op1->op2);

					//arrnode.push_back(s4g_statement(tmpnode2, &(tmpnode2->op1->op2)));
				}
				else
				{
					//иначе получается что это новое условие, и первая ветка, а значит и новая инструкция
					//записываем блок родителем, а тело ветки в нод для просчетов

					//записываем нод блока, и переходим на его выход (op3 - следующее звено цепи кода)
					arrnode[arrnode.count_obj - 1].root = tmpnode2;
					*arrnode[arrnode.count_obj - 1].node = tmpnode2;
					arrnode[arrnode.count_obj - 1].node = &(tmpnode2->op3);

					arrnode.push_back(s4g_statement(tmpnode2, &(tmpnode2->op1->op2)));
				}

				
			}
			//ветка условия else
			else if (tmplexs->id == S4GLKW_ELSE)
			{
				oldroot = arrnode.get(arrnode.count_obj - 1).root;
				//если предыдущий (прям с предыдущей итерации) родительский нод был блоком условия
				if (oldroot && oldroot->type == _block && oldroot->op1->type == _if)
				{
					//если ветка else уже есть значит что-то не то ...
					//вообще не должно сработать, но на всякий случай оставил
					if (oldroot->op1->op3)
					{
						status = -1;
						sprintf(this->error, "[%s]:%d - branch conditions already, fix error", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return(0);
					}

					lex_get_next0(tmplexs);

					//определяем является ли данная ветка самостоятельной либо состоит из дополнительного условия
					int isblock;
					if (tmplexs->type == word_key && tmplexs->id == S4GLKW_IF)
						isblock = 0;	//всетаки есть условие
					else
						isblock = s4g_begin_read_block();	//нет, чисто else

					lex_get_curr0(tmplexs);

					if (isblock)
					{
						//ветка else
						//добавляем к прошлому родителю (он же if к которому привязана данная ветка) третий ноl (op3) который и есть else
						oldroot->op1->op3 = NodePool.Alloc(_block, curr_lexid, (s4g_value*)0);
						//добавляем новую инструкцию, родитель новый блок ветки (но внутри не известно, ветка это или просто блок)
						//а нодом будет тело блока
						arrnode.push_back(s4g_statement(oldroot->op1->op3, &(oldroot->op1->op3->op1)));
					}
					else
					{
						//ветка else if скорее всего, но дальше будет видно
						//s4g_statement tmpn = arrnode[arrnode.count_obj - 1];
						oldroot->ud = 10;	//сообщаем что мы ждем продолжение в виде if
						//записываем новую инструкцию, родитель прошлый родитель (он же _block->op1 == _if)
						//нодом записываем ветку условия в _if ноде
						arrnode.push_back(s4g_statement(oldroot, &(oldroot->op1->op3)));
					}
				}
				//иначе ошибка, ветка else не привязана к условию
				else
				{
					status = -1;
					sprintf(this->error, "[%s]:%d - 'else' not found previous condition", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
					return(0);
				}
			}
			//цикл while
			else if (tmplexs->id == S4GLKW_WHILE)
			{
				if (dowhile > 0) // это завершение для do
				{
					--dowhile;
				}
				else // это простой while
				{
					--dowhile;
					bool oldisender = isender;
					isender = false;
					
					lex_get_next0(tmplexs);
					if (tmplexs->type != sym_group || tmplexs->id != 0)
					{
						status = -1;
						sprintf(this->error, "[%s]:%d - '(' expected but found '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return(0);
					}

					//считываем условие
					s4g_node* tmpnode = NodePool.Alloc(_while, curr_lexid, (s4g_value*)0, s4g_get_expr(false));	
					lex_get_prev(tmplexs);
					bst_cond_er(this);

					isender = oldisender;
					if (tmplexs->type != sym_group || tmplexs->id != 1)
					{
						status = -1;
						sprintf(this->error, "[%s]:%d - ')' expected but found '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return(0);
					}
					lex_get_next0(tmplexs);
					//tmpnode->op2 = s4g_read_block(); //s4g_gen_statement();	//и считываем то что while
					s4g_begin_read_block();
					lex_get_curr0(tmplexs);
					bst_cond_er(this);


					//tmpnode = NodePool.Alloc(_block, curr_lexid, (s4g_value*)0, tmpnode, one ? NodePool.Alloc(_empty, curr_lexid, (s4g_value*)0) : s4g_gen_statement());
					s4g_node* tmpnode2 = NodePool.Alloc(_block, curr_lexid, (s4g_value*)0, tmpnode);
					//записываем в последний нод, нод while и сразу же переходим на следующий нод который будет продолжением цепи
					/**arrnode[arrnode.size() - 1] = tmpnode2;
					arrnode[arrnode.size() - 1] = &(tmpnode2->op2);

					//записываем тело while, которое будем сейчас считать
					arrnode.push_back(&(tmpnode->op2));*/

					*arrnode[arrnode.count_obj - 1].node = tmpnode2;
					arrnode[arrnode.count_obj - 1].node = &(tmpnode2->op2);

					//записываем тело while, которое будем сейчас считать
					arrnode.push(s4g_statement(tmpnode2, &(tmpnode->op2)));

					++dowhile;
				}
			}
			else if (tmplexs->id == S4GLKW_FOR)
			{
				bool oldisender = isender;
				isender = false;

				lex_get_next0(tmplexs);
				//если сразу после for у нас не (
				if (tmplexs->type != sym_group || tmplexs->id != 0)
				{
					//ошибка, ибо должна быть группировка
					status = -1;
					sprintf(this->error, "[%s]:%d - '(' expected but found '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
					return(0);
				}

				lex_get_next0(tmplexs);
				//считываем первый блок с выражением (инициализация)
				s4g_node* expt_init = NodePool.Alloc(_expr, curr_lexid, (s4g_value*)0, s4g_get_expr(false), NodePool.Alloc(_empty));

				//возможно у нас список выражений через ,
				//{
				if (expt_init)
				{
					s4g_node* tmpnode_expt_init = expt_init->op2;
					while (tmpnode_expt_init)	//если последнее вырежение считано
					{
						lex_get_curr0(tmplexs);
						//если встретили ,
						if ((tmplexs->type == sym_delimiter && tmplexs->id == 1))
						{
							lex_get_next(tmplexs);	//просто шагаем дальше
						}
						//иначе если встретили ;
						else if (tmplexs->type == sym_delimiter && tmplexs->id == 0)
							break;	//прерываем цикл так как это уже другой блок выражений

						tmpnode_expt_init->op1 = NodePool.Alloc(_expr, curr_lexid, (s4g_value*)0, s4g_get_expr(false), NodePool.Alloc(_empty));

						//если мы не смогли прочитать выражение, но не стоит забывать что у нас была перед этим запятая
						if (!(tmpnode_expt_init->op1->op1))
							break;	//останавливаем цикл

						tmpnode_expt_init = tmpnode_expt_init->op1->op2;
						bst_cond_er(this);
					}
				}
				//}

				lex_get_curr0(tmplexs);
				bst_cond_er(this);

				//если теперь не следует ;
				if (!(tmplexs->type == sym_delimiter && tmplexs->id == 0))	//error
				{
					status = -1;
					sprintf(this->error, "[%s]:%d - ';' expected but found '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
					return 0;
				}

				lex_get_next0(tmplexs);

				//считываем условие выполнение цикла
				s4g_node* expt_cond = 0;
				//однако если условие у нас пустое, то есть сразу же следует ;
				if (tmplexs->type == sym_delimiter && tmplexs->id == 0)
				{
					//проинициализируем дефолтовым истинным выражением
					expt_cond = NodePool.Alloc(_bool, curr_lexid, gc->cr_val_bool(true, 0, S4G_GC_TYPE_DATA_SYS));
				}
				//иначе у нас там выражение
				else
				{
					expt_cond = s4g_get_expr(false);	//читаем выражение

					lex_get_curr0(tmplexs);
					bst_cond_er(this);

					//если текущая лексема не ;
					if (!(tmplexs->type == sym_delimiter && tmplexs->id == 0))	//error
					{
						status = -1;
						sprintf(this->error, "[%s]:%d - ';' expected but found '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return 0;
					}
				}

				lex_get_next0(tmplexs);
				--overge;

				s4g_node* expt_step = 0;	//обнуляем третий блок, ибо может быть его и нет вовсе
				//если текущая лексема не )
				if (!(tmplexs->type == sym_group && tmplexs->id == 1))
				{
					//считываем третий блок выражений
					expt_step = NodePool.Alloc(_expr, curr_lexid, (s4g_value*)0, s4g_get_expr(false), NodePool.Alloc(_empty));
					lex_get_curr0(tmplexs);
					bst_cond_er(this);

					//если удалось считать выражение, то возможно у нас список выражений ... анализируем
					//{
					if (expt_step)
					{
						s4g_node* tmpnode_expt_step = expt_step->op2;

						while (tmpnode_expt_step)	//если удалось считать последнее выражение
						{
							lex_get_curr0(tmplexs);

							//если текущая лексема ,
							if ((tmplexs->type == sym_delimiter && tmplexs->id == 1))
							{
								lex_get_next(tmplexs);	//просто шагаем дальше
							}
							//иначе если текущая лексема )
							else if (tmplexs->type == sym_group && tmplexs->id == 1)
							{
								break;	//прерываем цикл
							}

							tmpnode_expt_step->op1 = NodePool.Alloc(_expr, curr_lexid, (s4g_value*)0, s4g_get_expr(false), NodePool.Alloc(_empty));

							//если мы не смогли прочитать выражение, но не стоит забывать что у нас была перед этим запятая
							if (!(tmpnode_expt_step->op1->op1))
								break;	//останавливаем цикл

							tmpnode_expt_step = tmpnode_expt_step->op1->op2;
							lex_get_curr0(tmplexs);
							bst_cond_er(this);
						}
					}
					//}
				}
				else
					++overge;

				lex_get_curr0(tmplexs);
				bst_cond_er(this);

				//если текущий символ не )
				if (tmplexs->type != sym_group || tmplexs->id != 1)
				{
					//генерим ошибку ибо нам надо как то закрыть for
					status = -1;
					sprintf(this->error, "[%s]:%d - ')' expected but found '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
					return(0);
				}

				lex_get_next0(tmplexs);
				isender = oldisender;

				//считываем начало блока
				s4g_begin_read_block();

				lex_get_curr0(tmplexs);
				bst_cond_er(this);

				//op1 - _block
				//op1->op1 - инициализация
				//op2 - _for
				//op2->op1 - условие выполнения
				//op2->op2 - тело исполнения
				//op2->op3 - пост выражения, выполняются после каждой итерации
				s4g_node* tmpnode = NodePool.Alloc(_block, curr_lexid, (s4g_value*)0, expt_init,
					NodePool.Alloc(_for, curr_lexid, (s4g_value*)0, expt_cond, (s4g_node*)0, expt_step)
					);

				//помещаем все это в _empty нод, так как не влезает все в один нод
				s4g_node* tmpnode2 = NodePool.Alloc(_empty, curr_lexid, (s4g_value*)0, tmpnode);
				
				//продвигаемся дальше по просчетам дерева
				*arrnode[arrnode.count_obj - 1].node = tmpnode2;
				arrnode[arrnode.count_obj - 1].node = &(tmpnode2->op2);

				//записываем тело for, которое будем сейчас считать, родителем записываем блок этого for
				arrnode.push_back(s4g_statement(tmpnode, &(tmpnode->op2->op2)));

			}
			else if (tmplexs->id == S4GLKW_DO)
			{
				++dowhile;
			}
			//оператор break остановка цикла
			else if (tmplexs->id == S4GLKW_BREAK)
			{
				lex_get_next0(tmplexs);
				//сразу после должна быть лексема ; если это не так
				if (tmplexs->type != sym_delimiter || tmplexs->id != 0)
				{
					//генерим ошибку
					status = -1;
					sprintf(this->error, "[%s]:%d - ';' expected but found '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
					return(0);
				}
				//уходим дальше не генерацию звеньев
				*arrnode[arrnode.count_obj - 1].node = NodePool.Alloc(_break, curr_lexid, (s4g_value*)0);
				arrnode[arrnode.count_obj - 1].node = &((*arrnode[arrnode.count_obj - 1].node)->op1);
			}
			//оператор continue
			else if (tmplexs->id == S4GLKW_CONTINUE)
			{
				lex_get_next0(tmplexs);
				//сразу после должна быть лексема ; если это не так
				if (tmplexs->type != sym_delimiter || tmplexs->id != 0)
				{
					//генерим ошибку
					status = -1;
					sprintf(this->error, "[%s]:%d - ';' expected but found '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
					return(0);
				}
				//уходим дальше не генерацию звеньев
				*arrnode[arrnode.count_obj - 1].node = NodePool.Alloc(_continue, curr_lexid, (s4g_value*)0);
				arrnode[arrnode.count_obj - 1].node = &((*arrnode[arrnode.count_obj - 1].node)->op1);
			}
		}
		//разделитель
		else if (tmplexs->type == sym_delimiter)
		{
			//разделитель операций
			if (tmplexs->id == 0)
			{
				lex_get_next0(tmplexs);
				if (tmplexs)
				{
					s4g_node* tmpn = arrnode[arrnode.count_obj - 1].root;

					//добавляем новый пустой (empty) нод и тут же переходим на следующий, который будет продолжением цепи
					*arrnode[arrnode.count_obj - 1].node = NodePool.Alloc(_empty, curr_lexid, (s4g_value*)0);
					arrnode[arrnode.count_obj - 1].node = &((*arrnode[arrnode.count_obj - 1].node)->op1);
				}
			}
		}
		//если символ окончания таблицы, либо окончания блока
		else if (tmplexs->type == sym_table_create && tmplexs->id == 1)
		{
			this->overend++;	//инкрементируем количество таких найденных элементов
			//если мы не искали энд но нашли значит он здесь лишний
			if (this->overend > 0)
			{
				//генерим ошибку
				status = -1;
				sprintf(this->error, "[%s]:%d - not found operation for end", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
				return 0;
			}

			lex_get_next0(tmplexs);

			//получаем родительский нод, и ставим его как прошлый
			oldroot = arrnode[arrnode.count_obj - 1].root;

			//если родитльский нод блок да еще и условие у которого есть ветка else
			/*if (oldroot && oldroot->type == _block && oldroot->op1 && oldroot->op1->type == _if && oldroot->op1->op3)
			{
				//значит мы уже отработали этот нод, надо еще ниже спустится
				--(arrnode.count_obj);
				oldroot = arrnode[arrnode.count_obj - 1].root;
			}*/

			//пока оставить закоменченным ...
			/*if (oldroot && oldroot->ud == 10)
			{
				--(arrnode.count_obj);
				oldroot = arrnode[arrnode.count_obj - 1].root;
			}*/
			
			countinn = 0;

			s4g_statement tmpn = arrnode[arrnode.count_obj - 1];
			//удаляем из массива последний нод, так как именно в него мы и писали
			--(arrnode.count_obj);
		}
		//если у нас символ начала таблицы, значит у нас начало блока
		else if (tmplexs->type == sym_table_create && tmplexs->id == 0)
		{
			//устанавливаем внутренние параметры на чтение блока
			s4g_begin_read_block();

			//в последний нод записываем новый
			(*arrnode[arrnode.count_obj - 1].node) = NodePool.Alloc(_empty, curr_lexid, (s4g_value*)0, NodePool.Alloc(_block, curr_lexid, (s4g_value*)0));
			
			//добавляем новый нод, который есть тело блока, родитель блок, нод - тело блока
			arrnode.push(s4g_statement((*arrnode[arrnode.count_obj - 1].node), &((*arrnode[arrnode.count_obj - 1].node)->op1->op1)));
			//устанавливаем что после того как мы закончим читать тело блока переходим на следующий нод который будет продолжением всей цепи
			arrnode[arrnode.count_obj - 2].node = &((*arrnode[arrnode.count_obj - 2].node)->op2);
		}
		//иначе считываем выражение
		else
		{
			bool oldisender = isender;
			isender = false;
			s4g_node* tmpnode = NodePool.Alloc(_expr, curr_lexid, (s4g_value*)0, s4g_get_expr());

			lex_get_curr0(tmplexs);
			bst_cond_er(this);
			isender = oldisender;

			//если после выражения нет символа окончания операции ;
			if (!((tmplexs->type == sym_delimiter && tmplexs->id == 0) || (tmpnode->op1 && tmpnode->op1->type == _set && tmpnode->op1->op2 && tmpnode->op1->op2->type == _function)))	//error
			{
				status = -1;
				sprintf(this->error, "[%s]:%d - ';' expected but found '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
				return 0;
			}
			//иначе если все нормально
			else
			{
				//генерируем продолжение цепи
				tmpnode->op2 = NodePool.Alloc(_empty, curr_lexid, (s4g_value*)0);

				//обновляем звенья
				*arrnode[arrnode.count_obj - 1].node = tmpnode;
				arrnode[arrnode.size() - 1].node = &((tmpnode)->op2->op1);

				//если выражением было присвоение переменной значения функции
				if (tmpnode && tmpnode->op1->type == _set && tmpnode->op1->op2->type == _function)
				{
					//читаем блок
					s4g_begin_read_block();
					bst_cond_er(this);
					lex_get_curr0(tmplexs);
					//записываем новую инструкцию, родитель _expr, нод - тело функции
					arrnode.push_back(s4g_statement(tmpnode,&(tmpnode->op1->op2->op2)));
				}
				else
				{
					lex_get_next0(tmplexs);
				}
			}
		}

		++countinn;	//обновляем количество итераций с момента последнего обнаружения родителя
	}
	return node;
}

int s4g_builder_syntax_tree::s4g_begin_read_block()
{
	s4g_lexeme* lex_get_curr0(tmplexs);
	if (tmplexs && tmplexs->type == sym_table_create && tmplexs->id == 0)
	{
		overend--;
		lex_get_next(tmplexs);
		return 1;
	}
	else if (tmplexs)
	{
		status = -1;
		sprintf(this->error, "[%s]:%d - expected begin block, but got %s", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
		return 0;
	}
	else
	{
		status = -1;
		sprintf(this->error, "[%s]:%d - unexpected end of file", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
		return 0;
	}
}

int s4g_builder_syntax_tree::s4g_end_read_block()
{
	s4g_lexeme* lex_get_curr0(tmplexs);
	if (tmplexs && tmplexs->type == sym_table_create && tmplexs->id == 0)
	{
		overend--;
		lex_get_next(tmplexs);
		return 1;
	}
	else if (tmplexs)
	{
		status = -1;
		sprintf(this->error, "[%s]:%d - expected end block, but got %s", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
		return 0;
	}
	else
	{
		status = -1;
		sprintf(this->error, "[%s]:%d - unexpected end of file", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
		return 0;
	}
}

s4g_node* s4g_builder_syntax_tree::s4g_get_term()
{
	s4g_lexeme* lex_get_curr0(tmplexs);
	bst_cond_eof(tmplexs);
	//если тип лексемы пользовательское слово
	if (tmplexs->type == word_user)
	{
		//создаем тип нода - переменная со значением
		s4g_node* node = NodePool.Alloc(_var, curr_lexid, gc->cr_val(t_string, tmplexs->str.c_str(), 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS));
		s4g_node* tmpnode = node;
		bool isnext = true;

		//начинаем цикл проверки - а не обращение ли это к элемента таблицы
		while (1)
		{
			lex_get_next0(tmplexs);
			//если лексема относится к считыванию из таблицы .
			if (tmplexs->type == sym_table_elem && tmplexs->id == 2)
			{
				lex_get_next0(tmplexs);
				if (tmplexs->type == word_user)
				{
					tmpnode->op1 = NodePool.Alloc(_get, curr_lexid, (s4g_value*)0, NodePool.Alloc(_string, curr_lexid, gc->cr_val(t_string, tmplexs->str.c_str(), 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS)), NodePool.Alloc(_empty, curr_lexid));
					tmpnode = tmpnode->op1->op2;
				}
				
				else if (tmplexs->type == word_user_cr)
				{
					tmpnode->op1 = NodePool.Alloc(_get_cr, curr_lexid, (s4g_value*)0, NodePool.Alloc(_string, curr_lexid, gc->cr_val(t_string, tmplexs->str.c_str(), 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS)), NodePool.Alloc(_empty, curr_lexid));
					tmpnode = tmpnode->op1->op2;
				}
				else
				{
					lex_get_prev0(tmplexs);
					break;
				}
			}
			//иначе если лексема относится к считыванию из таблицы [
			else if ((tmplexs->type == sym_table_elem && tmplexs->id == 0) || (tmplexs->type == sym_table_elem && tmplexs->id == 3))
			{
				int isnumelem = 0;

				//если у нас доступ через числовое значение
				if (tmplexs->type == sym_table_elem && tmplexs->id == 3)
				{
					lex_get_next0(tmplexs);
					//если следующая лексема это не считывание из таблицы [
					if (!(tmplexs->type == sym_table_elem && tmplexs->id == 0))
					{
						//error
						status = -1;
						sprintf(this->error, "[%s]:%d - expected '[' after ':', but got %s", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return 0;
					}
					else
					{
						isnumelem = 1;
					}
				}
				lex_get_next0(tmplexs);

				//если у нас добавление в конец таблицы []
				if (tmplexs->type == sym_table_elem && tmplexs->id == 1)
				{
					tmpnode->op1 = NodePool.Alloc(_append_table, curr_lexid);
					break;
				}

				tmpnode->op1 = NodePool.Alloc(_get, curr_lexid, (s4g_value*)isnumelem, s4g_get_expr(), NodePool.Alloc(_empty, curr_lexid));
				tmpnode = tmpnode->op1->op2;

				lex_get_curr0(tmplexs);
				//если следующий символ за выражением не закрывающий доступ к таблице ]
				if (!(tmplexs->type == sym_table_elem && tmplexs->id == 1))
				{
					status = -1;
					sprintf(this->error, "[%s]:%d - none end for access in table", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
					return 0;
				}
			}
			//если терм это переменная и следующий символ это открывающая скобка
			else if (readcall && tmplexs->type == sym_group && tmplexs->id == 0)
			{
				//то у нас вызов функции
				long funccallidlex = curr_lexid;
				lex_get_next0(tmplexs);
				overge--;
				//считываем все выражения аргументов
				s4g_node* args = s4g_get_arg_call_func();

				bst_cond_er(this);
				lex_get_curr0(tmplexs);
				
				//op1 - имя функции
				//op2 - аргументы
				return NodePool.Alloc(_call, funccallidlex, (s4g_value*)0, node, (s4g_node*)args, (s4g_node*)0);
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
	//если пользовательское слово, может быть переменной
	else if (tmplexs->type == word_user_cr)
	{
		s4g_node* node = NodePool.Alloc(_crvar, curr_lexid, gc->cr_val(t_string, tmplexs->str.c_str(), 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS));
		lex_get_next0(tmplexs);
		return node;
	}
	//если тип лексемы пустое значение
	else if (tmplexs->type == word_null)
	{
		s4g_node* node = NodePool.Alloc(_null, curr_lexid, gc->cr_val_null(0, S4G_GC_TYPE_VAR_SYS));
		lex_get_next0(tmplexs);
		return node;
	}
	//если тип лексемы строка
	else if (tmplexs->type == word_string)
	{
		s4g_node* node = NodePool.Alloc(_string, curr_lexid, gc->cr_val(t_string, tmplexs->str.c_str(), 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS));
		lex_get_next0(tmplexs);
		return node;
	}
	else if (tmplexs->type == word_string_cr)
	{
		s4g_node* node = NodePool.Alloc(_string_cr, curr_lexid, gc->cr_val(t_string, tmplexs->str.c_str(), 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS));
		lex_get_next0(tmplexs);
		return node;
	}
	//если тип лексемы число с плавающей запятой
	else if (tmplexs->type == word_float)
	{
		s4g_node* node = NodePool.Alloc(_float, curr_lexid, gc->cr_val(t_float, tmplexs->str.c_str(), 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS));
		lex_get_next0(tmplexs);
		return node;
	}
	//если тип лексемы целочисленное значение
	else if (tmplexs->type == word_int)
	{
		s4g_node* node = NodePool.Alloc(_int, curr_lexid, gc->cr_val(t_int, tmplexs->str.c_str(), 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS));
		lex_get_next0(tmplexs);
		return node;
	}
	//если тип лексемы создание целого числа
	else if (tmplexs->type == word_int_cr)
	{
		s4g_node* node = NodePool.Alloc(_int_cr, curr_lexid, gc->cr_val(t_int, tmplexs->str.c_str(), 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS));
		lex_get_next0(tmplexs);
		return node;
	}
	//если тип лексемы беззнаковое целое число
	else if (tmplexs->type == word_uint)
	{
		s4g_node* node = NodePool.Alloc(_uint, curr_lexid, gc->cr_val(t_uint, tmplexs->str.c_str(), 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS));
		lex_get_next0(tmplexs);
		return node;
	}
	//если тип лексемы логическое значение
	else if (tmplexs->type == word_bool)
	{
		s4g_node* node = NodePool.Alloc(_bool, curr_lexid, gc->cr_val(t_bool, tmplexs->str.c_str(), 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS));
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

	bool done = false;
	bool donable = tmplexs->type == sym_group && tmplexs->id == 0;

	int pareth = 0;
	int how_type_next = -4;	//-3 - sym_arif_assign ++ , -2 - sym_arif_assign -- , -1 - sym_arif, 0 - sym_group (, 1 - sym_group ), 2 - числа
	while (1)
	{
		lex_get_curr0(tmplexs);
		if (!tmplexs)
		{
			lex_get_prev0(tmplexs);
			break;
		}
		bst_cond_eof(tmplexs);

		//если тип текущей лексемы арифметический либо это скобки
		if (tmplexs->type == sym_arif || tmplexs->type == sym_group)
		{
			if (how_type_next == -3 || how_type_next == -2)
			{
				status = -1;
				sprintf(this->error, "[%s]:%d - unexpected sequence arifmetic assignation and arifmetic operation", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
				return 0;
			}

			//определяем тип будущего нода
			int tmptypeop = -1;

			//если текущий символ арифметической операции
			if (tmplexs->type == sym_arif)
			{
				//если предыдущий тип был арифметической операцией
				if (how_type_next == -1)	//error
				{
					if (!(
						tmplexs->id == S4GLAO_BIT_NOT
						|| tmplexs->id == S4GLAO_LOG_NOT
						))
					{
						status = -1;
						sprintf(this->error, "[%s]:%d - Invalid sequence of arithmetic operations one after the other", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
						return 0;
					}
					else
					{
						//вставляем универсальную цифру 0
						stack_var.push(NodePool.Alloc(_numnull, curr_lexid, gc->cr_val(t_nnull, 0, 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS)));
					}
				}

				//если мы только начали анализировать выражение
				if (how_type_next == -4)
				{
					//и первым арифметическим выражением встречаем не -
					if (!(
						tmplexs->id == S4GLAO_SUB
						|| tmplexs->id == S4GLAO_BIT_NOT
						|| tmplexs->id == S4GLAO_LOG_NOT
						))	//error
					{
						status = -1;
						sprintf(this->error, "[%s]:%d - illegal inderection", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
						return 0;
					}
					//иначе
					else/* if(tmplexs->id == S4GLAO_SUB)*/
					{
						//вставляем универсальную цифру 0
						stack_var.push(NodePool.Alloc(_numnull, curr_lexid, gc->cr_val(t_nnull, 0, 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS)));
					}
				}

				//если предыдущей была открывающая скобка
				if (how_type_next == 0)
				{
					//и следующая арифметическая операция не -
					if (!(
						tmplexs->id == S4GLAO_SUB
						|| tmplexs->id == S4GLAO_BIT_NOT
						|| tmplexs->id == S4GLAO_LOG_NOT
						))	//error
					{
						status = -1;
						sprintf(this->error, "[%s]:%d - illegal inderection", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
						return 0;
					}
					//иначе
					else/* if(tmplexs->id == S4GLAO_SUB)*/
					{
						//вставляем универсальную цифру 0
						stack_var.push(NodePool.Alloc(_numnull, curr_lexid, gc->cr_val(t_nnull, 0, 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS)));
					}
				}

				tmptypeop = s4g_aop_map[tmplexs->id];

				//сообщаем что был арифметический символ
				how_type_next = -1;
			}
			//иначе если текущий символ группировки (скобок)
			else if (tmplexs->type == sym_group)
			{
				//если открывающая
				if (tmplexs->id == 0)
				{
					tmptypeop = _group_b;
					++pareth;
					how_type_next = 0;
				}
				//если сейчас закрывающая скобка
				else if (tmplexs->id == 1)
				{
					--pareth;
					//если мы только начали
					if (how_type_next == -4)
					{
						status = -1;
						sprintf(this->error, "[%s]:%d - unresolved begin for expression [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return 0;
					}
					//а предудщий символ арифметическая операция
					if (how_type_next == -1)	//error
					{
						status = -1;
						sprintf(this->error, "[%s]:%d - unresolved end for expression", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
						return 0;
					}
					tmptypeop = _group_e;
					how_type_next = 1;
					if (pareth == 0 && donable)
					{
						done = true;
					}
				}
			}

			//если определили тип будущего нода то начинаем формировать команды для стека
			if (tmptypeop != -1)
			{
				//если тип закрывающая скобка
				if (tmptypeop == _group_e)
				{
					//если в стеке операций чтото есть
					if (stack_op.count() > 0)
					{
						if (stack_op.get(stack_op.count() - 1)->type == _group_b && stack_var.count() == 0)
						{
							status = -1;
							sprintf(this->error, "[%s]:%d - unresolved  null expression ()", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
							return 0;
						}
						bool is_find_gb = false;//найдена ли открывающая скобка
						//циклом выталкиваем из стека операций все вплоть до первой открывающей скобки
						while (stack_op.count() > 0)
						{
							//если последняя операция в стеке открывающая скобка либо вызов функции
							if (stack_op.get(stack_op.count() - 1)->type == _group_b)
							{
								//выталкиваем ее из стека и завершаем цикл
								stack_op.pop(1);
								is_find_gb = true;
								break;
							}
							else
							{
								s4g_node* qwert = stack_op.get(stack_op.count() - 1);
								stack_var.push(qwert);
								stack_op.pop(1);
							}
						}

						//если не была найдена открывающая скобка то значит у нас ошибка
						if (!is_find_gb)	//error
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
				//если в стеке чтото есть либо операция открываюая скобка
				else if (stack_op.count() > 0 || tmptypeop == _group_b)
				{
					//если открывающая скобка
					if (tmptypeop == _group_b)
					{
						//просто записываем
						stack_op.push(NodePool.Alloc((s4g_type_op)tmptypeop, curr_lexid));
					}
					//иначе если приоритет текущего оператора меньше либо равен приоритеты предыдущего оператора
					else if (s4g_get_priority((s4g_type_op)tmptypeop) <= s4g_get_priority(stack_op.get(stack_op.count() - 1)->type))
					{
						//выталкиваем предыдущий оператор и помещаем в стек переменных
						s4g_node* qwert = stack_op.get(stack_op.count() - 1);
						stack_var.push(qwert/*stack_op.get(stack_op.count())*/);
						stack_op.pop(1);
						//помещаем в стек операторов текущий оператор
						stack_op.push(NodePool.Alloc((s4g_type_op)tmptypeop, curr_lexid));
					}
					//иначе ... не понятно что
					else
					{
						//помещаем
						stack_op.push(NodePool.Alloc((s4g_type_op)tmptypeop, curr_lexid));
					}
				}
				//иначе стек пуст и у нас какая-то операция
				else
					stack_op.push(NodePool.Alloc((s4g_type_op)tmptypeop, curr_lexid));
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
		else if ((tmpnode = s4g_get_term()) != 0)
		{
			bst_cond_er(this);
			//если предыдующий тип был термом то значит у нас идут 2 терма подряд
			if (how_type_next == 2)	//error
			{
				status = -1;
				sprintf(this->error, "[%s]:%d - Invalid sequence of terms to each other", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
				return 0;
			}
			//иначе у нас пре инкремент/декремент
			else if (how_type_next == -3 || how_type_next == -2)
			{
				//!!!ЕСТЬ ДУБЛИКАТ КОДА, РАЗОБРАТЬСЯ
				//надо выяснить к чему примерняем, возможно только два вариант:
				//к переменной (_var)
				//к полю таблицы (_get)
				s4g_node* tmpnode2 = tmpnode;
				if (tmpnode2->op1 && tmpnode2->op1->type == _get)
					tmpnode2 = tmpnode2->op1;
				while (tmpnode2->op2 && tmpnode2->op2->op1 && tmpnode2->op2->op1->type == _get)
				{
					tmpnode2 = tmpnode2->op2->op1;
				}
				if (tmpnode == tmpnode2)
					tmpnode2->type = (how_type_next == -3 ? _var_preincr : _var_predecr);
				else
					tmpnode2->type = (how_type_next == -3 ? _get_preincr : _get_predecr);
			}

			/*//если предыдущим символом была открывающая скобка а нынче мы считали вызов функции
			if (how_type_next == 0 && tmpnode->type == _call)
			{
				lex_get_curr(tmplexs);
				//если текущая лексема это закрывающая скобка
				if (tmplexs->type == sym_group && tmplexs->id == 1)
				{
					//если количество операндов больше нуля
					if (stack_op.count_obj > 0)
					{
						//если послдений операнд это закрывающая скобка
						s4g_node* tmptmpnode = stack_op.get(-1);
						if (tmptmpnode->type == _group_b)
						{
							tmpnode->value = (s4g_value*)1;
							lex_get_next(tmplexs);
							stack_op.pop(1);
						}
					}
				}
			}*/

			//сообщаем что у нас было число
			how_type_next = 2;
			stack_var.push(tmpnode);
		}
		//если текущая лексема относится к операции присваивания
		else if (tmplexs->type == sym_arif_assign)
		{
			//если предыдущая операция была арифметичксой то ошибка
			if (how_type_next == -1 && !(tmplexs->id == 0 || tmplexs->id == 1))
			{
				status = -1;
				sprintf(this->error, "[%s]:%d - unexpected sequence arifmetic operation and arifmetic assignation", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
				return 0;
			}

			//если предыдущая была инкрементом либо декрементом то ошибка
			//примечание: речь идет именно о неучтенном инкременте/декременте
			//++var || var++ - учтенный инкремент
			if (how_type_next == -3 || how_type_next == -2)
			{
				status = -1;
				sprintf(this->error, "[%s]:%d - expected 'number', but got '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
				return 0;
			}

			//если предыдущее значение было числом (переменной) и теперь у нас инкремент либо декремент
			if (how_type_next == 2 && (tmplexs->id == 0 || tmplexs->id == 1))
			{
				//надо выяснить к чему примерняем, возможно только два вариант:
				//к переменной (_var)
				//к полю таблицы (_get)
				s4g_node* tmpnode = stack_var.get(-1);
				s4g_node* tmpnode2 = tmpnode;
				if (tmpnode2->op1 && tmpnode2->op1->type == _get)
					tmpnode2 = tmpnode2->op1;

				//уходим внутрь до конца в нод, чтобы найти конечное звено в цепи обращений
				//короче, находим то что нужно инкрементировать/декрементировать
				while (tmpnode2->op2 && tmpnode2->op2->op1 && tmpnode2->op2->op1->type == _get)
				{
					tmpnode2 = tmpnode2->op2->op1;
				}

				//если указатели исходного нода и того который нашли одиновы, то значит мы ничего и не искали
				//а точнее это значит что операция применима к переменной
				if (tmpnode == tmpnode2)
					tmpnode2->type = (tmplexs->id == 0 ? _var_postincr : _var_postdecr);
				else //иначе операция применяется к полю таблицы
					tmpnode2->type = (tmplexs->id == 0 ? _get_postincr : _get_postdecr);

				how_type_next = 2;
			}
			else
			{
				//???
				if (tmplexs->id == 0)
					how_type_next = -3;
				else if (tmplexs->id == 1)
					how_type_next = -2;
				else
				{
					status = -1;
					sprintf(this->error, "[%s]:%d - unknown sequence of arithmetic assignation '%s'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
					return 0;
				}
			}

			lex_get_next(tmplexs);
		}
		//иначе у нас не операция и не терм
		else
		{
			//прерываем цикл
			break;
		}
		if (done)
		{
			break;
		}
	}

	//если последним символом следовала арифметическая операция
	if (how_type_next == -1)	//error
	{
		lex_get_prev0(tmplexs);
		status = -1;
		sprintf(this->error, "[%s]:%d - invalid end for expression", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
		return 0;
	}

	//если стек операций не опустошен
	if (stack_op.count() > 0)
	{
		//циклом выталкиваем все из стека операций 
		//и записываем все эти операции в переменные
		for (int i = 0; i<stack_op.count(); i++)
		{
			s4g_node* tmtm = stack_op.get((stack_op.count() - i) - 1);

			//если у нас встретилась открывающая скобка то значит мы ее не закрыли
			if (tmtm->type == _group_b)	//error
			{
				status = -1;
				sprintf(this->error, "[%s]:%d - no ')'", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
				return 0;
			}
			stack_var.push(tmtm);
		}

	}

	//если количество считанных термов больше нуля то записываем первый считанный в основной нод
	if (stack_var.count() > 0)
	{
		node = stack_var.get(0);

		tmpnode = node;
		for (int i = 1; i<stack_var.count(); i++)
		{
			//s4g_node* tmtm22= stack_var.get(i);

			tmpnode->op3 = stack_var.get(i);//заталкиваем третий нод, ибо в первом ноде может быть обращение к элементам таблицы, а во втором аргументы для вызова функции!!!
			tmpnode = tmpnode->op3;
		}
	}

	return node;
}

s4g_node* s4g_builder_syntax_tree::s4g_get_function_def_head()
{
	s4g_lexeme* lex_get_curr0(tmplexs);
	s4g_node* node = NodePool.Alloc(_arg, curr_lexid);
	s4g_node* tmpnode = node;
	s4g_node* tmpnode2 = 0;
	Array<String> arrarg;

	while (1)
	{
		lex_get_curr0(tmplexs);

		//если тип лексемы закрывающая скобка
		if (tmplexs->type == sym_group && tmplexs->id == 1)
		{
			lex_get_next(tmplexs);
			break;
		}
		//если тип лексемы не закрывающая скобка
		else if (!(tmplexs->type == sym_delimiter && tmplexs->id == 1))
		{
			//если тип лексемы пользвоательское слово
			if (tmplexs->type == word_user)
			{
				//проверяем аргумент на уникальность
				for (int i = 0; i<arrarg.size(); i++)
				{
					String str = arrarg[i];
					if (arrarg[i] == tmplexs->str)
					{
						this->status = -1;
						sprintf(this->error, "[%s]:%d - non unic argument [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return 0;
					}
				}

				//если мы еще не ушли внутрь нода
				if (node->value == 0)
				{
					tmpnode->value = gc->cr_val(t_string, tmplexs->str.c_str(), 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS);
				}
				else
				{
					tmpnode->op1 = NodePool.Alloc(_arg, curr_lexid, gc->cr_val(t_string, tmplexs->str.c_str(), 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS));
					tmpnode = tmpnode->op1;
				}

				arrarg.push_back(tmplexs->str);
				lex_get_next0(tmplexs);

				if ((tmplexs->type == sym_delimiter && tmplexs->id == 1))
				{
					lex_get_next(tmplexs);
				}
				else if (!(tmplexs->type == sym_group && tmplexs->id == 1))
				{
					this->status = -1;
					sprintf(this->error, "[%s]:%d - no separate arguments", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
					return 0;
				}
			}
			else if (tmplexs->type == marg)
			{
				tmpnode->op1 = NodePool.Alloc(_marg, curr_lexid);
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
				sprintf(this->error, "[%s]:%d - unresolved type for argument [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
				return 0;
			}
		}
		//иначе у нас какойто другой тип
		else //error
		{
			this->status = -1;
			sprintf(this->error, "[%s]:%d - unresolved definition for argument [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
			return 0;
		}
	}
	return node;
}

s4g_node* s4g_builder_syntax_tree::s4g_get_table()
{
	s4g_lexeme* lex_get_curr0(tmplexs);
	s4g_node* node = NodePool.Alloc(_create_table, curr_lexid, gc->cr_val_null(0, S4G_GC_TYPE_VAR_SYS));
	s4g_node* tmpnode = node;
	int type_last = -1;

	while (1)
	{
		lex_get_curr0(tmplexs);
		if (type_last == 1 && !(tmplexs->type == sym_delimiter && tmplexs->id == 1) && !(tmplexs->type == sym_table_create && tmplexs->id == 1))
		{
			this->status = -1;
			sprintf(this->error, "[%s]:%d - expected delimiter arg but got [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
			return 0;
		}
		//если символ оконцания таблицы }
		if (tmplexs->type == sym_table_create && tmplexs->id == 1)
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
		else if (!(tmplexs->type == sym_delimiter && tmplexs->id == 1))
		{
			s4g_node* tmpnode2 = s4g_get_expr(false);

			lex_get_curr0(tmplexs);
			bst_cond_er(this);

			if (tmpnode2->type != _set || (!tmpnode2->op1 && tmpnode2->op2 && tmpnode2->op2->type == _create_table))
			{
				if (!tmpnode2->op1 && tmpnode2->op2 && tmpnode2->op2->type == _create_table)
				{
					tmpnode2 = tmpnode2->op2;
				}
				tmpnode->op1 = NodePool.Alloc(_add_in_table, curr_lexid, (s4g_value*)0, tmpnode2, NodePool.Alloc(_empty, curr_lexid));
				tmpnode = tmpnode->op1->op2;
			}
			else
			{
				tmpnode2->type = _sett;
				tmpnode->op1 = tmpnode2;
				tmpnode->op2 = NodePool.Alloc(_empty, curr_lexid);
				tmpnode = tmpnode->op2;
			}
			type_last = 1;
		}
		else if ((tmplexs->type == sym_delimiter && tmplexs->id == 1))
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

	return node;
}

s4g_node* s4g_builder_syntax_tree::s4g_get_arg_call_func()
{
	s4g_lexeme* lex_get_curr0(tmplexs);
	s4g_node* node = NodePool.Alloc(_arg);
	s4g_node* tmpnode = node;
	s4g_node* tmpnode2 = 0;
	int type_last = -1;

	while (1)
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
		else if (!(tmplexs->type == sym_delimiter && tmplexs->id == 1))
		{
			tmpnode2 = s4g_get_expr();
			bst_cond_er(this);
			tmpnode->op1 = tmpnode2;//new s4g_node(_expr,0,tmpnode2,new s4g_node(_empty));
			tmpnode->op2 = NodePool.Alloc(_empty, curr_lexid);
			tmpnode = tmpnode->op2;
			type_last = 1;
		}
		else if ((tmplexs->type == sym_delimiter && tmplexs->id == 1))
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

s4g_node* s4g_builder_syntax_tree::s4g_get_expr(bool isfull)
{
	s4g_lexeme* lex_get_curr0(tmplexs);

	//считываем операцию
	s4g_node* node = s4g_get_op();

	lex_get_curr0(tmplexs);

	if (node == 0)
	{
		//если символ создания таблицы
		if (tmplexs->type == sym_table_create && tmplexs->id == 0)
		{
			//если у нас не полная интепретация выражений, значит таблицы создавать нельзя
			/*if (!isfull)
			{
				//генерим ошибку
				status = -1;
				sprintf(this->error, "[%s]:%d - unresolved create table", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
				return 0;
			}*/

			bst_cond_er(this);
			lex_get_next(tmplexs);
			return NodePool.Alloc(_set, curr_lexid, (s4g_value*)0, node, s4g_get_table());
		}
		else 
			return 0;
	}

	//если тип текущего нода переменная и следующая лексема присвоение
	if ((node->type == _var || node->type == _crvar) && tmplexs->type == sym_assign)
	{
		//если третий нод существует, то значит мы считали выражение
		if (node->op3)
		{
			//а это не допустимо, присваивать можно только переменным!!!
			this->status = -1;
			sprintf(this->error, "[%s]:%d - unresolved assignation", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
			return 0;
		}

		lex_get_next0(tmplexs);

		//если символ создания таблицы
		if (tmplexs->type == sym_table_create && tmplexs->id == 0)
		{
			//если у нас не полная интепретация выражений, значит таблицы создавать нельзя
			/*if (!isfull)
			{
				//генерим ошибку
				status = -1;
				sprintf(this->error, "[%s]:%d - unresolved create table", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
				return 0;
			}*/
			lex_get_next(tmplexs);
			//создаем нод и получаем содержимое таблицы
			return NodePool.Alloc(_set, curr_lexid, (s4g_value*)0, node, s4g_get_table());
		}
		//иначе если символ функции
		else if (tmplexs->type == word_key && tmplexs->id == 1)
		{
			//если у нас не полная интепретация выражений, значит функции создавать нельзя
			if (!isfull)
			{
				//генерим ошибку
				status = -1;
				sprintf(this->error, "[%s]:%d - unresolved create function", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr);
				return 0;
			}

			long cfunclexid = curr_lexid;
			//считанный нод есть имя функции
			s4g_node* name_func = node;	

			lex_get_next0(tmplexs);
			bst_cond_eof(tmplexs);

			//если лексема это открывающая скобка
			if (tmplexs->type == sym_group && tmplexs->id == 0)
				lex_get_next(tmplexs)
			else //иначе ошибка, ибо нам надо как-тосгруппировать аргументы
			{
				status = -1;
				sprintf(this->error, "[%s]:%d - expected '(' but got [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
				return 0;
			}

			//все аргументы функции
			s4g_node* head_func = s4g_get_function_def_head();
			bst_cond_er(this);
			lex_get_curr0(tmplexs);

			//возможно у нас есть данные которые мы должны брать из предыдущих пространств имен
			//данные могут быть переменными, причем которые существуют ...
			//{
			s4g_node* extern_data = 0;
			s4g_node* tmped = 0;
			s4g_stack<String> tmparrnames;
			//если текущая лексема это ключевое слово extern
			if (tmplexs->type == word_key && tmplexs->id == 4)
			{
				//тип последней лексемы
				//-1 - только начали цикл, никакая
				//0 - запятая ,
				//1 - переменная
				int type_last = -1;
				while (true)
				{
					lex_get_next0(tmplexs);
					s4g_node* tmpop = 0;
					//если предыдущий тип лексемы переменная и текущий тип не запятая (,) и не точка с запятой (;)
					if (type_last == 1 && !(tmplexs->type == sym_delimiter && tmplexs->id == 1) && !(tmplexs->type == sym_delimiter && tmplexs->id == 0))
					{
						//синтаксическая ошибка
						this->status = -1;
						sprintf(this->error, "[%s]:%d - ',' expected delimiter extern data but got [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str);
						return 0;
					}
					//если предыдущая лексема была либо запятой либо никакой и текущая лексема это переменная
					else if ((type_last == 0 || type_last == -1) && tmplexs->type == word_user)
					{
						for (int i = 0; i < tmparrnames.count_obj; ++i)
						{
							if (tmparrnames[i] == tmplexs->str)
							{
								this->status = -1;
								sprintf(this->error, "[%s]:%d - not unic variable [%s]", this->arr_lex->ArrFiles[tmplexs->fileid], tmplexs->numstr, tmplexs->str.c_str());
								return 0;
							}
						}
						tmparrnames.push(tmplexs->str);
						//создаем нод с переменной
						tmpop = NodePool.Alloc(_var, curr_lexid, gc->cr_val(t_string, tmplexs->str.c_str(), 0, S4G_GC_TYPE_VAR_SYS, S4G_GC_TYPE_DATA_SYS));
						type_last = 1;	//сообщаем о типе
						
						//если уже иницилазированы данные с подстановкой
						if (extern_data)
						{
							tmped->op1 = tmpop;
							tmped = tmped->op1;
						}
						//иначе еще не инициализирвоны данные с подстановкой
						else
						{
							//инициализируем
							extern_data = tmpop;
							tmped = extern_data;
						}
					}
					//если предыдущая лексема была пользовательским словом (переменной) и текущая лексема ,
					else if (type_last == 1 && tmplexs->type == sym_delimiter && tmplexs->id == 1)
					{
						type_last = 0;	//сообщаем об этом
					}
					//если текущая лексема ;
					else if (tmplexs->type == sym_delimiter && tmplexs->id == 0)
					{
						//берем следующую лексему и прерываем цикл
						lex_get_next0(tmplexs);
						break;
					}
				}
			}
			//}

			bst_cond_er(this);
			lex_get_curr0(tmplexs);

			//op1 - имя функции
			//op2 - тело, которое еще не записано
			//op3 - подстановочные данные из предыдущих контекстов
			s4g_node* nfunc = NodePool.Alloc(_function, cfunclexid, (s4g_value*)0, head_func, (s4g_node*)0, extern_data);
			return NodePool.Alloc(_set, cfunclexid - 1, (s4g_value*)0, name_func, nfunc);
		}
	
		//если мы здесь оказались то значит скорее всего мы считываем простое выражение
		return NodePool.Alloc(_set, curr_lexid, (s4g_value*)0, node, s4g_get_expr(isfull));
	}
	return node;
}

s4g_node* s4g_builder_syntax_tree::s4g_gen_tree()
{
	status = 0; error[0] = 0;
	s4g_node* tmpnode = 0;
	arr_lex->set_curr_num(0);
	tmpnode = s4g_gen_statement();

	return tmpnode;
}