
#ifndef __SXWINCREATOR_H
#define __SXWINCREATOR_H

//разобраться с кнопкой создания меню
//при нажатии говорит что она не нажата если обрабатывать lduttonup
//если ментить самому то все в норме

#include <windows.h>
#include <gdefines.h>

#include <Shlwapi.h>
#include <Shellapi.h>
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Comctl32.lib")

#if defined(_DEBUG)
#pragma comment(lib, "sxcore_d.lib")
#else
#pragma comment(lib, "sxcore.lib")
#endif
#include <core/sxcore.h>


#if defined(_DEBUG)
#pragma comment(lib, "sxguiwinapi_d.lib")
#else
#pragma comment(lib, "sxguiwinapi.lib")
#endif
#include <SXGUIWinApi/sxgui.h>

#include <common/Array.h>
#include <SXWinCreator/resource.h>

#include <sxwincreator\about_sxwincreator.h>
#include <sxwincreator\windowoutput.h>
#include <sxwincreator\sx_param_wnd_data.h>
#include <sxwincreator\callbacks.h>


#define SX_WINCREATOR_COUNT_ELEMENT 17

#define SX_WINCREATOR_STAT_CUR_TEXT "Statistic for cursor:"
#define SX_WINCREATOR_STAT_CUR_G "desktop:\t"
#define SX_WINCREATOR_STAT_CUR_PARENT "parent:\t\t"
#define SX_WINCREATOR_STAT_CUR_CLIENT "element:\t"

#define SX_WINCREATOR_STAT_ALL_ELEM_TEXT "Statistic for all elements:"
#define SX_WINCREATOR_STAT_ALL_ELEM_COUNT_CREATE "Create:\t"
#define SX_WINCREATOR_STAT_ALL_ELEM_COUNT_DELETE "Delete:\t"
#define SX_WINCREATOR_STAT_ALL_ELEM_COUNT_CURRENT "Current:\t"

#define SXWINCREATORVERSION "0.9.0"

namespace SXEngine
{
	extern HWND Hwnd;
	extern HINSTANCE Hinstance;
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
	extern HWND ArrMainWnd[4];
	//главное окно
	extern ISXGUIBaseWnd* MainWnd;
	extern ISXGUIBaseWnd* WndLog;
	extern ISXGUIListBox* ListBoxLog;
	extern ISXGUIBaseWnd* ParamWnd;
	extern ISXGUIListBox* ListBoxAllElements;

	extern ISXGUIButtonImg* ButtonArrow;

	extern ISXGUIStatic* StaticParamImgBLoadImg;
	extern ISXGUIButton* ButtomParamImgBLoadImg;

	extern ISXGUIStatic* StaticParamWinMenu;
	extern ISXGUICheckBox* CheckBoxParamWinMenu;

	extern ISXGUIStatic* StaticParamPosX;
	extern ISXGUIStatic* StaticParamPosY;
	extern ISXGUIStatic* StaticParamWidth;
	extern ISXGUIStatic* StaticParamHeight;

	extern ISXGUIEdit* EditParamPosX;
	extern ISXGUIEdit* EditParamPosY;
	extern ISXGUIEdit* EditParamWidth;
	extern ISXGUIEdit* EditParamHeight;


	extern ISXGUIStatic* StaticParamCaption;
	extern ISXGUIEdit* EditParamCaption;

	extern ISXGUIStatic* StaticParamVarName;
	extern ISXGUIEdit* EditParamVarName;


	extern ISXGUIStatic* StaticParamColorText;
	extern ISXGUIStatic* StaticParamColorBK;
	extern ISXGUIStatic* StaticParamColorBKText;
	extern ISXGUIStatic* StaticParamTransparentText;

	extern ISXGUIEdit* EditParamColorTextR;
	extern ISXGUIEdit* EditParamColorTextG;
	extern ISXGUIEdit* EditParamColorTextB;


	extern ISXGUIEdit* EditParamColorTextBKR;
	extern ISXGUIEdit* EditParamColorTextBKG;
	extern ISXGUIEdit* EditParamColorTextBKB;


	extern ISXGUIEdit* EditParamColorBKR;
	extern ISXGUIEdit* EditParamColorBKG;
	extern ISXGUIEdit* EditParamColorBKB;


	extern ISXGUIComboBox* ComboBoxParamTransparentText;


	extern ISXGUIStatic* StaticParamFont;
	extern ISXGUIStatic* StaticParamFontParent;
	extern ISXGUIComboBox* ComboBoxParamParentFont;
	extern ISXGUIButton* ButtonParamSelectFont;

	extern ISXGUIStatic* StaticParamHintText;
	extern ISXGUIEdit* EditParamHintText;

	extern ISXGUIStatic* StaticParamHintVisible;
	extern ISXGUIComboBox* ComboBoxParamHintVisible;


	extern ISXGUIStatic* StaticParamVisible;
	extern ISXGUIComboBox* ComboBoxParamVisible;

	extern ISXGUIStatic* StaticParamEnabled;
	extern ISXGUIComboBox* ComboBoxParamEnabled;
	//SXGUIComboBox* ComboBoxParamEnabled = 0;

	extern RECT* MainWndOldRect;

	extern ISXGUIToolBar* SettingsPanel;
	extern ISXGUIToolBar* ToolsPanel;

	//имена  всех компонентов которые можно создать
	extern char* NameElements[];
	extern DWORD ArrNumResource[];
	//порядковый номер создаваемого элемента
	//нужно для имени
	extern int NumSourceElements[SX_WINCREATOR_COUNT_ELEMENT];
	extern Array<SXCreateElement*> CreateElements;
	//массив кнопок с отображением возможно создаваемых элементов
	extern SXElement Elements[SX_WINCREATOR_COUNT_ELEMENT];

	//массив всех созданных элементов
	//Array<SXElement*> tmparr;
	//Array<SXCreateElement*> CreateElements;

	//порядковый номер активного элемента, если -1 значит ничего не выделено
	extern int NumActiveElement;

	//статики для статистики
	extern ISXGUIStatic* StaticStatisticMouseText;
	extern ISXGUIStatic* StaticStatisticMouseGlobal;
	extern ISXGUIStatic* StaticStatisticMouseParent;
	extern ISXGUIStatic* StaticStatisticMouseClient;

	extern ISXGUIStatic* StaticStatisticElemText;
	extern ISXGUIStatic* StaticStatisticElemCreate;
	extern ISXGUIStatic* StaticStatisticElemDelete;
	extern ISXGUIStatic* StaticStatisticElemCurrent;

	//количество созданных элементов
	extern int CountCreateNewElem;
	//количество удаленных элементов
	extern int CountDeleteElem;
	//текущее количество элементов
	extern int CountCurrentElem;

	extern ISXGUIMenuWindow* MainMenu;

	extern ISXGUIMenuWindow* MenuWindow;

	extern HCURSOR CurRePos, CurSW, CurSE, CurE, CurS, CurNS, CurWE;

	//
	extern ISXGUIBaseWnd* JobMainWnd;
	extern char NameJobWnd[256];

	extern POINT GlobalCurPos;

	//чтобы не выскакивали надоедающие уведомления
	//создано ли хоть чтото или это пустой проект
	extern bool IsCreatedEl;
	//загружался хотя бы один проект
	extern bool IsLoadedEl;
};

//##########################################################################

void InLog(const char* format, ...);

void CreateCursor();

bool Render();

#endif