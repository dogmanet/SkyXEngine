
#pragma once

LRESULT CALLBACK WndProcChildJob(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent *Component = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);// dynamic_cast<ISXGUIComponent*>((I*)GetWindowLong(hwnd, GWL_USERDATA));
	
	//переменная для отслеживания передвижения мыши
	//когда заканчиваем следить ставим значения в -1
	static POINT LastPosCursor = {-1,-1};
	//static HCURSOR CurRePos,CurSW,CurSE,CurE,CurS;
	//2----3----4
	//|			|
	//1			5
	//|			|
	//0----7----6
	//какую областим ресайзим, картинка выше, -1 никакую
	static int WhereResize = -1;
	//какое дейсвтие выполняется, -1 - никаое, 0 - перемещение, 1 - ресайз
	static int HowCom = -1;		//смена курсора
	static int HowComDown = -1;	//само действие
		/*if(msg == WM_CTLCOLORSTATIC)
			{
					if(Component)
					{
							if(Component->TransparentBf)
								SetBkMode((HDC)wParam,TRANSPARENT);
							else
								SetBkColor((HDC)wParam, RGB(Component->ColorBk.r, Component->ColorBk.g, Component->ColorBk.b));
						SetTextColor((HDC)wParam, RGB(Component->ColorText.r, Component->ColorText.g, Component->ColorText.b));
		
						return (long)Component->HBrush;
					}
			}*/
		
		//если мышка перемещается и зажата нужная кнопка для перемещения
		if(msg == WM_MOUSEMOVE && SXMainWndElem::NumActiveElement != -1 && SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetHWND() == hwnd)
		{
			POINT global_pos_cursor;
			GetCursorPos(&global_pos_cursor);
			RECT* grect = Component->GetWinRect();
				//если не объявлено действие перемещение
				if(HowComDown != 0 && WhereResize == -1)
				{
						if(global_pos_cursor.x <= grect->left+5 && global_pos_cursor.y <= grect->top+5)
						{
							HowCom = 1;
							//CurSE = LoadCursor(0,IDC_SIZENWSE);
							SetCursor(SXMainWndElem::CurSE);
							WhereResize = 2;
						}
						else if(global_pos_cursor.x <= grect->left+5 && global_pos_cursor.y >= grect->bottom-5)
						{
							HowCom = 1;
							//CurSW = LoadCursor(0,IDC_SIZENESW);
							SetCursor(SXMainWndElem::CurSW);
							WhereResize = 0;
						}

						else if(global_pos_cursor.x >= grect->right-5 && global_pos_cursor.y <= grect->top+5)
						{
							HowCom = 1;
							//CurSW = LoadCursor(0,IDC_SIZENESW);
							SetCursor(SXMainWndElem::CurSW);
							WhereResize = 4;
											
						}
						else if(global_pos_cursor.x >= grect->right-5 && global_pos_cursor.y >= grect->bottom-5)
						{
							HowCom = 1;
							//CurSE = LoadCursor(0,IDC_SIZENWSE);
							SetCursor(SXMainWndElem::CurSE);
							WhereResize = 6;
						}

						else if((global_pos_cursor.x <= grect->right && global_pos_cursor.x >= grect->left) && (global_pos_cursor.y >= grect->top-4 && global_pos_cursor.y <= grect->top+4))
						{
							HowCom = 1;
							//CurSE = LoadCursor(0,IDC_SIZENWSE);
							SetCursor(SXMainWndElem::CurNS);
							WhereResize = 3;
						}

						else if((global_pos_cursor.x <= grect->right && global_pos_cursor.x >= grect->left) && (global_pos_cursor.y >= grect->bottom-4 && global_pos_cursor.y <= grect->bottom+4))
						{
							HowCom = 1;
							//CurSE = LoadCursor(0,IDC_SIZENWSE);
							SetCursor(SXMainWndElem::CurNS);
							WhereResize = 7;
						}

						else if((global_pos_cursor.y <= grect->bottom && global_pos_cursor.y >= grect->top) && (global_pos_cursor.x >= grect->left-4 && global_pos_cursor.x <= grect->left+4))
						{
							HowCom = 1;
							//CurSE = LoadCursor(0,IDC_SIZENWSE);
							SetCursor(SXMainWndElem::CurWE);
							WhereResize = 1;
						}

						else if((global_pos_cursor.y <= grect->bottom && global_pos_cursor.y >= grect->top) && (global_pos_cursor.x >= grect->right-4 && global_pos_cursor.x <= grect->right+4))
						{
							HowCom = 1;
							//CurSE = LoadCursor(0,IDC_SIZENWSE);
							SetCursor(SXMainWndElem::CurWE);
							WhereResize = 5;
						}
				}

				if(HowCom != 1 && HowComDown != 1)
				{
					HowCom = 0;
					//CurRePos = LoadCursor(0,IDC_SIZEALL);
					SetCursor(SXMainWndElem::CurRePos);
				}

				if(wParam == MK_RBUTTON)
				{
					//действие началось указываем это
					HowComDown = HowCom;
					SetCapture(hwnd);

						//если прошлой позиции курсора нет то даем
						if(LastPosCursor.x == -1 && LastPosCursor.y == -1)
							LastPosCursor = global_pos_cursor;
						else
						{
							RECT* rect = Component->GetClientRect();
								
								if(HowComDown == 1)
								{
									int moveX = ((global_pos_cursor.x - LastPosCursor.x));
									int moveY = ((global_pos_cursor.y - LastPosCursor.y));
									
										if(WhereResize == 0)
										{
											rect->bottom += moveY;
											rect->left += moveX;
										}
										else if(WhereResize == 1)
										{
											//rect->top += moveY;
											rect->left += moveX;
										}
										else if(WhereResize == 2)
										{
											rect->top += moveY;
											rect->left += moveX;
										}
										else if(WhereResize == 3)
										{
											rect->top += moveY;
											//rect->left += moveX;
										}
										else if(WhereResize == 4)
										{
											rect->top += moveY;
											rect->right += moveX;
										}
										else if(WhereResize == 5)
										{
											//rect->top += moveY;
											rect->right += moveX;
										}
										else if(WhereResize == 6)
										{
										
											rect->bottom += moveY;
											rect->right += moveX;
										}
										else if(WhereResize == 7)
										{
											rect->bottom += moveY;
											//rect->right += moveX;
										}

										if(rect->right < rect->left)
										{
											int r = rect->right;
											int l = rect->left;

											rect->right = l;
											rect->left = r;
										}

										if(rect->top > rect->bottom)
										{
											int t = rect->top;
											int b = rect->bottom;

											rect->top = b;
											rect->bottom = t;
										}

									
								}
								else if(HowComDown == 0)
								{
									int moveX = ((global_pos_cursor.x - LastPosCursor.x));
									int moveY = ((global_pos_cursor.y - LastPosCursor.y));

									rect->top += moveY;
									rect->bottom += moveY;
									rect->left += moveX;
									rect->right += moveX;									
								}
							bool bf = Component->SetClientRect(rect,true);
							LastPosCursor = global_pos_cursor;
							InvalidateRect(hwnd,0,1);

							SXUpdateParam::UpdateAll();
						}
					SXUpdateParam::UpdateAll();
				}
				//обнуляем если действие не началось
				else
				{
					HowCom = -1;
					HowComDown = -1;
					WhereResize = -1;
				}
			return 0;
		}
		//если отпускаем правую кнопку мыши
		//то останавливаем слежение за движением мыши
		else if(msg == WM_RBUTTONUP)
		{
			ReleaseCapture();
			LastPosCursor.x = -1;
			LastPosCursor.y = -1;
			HowCom = -1;
			HowComDown = -1;
			WhereResize = -1;
		}
		//если совершаем клик/двойной клик левой кнопкой мыши то значит активируем элемент, а остальные дизактивируем
		else if(msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK)
		{
				for(int i=0;i<SXMainWndElem::CreateElements.size();i++)
				{
						if(hwnd == SXMainWndElem::CreateElements[i]->Object->GetHWND())
						{
							SXMainWndElem::NumActiveElement = i;
							SXMainWndElem::ListBoxAllElements->SetSel(i);
							//break;
						}
					InvalidateRect(SXMainWndElem::CreateElements[i]->Object->GetHWND(),0,1);
				}
			
				if (SXMainWndElem::NumActiveElement != -1)
				{
					InLog("%s%s%s%s%s%d%s", "Activated element [", SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Name, "], sx_class [", SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SXClassName, "], serial number [", SXMainWndElem::NumActiveElement, "]");
					//обновляем все данные
					SXUpdateParam::UpdateAll();
					InLog("%s", "Parameters ParamWnd updated");
					return 1;
				}
		}
		//если нужно отрисовать
		//и есть активный элемент
		//и тот кому пришло это сообщение и есть актинвный элемент
		//то дорисовываем ему рамку
		else if(msg == WM_PAINT && SXMainWndElem::NumActiveElement != -1 && SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetHWND() == hwnd)
		{
				if(strcmp(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SXClassName,"SXGUIButtonImg") == 0)
					WndProcButtonImgDefault(hwnd, msg, wParam, lParam);
				else
					CallWindowProc(Component->OldProc,hwnd, msg, wParam, lParam);

			HDC hdcp = GetDC(hwnd);
			RECT clrect;
			GetClientRect(hwnd,&clrect);
			
			//в пределах элемента
			Rectangle(hdcp,0,0,4,4);
			Rectangle(hdcp,(clrect.right/2)-2,0,(clrect.right/2) + 2,4);
			Rectangle(hdcp,clrect.right-4,0,clrect.right,4);

			Rectangle(hdcp,clrect.right-4,(clrect.bottom/2) - 2,clrect.right,(clrect.bottom/2)+2);
			Rectangle(hdcp,clrect.right-4,clrect.bottom-4,clrect.right,clrect.bottom);

			Rectangle(hdcp,(clrect.right/2)-2,clrect.bottom-4,(clrect.right/2)+2,clrect.bottom);
			Rectangle(hdcp,0,clrect.bottom-4,4,clrect.bottom);
			Rectangle(hdcp,0,(clrect.bottom/2) - 2,4,(clrect.bottom/2)+2);

			ReleaseDC(hwnd,hdcp);
			return 1;
		}
		//иначе если поступило сообщение о рисовании (и текущий элемент неактивен либо вообще нет активных элементов)
		else if(msg == WM_PAINT)
		{
			char ClassName[256];
			int error = GetClassName(hwnd,ClassName,256);
				if(strcmp(ClassName,"SXGUIBUTTONIMG") == 0)
					WndProcButtonImgDefault(hwnd, msg, wParam, lParam);
				else
					CallWindowProc(Component->OldProc, hwnd, msg, wParam, lParam);
			return 1;
		}

	return CallWindowProc(Component->OldProc, hwnd, msg, wParam, lParam);
}

LRESULT WndProcChildJob2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return WndProcChildJob(hwnd, msg, wParam, lParam);
}

//найден ли хотя бы один groupbox или static
bool IsFindGroupBox = false;;

BOOL CALLBACK GroupBoxLButtonDown(HWND hwnd,LPARAM lParam)
{
	ISXGUIComponent *Component = (ISXGUIComponent *)GetWindowLong(hwnd,GWL_USERDATA);
	char ClassName[256];
	int error = GetClassName(hwnd,ClassName,256);
		if(error && ((strcmp(ClassName,"Button") == 0 && GetWindowLong(hwnd,GWL_STYLE) & BS_GROUPBOX) || strcmp(ClassName,"Static") == 0) && Component)
		{
			int Style;
			Style = GetWindowLong(hwnd,GWL_STYLE);
			POINT p;
			GetCursorPos(&p);
			RECT rect;
			GetWindowRect(hwnd,&rect);

				if(((p.x >= rect.left && p.x <= rect.right) && (p.y >= rect.top && p.y <= rect.bottom)))
				{
					SendMessage(Component->GetHWND(),WM_LBUTTONDOWN,0,lParam);
					IsFindGroupBox = true;
				}
		}
	return TRUE;
}

//сообщение клика левой кнопкой мыши по рабочему окну
LRESULT AddElement(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EnumChildWindows(hwnd,(WNDENUMPROC)&GroupBoxLButtonDown,lParam);
		if(!IsFindGroupBox)
		{
			bool IsCreateNewObject = false;
			WORD xPos   = LOWORD(lParam);
			WORD yPos   = HIWORD(lParam);
				for(int i=0;i<SX_WINCREATOR_COUNT_ELEMENT;i++)
				{
						if(SXMainWndElem::Elements[i].Object->EnableState)
						{
							SXCreateElement* Object = new SXCreateElement();
							Object->SysClassName[0] = 0;
							Object->SXClassName[0] = 0;
							char NameElem[256];
							sprintf(NameElem,"%s%d",SXMainWndElem::Elements[i].Name,SXMainWndElem::NumSourceElements[i]);
								if(strcmp(SXMainWndElem::Elements[i].Name,"Button") == 0)
								{
									Object->Object = SXGUICrButton(NameElem,xPos,yPos,100,20,0,hwnd,WndProcChildJob,0);
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUIButton");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"ImgButton") == 0)
								{
									ISXGUIButtonImg* tmpButtonImg = SXGUICrButtonImgEx("",xPos,yPos,24,24,RGB(0,0,0),RGB(100,100,100),hwnd,WndProcChildJob,0);
									tmpButtonImg->FrameColor = RGB(100,100,100);
									tmpButtonImg->EnableBf = true;
									tmpButtonImg->EnableState = true;

									Object->Object = tmpButtonImg;
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUIButtonImg");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"Edit") == 0)
								{
									Object->Object = SXGUICrEdit(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0);
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUIEdit");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"CheckBox") == 0)
								{
									Object->Object = SXGUICrCheckBox(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0,true);
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUICheckBox");
								}

								else if(strcmp(SXMainWndElem::Elements[i].Name,"ComboBox") == 0)
								{
									Object->Object = SXGUICrComboBox(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0);
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUIComboBox");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"GroupBox") == 0)
								{
									Object->Object = SXGUICrGroupBox(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0);
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUIGroupBox");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"ListBox") == 0)
								{
									Object->Object = SXGUICrListBox(NameElem,xPos,yPos,100,100,hwnd,WndProcChildJob,0,true);
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUIListBox");
									SendMessage(Object->Object->GetHWND(),LB_SETITEMHEIGHT,0,1);
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"Memo") == 0)
								{
									Object->Object = SXGUICrMemo(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0);
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUIMemo");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"ProgressBar") == 0)
								{
									Object->Object = SXGUICrProgressBar(xPos,yPos,100,20,hwnd,WndProcChildJob,0,true,true);
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUIProgressBar");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"RadioButton") == 0)
								{
									Object->Object = SXGUICrRadioButton(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0);
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUIRadioButton");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"Static") == 0)
								{
									Object->Object = SXGUICrStatic(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0);
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUIStatic");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"TrackBar") == 0)
								{
									Object->Object = SXGUICrTrackBar(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0);
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUITrackBar");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"StatusBar") == 0)
								{
									Object->Object = SXGUICrStatusBar(NameElem,hwnd,WndProcChildJob,0);
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUIStatusBar");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"ToolBar") == 0)
								{
									Object->Object = SXGUICrToolBar(NameElem,xPos,yPos,100,20,20,20,hwnd,WndProcChildJob,0);
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUIToolBar");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"UpDown") == 0)
								{
									Object->Object = SXGUICrUpDown(xPos,yPos,100,20,hwnd,WndProcChildJob,0,0,0);
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUIUpDown");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"Window") == 0)
								{
									Object->Object = SXGUICrBaseWnd(NameElem,NameElem,0,
																	0,xPos,yPos,200,200,
																	0,0,CreateSolidBrush(RGB(200,200,200)),
																	0,CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER /*| WS_CAPTION | WS_MAXIMIZEBOX | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME*/,
																	hwnd,WndProcChildJob);
									GetClassName(Object->Object->GetHWND(),Object->SysClassName,64);
									sprintf(Object->SXClassName,"%s","SXGUIBaseWnd");
								}


								//если нашли среди элементов то что надо
								if(Object->SysClassName[0] != 0)
								{
									sprintf(Object->Name,"%s",NameElem);
									SXMainWndElem::CountCreateNewElem++;
									//устанавливаем активным элементов вновь созданный
									SXMainWndElem::NumActiveElement = SXMainWndElem::CreateElements.size();
									SXMainWndElem::CreateElements.push_back(Object);
									//увеличиваем счетчик количества созданных элементов
									SXMainWndElem::NumSourceElements[i]++;
									InLog("%s%s%s%s%s","Created element sx_class [",Object->SXClassName,"], sys_class [",Object->SysClassName,"]");
										//заново отрисовываем все созданные элементы
										for(int e=0;e<SXMainWndElem::CreateElements.size();e++)
										{
											InvalidateRect(SXMainWndElem::CreateElements[e]->Object->GetHWND(),0,1);
										}
									IsCreateNewObject = true;
									
									//перерисовываем рабочее окно (нужно дял рамки на некоторых элементах)
									InvalidateRect(hwnd,0,TRUE);
									InLog("%s","Invalidate all elements");

									//далее добавляем новую строку в list box
									//устнавливаем в юзердату строки порядковый номер созданного элемента
									char tmpAddItem[256];
									sprintf(tmpAddItem,"%s%s%s%s%s",NameElem, " - ",Object->SXClassName," : ",Object->SysClassName);
									SXMainWndElem::ListBoxAllElements->AddItem(tmpAddItem);

									int countitem = SXMainWndElem::ListBoxAllElements->GetCountItem();
									SXMainWndElem::ListBoxAllElements->SetItemData(SXMainWndElem::ListBoxAllElements->GetCountItem()-1,(LPARAM)SXMainWndElem::CreateElements.size()-1);
									SXMainWndElem::ListBoxAllElements->SetSel(SXMainWndElem::NumActiveElement);

									//обновляем все данные
									//SXUpdateParam::PosSize();
									InLog("%s","Parameters ParamWnd overwritten, serial number of elements writed (userdata32)");
									SendMessage(SXMainWndElem::ButtonArrow->GetHWND(),WM_LBUTTONUP,0,0);

									SXMainWndElem::IsCreatedEl = true;
								}
							break;
						}
				}

				//если не был создан новый элемент то значит просто щелкнули по рабочему окну дизактивировав все элементы
				if(SXMainWndElem::NumActiveElement != -1 && !IsCreateNewObject)
				{
					int tmptmp = SXMainWndElem::NumActiveElement;
					SXMainWndElem::NumActiveElement = -1;
					InvalidateRect(SXMainWndElem::CreateElements[tmptmp]->Object->GetHWND(),0,1);
					InLog("%s","Elements of job window deactivated (AddElement)");
				}
		}
		else
			InvalidateRect(hwnd,0,TRUE);
	IsFindGroupBox = false;
	SXUpdateParam::UpdateAll();
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

BOOL CALLBACK GroupBoxMouseMove(HWND hwnd,LPARAM lParam)
{
	ISXGUIComponent *Component = (ISXGUIComponent *)GetWindowLong(hwnd,GWL_USERDATA);
	char ClassName[256];
	int error = GetClassName(hwnd,ClassName,256);
		if(error && ((strcmp(ClassName,"Button") == 0 && GetWindowLong(hwnd,GWL_STYLE) & BS_GROUPBOX) || strcmp(ClassName,"Static") == 0) && Component)
		{
			int Style;
			Style = GetWindowLong(hwnd,GWL_STYLE);
			POINT p;
			GetCursorPos(&p);
			RECT rect;
			GetWindowRect(hwnd,&rect);

				if(((p.x >= rect.left && p.x <= rect.right) && (p.y >= rect.top && p.y <= rect.bottom)))
				{
					SendMessage(Component->GetHWND(),WM_MOUSEMOVE,lParam,0);
				}
		}
	return TRUE;
}

BOOL CALLBACK GroupBoxRButtonUp(HWND hwnd,LPARAM lParam)
{
	ISXGUIComponent *Component = (ISXGUIComponent *)GetWindowLong(hwnd,GWL_USERDATA);
	char ClassName[256];
	int error = GetClassName(hwnd,ClassName,256);
		if(error && ((strcmp(ClassName,"Button") == 0 && GetWindowLong(hwnd,GWL_STYLE) & BS_GROUPBOX) || strcmp(ClassName,"Static") == 0) && Component)
		{
			int Style;
			Style = GetWindowLong(hwnd,GWL_STYLE);
			SendMessage(Component->GetHWND(),WM_RBUTTONUP,0,lParam);
		}
	return TRUE;
}



LRESULT MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SetCursor(LoadCursor(0,IDC_ARROW));
	EnumChildWindows(hwnd,(WNDENUMPROC)&GroupBoxMouseMove,wParam);

	return ::DefWindowProc(hwnd, msg, wParam, lParam);;
}

LRESULT RButtonUp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EnumChildWindows(hwnd,(WNDENUMPROC)&GroupBoxRButtonUp,lParam);

	return ::DefWindowProc(hwnd, msg, wParam, lParam);;
}

LRESULT ListBoxAllElementsDBLClick(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int tmpNumSel = SXMainWndElem::ListBoxAllElements->GetSel();
		if(tmpNumSel != -1)
		{
			int tmpNumElem = (SXMainWndElem::ListBoxAllElements->GetItemData(tmpNumSel));
			SXMainWndElem::NumActiveElement = tmpNumElem;

				for(int i=0;i<SXMainWndElem::CreateElements.size();i++)
					InvalidateRect(SXMainWndElem::CreateElements[i]->Object->GetHWND(),0,1);

			InvalidateRect(SXMainWndElem::JobMainWnd->GetHWND(),0,1);
			SXUpdateParam::UpdateAll();
		}

	return CallWindowProc(SXMainWndElem::ListBoxAllElements->OldProc,hwnd, msg, wParam, lParam);
}

LRESULT InitColorText(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CHOOSECOLOR cc;
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.hwndOwner = hwnd;
	cc.lStructSize = sizeof(CHOOSECOLOR);
	COLORREF cust_colors[16];
	cc.lpCustColors = cust_colors;
	char bufr[160],bufg[160],bufb[160];
		if(hwnd == SXMainWndElem::EditParamColorTextR->GetHWND() || hwnd == SXMainWndElem::EditParamColorTextG->GetHWND() || hwnd == SXMainWndElem::EditParamColorTextB->GetHWND())
		{
			SXMainWndElem::EditParamColorTextR->GetText(bufr,4);
			SXMainWndElem::EditParamColorTextG->GetText(bufg,4);
			SXMainWndElem::EditParamColorTextB->GetText(bufb,4);
		}
		else if(hwnd == SXMainWndElem::EditParamColorTextBKR->GetHWND() || hwnd == SXMainWndElem::EditParamColorTextBKG->GetHWND() || hwnd == SXMainWndElem::EditParamColorTextBKB->GetHWND())
		{
			SXMainWndElem::EditParamColorTextBKR->GetText(bufr,4);
			SXMainWndElem::EditParamColorTextBKG->GetText(bufg,4);
			SXMainWndElem::EditParamColorTextBKB->GetText(bufb,4);
		}
		else if(hwnd == SXMainWndElem::EditParamColorBKR->GetHWND() || hwnd == SXMainWndElem::EditParamColorBKG->GetHWND() || hwnd == SXMainWndElem::EditParamColorBKB->GetHWND())
		{
			SXMainWndElem::EditParamColorBKR->GetText(bufr,4);
			SXMainWndElem::EditParamColorBKG->GetText(bufg,4);
			SXMainWndElem::EditParamColorBKB->GetText(bufb,4);
		}
	int tmpr=0,tmpg=0,tmpb=0;
	sscanf(bufr,"%d",&tmpr);
	sscanf(bufg,"%d",&tmpg);
	sscanf(bufb,"%d",&tmpb);
	cc.rgbResult = RGB(tmpr,tmpg,tmpb);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
 
	SXMainWndElem::MainWnd->Enable(false);
	SXMainWndElem::JobMainWnd->Enable(false);
	SXMainWndElem::ParamWnd->Enable(false);
	SXMainWndElem::WndLog->Enable(false);
		if(ChooseColor(&cc)) 
		{
				if(cc.rgbResult != RGB(tmpr,tmpg,tmpb))
				{
					tmpr = GetRValue(cc.rgbResult);
					tmpg = GetGValue(cc.rgbResult);
					tmpb = GetBValue(cc.rgbResult);
					sprintf(bufr,"%d",tmpr);
					sprintf(bufg,"%d",tmpg);
					sprintf(bufb,"%d",tmpb);

						if(hwnd == SXMainWndElem::EditParamColorTextR->GetHWND() || hwnd == SXMainWndElem::EditParamColorTextG->GetHWND() || hwnd == SXMainWndElem::EditParamColorTextB->GetHWND())
						{
							SXMainWndElem::EditParamColorTextR->SetText(bufr);
							SXMainWndElem::EditParamColorTextG->SetText(bufg);
							SXMainWndElem::EditParamColorTextB->SetText(bufb);
							SXUpdateParam::SetColorText();
						}
						else if(hwnd == SXMainWndElem::EditParamColorTextBKR->GetHWND() || hwnd == SXMainWndElem::EditParamColorTextBKG->GetHWND() || hwnd == SXMainWndElem::EditParamColorTextBKB->GetHWND())
						{
							SXMainWndElem::EditParamColorTextBKR->SetText(bufr);
							SXMainWndElem::EditParamColorTextBKG->SetText(bufg);
							SXMainWndElem::EditParamColorTextBKB->SetText(bufb);
							SXUpdateParam::SetColorTextBK();
						}
						else if(hwnd == SXMainWndElem::EditParamColorBKR->GetHWND() || hwnd == SXMainWndElem::EditParamColorBKG->GetHWND() || hwnd == SXMainWndElem::EditParamColorBKB->GetHWND())
						{
							SXMainWndElem::EditParamColorBKR->SetText(bufr);
							SXMainWndElem::EditParamColorBKG->SetText(bufg);
							SXMainWndElem::EditParamColorBKB->SetText(bufb);
							SXUpdateParam::SetColorBK();
						}
				}
		}
	SXMainWndElem::MainWnd->Enable(true);
	SXMainWndElem::JobMainWnd->Enable(true);
	SXMainWndElem::ParamWnd->Enable(true);
	SXMainWndElem::WndLog->Enable(true);

	return 1;
}


LRESULT InputInfoEdit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
		if(wParam == VK_RETURN)
		{
			bool is_execute = false;
				if(hwnd == SXMainWndElem::EditParamPosX->GetHWND())
				{
					//InLog("%s","Лог создан, запуск цикла ...");
					SXUpdateParam::SetPosX();
					is_execute = true;
				}
				else if(hwnd == SXMainWndElem::EditParamPosY->GetHWND())
				{
					SXUpdateParam::SetPosY();
					is_execute = true;
				}
				else if(hwnd == SXMainWndElem::EditParamWidth->GetHWND())
				{
					SXUpdateParam::SetWidth();
					is_execute = true;
				}
				else if(hwnd == SXMainWndElem::EditParamHeight->GetHWND())
				{
					SXUpdateParam::SetHeight();
					is_execute = true;
				}
				else if(hwnd == SXMainWndElem::EditParamHintText->GetHWND())
				{
					SXUpdateParam::SetHintText();
				}
				else if(hwnd == SXMainWndElem::EditParamCaption->GetHWND())
				{
					SXUpdateParam::SetCaption();
				}
				else if(hwnd == SXMainWndElem::EditParamVarName->GetHWND())
				{
					SXUpdateParam::SetVarName();
				}

				else if(hwnd == SXMainWndElem::EditParamColorBKR->GetHWND())
				{
					SXUpdateParam::SetColorBK();
				}
				else if(hwnd == SXMainWndElem::EditParamColorBKG->GetHWND())
				{
					SXUpdateParam::SetColorBK();
				}
				else if(hwnd == SXMainWndElem::EditParamColorBKB->GetHWND())
				{
					SXUpdateParam::SetColorBK();
				}

				else if(hwnd == SXMainWndElem::EditParamColorTextR->GetHWND())
				{
					SXUpdateParam::SetColorText();
				}
				else if(hwnd == SXMainWndElem::EditParamColorTextG->GetHWND())
				{
					SXUpdateParam::SetColorText();
				}
				else if(hwnd == SXMainWndElem::EditParamColorTextB->GetHWND())
				{
					SXUpdateParam::SetColorText();
				}

				else if(hwnd == SXMainWndElem::EditParamColorTextBKR->GetHWND())
				{
					SXUpdateParam::SetColorTextBK();
				}
				else if(hwnd == SXMainWndElem::EditParamColorTextBKG->GetHWND())
				{
					SXUpdateParam::SetColorTextBK();
				}
				else if(hwnd == SXMainWndElem::EditParamColorTextBKB->GetHWND())
				{
					SXUpdateParam::SetColorTextBK();
				}

				if(is_execute)
				{
					SXUpdateParam::PosSize();
				}
		}
	return 0;
}

LRESULT InputInfoComboBox(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool is_execute = false;
		if(hwnd == SXMainWndElem::ComboBoxParamEnabled->GetHWND())
		{
			SXUpdateParam::SetEnable();
			is_execute = true;
		}
		else if(hwnd == SXMainWndElem::ComboBoxParamVisible->GetHWND())
		{
			SXUpdateParam::SetVisible();
			is_execute = true;
		}
		else if(hwnd == SXMainWndElem::ComboBoxParamHintVisible->GetHWND())
		{
			SXUpdateParam::SetVisibleHint();
			is_execute = true;
		}
		else if(hwnd == SXMainWndElem::ComboBoxParamTransparentText->GetHWND())
		{
			SXUpdateParam::SetTransparentTextBk();
			is_execute = true;
		}
		else if(hwnd == SXMainWndElem::ComboBoxParamParentFont->GetHWND())
		{
			SXUpdateParam::SetParentFont();
			is_execute = true;
		}


		if(is_execute)
		{
			SXUpdateParam::Visible();
			SXUpdateParam::Enable();
			SXUpdateParam::SetVisibleHint();
		}

	return 0;
}

LRESULT InputToEditColor(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIEdit *Component = (ISXGUIEdit *)GetWindowLong(hwnd,GWL_USERDATA);
	char buf[32];
	Component->GetText(buf,32);
	int numbuf;
	sscanf(buf,"%d",&numbuf);
		if(numbuf > 255 || strlen(buf) > 3)
			Component->SetText("255");
		else if(numbuf < 0 )
			Component->SetText("0");
	InvalidateRect(hwnd,0,1);
	return 0;
}

LRESULT InitFont(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXUpdateParam::SetFont();
		
	return 0;
}

LRESULT InitImageForButtonImg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXUpdateParam::SetImgButton();
		
	return 0;
}

LRESULT CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);
		if(Notification == BN_CLICKED)
		{
			HWND handle_elem = (HWND)(lParam);
				if(handle_elem == SXMainWndElem::CheckBoxParamWinMenu->GetHWND())
					SXUpdateParam::SetMenuWindow();
		}
			
		
	return 0;
}

void SaveResult()
{
	OPENFILENAME ofn;
	char szFileName[1024];
	szFileName[0] = szFileName[1] = 0;

	char szFileTitle[256];
	szFileTitle[0] = szFileTitle[1] = 0;

	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hInstance	= GetModuleHandle(0);
	ofn.hwndOwner	= 0;         
	ofn.lpstrFilter	= "Binary file(.bin)\0*.bin\0Все файлы\0*.*\0\0";
	ofn.lpstrFile	= szFileName;     
	ofn.nMaxFile	= sizeof(szFileName);

	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);

	ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	InLog("%s","Deactivation windows and a dialog box to save the file ...");
	SXMainWndElem::MainWnd->Enable(false);
	SXMainWndElem::JobMainWnd->Enable(false);
	SXMainWndElem::ParamWnd->Enable(false);
	SXMainWndElem::WndLog->Enable(false);
		if(GetSaveFileName(&ofn) == TRUE)
		{
				if(ofn.nFileExtension == 0 || strcmp(szFileName+ofn.nFileExtension,"bin") != 0)
					sprintf(szFileName+strlen(szFileName),"%s",".bin");
			SaveFile(szFileName);
		}
	SXMainWndElem::MainWnd->Enable(true);
	SXMainWndElem::JobMainWnd->Enable(true);
	SXMainWndElem::ParamWnd->Enable(true);
	SXMainWndElem::WndLog->Enable(true);
	SetActiveWindow(SXMainWndElem::MainWnd->GetHWND());
	InLog("%s","Call dialog box to save the file successfully completed, activation window");
}



void LoadResult()
{
	OPENFILENAME ofn;
	char szFileName[256];
	szFileName[0] = szFileName[1] = 0;

	char szFileTitle[256];
	szFileTitle[0] = szFileTitle[1] = 0;

	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hInstance	= GetModuleHandle(0);
	ofn.hwndOwner	= 0;         
	ofn.lpstrFilter	= "Binary file(.bin)\0*.bin\0Все файлы\0*.*\0\0";
	ofn.lpstrFile	= szFileName;     
	ofn.nMaxFile	= sizeof(szFileName);

	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);

	ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	InLog("%s","Deactivation windows and a dialog box to open the file ...");
	SXMainWndElem::MainWnd->Enable(false);
	SXMainWndElem::JobMainWnd->Enable(false);
	SXMainWndElem::ParamWnd->Enable(false);
	SXMainWndElem::WndLog->Enable(false);
		if(GetOpenFileName(&ofn) == TRUE)
		{
			CreateNew(false);
			LoadFile(szFileTitle);
			SXMainWndElem::IsLoadedEl = true;
		}
	SXMainWndElem::MainWnd->Enable(true);
	SXMainWndElem::JobMainWnd->Enable(true);
	SXMainWndElem::ParamWnd->Enable(true);
	SXMainWndElem::WndLog->Enable(true);
	SetActiveWindow(SXMainWndElem::MainWnd->GetHWND());
	InLog("%s","Call dialog box to open the file successfully completed, activation window");
}

void CallLoadResult()
{
		if(SXMainWndElem::IsLoadedEl || SXMainWndElem::IsCreatedEl)
		{
				if(MessageBox(0,"All the results of this project will be lost! \nAre you sure you want to open a saved project?","Warning!",MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
					LoadResult();
		}
		else
			LoadResult();
}

void CallOutputWnd()
{
	SXMainWndElem::MainWnd->Enable(false);
	SXMainWndElem::JobMainWnd->Enable(false);
	SXMainWndElem::ParamWnd->Enable(false);
	SXMainWndElem::WndLog->Enable(false);

	SXNameSapce::OutputGUIInFile->Enable(true);
	SXNameSapce::OutputGUIInFile->Visible(true);
}

void CallAboutSXWinCreator()
{	
	SXMainWndElem::MainWnd->Enable(false);
	SXMainWndElem::JobMainWnd->Enable(false);
	SXMainWndElem::ParamWnd->Enable(false);
	SXMainWndElem::WndLog->Enable(false);
	AboutSXWinCreator::JobWindow->Visible(true);
}

LRESULT ComMenuId(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	 WORD id = LOWORD(wParam);

	//раздел view
	 //{
		if(id == ID_VIEW_JOB)
		{
				if(!SXMainWndElem::MainMenu->GetCheckedItem(id))
				{
					ShowWindow(SXMainWndElem::JobMainWnd->GetHWND(),SW_SHOW);
					SetActiveWindow(SXMainWndElem::JobMainWnd->GetHWND());
					SXMainWndElem::MainMenu->CheckItem(id,true);
				}
				else
				{
					ShowWindow(SXMainWndElem::JobMainWnd->GetHWND(),SW_HIDE);
					SXMainWndElem::MainMenu->CheckItem(id,false);
				}
		}
		else if(id == ID_VIEW_PARAM)
		{
				if(!SXMainWndElem::MainMenu->GetCheckedItem(id))
				{
					ShowWindow(SXMainWndElem::ParamWnd->GetHWND(),SW_SHOW);
					SetActiveWindow(SXMainWndElem::ParamWnd->GetHWND());
				}
				else
				{
					ShowWindow(SXMainWndElem::ParamWnd->GetHWND(),SW_HIDE);
					SXMainWndElem::MainMenu->CheckItem(id,false);
				}
		}
		else if(id == ID_VIEW_LOGWINDOW)
		{
				if(!SXMainWndElem::MainMenu->GetCheckedItem(id))
				{
					ShowWindow(SXMainWndElem::WndLog->GetHWND(),SW_SHOW);
					SetActiveWindow(SXMainWndElem::WndLog->GetHWND());
				}
				else
				{
					ShowWindow(SXMainWndElem::WndLog->GetHWND(),SW_HIDE);
					SXMainWndElem::MainMenu->CheckItem(id,false);
				}
		}
	//}
		if(id == ID_FILE_NEW)
		{
			CallCreateNew();
		}
		else if(id == ID_FILE_OPEN)
		{
			CallLoadResult();
		}
		else if(id == ID_FILE_SAVE)
		{
			SaveResult();
		}
		else if(id == ID_FILE_EXIT)
		{
				if(MessageBox(0,"Are you sure you want to quit?","Warning!",MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
					PostQuitMessage(0);
		}

		else if(id == ID_BUILD_C)
		{
			CallOutputWnd();
			//OutputInFile("C:\\SkyX Engine source\\редакторы\\SXWinCreator\\Debug\\cpp.cpp");
		}

		/*else if(id == ID_HELP_HOMEPAGE)
		{
			ShellExecute(0, 0, "http://sip-game.su/", 0, 0, SW_RESTORE);
		}

		else if(id == ID_HELP_TECHNICALSUPPORT)
		{
			ShellExecute(0, 0, "http://sip-game.su/forum/viewforum.php?f=14", 0, 0, SW_RESTORE);
		}

		else if(id == ID_HELP_DOCUMENTATION)
		{
			char HelpPath[1024];
			GetModuleFileName(GetModuleHandle(0), HelpPath, 1024);
			PathRemoveFileSpec(HelpPath);
			sprintf(HelpPath+strlen(HelpPath),"%s","\\SXWinCreator.chm");
			ShellExecute(0, 0, HelpPath, 0, 0, SW_RESTORE);
		}

		else if(id == ID_HELP_ABOUTSXWINCREATOR)
		{
			CallAboutSXWinCreator();
		}*/
		
	return 0;
}

LRESULT MinimuzeWinInsteadClose(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ShowWindow(hwnd,SW_HIDE);
		
	return 0;
}


LRESULT TrueExit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
		if(MessageBox(0,"Are you sure you want to quit?","Warning!",MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
			DefWindowProc(hwnd, msg, wParam, lParam);
		
	return 0;
}

LRESULT CallImgNew(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CallCreateNew();
		
	return 0;
}

LRESULT CallImgSave(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SaveResult();
		
	return 0;
}

LRESULT CallImgLoad(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CallLoadResult();
		
	return 0;
}

LRESULT CallImgBuild(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	CallOutputWnd();
			
	return 0;
}

