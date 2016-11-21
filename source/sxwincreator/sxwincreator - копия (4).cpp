
//разобраться с кнопкой создания меню
//при нажатии говорит что она не нажата если обрабатывать lduttonup
//если ментить самому то все в норме
#include <SXGUIWinApi\SXGUI.h>
#include <редакторы\SXWinCreator\SXWinCreator\resource.h>
#include <SXArray.h>
#include <vector>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Comdlg32.lib")

#define SX_WINCREATOR_COUNT_ELEMENT 16

#define SX_WINCREATOR_STAT_CUR_TEXT "Статистика курсора:"
#define SX_WINCREATOR_STAT_CUR_G "desktop:\t"
#define SX_WINCREATOR_STAT_CUR_PARENT "parent:\t\t"
#define SX_WINCREATOR_STAT_CUR_CLIENT "element:\t"


#define SX_WINCREATOR_STAT_ELEM_COUNT_CREATE "Создано:\t"
#define SX_WINCREATOR_STAT_ELEM_COUNT_DELETE "Удалено:\t"
#define SX_WINCREATOR_STAT_ELEM_COUNT_CURRENT "Текущее:\t"

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
	char Name[256];
	char SysClassName[64];
	char SXClassName[64];
};

namespace SXMainWndElem
{
	HWND ArrMainWnd[4];
	//главное окно
	SXGUIBaseWnd* MainWnd = 0;
	SXGUIBaseWnd* WndLog = 0;
	SXGUIListBox* ListBoxLog = 0;
	SXGUIBaseWnd* ParamWnd = 0;
	SXGUIListBox* ListBoxAllElements = 0;

	SXGUIButtonImg* ButtonArrow;

	SXGUIStatic* StaticParamImgBLoadImg = 0;
	SXGUIButton* ButtomParamImgBLoadImg = 0;

	SXGUIStatic* StaticParamWinMenu = 0;
	SXGUICheckBox* CheckBoxParamWinMenu = 0;

	SXGUIStatic* StaticParamPosX = 0;
	SXGUIStatic* StaticParamPosY = 0;
	SXGUIStatic* StaticParamWidth = 0;
	SXGUIStatic* StaticParamHeight = 0;

	SXGUIEdit* EditParamPosX = 0;
	SXGUIEdit* EditParamPosY = 0;
	SXGUIEdit* EditParamWidth = 0;
	SXGUIEdit* EditParamHeight = 0;


	SXGUIStatic* StaticParamCaption = 0;
	SXGUIEdit* EditParamCaption = 0;


	SXGUIStatic* StaticParamColorText = 0;
	SXGUIStatic* StaticParamColorBK = 0;
	SXGUIStatic* StaticParamColorBKText = 0;
	SXGUIStatic* StaticParamTransparentText = 0;

	SXGUIEdit* EditParamColorTextR = 0;
	SXGUIEdit* EditParamColorTextG = 0;
	SXGUIEdit* EditParamColorTextB = 0;


	SXGUIEdit* EditParamColorTextBKR = 0;
	SXGUIEdit* EditParamColorTextBKG = 0;
	SXGUIEdit* EditParamColorTextBKB = 0;


	SXGUIEdit* EditParamColorBKR = 0;
	SXGUIEdit* EditParamColorBKG = 0;
	SXGUIEdit* EditParamColorBKB = 0;


	SXGUIComboBox* ComboBoxParamTransparentText = 0;


	SXGUIStatic* StaticParamFont = 0;
	SXGUIStatic* StaticParamFontParent = 0;
	SXGUIComboBox* ComboBoxParamParentFont;
	SXGUIButton* ButtonParamSelectFont;

	SXGUIStatic* StaticParamHintText = 0;
	SXGUIEdit* EditParamHintText = 0;

	SXGUIStatic* StaticParamHintVisible = 0;
	SXGUIComboBox* ComboBoxParamHintVisible = 0;


	SXGUIStatic* StaticParamVisible = 0;
	SXGUIComboBox* ComboBoxParamVisible = 0;

	SXGUIStatic* StaticParamEnabled = 0;
	SXGUIComboBox* ComboBoxParamEnabled = 0;
	//SXGUIComboBox* ComboBoxParamEnabled = 0;

	RECT* MainWndOldRect = 0;

	SXGUIToolBar* SettingsPanel = 0;
	SXGUIToolBar* ToolsPanel = 0;

	//имена  всех компонентов которые можно создать
	char* NameElements[] = 
								{
									"Button",
									"ImgButton",
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
	DWORD ArrNumResource[] = {103,121,104,105,106,107,108,109,110,111,112,113,114,115,116,117};
	//порядковый номер создаваемого элемента
	//нужно для имени
	int NumSourceElements[SX_WINCREATOR_COUNT_ELEMENT];

	//массив кнопок с отображением возможно создаваемых элементов
	SXElement Elements[SX_WINCREATOR_COUNT_ELEMENT];

	//массив всех созданных элементов
	SXArray<SXCreateElement*> CreateElements;
	//порядковый номер активного элемента, если -1 значит ничего не выделено
	int NumActiveElement = -1;

	//статики для статистики
	SXGUIStatic* StaticStatisticMouseText;
	SXGUIStatic* StaticStatisticMouseGlobal;
	SXGUIStatic* StaticStatisticMouseParent;
	SXGUIStatic* StaticStatisticMouseClient;

	SXGUIStatic* StaticStatisticElemText;
	SXGUIStatic* StaticStatisticElemCreate;
	SXGUIStatic* StaticStatisticElemDelete;
	SXGUIStatic* StaticStatisticElemCurrent;

	//количество созданных элементов
	int CountCreateNewElem = 0;
	//количество удаленных элементов
	int CountDeleteElem = 0;
	//текущее количество элементов
	int CountCurrentElem = 0;

	SXGUIMenu* MainMenu;

	SXGUIMenu* MenuWindow;

	HCURSOR CurRePos,CurSW,CurSE,CurE,CurS,  CurNS, CurWE;

	//
	SXGUIBaseWnd* JobMainWnd;

	POINT GlobalCurPos;
};

#include <sxwincreator\sx_param_wnd_data.cpp>

void InLog(const char* format, ...)
{
	va_list va;
	char * buf = new char[1024];
	va_start(va, format);
	vsprintf_s(buf, 1024, format, va);
	va_end(va);

		if(SXMainWndElem::ListBoxLog)
		{
			SXMainWndElem::ListBoxLog->AddItem(buf);
			SXMainWndElem::ListBoxLog->ScrollLine(SXGUI_VERT_SCROLL,SXGUI_DOWN_SCROLL,SXMainWndElem::ListBoxLog->GetCountItem());
		}
	SX_SAFE_DELETE_A(buf);
}

bool Render()
{
	RECT rect_main_wnd;
	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),&rect_main_wnd);

	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),SXMainWndElem::MainWndOldRect);

		//если нажата кнопка delete и есть активный элемент то удаляем его
		//и ставим порядковый номер активного элемента -1
		if(GetAsyncKeyState(VK_DELETE) && SXMainWndElem::NumActiveElement != -1)
		{
			InLog("%s%s%s","Удаление объекта [",SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Name,"] ...");
			SXMainWndElem::CountDeleteElem++;
			SXMainWndElem::ListBoxAllElements->DeleteItem(SXMainWndElem::NumActiveElement);
				for(int i=SXMainWndElem::NumActiveElement;i<SXMainWndElem::ListBoxAllElements->GetCountItem();i++)
				{
					int ud = SXMainWndElem::ListBoxAllElements->GetItemData(i);
					SXMainWndElem::ListBoxAllElements->SetItemData(i,(LPARAM)(ud-1));
				}
				if(strcmp(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SXClassName,"SXGUIBaseWnd") == 0)
					delete ((SXGUIBaseWnd*)SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object);
				else
					delete SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object;
			SXMainWndElem::CreateElements.erase(SXMainWndElem::NumActiveElement);
			SXMainWndElem::NumActiveElement = -1;
			InLog("%s","Удаление объекта завершено, порядковые номера переписаны (userdata32)");
		}

	POINT global_pos_cursor;
	GetCursorPos(&global_pos_cursor);
		//if(SXMainWndElem::GlobalCurPos.x != global_pos_cursor.x && SXMainWndElem::GlobalCurPos.y != global_pos_cursor.y)
		//{
			char CurPos[256];
			sprintf(CurPos,"%s%d%s%d",SX_WINCREATOR_STAT_CUR_G,global_pos_cursor.x," | ",global_pos_cursor.y);
			SXMainWndElem::StaticStatisticMouseGlobal->SetText(CurPos);
			MapWindowPoints(0, SXMainWndElem::JobMainWnd->GetHWND(), &global_pos_cursor, 1);
			sprintf(CurPos,"%s%d%s%d",SX_WINCREATOR_STAT_CUR_PARENT,global_pos_cursor.x," | ",global_pos_cursor.y);
			SXMainWndElem::StaticStatisticMouseParent->SetText(CurPos);
				if(SXMainWndElem::NumActiveElement != -1)
				{
					MapWindowPoints(SXMainWndElem::JobMainWnd->GetHWND(), SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetHWND(), &global_pos_cursor, 1);
					sprintf(CurPos,"%s%d%s%d",SX_WINCREATOR_STAT_CUR_CLIENT,global_pos_cursor.x," | ",global_pos_cursor.y);
					SXMainWndElem::StaticStatisticMouseClient->SetText(CurPos);
				}
				else
				{
					sprintf(CurPos,"%s%d%s%d",SX_WINCREATOR_STAT_CUR_CLIENT,-1," | ",-1);
					SXMainWndElem::StaticStatisticMouseClient->SetText(CurPos);
				}
			//SXMainWndElem::GlobalCurPos = global_pos_cursor;
		//}

	char ElemStat[256];
	sprintf(ElemStat,"%s%d",SX_WINCREATOR_STAT_ELEM_COUNT_CREATE,SXMainWndElem::CountCreateNewElem);
	SXMainWndElem::StaticStatisticElemCreate->SetText(ElemStat);

	sprintf(ElemStat,"%s%d",SX_WINCREATOR_STAT_ELEM_COUNT_DELETE,SXMainWndElem::CountDeleteElem);
	SXMainWndElem::StaticStatisticElemDelete->SetText(ElemStat);

	SXMainWndElem::CountCurrentElem = SXMainWndElem::CreateElements.size();
	sprintf(ElemStat,"%s%d",SX_WINCREATOR_STAT_ELEM_COUNT_CURRENT,SXMainWndElem::CountCurrentElem);
	SXMainWndElem::StaticStatisticElemCurrent->SetText(ElemStat);


	//находим активное окно
	HWND ActivHwnd = GetActiveWindow();
		//если хотя бы одно из наших окон активно
		if(SXMainWndElem::ArrMainWnd[0] == ActivHwnd)
		{
				//проходим еще раз по всем окнам
				for(int k=0;k<4;k++)
				{
					bool IsNext = false;
					HWND tmph = ActivHwnd;
						//если наше окно SXMainWndElem::ArrMainWnd[k] находится в пределах от 2 до 4 zпоследовательности
						for(int j=0;j<4;j++)
						{
							tmph = GetNextWindow(tmph,GW_HWNDNEXT);
								if(SXMainWndElem::ArrMainWnd[k] == tmph)
								{
									IsNext = true;
									break;
								}
						}

						if(!IsNext)
						{
								if(IsIconic(SXMainWndElem::ArrMainWnd[k]))
									ShowWindow(SXMainWndElem::ArrMainWnd[k],SW_RESTORE);
							SetActiveWindow(SXMainWndElem::ArrMainWnd[k]);
						}
				}
		}


	/*HWND ActivHwnd = GetActiveWindow();
		//если активное окно это наше главнео окно
		if(SXMainWndElem::MainWnd->GetHWND() == ActivHwnd)
		{
			HWND tmphwnd = GetNextWindow(ActivHwnd,GW_HWNDNEXT);
				//если ниже по уровню окно наше окно
				if(tmphwnd == SXMainWndElem::JobMainWnd->GetHWND())
				{
					HWND tmphwnd2 = GetNextWindow(tmphwnd,GW_HWNDNEXT);
						//если следующее окно не наше
						if(tmphwnd2 != SXMainWndElem::ParamWnd->GetHWND())
						{
							//активируем его
								if(IsIconic(SXMainWndElem::ParamWnd->GetHWND()))
									ShowWindow(SXMainWndElem::ParamWnd->GetHWND(),SW_RESTORE);
							SetActiveWindow(SXMainWndElem::ParamWnd->GetHWND());
						}
				}
				else if(tmphwnd == SXMainWndElem::ParamWnd->GetHWND())
				{
					HWND tmphwnd2 = GetNextWindow(tmphwnd,GW_HWNDNEXT);
						//если следующее окно не наше
						if(tmphwnd2 != SXMainWndElem::JobMainWnd->GetHWND())
						{
							//активируем его
								if(IsIconic(SXMainWndElem::JobMainWnd->GetHWND()))
									ShowWindow(SXMainWndElem::JobMainWnd->GetHWND(),SW_RESTORE);
							SetActiveWindow(SXMainWndElem::JobMainWnd->GetHWND());
						}
				}
				else
				{
						if(IsIconic(SXMainWndElem::JobMainWnd->GetHWND()))
							ShowWindow(SXMainWndElem::JobMainWnd->GetHWND(),SW_RESTORE);
					SetActiveWindow(SXMainWndElem::JobMainWnd->GetHWND());
						if(IsIconic(SXMainWndElem::ParamWnd->GetHWND()))
							ShowWindow(SXMainWndElem::ParamWnd->GetHWND(),SW_RESTORE);
					SetActiveWindow(SXMainWndElem::ParamWnd->GetHWND());
				}

		}*/

	return true;
};


#include <sxwincreator\callbacks.cpp>
#include <sxwincreator\create_current_gui.cpp>

int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE prevInstance,PSTR cmdLine,int showCmd)
{
	SXEngine::Hinstance = hinstance;

		for(int i=0;i<SX_WINCREATOR_COUNT_ELEMENT;i++)
			SXMainWndElem::NumSourceElements[i] = 1;

	CreateCursor();
	
	SXGUIRegClass::RegButtonImg();

	INITCOMMONCONTROLSEX icex;
    
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC  = ICC_BAR_CLASSES;
	InitCommonControlsEx(&icex);

	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);

	SXMainWndElem::MainWnd = new SXGUIBaseWnd(
							"SkyX Windows Creator","SkyX Windows Creator",0,
							0,0,0,wrect.right,150,
							0,0,CreateSolidBrush(RGB(220,220,220)),
							0,CS_HREDRAW | CS_VREDRAW,WS_DLGFRAME | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
							0,WndProcAllDefault);

	SXGUIBaseHandlers::InitHandlerMsg(SXMainWndElem::MainWnd);
	SXMainWndElem::MainWnd->AddHandler(ComMenuId,WM_COMMAND);
	SXMainWndElem::MainWnd->AddHandler(TrueExit,WM_CLOSE,0,0,0,0,true);
	
	SXMainWndElem::MainWnd->BFMinSize = true;
	SXMainWndElem::MainWnd->MinSizeX = wrect.right;
	SXMainWndElem::MainWnd->MinSizeY = 150;

	SXMainWndElem::MainWnd->BFSizingChangeLeft = true;
	SXMainWndElem::MainWnd->BFSizingChangeTop = true;

	SXMainWndElem::MainWnd->BFSizingChangeRight = true;
	SXMainWndElem::MainWnd->BFSizingChangeBottom = true;

	SXMainWndElem::MainMenu = new SXGUIMenu(IDR_MENU1);
	SXMainWndElem::MainMenu->SetToWindow(SXMainWndElem::MainWnd->GetHWND());

	SXMainWndElem::MenuWindow = new SXGUIMenu(IDR_MENU2);

	SXMainWndElem::ParamWnd = new SXGUIBaseWnd(
							"ParamWnd","ParamWnd",0,
							0,wrect.right-256,160,256,wrect.bottom-160,
							0,0,CreateSolidBrush(RGB(220,220,220)),
							WS_EX_TOOLWINDOW,CS_HREDRAW | CS_VREDRAW,WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_OVERLAPPED,
							0,WndProcAllDefault);

	SXGUIBaseHandlers::InitHandlerMsg(SXMainWndElem::ParamWnd);
	SXMainWndElem::ParamWnd->AddHandler(MinimuzeWinInsteadClose,WM_CLOSE,0,0,0,0,true);
	SXMainWndElem::ParamWnd->AddHandler(CallWmCommand,WM_COMMAND,0,0,0,0,true);

	SXMainWndElem::ListBoxAllElements = new SXGUIListBox("ListBoxAllElements",5,5,240,150,SXMainWndElem::ParamWnd->GetHWND(),0,0,false);
	SXMainWndElem::ListBoxAllElements->AddHandler(ListBoxAllElementsDBLClick,WM_LBUTTONDBLCLK);


	RECT rect_main_wnd;
	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),&rect_main_wnd);

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
	tmpX = 30;
	tmpY = 1;
	tmpAddition = 27;

	SXMainWndElem::ButtonArrow = new SXGUIButtonImg(IDB_BITMAP19,3,tmpY,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::ButtonArrow->FrameColor = SXColor(100,100,100);
	SXMainWndElem::ButtonArrow->InitCallBack();

	SXMainWndElem::ButtonArrow->GAlign.top = true;
	SXMainWndElem::ButtonArrow->GAlign.left = true;
	SXMainWndElem::ButtonArrow->GAlign.bottom = false;
	SXMainWndElem::ButtonArrow->GAlign.right = false;
			
	SXMainWndElem::ButtonArrow->EnableBf = true;
	SXMainWndElem::ButtonArrow->EnableGroup = true;

		for(int i=0;i<SX_WINCREATOR_COUNT_ELEMENT;i++)
		{
			sprintf(SXMainWndElem::Elements[i].Name,"%s",SXMainWndElem::NameElements[i]);
			SXMainWndElem::Elements[i].Object = new SXGUIButtonImg(SXMainWndElem::ArrNumResource[i],tmpX,tmpY,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
			
			SXMainWndElem::Elements[i].Object->InitCallBack();
			
			SXMainWndElem::Elements[i].Object->FrameColor = SXColor(100,100,100);
			SXMainWndElem::Elements[i].Object->GAlign.top = true;
			SXMainWndElem::Elements[i].Object->GAlign.left = true;
			SXMainWndElem::Elements[i].Object->GAlign.bottom = false;
			SXMainWndElem::Elements[i].Object->GAlign.right = false;

			SXMainWndElem::Elements[i].Object->ShowHint(true);
			SXMainWndElem::Elements[i].Object->SetHintText(SXMainWndElem::NameElements[i]);
			
			SXMainWndElem::Elements[i].Object->EnableBf = true;
			SXMainWndElem::Elements[i].Object->EnableGroup = true;
			tmpX += tmpAddition;
		}

	

	SXMainWndElem::JobMainWnd = new SXGUIBaseWnd(
							"Window 1","Window 1",0,
							0,100,250,wrect.right - 456,wrect.bottom-350,
							0,0,CreateSolidBrush(RGB(220,220,220)),
							0,CS_HREDRAW | CS_VREDRAW,WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
							0,WndProcAllDefault);
	SXGUIBaseHandlers::InitHandlerMsg(SXMainWndElem::JobMainWnd);
	SXMainWndElem::JobMainWnd->AddHandler(AddElement,WM_LBUTTONUP);
	SXMainWndElem::JobMainWnd->AddHandler(MouseMove,WM_MOUSEMOVE);
	SXMainWndElem::JobMainWnd->AddHandler(RButtonUp,WM_RBUTTONUP);
	SXMainWndElem::JobMainWnd->AddHandler(MinimuzeWinInsteadClose,WM_CLOSE,0,0,0,0,true);
	
	//SXMainWndElem::JobMainWnd->AddHandler(LButtonDown,WM_LBUTTONDOWN);
	//SXMainWndElem::JobMainWnd->AddHandler(ActivateAllWindows,WM_ACTIVATE/*,0,false,0,false,true*/);


	//установка статистики курсора
	SXMainWndElem::StaticStatisticMouseText = new SXGUIStatic("Статистика курсора:",10,73,110,15,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::StaticStatisticMouseText->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticMouseText->SetFont(0,14,0,0,0,0,0);
	SXMainWndElem::StaticStatisticMouseGlobal = new SXGUIStatic("Относительно рабочего стола:",120,58,150,15,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::StaticStatisticMouseGlobal->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticMouseGlobal->SetFont(0,14,0,0,0,0,0);
	SXMainWndElem::StaticStatisticMouseParent = new SXGUIStatic("Относительно родителя:",120,73,150,15,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::StaticStatisticMouseParent->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticMouseParent->SetFont(0,14,0,0,0,0,0);
	SXMainWndElem::StaticStatisticMouseClient = new SXGUIStatic("Относительно элемента:",120,88,150,15,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::StaticStatisticMouseClient->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticMouseClient->SetFont(0,14,0,0,0,0,0);

	//установка статистики элементов
	SXMainWndElem::StaticStatisticElemText = new SXGUIStatic("Статистика элементов:",280,73,130,15,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::StaticStatisticElemText->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticElemText->SetFont(0,14,0,0,0,0,0);
	SXMainWndElem::StaticStatisticElemCreate = new SXGUIStatic("Создано:",410,58,250,15,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::StaticStatisticElemCreate->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticElemCreate->SetFont(0,14,0,0,0,0,0);
	SXMainWndElem::StaticStatisticElemDelete = new SXGUIStatic("Удалено:",410,73,250,15,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::StaticStatisticElemDelete->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticElemDelete->SetFont(0,14,0,0,0,0,0);
	SXMainWndElem::StaticStatisticElemCurrent = new SXGUIStatic("Текущее:",410,88,250,15,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::StaticStatisticElemCurrent->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticElemCurrent->SetFont(0,14,0,0,0,0,0);


	int tmpPosX = 5;
	int tmpPosY = 180;

	//установка статиков для отображения параметров
	SXMainWndElem::StaticParamPosX = new SXGUIStatic("PosX:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamPosX->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamPosX->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamPosX = new SXGUIEdit("PosX:",90,tmpPosY,155,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::EditParamPosX->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamPosX->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamPosX->AddHandler(InputInfoEdit,WM_KEYDOWN);

	tmpPosY += 20;


	SXMainWndElem::StaticParamPosY = new SXGUIStatic("PosY:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamPosY->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamPosY->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamPosY = new SXGUIEdit("PosY:",90,tmpPosY,155,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::EditParamPosY->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamPosY->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamPosY->AddHandler(InputInfoEdit,WM_KEYDOWN);

	tmpPosY+=20;

	SXMainWndElem::StaticParamWidth = new SXGUIStatic("Width:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamWidth->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamWidth->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamWidth = new SXGUIEdit("Width:",90,tmpPosY,155,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::EditParamWidth->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamWidth->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamWidth->AddHandler(InputInfoEdit,WM_KEYDOWN);

	tmpPosY+= 20;

	SXMainWndElem::StaticParamHeight = new SXGUIStatic("Height:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamHeight->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamHeight->Font(0,15,0,0,0,0);
	
	SXMainWndElem::EditParamHeight = new SXGUIEdit("Height:",90,tmpPosY,155,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::EditParamHeight->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamHeight->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamHeight->AddHandler(InputInfoEdit,WM_KEYDOWN);

	tmpPosY+= 20;

	SXMainWndElem::StaticParamCaption = new SXGUIStatic("Caption:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamCaption->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamCaption->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamCaption = new SXGUIEdit("Caption:",90,tmpPosY,155,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::EditParamCaption->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamCaption->AddHandler(InputInfoEdit,WM_KEYDOWN);

	tmpPosY+=20;

	SXMainWndElem::StaticParamColorText = new SXGUIStatic("Color text:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamColorText->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamColorText->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamColorTextR = new SXGUIEdit("0",90,tmpPosY,45,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::EditParamColorTextR->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextR->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorTextR->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextR->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextR->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextR->AddHandler(InputToEditColor,WM_KEYUP);

	SXMainWndElem::EditParamColorTextG = new SXGUIEdit("0",145,tmpPosY,45,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::EditParamColorTextG->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextG->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorTextG->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextG->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextG->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextG->AddHandler(InputToEditColor,WM_KEYUP);

	SXMainWndElem::EditParamColorTextB = new SXGUIEdit("0",200,tmpPosY,45,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::EditParamColorTextB->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextB->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorTextB->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextB->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextB->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextB->AddHandler(InputToEditColor,WM_KEYUP);

	tmpPosY+=20;

	SXMainWndElem::StaticParamColorBKText = new SXGUIStatic("Color bk text:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamColorBKText->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamColorBKText->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamColorTextBKR = new SXGUIEdit("0",90,tmpPosY,45,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::EditParamColorTextBKR->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextBKR->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorTextBKR->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKR->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextBKR->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKR->AddHandler(InputToEditColor,WM_KEYUP);

	SXMainWndElem::EditParamColorTextBKG = new SXGUIEdit("0",145,tmpPosY,45,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::EditParamColorTextBKG->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextBKG->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorTextBKG->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKG->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextBKG->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKG->AddHandler(InputToEditColor,WM_KEYUP);

	SXMainWndElem::EditParamColorTextBKB = new SXGUIEdit("0",200,tmpPosY,45,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::EditParamColorTextBKB->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextBKB->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorTextBKB->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKB->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextBKB->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKB->AddHandler(InputToEditColor,WM_KEYUP);

	tmpPosY+=20;

	SXMainWndElem::StaticParamTransparentText = new SXGUIStatic("Alpha bk text:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamTransparentText->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamTransparentText->Font(0,15,0,0,0,0);

	SXMainWndElem::ComboBoxParamTransparentText = new SXGUIComboBox("ComboBoxParamTransparentText",90,tmpPosY-2,155,70,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::ComboBoxParamTransparentText->Font(0,15,0,0,0,0);
	SendMessage(SXMainWndElem::ComboBoxParamTransparentText->GetHWND(),CB_SETITEMHEIGHT,-1,14);
	SXMainWndElem::ComboBoxParamTransparentText->AddItem("true");
	SXMainWndElem::ComboBoxParamTransparentText->AddItem("false");
	SXMainWndElem::ComboBoxParamTransparentText->SetSel(1);
	//SXMainWndElem::ComboBoxParamTransparentText->AddHandler(InputInfoComboBox,WM_LBUTTONUP);
	SXMainWndElem::ComboBoxParamTransparentText->AddHandler(InputInfoComboBox,WM_KEYDOWN,VK_RETURN,1,0,0,0);

	tmpPosY+=20;

	SXMainWndElem::StaticParamColorBK = new SXGUIStatic("Color bk:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamColorBK->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamColorBK->Font(0,15,0,0,0,0);
	
	SXMainWndElem::EditParamColorBKR = new SXGUIEdit("0",90,tmpPosY+1,45,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::EditParamColorBKR->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorBKR->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorBKR->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKR->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorBKR->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKR->AddHandler(InputToEditColor,WM_KEYUP);

	SXMainWndElem::EditParamColorBKG = new SXGUIEdit("0",145,tmpPosY+1,45,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::EditParamColorBKG->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorBKG->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorBKG->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKG->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorBKG->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKG->AddHandler(InputToEditColor,WM_KEYUP);

	SXMainWndElem::EditParamColorBKB = new SXGUIEdit("0",200,tmpPosY+1,45,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::EditParamColorBKB->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorBKB->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorBKB->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKB->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorBKB->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKB->AddHandler(InputToEditColor,WM_KEYUP);

	tmpPosY+=20;

	SXMainWndElem::StaticParamFont = new SXGUIStatic("Font:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamFont->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamFont->Font(0,15,0,0,0,0);
	SXMainWndElem::ButtonParamSelectFont = new SXGUIButton("View font",90,tmpPosY,155,15,0,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::ButtonParamSelectFont->Font(0,13,0,0,0,0);
	SXMainWndElem::ButtonParamSelectFont->AddHandler(InitFont,WM_LBUTTONUP);

	tmpPosY+=20;

	SXMainWndElem::StaticParamFontParent = new SXGUIStatic("Regulation Font:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamFontParent->SetColorBrush(220,220,220);

	SXMainWndElem::ComboBoxParamParentFont = new SXGUIComboBox("Parent",90,tmpPosY-1,155,70,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SendMessage(SXMainWndElem::ComboBoxParamParentFont->GetHWND(),CB_SETITEMHEIGHT,-1,15);
	SXMainWndElem::ComboBoxParamParentFont->AddItem("Parent");
	SXMainWndElem::ComboBoxParamParentFont->AddItem("Default gui");
	SXMainWndElem::ComboBoxParamParentFont->SetSel(1);
	SXMainWndElem::ComboBoxParamParentFont->AddHandler(InputInfoComboBox,WM_KEYDOWN,VK_RETURN,1,0,0,0);

	tmpPosY+= 20;

	SXMainWndElem::StaticParamHintText = new SXGUIStatic("Hint text:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamHintText->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamHintText->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamHintText = new SXGUIEdit("",90,tmpPosY+2,155,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::EditParamHintText->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamHintText->AddHandler(InputInfoEdit,WM_KEYDOWN);

	tmpPosY+= 20;

	SXMainWndElem::StaticParamHintVisible = new SXGUIStatic("Hint visible:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamHintVisible->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamHintVisible->Font(0,15,0,0,0,0);

	SXMainWndElem::ComboBoxParamHintVisible = new SXGUIComboBox("ComboBoxParamHintVisible",90,tmpPosY,155,70,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::ComboBoxParamHintVisible->Font(0,15,0,0,0,0);
	SendMessage(SXMainWndElem::ComboBoxParamHintVisible->GetHWND(),CB_SETITEMHEIGHT,-1,15);
	SXMainWndElem::ComboBoxParamHintVisible->AddItem("true");
	SXMainWndElem::ComboBoxParamHintVisible->AddItem("false");
	SXMainWndElem::ComboBoxParamHintVisible->SetSel(1);
	SXMainWndElem::ComboBoxParamHintVisible->AddHandler(InputInfoComboBox,WM_KEYDOWN,VK_RETURN,1,0,0,0);

	tmpPosY+=20;

	SXMainWndElem::StaticParamVisible = new SXGUIStatic("Visible:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamVisible->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamVisible->Font(0,15,0,0,0,0);
	SXMainWndElem::ComboBoxParamVisible = new SXGUIComboBox("ComboBoxParamVisible",90,tmpPosY,155,70,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::ComboBoxParamVisible->Font(0,15,0,0,0,0);
	SendMessage(SXMainWndElem::ComboBoxParamVisible->GetHWND(),CB_SETITEMHEIGHT,-1,15);
	SXMainWndElem::ComboBoxParamVisible->AddItem("true");
	SXMainWndElem::ComboBoxParamVisible->AddItem("false");
	SXMainWndElem::ComboBoxParamVisible->SetSel(0);
	SXMainWndElem::ComboBoxParamVisible->AddHandler(InputInfoComboBox,WM_KEYDOWN,VK_RETURN,1,0,0,0);

	tmpPosY+=20;

	SXMainWndElem::StaticParamEnabled = new SXGUIStatic("Enabled:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamEnabled->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamEnabled->Font(0,15,0,0,0,0);
	SXMainWndElem::ComboBoxParamEnabled = new SXGUIComboBox("ComboBoxParamEnabled",90,tmpPosY,155,70,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	//SXMainWndElem::ComboBoxParamEnabled->Font(0,15,0,0,0,0);
	SXMainWndElem::ComboBoxParamEnabled->AddItem("true");
	SXMainWndElem::ComboBoxParamEnabled->AddItem("false");
	SXMainWndElem::ComboBoxParamEnabled->SetSel(0);
	SXMainWndElem::ComboBoxParamEnabled->AddHandler(InputInfoComboBox,WM_KEYDOWN,VK_RETURN,1,0,0,0);

	tmpPosY+=20;

	SXMainWndElem::StaticParamImgBLoadImg = new SXGUIStatic("Image:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamImgBLoadImg->SetColorBrush(220,220,220);
	SXMainWndElem::ButtomParamImgBLoadImg = new SXGUIButton("LoadImg",90,tmpPosY,155,15,0,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::ButtomParamImgBLoadImg->AddHandler(InitImageForButtonImg,WM_LBUTTONUP);

	tmpPosY+=20;

	SXMainWndElem::StaticParamWinMenu = new SXGUIStatic("Menu window:",5,tmpPosY,80,15,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticParamWinMenu->SetColorBrush(220,220,220);
	SXMainWndElem::CheckBoxParamWinMenu = new SXGUICheckBox("Enable",90,tmpPosY,155,15,SXMainWndElem::ParamWnd->GetHWND(),0,0,false);
	SXMainWndElem::CheckBoxParamWinMenu->SetColorBrush(220,220,220);
	//SXMainWndElem::CheckBoxParamWinMenu->AddHandler(InitWindowMenu,WM_LBUTTONUP);

	SXMainWndElem::WndLog = new SXGUIBaseWnd(
							"WndLog","WndLog",0,
							0,0,wrect.bottom-220,500,220,
							0,0,CreateSolidBrush(RGB(220,220,220)),
							WS_EX_TOOLWINDOW,CS_HREDRAW | CS_VREDRAW,WS_CAPTION | WS_MINIMIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_OVERLAPPED,
							0,WndProcAllDefault);
	SXMainWndElem::WndLog->AddHandler(MinimuzeWinInsteadClose,WM_CLOSE,0,0,0,0,true);
	RECT crWndLog;
	GetClientRect(SXMainWndElem::WndLog->GetHWND(),&crWndLog);
	SXMainWndElem::ListBoxLog = new SXGUIListBox("ListBoxLog",crWndLog.left,crWndLog.top,crWndLog.right,crWndLog.bottom,SXMainWndElem::WndLog->GetHWND(),0,0,false);
	//SXMainWndElem::ListBoxLog->Font(0,14,0,0,0,0);
	InLog("%s","Лог создан, запуск цикла ...");

	//SXMainWndElem::JobMainWnd->GetHWND();
	
	SXMainWndElem::ArrMainWnd[0] = SXMainWndElem::MainWnd->GetHWND();
	SXMainWndElem::ArrMainWnd[1] = SXMainWndElem::JobMainWnd->GetHWND();
	SXMainWndElem::ArrMainWnd[2] = SXMainWndElem::ParamWnd->GetHWND();
	SXMainWndElem::ArrMainWnd[3] = SXMainWndElem::WndLog->GetHWND();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

		while(GetMessage(&msg,0,0,0) != 0)
		{
			
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			Render();
				
		}

	return msg.wParam;
}