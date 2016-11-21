
#include <d3d9.h>
#include <d3dx9.h>
#include <ctime>
#include <vector>
#include <core\sxconvertdata.cpp>
#include <SXGUIWinApi\SXGUI.h>
#include <редакторы\SXWinCreator\SXWinCreator\resource.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "Gdi32.lib")
//#pragma comment(lib, "Comdlg32.lib")
#pragma comment(lib, "Comctl32.lib")

#define SX_WINCREATOR_STAT_CUR_TEXT "Статистика курсора:"
#define SX_WINCREATOR_STAT_CUR_G "desktop:\t"
#define SX_WINCREATOR_STAT_CUR_PARENT "parent:  \t"
#define SX_WINCREATOR_STAT_CUR_CLIENT "element:\t"


namespace SXEngine
{
	HWND				Hwnd		= 0;
	HINSTANCE			Hinstance	= 0;
};

struct SXElement
{
	SXGUIButtonImg* Object;
	char Name[256];
};

struct SXCreateElement
{
	SXGUIComponent* Object;
	char ClassName[256];
	bool IsActive;
};

namespace SXMainWndElem
{
	//главное окно
	SXGUIBaseWnd* MainWnd;

	RECT* MainWndOldRect;

	SXGUIToolBar* SettingsPanel;
	SXGUIToolBar* ToolsPanel;

	//имена  всех компонентов которые можно создать
	char* NameElements[] = 
								{
									"Button",
									"Edit",
									"CheckBox",
									"ComboBox",
									"GroupBox",
									"ListBox",
									"Memo",
									"ProgressBar",
									"RadioButton",
									"Static",
									"TrackBar",
									"StatusBar",
									"ToolBar",
									"UpDown",
									"Window",
									"MainMenu",
									"PopupMenu"
								};
	//порядковый номер создаваемого элемента
	//нужно для имени
	int NumSourceElements[17];

	//массив кнопок с отображением возможно создаваемых элементов
	SXElement Elements[17];


	std::vector<SXCreateElement> CreateElements;
	//порядковый номер активного элемента, если -1 значит ничего не выделено
	int NumActiveElement = -1;


	SXGUIStatusBar* StatusBar;
	SXGUIButton* Button;
	SXGUIStatic* StaticStatisticMouseText;
	SXGUIStatic* StaticStatisticMouseGlobal;
	SXGUIStatic* StaticStatisticMouseParent;
	SXGUIStatic* StaticStatisticMouseClient;

	SXGUIMenu* MainMenu;

	//
	SXGUIBaseWnd* JobMainWnd;

	POINT GlobalCurPos;
};

namespace SXBoolData
{
	bool ResizeScene	= false;
};

bool Render()
{
	RECT rect_main_wnd;
	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),&rect_main_wnd);

		if(SXMainWndElem::StatusBar && rect_main_wnd.right != SXMainWndElem::MainWndOldRect->right && rect_main_wnd.bottom != SXMainWndElem::MainWndOldRect->bottom)
			SendMessage(SXMainWndElem::StatusBar->GetHWND(),WM_SIZE,0,0);
	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),SXMainWndElem::MainWndOldRect);

		//если нажата кнопка delete и есть активный элемент то удаляем его
		//и ставим порядковый номер активного элемента -1
		if(GetAsyncKeyState(VK_DELETE) && SXMainWndElem::NumActiveElement != -1)
		{
			delete SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement].Object;
			SXMainWndElem::NumActiveElement = -1;
		}

	POINT global_pos_cursor;
	GetCursorPos(&global_pos_cursor);
		//if(SXMainWndElem::GlobalCurPos.x != global_pos_cursor.x && SXMainWndElem::GlobalCurPos.y != global_pos_cursor.y)
		//{
			char CurPos[256];
			sprintf(CurPos,"&%s%d%s%d",SX_WINCREATOR_STAT_CUR_G,global_pos_cursor.x," | ",global_pos_cursor.y);
			SXMainWndElem::StaticStatisticMouseGlobal->SetText(CurPos);
			MapWindowPoints(0, SXMainWndElem::JobMainWnd->GetHWND(), &global_pos_cursor, 1);
			sprintf(CurPos,"&%s%d%s%d",SX_WINCREATOR_STAT_CUR_PARENT,global_pos_cursor.x," | ",global_pos_cursor.y);
			SXMainWndElem::StaticStatisticMouseParent->SetText(CurPos);
				if(SXMainWndElem::NumActiveElement != -1)
				{
					MapWindowPoints(SXMainWndElem::JobMainWnd->GetHWND(), SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement].Object->GetHWND(), &global_pos_cursor, 1);
					sprintf(CurPos,"&%s%d%s%d",SX_WINCREATOR_STAT_CUR_CLIENT,global_pos_cursor.x," | ",global_pos_cursor.y);
					SXMainWndElem::StaticStatisticMouseClient->SetText(CurPos);
				}
				else
				{
					sprintf(CurPos,"&%s%d%s%d",SX_WINCREATOR_STAT_CUR_CLIENT,-1," | ",-1);
					SXMainWndElem::StaticStatisticMouseClient->SetText(CurPos);
				}
			//SXMainWndElem::GlobalCurPos = global_pos_cursor;
		//}
	

	return true;
};

;

LRESULT CALLBACK WndProcChildJob(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXGUIComponent *Component = (SXGUIComponent *)GetWindowLong(hwnd,GWL_USERDATA);
	
	//переменная для отслеживания передвижения мыши
	//когда заканчиваем следить ставим значения в -1
	static POINT LastPosCursor = {-1,-1};
	static HCURSOR CurRePos,CurSW,CurSE,CurE,CurS;
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
		
		//если мышка перемещается и зажата нужная кнопка для перемещения
		if(msg == WM_MOUSEMOVE)
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
							CurSE = LoadCursor(0,IDC_SIZENWSE);
							SetCursor(CurSE);
							WhereResize = 2;
						}
						else if(global_pos_cursor.x <= grect->left+5 && global_pos_cursor.y >= grect->bottom-5)
						{
							HowCom = 1;
							CurSW = LoadCursor(0,IDC_SIZENESW);
							SetCursor(CurSW);
							WhereResize = 0;
						}

						else if(global_pos_cursor.x >= grect->right-5 && global_pos_cursor.y <= grect->top+5)
						{
							HowCom = 1;
							CurSW = LoadCursor(0,IDC_SIZENESW);
							SetCursor(CurSW);
							WhereResize = 4;
											
						}
						else if(global_pos_cursor.x >= grect->right-5 && global_pos_cursor.y >= grect->bottom-5)
						{
							HowCom = 1;
							CurSE = LoadCursor(0,IDC_SIZENWSE);
							SetCursor(CurSE);
							WhereResize = 6;
						}
				}

				if(HowCom != 1 && HowComDown != 1)
				{
					HowCom = 0;
					CurRePos = LoadCursor(0,IDC_SIZEALL);
					SetCursor(CurRePos);
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
											//rect->left += moveX;
										}
										else if(WhereResize == 2)
										{
											rect->top += moveY;
											rect->left += moveX;
										}
										else if(WhereResize == 3)
										{
											//rect->top += moveY;
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
											//rect->right += moveX;
										}
										else if(WhereResize == 6)
										{
											rect->bottom += moveY;
											rect->right += moveX;
										}
										else if(WhereResize == 7)
										{
											//rect->bottom += moveY;
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
							Component->SetClientRect(rect,true);
							LastPosCursor = global_pos_cursor;
							InvalidateRect(hwnd,0,1);
						}
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
		//если совершаем кли/двойной клик левой кнопкой мыши то значит активируем элемент, а остальные дизактивируем
		else if(msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK)
		{
				for(int i=0;i<SXMainWndElem::CreateElements.size();i++)
				{
						if(hwnd == SXMainWndElem::CreateElements[i].Object->GetHWND())
						{
							SXMainWndElem::NumActiveElement = i;
							//break;
						}
					InvalidateRect(SXMainWndElem::CreateElements[i].Object->GetHWND(),0,1);
				}
			return 1;
		}
		//если нужно отрисовать
		//и есть активный элемент
		//и тот кому пришло это сообщение и есть актинвный элемент
		//тодорисовываем ему рамку
		else if(msg == WM_PAINT && SXMainWndElem::NumActiveElement != -1 && SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement].Object->GetHWND() == hwnd)
		{
			CallWindowProc(Component->OldProc,hwnd, msg, wParam, lParam);

			HDC hdcp = GetDC(hwnd);
			RECT clrect;
			GetClientRect(hwnd,&clrect);

			//за пределами элемента
			/*
			clrect.left -= 4;
			clrect.top -= 4;
			clrect.right += 4;
			clrect.bottom +=4;

			Rectangle(hdcp,clrect.left,clrect.top,0,0);
			Rectangle(hdcp,(clrect.right/2)-2,clrect.top,(clrect.right/2) + 2,clrect.top+4);
			Rectangle(hdcp,clrect.right-4,clrect.top,clrect.right,clrect.top+4);

			Rectangle(hdcp,clrect.right-4,(clrect.bottom/2)-4,clrect.right,(clrect.bottom/2));
			Rectangle(hdcp,clrect.right-4,clrect.bottom-4,clrect.right,clrect.bottom);

			Rectangle(hdcp,(clrect.right/2)-2,clrect.bottom-4,(clrect.right/2)+2,clrect.bottom);
			Rectangle(hdcp,clrect.left,clrect.bottom-4,0,clrect.bottom);
			Rectangle(hdcp,clrect.left,(clrect.bottom/2)-4,0,(clrect.bottom/2));*/
			
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
	return CallWindowProc(Component->OldProc,hwnd, msg, wParam, lParam);
}

//найден ли хотя бы один groupbox или static
bool IsFindGroupBox = false;;

BOOL CALLBACK GroupBoxLButtonDown(HWND hwnd,LPARAM lParam)
{
	SXGUIComponent *Component = (SXGUIComponent *)GetWindowLong(hwnd,GWL_USERDATA);
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

LRESULT AddElement(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EnumChildWindows(hwnd,(WNDENUMPROC)&GroupBoxLButtonDown,lParam);
		if(!IsFindGroupBox)
		{
			bool IsCreateNewObject = false;
			WORD xPos   = LOWORD(lParam);
			WORD yPos   = HIWORD(lParam);
				for(int i=0;i<17;i++)
				{
						if(SXMainWndElem::Elements[i].Object->EnableState)
						{
							SXCreateElement Object;
							Object.ClassName[0] = 0;
							char NameElem[256];
							sprintf(NameElem,"%s%d",SXMainWndElem::Elements[i].Name,SXMainWndElem::NumSourceElements[i]);
							
								if(strcmp(SXMainWndElem::Elements[i].Name,"Button") == 0)
								{
									Object.Object = new SXGUIButton(NameElem,xPos,yPos,100,20,0,hwnd,WndProcChildJob,0);
									sprintf(Object.ClassName,"%s","BUTTON");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"Edit") == 0)
								{
									Object.Object = new SXGUIEdit(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0);
									sprintf(Object.ClassName,"%s","EDIT");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"CheckBox") == 0)
								{
									Object.Object = new SXGUICheckBox(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0,true);
									sprintf(Object.ClassName,"%s","BUTTON");
								}

								else if(strcmp(SXMainWndElem::Elements[i].Name,"ComboBox") == 0)
								{
									Object.Object = new SXGUIComboBox(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0);
									sprintf(Object.ClassName,"%s","COMBOBOX");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"GroupBox") == 0)
								{
									Object.Object = new SXGUIGroupBox(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0);
									sprintf(Object.ClassName,"%s","BUTTON");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"ListBox") == 0)
								{
									Object.Object = new SXGUIListBox(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0,true);
									sprintf(Object.ClassName,"%s","LISTBOX");
								}

								else if(strcmp(SXMainWndElem::Elements[i].Name,"Memo") == 0)
								{
									Object.Object = new SXGUIMemo(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0);
									sprintf(Object.ClassName,"%s","EDIT");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"ProgressBar") == 0)
								{
									Object.Object = new SXGUIProgressBar(xPos,yPos,100,20,hwnd,WndProcChildJob,0,true,true);
									sprintf(Object.ClassName,"%s",PROGRESS_CLASS);
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"RadioButton") == 0)
								{
									Object.Object = new SXGUIRadioButton(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0);
									sprintf(Object.ClassName,"%s","BUTTON");
								}

								else if(strcmp(SXMainWndElem::Elements[i].Name,"Static") == 0)
								{
									Object.Object = new SXGUIStatic(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0);
									sprintf(Object.ClassName,"%s","STATIC");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"TrackBar") == 0)
								{
									Object.Object = new SXGUITrackBar(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0);
									sprintf(Object.ClassName,"%s",TRACKBAR_CLASS);
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"StatusBar") == 0)
								{
									Object.Object = new SXGUIStatusBar(NameElem,xPos,yPos,100,20,hwnd,WndProcChildJob,0);
									sprintf(Object.ClassName,"%s",STATUSCLASSNAME);
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"ToolBar") == 0)
								{
									Object.Object = new SXGUIToolBar(NameElem,xPos,yPos,100,20,20,20,hwnd,WndProcChildJob,0);
									sprintf(Object.ClassName,"%s","SXGUITOOLBAR");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"UpDown") == 0)
								{
									Object.Object = new SXGUIUpDown(xPos,yPos,100,20,hwnd,WndProcChildJob,0,0,0);
									sprintf(Object.ClassName,"%s","UpDown");
								}
								else if(strcmp(SXMainWndElem::Elements[i].Name,"Window") == 0)
								{
									Object.Object = new SXGUIBaseWnd(NameElem,NameElem,0,
																	0,xPos,yPos,200,200,
																	0,0,CreateSolidBrush(RGB(220,220,220)),
																	0,CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME,
																	hwnd,0);
									sprintf(Object.ClassName,"%s",NameElem);
								}


								if(Object.ClassName[0] != 0)
								{
									SXMainWndElem::NumActiveElement = SXMainWndElem::CreateElements.size();
									SXMainWndElem::CreateElements.push_back(Object);
									SXMainWndElem::NumSourceElements[i]++;
										for(int e=0;e<SXMainWndElem::CreateElements.size();e++)
										{
											InvalidateRect(SXMainWndElem::CreateElements[e].Object->GetHWND(),0,1);
										}
									IsCreateNewObject = true;
									InvalidateRect(hwnd,0,TRUE);
								}
							break;
						}
				}

				if(SXMainWndElem::NumActiveElement != -1 && !IsCreateNewObject)
				{
					int tmptmp = SXMainWndElem::NumActiveElement;
					SXMainWndElem::NumActiveElement = -1;
					InvalidateRect(SXMainWndElem::CreateElements[tmptmp].Object->GetHWND(),0,1);
				}
		}
		else
			InvalidateRect(hwnd,0,TRUE);
	IsFindGroupBox = false;
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

BOOL CALLBACK GroupBoxMouseMove(HWND hwnd,LPARAM lParam)
{
	SXGUIComponent *Component = (SXGUIComponent *)GetWindowLong(hwnd,GWL_USERDATA);
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
	SXGUIComponent *Component = (SXGUIComponent *)GetWindowLong(hwnd,GWL_USERDATA);
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

int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE prevInstance,PSTR cmdLine,int showCmd)
{
	srand((unsigned int)time(0));
	SXEngine::Hinstance = hinstance;

		for(int i=0;i<17;i++)
			SXMainWndElem::NumSourceElements[i] = 1;
	
	SXGUIRegClass::RegButtonImg();
	SXGUIRegClass::RegToolBar();

	INITCOMMONCONTROLSEX icex;
    
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC  = ICC_BAR_CLASSES;
	InitCommonControlsEx(&icex);

	RECT* wrect = new RECT;
	SystemParametersInfo(SPI_GETWORKAREA, 0, wrect, 0);
	int WidthDeskTop = wrect->right;
	int JobHeightDeskTop = wrect->bottom;

	//int WidthDeskTop = 1600;
	//int JobHeightDeskTop = 800;

	SXMainWndElem::MainWnd = new SXGUIBaseWnd(
							"SkyX Windows Creator","SkyX Windows Creator",0,
							0,0,0,WidthDeskTop,150,
							0,0,CreateSolidBrush(RGB(220,220,220)),
							0,CS_HREDRAW | CS_VREDRAW,WS_DLGFRAME | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
							0,0);

	SXGUIBaseHandlers::InitHandlerMsg(SXMainWndElem::MainWnd);
	
	SXMainWndElem::MainWnd->BFMinSize = true;
	SXMainWndElem::MainWnd->MinSizeX = 1600;
	SXMainWndElem::MainWnd->MinSizeY = 150;

	SXMainWndElem::MainWnd->BFSizingChangeLeft = true;
	SXMainWndElem::MainWnd->BFSizingChangeTop = true;

	SXMainWndElem::MainWnd->BFSizingChangeRight = true;
	SXMainWndElem::MainWnd->BFSizingChangeBottom = true;

	SXMainWndElem::MainMenu = new SXGUIMenu(IDR_MENU1);
	SXMainWndElem::MainMenu->SetToWindow(SXMainWndElem::MainWnd->GetHWND());

	SXMainWndElem::Button = new SXGUIButton("button",1000,1000,100,32,0,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::Button->GAlign.top = false;
	SXMainWndElem::Button->GAlign.left = true;
	SXMainWndElem::Button->GAlign.bottom = true;
	SXMainWndElem::Button->GAlign.right = false;


	RECT rect_main_wnd;
	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),&rect_main_wnd);

	//SXMainWndElem::StatusBar = new SXGUIStatusBar("",0,0,0,0,SXMainWndElem::MainWnd->GetHWND(),0,0);
	/*SXMainWndElem::StatusBar->GAlign.top = false;
	SXMainWndElem::StatusBar->GAlign.left = true;
	SXMainWndElem::StatusBar->GAlign.bottom = true;
	SXMainWndElem::StatusBar->GAlign.right = true;*/

	/*int *arr = new int[5];
	arr[0] = 150; //всего моделей в сцене
	arr[1] = 300; //всего полигонов в сцене
	arr[2] = 450; //всего вершин в сцене
	arr[3] = 600; //всего подгрупп в сцене
	arr[4] = 750; //всего уникальных текстур

	SXMainWndElem::StatusBar->SetCountParts(5,arr);*/

	/*RECT rect_sb;
	GetClientRect(SXMainWndElem::StatusBar->GetHWND(),&rect_sb);*/

	SXMainWndElem::MainWndOldRect = new RECT;
	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),SXMainWndElem::MainWndOldRect);

	SXMainWndElem::SettingsPanel = new SXGUIToolBar("",0,0,SXMainWndElem::MainWndOldRect->right,24,24,24,SXMainWndElem::MainWnd->GetHWND(),0,123);
	SXMainWndElem::SettingsPanel->GAlign.top = true;
	SXMainWndElem::SettingsPanel->GAlign.left = true;
	SXMainWndElem::SettingsPanel->GAlign.bottom = false;
	SXMainWndElem::SettingsPanel->GAlign.right = true;

	SXMainWndElem::ToolsPanel = new SXGUIToolBar("",0,25,SXMainWndElem::MainWndOldRect->right,31,24,24,SXMainWndElem::MainWnd->GetHWND(),0,123);
	SXMainWndElem::ToolsPanel->GAlign.top = true;
	SXMainWndElem::ToolsPanel->GAlign.left = true;
	SXMainWndElem::ToolsPanel->GAlign.bottom = false;
	SXMainWndElem::ToolsPanel->GAlign.right = true;

	int tmpX,tmpY,tmpAddition;
	tmpX = 3;
	tmpY = 1;
	tmpAddition = 27;
		for(int i=0;i<17;i++)
		{
			sprintf(SXMainWndElem::Elements[i].Name,"%s",SXMainWndElem::NameElements[i]);
			SXMainWndElem::Elements[i].Object = new SXGUIButtonImg(103+i,tmpX,tmpY,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
			SXMainWndElem::Elements[i].Object->FrameColor = SXColor(100,100,100);
			SXMainWndElem::Elements[i].Object->GAlign.top = true;
			SXMainWndElem::Elements[i].Object->GAlign.left = true;
			SXMainWndElem::Elements[i].Object->GAlign.bottom = false;
			SXMainWndElem::Elements[i].Object->GAlign.right = false;

			SXMainWndElem::Elements[i].Object->ShowHint(true);
			SXMainWndElem::Elements[i].Object->HintText(SXMainWndElem::NameElements[i]);
			
			SXMainWndElem::Elements[i].Object->EnableBf = true;
			SXMainWndElem::Elements[i].Object->EnableGroup = true;
			tmpX += tmpAddition;
		}

	

	SXMainWndElem::JobMainWnd = new SXGUIBaseWnd(
							"Window 1","Window 1",0,
							0,100,250,WidthDeskTop - 456,JobHeightDeskTop-350,
							0,0,CreateSolidBrush(RGB(220,220,220)),
							0,CS_HREDRAW | CS_VREDRAW,WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
							0,0);
	SXMainWndElem::JobMainWnd->AddHandler(AddElement,WM_LBUTTONUP);
	SXMainWndElem::JobMainWnd->AddHandler(MouseMove,WM_MOUSEMOVE);
	SXMainWndElem::JobMainWnd->AddHandler(RButtonUp,WM_RBUTTONUP);
	//SXMainWndElem::JobMainWnd->AddHandler(LButtonDown,WM_LBUTTONDOWN);

	SXMainWndElem::StaticStatisticMouseText = new SXGUIStatic("Статистика курсора:",10,73,110,15,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::StaticStatisticMouseText->Font(0,14,0,0,0,0);
	SXMainWndElem::StaticStatisticMouseGlobal = new SXGUIStatic("Относительно рабочего стола:",120,58,250,15,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::StaticStatisticMouseGlobal->Font(0,14,0,0,0,0);
	SXMainWndElem::StaticStatisticMouseParent = new SXGUIStatic("Относительно родителя:",120,73,250,15,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::StaticStatisticMouseParent->Font(0,14,0,0,0,0);
	SXMainWndElem::StaticStatisticMouseClient = new SXGUIStatic("Относительно элемента:",120,88,250,15,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::StaticStatisticMouseClient->Font(0,14,0,0,0,0);

	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

		while(msg.message != WM_QUIT)
		{
				if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
						if(msg.message == WM_MOUSEMOVE)
							Render();
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
		}

	return msg.wParam;
}