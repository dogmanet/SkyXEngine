
#define SX_EXE

//разобраться с кнопкой создания меню
//при нажатии говорит что она не нажата если обрабатывать lduttonup
//если ментить самому то все в норме
#include <windows.h>
//#include <core\\sxcore.cpp>
//#include <fstream>
#include <gdefines.h>


#if defined(_DEBUG)
#pragma comment(lib, "sxcore_d.lib")
#else
#pragma comment(lib, "sxcore.lib")
#endif
#include <core\\sxcore.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxguiwinapi_d.lib")
#else
#pragma comment(lib, "sxguiwinapi.lib")
#endif
#include <SXGUIWinApi\sxguielements.h>

#include <core\\Array.h>
#include <D:\project\engine\SkyXEngine\SXWinCreator\resource.h>


#include <Shlwapi.h>
#include <Shellapi.h>
#pragma comment(lib, "Shlwapi.lib")

#pragma comment(lib, "Comctl32.lib")


#define SX_SAFE_DELETE_A
#define SX_SAFE_DELETE

#define SX_WINCREATOR_COUNT_ELEMENT 16

#define SX_WINCREATOR_STAT_CUR_TEXT "Statistic for cursor:"
#define SX_WINCREATOR_STAT_CUR_G "desktop:\t"
#define SX_WINCREATOR_STAT_CUR_PARENT "parent:\t\t"
#define SX_WINCREATOR_STAT_CUR_CLIENT "element:\t"

#define SX_WINCREATOR_STAT_ALL_ELEM_TEXT "Statistic for all elements:"
#define SX_WINCREATOR_STAT_ALL_ELEM_COUNT_CREATE "Create:\t"
#define SX_WINCREATOR_STAT_ALL_ELEM_COUNT_DELETE "Delete:\t"
#define SX_WINCREATOR_STAT_ALL_ELEM_COUNT_CURRENT "Current:\t"

#define SXWINCREATORVERSION "0.0.9.0"

namespace SXEngine
{
	HWND				Hwnd		= 0;
	HINSTANCE			Hinstance	= 0;
};

struct SXElement
{
	ISXGUIButtonImg* Object;
	char Name[256];
};

struct SXCreateElement
{
	SXCreateElement(){ Object = 0; Name[0] = SysClassName[0] = SXClassName[0] = 0; }
	ISXGUIComponent* Object;
	char Name[256];
	char SysClassName[64];
	char SXClassName[64];
};

namespace SXMainWndElem
{
	HWND ArrMainWnd[4];
	//главное окно
	ISXGUIBaseWnd* MainWnd = 0;
	ISXGUIBaseWnd* WndLog = 0;
	ISXGUIListBox* ListBoxLog = 0;
	ISXGUIBaseWnd* ParamWnd = 0;
	ISXGUIListBox* ListBoxAllElements = 0;

	ISXGUIButtonImg* ButtonArrow;

	ISXGUIStatic* StaticParamImgBLoadImg = 0;
	ISXGUIButton* ButtomParamImgBLoadImg = 0;

	ISXGUIStatic* StaticParamWinMenu = 0;
	ISXGUICheckBox* CheckBoxParamWinMenu = 0;

	ISXGUIStatic* StaticParamPosX = 0;
	ISXGUIStatic* StaticParamPosY = 0;
	ISXGUIStatic* StaticParamWidth = 0;
	ISXGUIStatic* StaticParamHeight = 0;

	ISXGUIEdit* EditParamPosX = 0;
	ISXGUIEdit* EditParamPosY = 0;
	ISXGUIEdit* EditParamWidth = 0;
	ISXGUIEdit* EditParamHeight = 0;


	ISXGUIStatic* StaticParamCaption = 0;
	ISXGUIEdit* EditParamCaption = 0;

	ISXGUIStatic* StaticParamVarName = 0;
	ISXGUIEdit* EditParamVarName = 0;


	ISXGUIStatic* StaticParamColorText = 0;
	ISXGUIStatic* StaticParamColorBK = 0;
	ISXGUIStatic* StaticParamColorBKText = 0;
	ISXGUIStatic* StaticParamTransparentText = 0;

	ISXGUIEdit* EditParamColorTextR = 0;
	ISXGUIEdit* EditParamColorTextG = 0;
	ISXGUIEdit* EditParamColorTextB = 0;


	ISXGUIEdit* EditParamColorTextBKR = 0;
	ISXGUIEdit* EditParamColorTextBKG = 0;
	ISXGUIEdit* EditParamColorTextBKB = 0;


	ISXGUIEdit* EditParamColorBKR = 0;
	ISXGUIEdit* EditParamColorBKG = 0;
	ISXGUIEdit* EditParamColorBKB = 0;


	ISXGUIComboBox* ComboBoxParamTransparentText = 0;


	ISXGUIStatic* StaticParamFont = 0;
	ISXGUIStatic* StaticParamFontParent = 0;
	ISXGUIComboBox* ComboBoxParamParentFont;
	ISXGUIButton* ButtonParamSelectFont;

	ISXGUIStatic* StaticParamHintText = 0;
	ISXGUIEdit* EditParamHintText = 0;

	ISXGUIStatic* StaticParamHintVisible = 0;
	ISXGUIComboBox* ComboBoxParamHintVisible = 0;


	ISXGUIStatic* StaticParamVisible = 0;
	ISXGUIComboBox* ComboBoxParamVisible = 0;

	ISXGUIStatic* StaticParamEnabled = 0;
	ISXGUIComboBox* ComboBoxParamEnabled = 0;
	//SXGUIComboBox* ComboBoxParamEnabled = 0;

	RECT* MainWndOldRect = 0;

	ISXGUIToolBar* SettingsPanel = 0;
	ISXGUIToolBar* ToolsPanel = 0;

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
	DWORD ArrNumResource[] = { 
		IDB_BITMAP6, 
		IDB_BITMAP7, 
		IDB_BITMAP10, 
		IDB_BITMAP8, 
		IDB_BITMAP9, 
		IDB_BITMAP11, 
		IDB_BITMAP13, 
		IDB_BITMAP15,
		IDB_BITMAP17,
		IDB_BITMAP18,
		IDB_BITMAP19,
		IDB_BITMAP22,
		IDB_BITMAP20,
		IDB_BITMAP21,
		IDB_BITMAP23,
		IDB_BITMAP24 };
	//порядковый номер создаваемого элемента
	//нужно для имени
	int NumSourceElements[SX_WINCREATOR_COUNT_ELEMENT];
	Array<SXCreateElement*> CreateElements;
	//массив кнопок с отображением возможно создаваемых элементов
	SXElement Elements[SX_WINCREATOR_COUNT_ELEMENT];

	//массив всех созданных элементов
	//Array<SXElement*> tmparr;
	//Array<SXCreateElement*> CreateElements;
	
	//порядковый номер активного элемента, если -1 значит ничего не выделено
	int NumActiveElement = -1;

	//статики для статистики
	ISXGUIStatic* StaticStatisticMouseText;
	ISXGUIStatic* StaticStatisticMouseGlobal;
	ISXGUIStatic* StaticStatisticMouseParent;
	ISXGUIStatic* StaticStatisticMouseClient;

	ISXGUIStatic* StaticStatisticElemText;
	ISXGUIStatic* StaticStatisticElemCreate;
	ISXGUIStatic* StaticStatisticElemDelete;
	ISXGUIStatic* StaticStatisticElemCurrent;

	//количество созданных элементов
	int CountCreateNewElem = 0;
	//количество удаленных элементов
	int CountDeleteElem = 0;
	//текущее количество элементов
	int CountCurrentElem = 0;

	ISXGUIMenu* MainMenu;

	ISXGUIMenu* MenuWindow;

	HCURSOR CurRePos,CurSW,CurSE,CurE,CurS,  CurNS, CurWE;

	//
	ISXGUIBaseWnd* JobMainWnd;
	char NameJobWnd[256];

	POINT GlobalCurPos;

	//чтобы не выскакивали надоедающие уведомления
	//создано ли хоть чтото или это пустой проект
	bool IsCreatedEl;
	//загружался хотя бы один проект
	bool IsLoadedEl;
};

#include <sxwincreator\about_sxwincreator.cpp>
#include <sxwincreator\windowoutput.cpp>
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
			InLog("%s%s%s","Deletion of the elements [",SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Name,"] ...");
			SXMainWndElem::CountDeleteElem++;
			SXMainWndElem::ListBoxAllElements->DeleteItem(SXMainWndElem::NumActiveElement);
				for(int i=SXMainWndElem::NumActiveElement;i<SXMainWndElem::ListBoxAllElements->GetCountItem();i++)
				{
					int ud = SXMainWndElem::ListBoxAllElements->GetItemData(i);
					SXMainWndElem::ListBoxAllElements->SetItemData(i,(LPARAM)(ud-1));
				}
				if (strcmp(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SXClassName, "SXGUIBaseWnd") == 0)
				{
					ISXGUIBaseWnd* tmpwin = dynamic_cast<ISXGUIBaseWnd*>(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object);
					mem_release_del(tmpwin);
				}
				else
					mem_release_del(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object);
			SXMainWndElem::CreateElements.erase(SXMainWndElem::NumActiveElement);
			SXMainWndElem::NumActiveElement = -1;
			InLog("%s","Deletiing an element is completed, serial number overwritten (userdata32)");

				if(SXMainWndElem::CreateElements.size() == 0)
					SXMainWndElem::IsCreatedEl = SXMainWndElem::IsLoadedEl = false;
		}

	POINT global_pos_cursor;
	GetCursorPos(&global_pos_cursor);
	
		//if(SXMainWndElem::GlobalCurPos.x != global_pos_cursor.x && SXMainWndElem::GlobalCurPos.y != global_pos_cursor.y)
		//{
			char CurPos[256];
			//SXMainWndElem::StaticStatisticMouseGlobal->GetText(CurPos,256);
			sprintf(CurPos,"%s%d%s%d",SX_WINCREATOR_STAT_CUR_G,global_pos_cursor.x," | ",global_pos_cursor.y);
			//InLog("%s",CurPos);
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
	sprintf(ElemStat,"%s%d",SX_WINCREATOR_STAT_ALL_ELEM_COUNT_CREATE,SXMainWndElem::CountCreateNewElem);
	SXMainWndElem::StaticStatisticElemCreate->SetText(ElemStat);

	sprintf(ElemStat,"%s%d",SX_WINCREATOR_STAT_ALL_ELEM_COUNT_DELETE,SXMainWndElem::CountDeleteElem);
	SXMainWndElem::StaticStatisticElemDelete->SetText(ElemStat);

	SXMainWndElem::CountCurrentElem = SXMainWndElem::CreateElements.size();
	sprintf(ElemStat,"%s%d",SX_WINCREATOR_STAT_ALL_ELEM_COUNT_CURRENT,SXMainWndElem::CountCurrentElem);
	SXMainWndElem::StaticStatisticElemCurrent->SetText(ElemStat);


	//вывод всех окон на передний план
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

	return true;
};


#include <sxwincreator\callbacks.cpp>
#include <sxwincreator\create_current_gui.cpp>

int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE prevInstance,PSTR cmdLine,int showCmd)
{
	SXEngine::Hinstance = hinstance;
	SXMainWndElem::IsCreatedEl = false;
	SXMainWndElem::IsLoadedEl = false;
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

	SXMainWndElem::MainWnd = SXGUICrBaseWnd(
							"SkyX Windows Creator","SkyX Windows Creator",0,
							0,
							wrect.left, wrect.top, wrect.right, 150,
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

	SXMainWndElem::MainMenu = SXGUICrMenuEx(IDR_MENU1);
	SXMainWndElem::MainMenu->SetToWindow(SXMainWndElem::MainWnd->GetHWND());

	SXMainWndElem::MenuWindow = SXGUICrMenuEx(IDR_MENU2);

	SXMainWndElem::ParamWnd = SXGUICrBaseWnd(
							"ParamWnd","ParamWnd",0,
							0, wrect.right - 256, wrect.top+160, 256, wrect.bottom - 160,
							0,0,CreateSolidBrush(RGB(220,220,220)),
							WS_EX_TOOLWINDOW,CS_HREDRAW | CS_VREDRAW,WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_OVERLAPPED,
							0,WndProcAllDefault);

	SXGUIBaseHandlers::InitHandlerMsg(SXMainWndElem::ParamWnd);
	SXMainWndElem::ParamWnd->AddHandler(MinimuzeWinInsteadClose,WM_CLOSE,0,0,0,0,true);
	SXMainWndElem::ParamWnd->AddHandler(CallWmCommand,WM_COMMAND,0,0,0,0,true);

	SXMainWndElem::ListBoxAllElements = SXGUICrListBox("ListBoxAllElements", 5, 5, 240, 150, SXMainWndElem::ParamWnd->GetHWND(), 0, 0, false);
	SXMainWndElem::ListBoxAllElements->AddHandler(ListBoxAllElementsDBLClick,WM_LBUTTONDBLCLK);


	RECT rect_main_wnd;
	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),&rect_main_wnd);

	SXMainWndElem::MainWndOldRect = new RECT;
	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),SXMainWndElem::MainWndOldRect);

	SXMainWndElem::SettingsPanel = SXGUICrToolBar("", 0, 0, SXMainWndElem::MainWndOldRect->right, 26, 24, 24, SXMainWndElem::MainWnd->GetHWND(), 0, 123);
	SXMainWndElem::SettingsPanel->GAlign.top = true;
	SXMainWndElem::SettingsPanel->GAlign.left = true;
	SXMainWndElem::SettingsPanel->GAlign.bottom = false;
	SXMainWndElem::SettingsPanel->GAlign.right = true;

	ISXGUIButtonImg* ImgNew = SXGUICrButtonImg(IDB_BITMAP1, 3, 1, 20, 20, RGB(255, 0, 110), RGB(220, 220, 220), SXMainWndElem::SettingsPanel->GetHWND(), 0, 0);
	ImgNew->InitCallBack();
	ImgNew->AddHandler(CallImgNew,WM_LBUTTONUP);
	//ImgNew->ShowHint(true);
	//ImgNew->SetHintText(SXMainWndElem::NameElements[i]);

	ISXGUIButtonImg* ImgLoad = SXGUICrButtonImg(IDB_BITMAP2, 30, 1, 20, 20, RGB(255, 0, 110), RGB(220, 220, 220), SXMainWndElem::SettingsPanel->GetHWND(), 0, 0);
	ImgLoad->InitCallBack();
	ImgLoad->AddHandler(CallImgLoad,WM_LBUTTONUP);

	//ImgNew->ShowHint(true);
	//ImgNew->SetHintText(SXMainWndElem::NameElements[i]);

	ISXGUIButtonImg* ImgSave = SXGUICrButtonImg(IDB_BITMAP4, 57, 1, 20, 20, RGB(255, 0, 110), RGB(220, 220, 220), SXMainWndElem::SettingsPanel->GetHWND(), 0, 0);
	ImgSave->InitCallBack();
	ImgSave->AddHandler(CallImgSave,WM_LBUTTONUP);

	//ImgNew->ShowHint(true);
	//ImgNew->SetHintText(SXMainWndElem::NameElements[i]);

	ISXGUIButtonImg* ImgBuild = SXGUICrButtonImg(IDB_BITMAP3, 84, 1, 20, 20, RGB(255, 0, 110), RGB(220, 220, 220), SXMainWndElem::SettingsPanel->GetHWND(), 0, 0);
	ImgBuild->InitCallBack();
	ImgBuild->AddHandler(CallImgBuild,WM_LBUTTONUP);

	//ImgNew->ShowHint(true);
	//ImgNew->SetHintText(SXMainWndElem::NameElements[i]);
	

	SXMainWndElem::ToolsPanel = SXGUICrToolBar("", 0, 25, SXMainWndElem::MainWndOldRect->right, 31, 24, 24, SXMainWndElem::MainWnd->GetHWND(), 0, 123);
	SXMainWndElem::ToolsPanel->GAlign.top = true;
	SXMainWndElem::ToolsPanel->GAlign.left = true;
	SXMainWndElem::ToolsPanel->GAlign.bottom = false;
	SXMainWndElem::ToolsPanel->GAlign.right = true;

	int tmpX,tmpY,tmpAddition;
	tmpX = 30;
	tmpY = 1;
	tmpAddition = 27;

	SXMainWndElem::ButtonArrow = SXGUICrButtonImg(IDB_BITMAP5, 3, tmpY, 24, 24, RGB(0, 0, 0), RGB(0, 0, 0), SXMainWndElem::ToolsPanel->GetHWND(), 0, 0);
	SXMainWndElem::ButtonArrow->FrameColor = RGB(100,100,100);
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
			SXMainWndElem::Elements[i].Object = SXGUICrButtonImg(SXMainWndElem::ArrNumResource[i], tmpX, tmpY, 24, 24, RGB(0, 0, 0), RGB(0, 0, 0), SXMainWndElem::ToolsPanel->GetHWND(), 0, 0);
			
			SXMainWndElem::Elements[i].Object->InitCallBack();
			
			SXMainWndElem::Elements[i].Object->FrameColor = RGB(100,100,100);
			SXMainWndElem::Elements[i].Object->GAlign.top = true;
			SXMainWndElem::Elements[i].Object->GAlign.left = true;
			SXMainWndElem::Elements[i].Object->GAlign.bottom = false;
			SXMainWndElem::Elements[i].Object->GAlign.right = false;

			ISXGUIButtonImg* tmptmp = dynamic_cast<ISXGUIButtonImg*>(SXMainWndElem::Elements[i].Object);
			tmptmp->GAlign.top;

			SXMainWndElem::Elements[i].Object->ShowHint(true);
			SXMainWndElem::Elements[i].Object->SetHintText(SXMainWndElem::NameElements[i]);
			
			SXMainWndElem::Elements[i].Object->EnableBf = true;
			SXMainWndElem::Elements[i].Object->EnableGroup = true;
			tmpX += tmpAddition;
		}

	
	sprintf(SXMainWndElem::NameJobWnd,"%s","JobWindow");
	SXMainWndElem::JobMainWnd = SXGUICrBaseWnd(
							SXMainWndElem::NameJobWnd,SXMainWndElem::NameJobWnd,0,
							0,100,250,wrect.right - 456,wrect.bottom-350,
							0,0,CreateSolidBrush(RGB(220,220,220)),
							0,CS_HREDRAW | CS_VREDRAW,WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
							0,WndProcAllDefault);
	SXGUIBaseHandlers::InitHandlerMsg(SXMainWndElem::JobMainWnd);
	SXMainWndElem::JobMainWnd->AddHandler(AddElement,WM_LBUTTONUP);
	SXMainWndElem::JobMainWnd->AddHandler(MouseMove,WM_MOUSEMOVE);
	SXMainWndElem::JobMainWnd->AddHandler(RButtonUp,WM_RBUTTONUP);
	SXMainWndElem::JobMainWnd->AddHandler(MinimuzeWinInsteadClose,WM_CLOSE,0,0,0,0,true);

	//ISXGUIButton* tmpbutton = SXGUICrButton("NameElem", 100, 100, 100, 20, 0, SXMainWndElem::JobMainWnd->GetHWND(), WndProcChildJob, 0);
	//int cerr = SetWindowLong(tmpbutton->GetHWND(), GWL_USERDATA, (LONG)dynamic_cast<ISXGUIButton*>(tmpbutton));
	//SXMainWndElem::JobMainWnd->AddHandler(LButtonDown,WM_LBUTTONDOWN);
	//SXMainWndElem::JobMainWnd->AddHandler(ActivateAllWindows,WM_ACTIVATE/*,0,false,0,false,true*/);


	//установка статистики курсора
	SXMainWndElem::StaticStatisticMouseText = SXGUICrStatic(SX_WINCREATOR_STAT_CUR_TEXT, 10, 73, 110, 15, SXMainWndElem::MainWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticStatisticMouseText->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticMouseText->SetFont(0,14,0,0,0,0,0);
	SXMainWndElem::StaticStatisticMouseGlobal = SXGUICrStatic("", 120, 58, 150, 15, SXMainWndElem::MainWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticStatisticMouseGlobal->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticMouseGlobal->SetFont(0,14,0,0,0,0,0);
	SXMainWndElem::StaticStatisticMouseParent = SXGUICrStatic("", 120, 73, 150, 15, SXMainWndElem::MainWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticStatisticMouseParent->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticMouseParent->SetFont(0,14,0,0,0,0,0);
	SXMainWndElem::StaticStatisticMouseClient = SXGUICrStatic("", 120, 88, 150, 15, SXMainWndElem::MainWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticStatisticMouseClient->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticMouseClient->SetFont(0,14,0,0,0,0,0);

	//установка статистики элементов
	SXMainWndElem::StaticStatisticElemText = SXGUICrStatic(SX_WINCREATOR_STAT_ALL_ELEM_TEXT, 280, 73, 130, 15, SXMainWndElem::MainWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticStatisticElemText->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticElemText->SetFont(0,14,0,0,0,0,0);
	SXMainWndElem::StaticStatisticElemCreate = SXGUICrStatic("", 410, 58, 250, 15, SXMainWndElem::MainWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticStatisticElemCreate->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticElemCreate->SetFont(0,14,0,0,0,0,0);
	SXMainWndElem::StaticStatisticElemDelete = SXGUICrStatic("", 410, 73, 250, 15, SXMainWndElem::MainWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticStatisticElemDelete->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticElemDelete->SetFont(0,14,0,0,0,0,0);
	SXMainWndElem::StaticStatisticElemCurrent = SXGUICrStatic("", 410, 88, 250, 15, SXMainWndElem::MainWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticStatisticElemCurrent->SetColorBrush(220,220,220);
	SXMainWndElem::StaticStatisticElemCurrent->SetFont(0,14,0,0,0,0,0);


	int tmpPosX = 5;
	int tmpPosY = 160;

	//установка статиков для отображения параметров
	SXMainWndElem::StaticParamPosX = SXGUICrStatic("PosX:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamPosX->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamPosX->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamPosX = SXGUICrEdit("PosX:", 90, tmpPosY, 155, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamPosX->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamPosX->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamPosX->AddHandler(InputInfoEdit,WM_KEYDOWN);

	tmpPosY += 20;


	SXMainWndElem::StaticParamPosY = SXGUICrStatic("PosY:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamPosY->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamPosY->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamPosY = SXGUICrEdit("PosY:", 90, tmpPosY, 155, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamPosY->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamPosY->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamPosY->AddHandler(InputInfoEdit,WM_KEYDOWN);

	tmpPosY+=20;

	SXMainWndElem::StaticParamWidth = SXGUICrStatic("Width:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamWidth->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamWidth->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamWidth = SXGUICrEdit("Width:", 90, tmpPosY, 155, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamWidth->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamWidth->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamWidth->AddHandler(InputInfoEdit,WM_KEYDOWN);

	tmpPosY+= 20;

	SXMainWndElem::StaticParamHeight = SXGUICrStatic("Height:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamHeight->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamHeight->Font(0,15,0,0,0,0);
	
	SXMainWndElem::EditParamHeight = SXGUICrEdit("Height:", 90, tmpPosY, 155, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamHeight->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamHeight->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamHeight->AddHandler(InputInfoEdit,WM_KEYDOWN);

	tmpPosY+= 20;

	SXMainWndElem::StaticParamCaption = SXGUICrStatic("Caption:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamCaption->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamCaption->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamCaption = SXGUICrEdit("Caption:", 90, tmpPosY, 155, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamCaption->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamCaption->AddHandler(InputInfoEdit,WM_KEYDOWN);

	tmpPosY+= 20;

	SXMainWndElem::StaticParamVarName = SXGUICrStatic("Variable name:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamVarName->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamCaption->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamVarName = SXGUICrEdit("name", 90, tmpPosY, 155, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamCaption->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamVarName->AddHandler(InputInfoEdit,WM_KEYDOWN);

	tmpPosY+=20;

	SXMainWndElem::StaticParamColorText = SXGUICrStatic("Color text:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamColorText->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamColorText->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamColorTextR = SXGUICrEdit("0", 90, tmpPosY, 45, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamColorTextR->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextR->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorTextR->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextR->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextR->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextR->AddHandler(InputToEditColor,WM_KEYUP);

	SXMainWndElem::EditParamColorTextG = SXGUICrEdit("0", 145, tmpPosY, 45, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamColorTextG->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextG->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorTextG->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextG->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextG->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextG->AddHandler(InputToEditColor,WM_KEYUP);

	SXMainWndElem::EditParamColorTextB = SXGUICrEdit("0", 200, tmpPosY, 45, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamColorTextB->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextB->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorTextB->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextB->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextB->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextB->AddHandler(InputToEditColor,WM_KEYUP);

	tmpPosY+=20;

	SXMainWndElem::StaticParamColorBKText = SXGUICrStatic("Color bk text:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamColorBKText->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamColorBKText->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamColorTextBKR = SXGUICrEdit("0", 90, tmpPosY, 45, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamColorTextBKR->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextBKR->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorTextBKR->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKR->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextBKR->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKR->AddHandler(InputToEditColor,WM_KEYUP);

	SXMainWndElem::EditParamColorTextBKG = SXGUICrEdit("0", 145, tmpPosY, 45, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamColorTextBKG->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextBKG->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorTextBKG->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKG->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextBKG->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKG->AddHandler(InputToEditColor,WM_KEYUP);

	SXMainWndElem::EditParamColorTextBKB = SXGUICrEdit("0", 200, tmpPosY, 45, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamColorTextBKB->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextBKB->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorTextBKB->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKB->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextBKB->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKB->AddHandler(InputToEditColor,WM_KEYUP);

	tmpPosY+=20;

	SXMainWndElem::StaticParamTransparentText = SXGUICrStatic("Alpha bk text:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamTransparentText->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamTransparentText->Font(0,15,0,0,0,0);

	SXMainWndElem::ComboBoxParamTransparentText = SXGUICrComboBox("ComboBoxParamTransparentText", 90, tmpPosY - 2, 155, 70, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::ComboBoxParamTransparentText->Font(0,15,0,0,0,0);
	SendMessage(SXMainWndElem::ComboBoxParamTransparentText->GetHWND(),CB_SETITEMHEIGHT,-1,14);
	SXMainWndElem::ComboBoxParamTransparentText->AddItem("true");
	SXMainWndElem::ComboBoxParamTransparentText->AddItem("false");
	SXMainWndElem::ComboBoxParamTransparentText->SetSel(1);
	//SXMainWndElem::ComboBoxParamTransparentText->AddHandler(InputInfoComboBox,WM_LBUTTONUP);
	SXMainWndElem::ComboBoxParamTransparentText->AddHandler(InputInfoComboBox,WM_KEYDOWN,VK_RETURN,1,0,0,0);

	tmpPosY+=20;

	SXMainWndElem::StaticParamColorBK = SXGUICrStatic("Color bk:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamColorBK->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamColorBK->Font(0,15,0,0,0,0);
	
	SXMainWndElem::EditParamColorBKR = SXGUICrEdit("0", 90, tmpPosY + 1, 45, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamColorBKR->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorBKR->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorBKR->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKR->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorBKR->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKR->AddHandler(InputToEditColor,WM_KEYUP);

	SXMainWndElem::EditParamColorBKG = SXGUICrEdit("0", 145, tmpPosY + 1, 45, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamColorBKG->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorBKG->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorBKG->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKG->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorBKG->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKG->AddHandler(InputToEditColor,WM_KEYUP);

	SXMainWndElem::EditParamColorBKB = SXGUICrEdit("0", 200, tmpPosY + 1, 45, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamColorBKB->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorBKB->ModifyStyle(ES_NUMBER,0);
	SXMainWndElem::EditParamColorBKB->AddHandler(InputInfoEdit,WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKB->AddHandler(InitColorText,WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorBKB->AddHandler(InputToEditColor,WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKB->AddHandler(InputToEditColor,WM_KEYUP);

	tmpPosY+=20;

	SXMainWndElem::StaticParamFont = SXGUICrStatic("Font:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamFont->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamFont->Font(0,15,0,0,0,0);
	SXMainWndElem::ButtonParamSelectFont = SXGUICrButton("View font", 90, tmpPosY, 155, 15, 0, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::ButtonParamSelectFont->Font(0,13,0,0,0,0);
	SXMainWndElem::ButtonParamSelectFont->AddHandler(InitFont,WM_LBUTTONUP);

	tmpPosY+=20;

	SXMainWndElem::StaticParamFontParent = SXGUICrStatic("Regulation Font:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamFontParent->SetColorBrush(220,220,220);

	SXMainWndElem::ComboBoxParamParentFont = SXGUICrComboBox("Parent", 90, tmpPosY - 1, 155, 70, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SendMessage(SXMainWndElem::ComboBoxParamParentFont->GetHWND(),CB_SETITEMHEIGHT,-1,15);
	SXMainWndElem::ComboBoxParamParentFont->AddItem("Parent");
	SXMainWndElem::ComboBoxParamParentFont->AddItem("Default gui");
	SXMainWndElem::ComboBoxParamParentFont->AddItem("User selected");
	SXMainWndElem::ComboBoxParamParentFont->SetSel(1);
	SXMainWndElem::ComboBoxParamParentFont->AddHandler(InputInfoComboBox,WM_KEYDOWN,VK_RETURN,1,0,0,0);

	tmpPosY+= 20;

	SXMainWndElem::StaticParamHintText = SXGUICrStatic("Hint text:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamHintText->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamHintText->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamHintText = SXGUICrEdit("", 90, tmpPosY + 2, 155, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::EditParamHintText->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamHintText->AddHandler(InputInfoEdit,WM_KEYDOWN);

	tmpPosY+= 20;

	SXMainWndElem::StaticParamHintVisible = SXGUICrStatic("Hint visible:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamHintVisible->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamHintVisible->Font(0,15,0,0,0,0);

	SXMainWndElem::ComboBoxParamHintVisible = SXGUICrComboBox("ComboBoxParamHintVisible", 90, tmpPosY, 155, 70, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::ComboBoxParamHintVisible->Font(0,15,0,0,0,0);
	SendMessage(SXMainWndElem::ComboBoxParamHintVisible->GetHWND(),CB_SETITEMHEIGHT,-1,15);
	SXMainWndElem::ComboBoxParamHintVisible->AddItem("true");
	SXMainWndElem::ComboBoxParamHintVisible->AddItem("false");
	SXMainWndElem::ComboBoxParamHintVisible->SetSel(1);
	SXMainWndElem::ComboBoxParamHintVisible->AddHandler(InputInfoComboBox,WM_KEYDOWN,VK_RETURN,1,0,0,0);

	tmpPosY+=20;

	SXMainWndElem::StaticParamVisible = SXGUICrStatic("Visible:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamVisible->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamVisible->Font(0,15,0,0,0,0);
	SXMainWndElem::ComboBoxParamVisible = SXGUICrComboBox("ComboBoxParamVisible", 90, tmpPosY, 155, 70, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::ComboBoxParamVisible->Font(0,15,0,0,0,0);
	SendMessage(SXMainWndElem::ComboBoxParamVisible->GetHWND(),CB_SETITEMHEIGHT,-1,15);
	SXMainWndElem::ComboBoxParamVisible->AddItem("true");
	SXMainWndElem::ComboBoxParamVisible->AddItem("false");
	SXMainWndElem::ComboBoxParamVisible->SetSel(0);
	SXMainWndElem::ComboBoxParamVisible->AddHandler(InputInfoComboBox,WM_KEYDOWN,VK_RETURN,1,0,0,0);

	tmpPosY+=20;

	SXMainWndElem::StaticParamEnabled = SXGUICrStatic("Enabled:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamEnabled->SetColorBrush(220,220,220);
	//SXMainWndElem::StaticParamEnabled->Font(0,15,0,0,0,0);
	SXMainWndElem::ComboBoxParamEnabled = SXGUICrComboBox("ComboBoxParamEnabled", 90, tmpPosY, 155, 70, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	//SXMainWndElem::ComboBoxParamEnabled->Font(0,15,0,0,0,0);
	SXMainWndElem::ComboBoxParamEnabled->AddItem("true");
	SXMainWndElem::ComboBoxParamEnabled->AddItem("false");
	SXMainWndElem::ComboBoxParamEnabled->SetSel(0);
	SXMainWndElem::ComboBoxParamEnabled->AddHandler(InputInfoComboBox,WM_KEYDOWN,VK_RETURN,1,0,0,0);

	tmpPosY+=20;

	SXMainWndElem::StaticParamImgBLoadImg = SXGUICrStatic("Image:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamImgBLoadImg->SetColorBrush(220,220,220);
	SXMainWndElem::ButtomParamImgBLoadImg = SXGUICrButton("LoadImg", 90, tmpPosY, 155, 15, 0, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::ButtomParamImgBLoadImg->AddHandler(InitImageForButtonImg,WM_LBUTTONUP);

	tmpPosY+=20;

	SXMainWndElem::StaticParamWinMenu = SXGUICrStatic("Menu window:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0);
	SXMainWndElem::StaticParamWinMenu->SetColorBrush(220,220,220);
	SXMainWndElem::CheckBoxParamWinMenu = SXGUICrCheckBox("Enable", 90, tmpPosY, 155, 15, SXMainWndElem::ParamWnd->GetHWND(), 0, 0, false);
	SXMainWndElem::CheckBoxParamWinMenu->SetColorBrush(220,220,220);
	//SXMainWndElem::CheckBoxParamWinMenu->AddHandler(InitWindowMenu,WM_LBUTTONUP);

	SXMainWndElem::WndLog = SXGUICrBaseWnd(
							"WndLog","WndLog",0,
							0,0,wrect.bottom-220,500,220,
							0,0,CreateSolidBrush(RGB(220,220,220)),
							WS_EX_TOOLWINDOW,CS_HREDRAW | CS_VREDRAW,WS_CAPTION | WS_MINIMIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_OVERLAPPED,
							0,WndProcAllDefault);
	SXMainWndElem::WndLog->AddHandler(MinimuzeWinInsteadClose,WM_CLOSE,0,0,0,0,true);
	RECT crWndLog;
	GetClientRect(SXMainWndElem::WndLog->GetHWND(),&crWndLog);
	SXMainWndElem::ListBoxLog = SXGUICrListBox("ListBoxLog", crWndLog.left, crWndLog.top, crWndLog.right, crWndLog.bottom, SXMainWndElem::WndLog->GetHWND(), 0, 0, false);
	//SXMainWndElem::ListBoxLog->Font(0,14,0,0,0,0);
	InLog("%s","Log created, run cycle ...");

	//SXMainWndElem::JobMainWnd->GetHWND();
	
	SXMainWndElem::ArrMainWnd[0] = SXMainWndElem::MainWnd->GetHWND();
	SXMainWndElem::ArrMainWnd[1] = SXMainWndElem::JobMainWnd->GetHWND();
	SXMainWndElem::ArrMainWnd[2] = SXMainWndElem::ParamWnd->GetHWND();
	SXMainWndElem::ArrMainWnd[3] = SXMainWndElem::WndLog->GetHWND();

	SXNameSapce::InitAllElements();
	SXNameSapce::OutputGUIInFile->Visible(false);
	AboutSXWinCreator::InitAllElements();
	AboutSXWinCreator::JobWindow->Visible(false);

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