

#include "sxwincreator.h"

namespace SXEngine
{
	HWND				Hwnd = 0;
	HINSTANCE			Hinstance = 0;
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
		"ListView",
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
		IDB_BITMAP25,
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

	ISXGUIMenuWindow* MainMenu;

	ISXGUIMenuWindow* MenuWindow;

	HCURSOR CurRePos, CurSW, CurSE, CurE, CurS, CurNS, CurWE;

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

void InLog(const char* format, ...)
{
	va_list va;
	char buf[1024];
	va_start(va, format);
	vsprintf_s(buf, 1024, format, va);
	va_end(va);

	if (SXMainWndElem::ListBoxLog)
	{
		SXMainWndElem::ListBoxLog->addItem(buf);
		gui_func::scrollbar::ScrollLine(SXMainWndElem::ListBoxLog, SXGUI_SCROLL_TYPE_V, SXGUI_SCROLL_DIR_DOWN, SXMainWndElem::ListBoxLog->getItemCount());
	}
}

void CreateCursor()
{
	SXMainWndElem::CurSE = LoadCursor(0, IDC_SIZENWSE);
	SXMainWndElem::CurSW = LoadCursor(0, IDC_SIZENESW);
	SXMainWndElem::CurRePos = LoadCursor(0, IDC_SIZEALL);

	SXMainWndElem::CurNS = LoadCursor(0, IDC_SIZENS);
	SXMainWndElem::CurWE = LoadCursor(0, IDC_SIZEWE);
}

bool Render()
{
	RECT rect_main_wnd;
	GetClientRect(SXMainWndElem::MainWnd->getHWND(), &rect_main_wnd);

	GetClientRect(SXMainWndElem::MainWnd->getHWND(), SXMainWndElem::MainWndOldRect);

	//если нажата кнопка delete и есть активный элемент то удаляем его
	//и ставим порядковый номер активного элемента -1
	if (GetAsyncKeyState(VK_DELETE) < 0 && SXMainWndElem::NumActiveElement != -1)
	{
		InLog("%s%s%s", "Deletion of the elements [", SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Name, "] ...");
		SXMainWndElem::CountDeleteElem++;
		SXMainWndElem::ListBoxAllElements->deleteItem(SXMainWndElem::NumActiveElement);
		for (int i = SXMainWndElem::NumActiveElement; i<SXMainWndElem::ListBoxAllElements->getItemCount(); i++)
		{
			int ud = SXMainWndElem::ListBoxAllElements->getItemData(i);
			SXMainWndElem::ListBoxAllElements->setItemData(i, (LPARAM)(ud - 1));
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
		InLog("%s", "Deletiing an element is completed, serial number overwritten (userdata32)");

		if (SXMainWndElem::CreateElements.size() == 0)
			SXMainWndElem::IsCreatedEl = SXMainWndElem::IsLoadedEl = false;
	}

	POINT global_pos_cursor;
	GetCursorPos(&global_pos_cursor);

	//if(SXMainWndElem::GlobalCurPos.x != global_pos_cursor.x && SXMainWndElem::GlobalCurPos.y != global_pos_cursor.y)
	//{
	char CurPos[256];
	//SXMainWndElem::StaticStatisticMouseGlobal->GetText(CurPos,256);
	sprintf(CurPos, "%s%d%s%d", SX_WINCREATOR_STAT_CUR_G, global_pos_cursor.x, " | ", global_pos_cursor.y);
	//InLog("%s",CurPos);
	SXMainWndElem::StaticStatisticMouseGlobal->setText(CurPos);
	MapWindowPoints(0, SXMainWndElem::JobMainWnd->getHWND(), &global_pos_cursor, 1);
	sprintf(CurPos, "%s%d%s%d", SX_WINCREATOR_STAT_CUR_PARENT, global_pos_cursor.x, " | ", global_pos_cursor.y);
	SXMainWndElem::StaticStatisticMouseParent->setText(CurPos);
	if (SXMainWndElem::NumActiveElement != -1)
	{
		MapWindowPoints(SXMainWndElem::JobMainWnd->getHWND(), SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getHWND(), &global_pos_cursor, 1);
		sprintf(CurPos, "%s%d%s%d", SX_WINCREATOR_STAT_CUR_CLIENT, global_pos_cursor.x, " | ", global_pos_cursor.y);
		SXMainWndElem::StaticStatisticMouseClient->setText(CurPos);
	}
	else
	{
		sprintf(CurPos, "%s%d%s%d", SX_WINCREATOR_STAT_CUR_CLIENT, -1, " | ", -1);
		SXMainWndElem::StaticStatisticMouseClient->setText(CurPos);
	}
	//SXMainWndElem::GlobalCurPos = global_pos_cursor;
	//}

	char ElemStat[256];
	sprintf(ElemStat, "%s%d", SX_WINCREATOR_STAT_ALL_ELEM_COUNT_CREATE, SXMainWndElem::CountCreateNewElem);
	SXMainWndElem::StaticStatisticElemCreate->setText(ElemStat);

	sprintf(ElemStat, "%s%d", SX_WINCREATOR_STAT_ALL_ELEM_COUNT_DELETE, SXMainWndElem::CountDeleteElem);
	SXMainWndElem::StaticStatisticElemDelete->setText(ElemStat);

	SXMainWndElem::CountCurrentElem = SXMainWndElem::CreateElements.size();
	sprintf(ElemStat, "%s%d", SX_WINCREATOR_STAT_ALL_ELEM_COUNT_CURRENT, SXMainWndElem::CountCurrentElem);
	SXMainWndElem::StaticStatisticElemCurrent->setText(ElemStat);


	//вывод всех окон на передний план
	//находим активное окно
	HWND ActivHwnd = GetActiveWindow();
	//если хотя бы одно из наших окон активно
	if (SXMainWndElem::ArrMainWnd[0] == ActivHwnd)
	{
		//проходим еще раз по всем окнам
		for (int k = 0; k<4; k++)
		{
			bool IsNext = false;
			HWND tmph = ActivHwnd;
			//если наше окно SXMainWndElem::ArrMainWnd[k] находится в пределах от 2 до 4 zпоследовательности
			for (int j = 0; j<4; j++)
			{
				tmph = GetNextWindow(tmph, GW_HWNDNEXT);
				if (SXMainWndElem::ArrMainWnd[k] == tmph)
				{
					IsNext = true;
					break;
				}
			}

			if (!IsNext)
			{
				if (IsIconic(SXMainWndElem::ArrMainWnd[k]))
					ShowWindow(SXMainWndElem::ArrMainWnd[k], SW_RESTORE);
				SetActiveWindow(SXMainWndElem::ArrMainWnd[k]);
			}
		}
	}

	return true;
};


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	SXEngine::Hinstance = hinstance;
	SXMainWndElem::IsCreatedEl = false;
	SXMainWndElem::IsLoadedEl = false;
	for (int i = 0; i<SX_WINCREATOR_COUNT_ELEMENT; i++)
		SXMainWndElem::NumSourceElements[i] = 1;

	CreateCursor();

	SXGUIinit();

	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);

	SXMainWndElem::MainWnd = SXGUICrBaseWndEx(
		"SkyX Windows Creator", "SkyX Windows Creator",
		wrect.left, wrect.top, wrect.right, 150,
		0, 0, CreateSolidBrush(RGB(220, 220, 220)),
		0, CS_HREDRAW | CS_VREDRAW, WS_DLGFRAME | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
		0, WndProcAllDefault);
	SXMainWndElem::MainWnd->setVisible(true);
	gui_func::base_handlers::InitHandlerMsg(SXMainWndElem::MainWnd);
	SXMainWndElem::MainWnd->addHandler(ComMenuId, WM_COMMAND);
	SXMainWndElem::MainWnd->addHandler(TrueExit, WM_CLOSE, 0, 0, 0, 0, true);

	SXMainWndElem::MainWnd->setMixSize(wrect.right, 150);

	SXMainWndElem::MainWnd->setStretchSides(true, true, true, true);

	SXMainWndElem::MainMenu = SXGUICrMenuWindowEx(IDR_MENU1);
	SXMainWndElem::MainMenu->setToWindow(SXMainWndElem::MainWnd->getHWND());

	SXMainWndElem::MenuWindow = SXGUICrMenuWindowEx(IDR_MENU2);

	SXMainWndElem::ParamWnd = SXGUICrBaseWndEx(
		"ParamWnd", "ParamWnd", 
		wrect.right - 256, wrect.top + 160, 256, wrect.bottom - 160,
		0, 0, CreateSolidBrush(RGB(220, 220, 220)),
		WS_EX_TOOLWINDOW, CS_HREDRAW | CS_VREDRAW, WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_OVERLAPPED,
		0, WndProcAllDefault);
	SXMainWndElem::ParamWnd->setVisible(true);
	gui_func::base_handlers::InitHandlerMsg(SXMainWndElem::ParamWnd);
	SXMainWndElem::ParamWnd->addHandler(MinimuzeWinInsteadClose, WM_CLOSE, 0, 0, 0, 0, true);
	SXMainWndElem::ParamWnd->addHandler(CallWmCommand, WM_COMMAND, 0, 0, 0, 0, true);

	SXMainWndElem::ListBoxAllElements = SXGUICrListBox(5, 5, 240, 150, SXMainWndElem::ParamWnd->getHWND(), 0, 0, false);
	SXMainWndElem::ListBoxAllElements->addHandler(ListBoxAllElementsDBLClick, WM_LBUTTONDBLCLK);


	RECT rect_main_wnd;
	GetClientRect(SXMainWndElem::MainWnd->getHWND(), &rect_main_wnd);

	SXMainWndElem::MainWndOldRect = new RECT;
	GetClientRect(SXMainWndElem::MainWnd->getHWND(), SXMainWndElem::MainWndOldRect);

	SXMainWndElem::SettingsPanel = SXGUICrToolBar(0, 0, SXMainWndElem::MainWndOldRect->right, 26, SXMainWndElem::MainWnd->getHWND(), 0, 123);
	SXMainWndElem::SettingsPanel->setFollowParentSides(true, false, true, true);

	ISXGUIButtonImg* ImgNew = SXGUICrButtonImgRes(IDB_BITMAP1, 3, 1, 20, 20, RGB(255, 0, 110), RGB(220, 220, 220), SXMainWndElem::SettingsPanel->getHWND(), 0, 0);
	ImgNew->initCallBack();
	ImgNew->addHandler(CallImgNew, WM_LBUTTONUP);
	//ImgNew->ShowHint(true);
	//ImgNew->setHintText(SXMainWndElem::NameElements[i]);

	ISXGUIButtonImg* ImgLoad = SXGUICrButtonImgRes(IDB_BITMAP2, 30, 1, 20, 20, RGB(255, 0, 110), RGB(220, 220, 220), SXMainWndElem::SettingsPanel->getHWND(), 0, 0);
	ImgLoad->initCallBack();
	ImgLoad->addHandler(CallImgLoad, WM_LBUTTONUP);

	//ImgNew->ShowHint(true);
	//ImgNew->setHintText(SXMainWndElem::NameElements[i]);

	ISXGUIButtonImg* ImgSave = SXGUICrButtonImgRes(IDB_BITMAP4, 57, 1, 20, 20, RGB(255, 0, 110), RGB(220, 220, 220), SXMainWndElem::SettingsPanel->getHWND(), 0, 0);
	ImgSave->initCallBack();
	ImgSave->addHandler(CallImgSave, WM_LBUTTONUP);

	//ImgNew->ShowHint(true);
	//ImgNew->setHintText(SXMainWndElem::NameElements[i]);

	ISXGUIButtonImg* ImgBuild = SXGUICrButtonImgRes(IDB_BITMAP3, 84, 1, 20, 20, RGB(255, 0, 110), RGB(220, 220, 220), SXMainWndElem::SettingsPanel->getHWND(), 0, 0);
	ImgBuild->initCallBack();
	ImgBuild->addHandler(CallImgBuild, WM_LBUTTONUP);

	//ImgNew->ShowHint(true);
	//ImgNew->setHintText(SXMainWndElem::NameElements[i]);


	SXMainWndElem::ToolsPanel = SXGUICrToolBar(0, 25, SXMainWndElem::MainWndOldRect->right, 31, SXMainWndElem::MainWnd->getHWND(), 0, 123);
	SXMainWndElem::ToolsPanel->setFollowParentSides(true, false, true, true);

	int tmpX, tmpY, tmpAddition;
	tmpX = 30;
	tmpY = 1;
	tmpAddition = 27;

	SXMainWndElem::ButtonArrow = SXGUICrButtonImgRes(IDB_BITMAP5, 3, tmpY, 24, 24, RGB(0, 0, 0), RGB(0, 0, 0), SXMainWndElem::ToolsPanel->getHWND(), 0, 0);
	SXMainWndElem::ButtonArrow->setColorFrame(RGB(100, 100, 100));
	SXMainWndElem::ButtonArrow->initCallBack();

	SXMainWndElem::ButtonArrow->setFollowParentSides(true, false, false, true);

	SXMainWndElem::ButtonArrow->setActive(true);
	SXMainWndElem::ButtonArrow->setParentGroup(true);

	for (int i = 0; i<SX_WINCREATOR_COUNT_ELEMENT; i++)
	{
		sprintf(SXMainWndElem::Elements[i].Name, "%s", SXMainWndElem::NameElements[i]);
		SXMainWndElem::Elements[i].Object = SXGUICrButtonImgRes(SXMainWndElem::ArrNumResource[i], tmpX, tmpY, 24, 24, RGB(0, 0, 0), RGB(0, 0, 0), SXMainWndElem::ToolsPanel->getHWND(), 0, 0);

		SXMainWndElem::Elements[i].Object->initCallBack();

		SXMainWndElem::Elements[i].Object->setColorFrame(RGB(100, 100, 100));
		SXMainWndElem::Elements[i].Object->setFollowParentSides(true, false, false, true);

		SXMainWndElem::Elements[i].Object->setShowHint(true);
		SXMainWndElem::Elements[i].Object->setHintText(SXMainWndElem::NameElements[i]);

		SXMainWndElem::Elements[i].Object->setActive(true);
		SXMainWndElem::Elements[i].Object->setParentGroup(true);
		tmpX += tmpAddition;
	}


	sprintf(SXMainWndElem::NameJobWnd, "%s", "JobWindow");
	SXMainWndElem::JobMainWnd = SXGUICrBaseWndEx(
		SXMainWndElem::NameJobWnd, SXMainWndElem::NameJobWnd, 
		100, 250, wrect.right - 456, wrect.bottom - 350,
		0, 0, CreateSolidBrush(RGB(220, 220, 220)),
		0, CS_HREDRAW | CS_VREDRAW, WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
		0, WndProcAllDefault);
	SXMainWndElem::JobMainWnd->setVisible(true);
	gui_func::base_handlers::InitHandlerMsg(SXMainWndElem::JobMainWnd);
	SXMainWndElem::JobMainWnd->addHandler(AddElement, WM_LBUTTONUP);
	SXMainWndElem::JobMainWnd->addHandler(MouseMove, WM_MOUSEMOVE);
	SXMainWndElem::JobMainWnd->addHandler(RButtonUp, WM_RBUTTONUP);
	SXMainWndElem::JobMainWnd->addHandler(MinimuzeWinInsteadClose, WM_CLOSE, 0, 0, 0, 0, true);

	//ISXGUIButton* tmpbutton = SXGUICrButton("NameElem", 100, 100, 100, 20, 0, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0);
	//int cerr = SetWindowLong(tmpbutton->getHWND(), GWL_USERDATA, (LONG)dynamic_cast<ISXGUIButton*>(tmpbutton));
	//SXMainWndElem::JobMainWnd->addHandler(LButtonDown,WM_LBUTTONDOWN);
	//SXMainWndElem::JobMainWnd->addHandler(ActivateAllWindows,WM_ACTIVATE/*,0,false,0,false,true*/);


	//установка статистики курсора
	SXMainWndElem::StaticStatisticMouseText = SXGUICrStatic(SX_WINCREATOR_STAT_CUR_TEXT, 10, 73, 110, 15, SXMainWndElem::MainWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticStatisticMouseText->setColorBrush(RGB(220, 220, 220));
	SXMainWndElem::StaticStatisticMouseText->setFont(0, 14, 0, 0, 0, 0, 0);
	SXMainWndElem::StaticStatisticMouseGlobal = SXGUICrStatic("", 120, 58, 150, 15, SXMainWndElem::MainWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticStatisticMouseGlobal->setColorBrush(RGB(220, 220, 220));
	SXMainWndElem::StaticStatisticMouseGlobal->setFont(0, 14, 0, 0, 0, 0, 0);
	SXMainWndElem::StaticStatisticMouseParent = SXGUICrStatic("", 120, 73, 150, 15, SXMainWndElem::MainWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticStatisticMouseParent->setColorBrush(RGB(220, 220, 220));
	SXMainWndElem::StaticStatisticMouseParent->setFont(0, 14, 0, 0, 0, 0, 0);
	SXMainWndElem::StaticStatisticMouseClient = SXGUICrStatic("", 120, 88, 150, 15, SXMainWndElem::MainWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticStatisticMouseClient->setColorBrush(RGB(220, 220, 220));
	SXMainWndElem::StaticStatisticMouseClient->setFont(0, 14, 0, 0, 0, 0, 0);

	//установка статистики элементов
	SXMainWndElem::StaticStatisticElemText = SXGUICrStatic(SX_WINCREATOR_STAT_ALL_ELEM_TEXT, 280, 73, 130, 15, SXMainWndElem::MainWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticStatisticElemText->setColorBrush(RGB(220, 220, 220));
	SXMainWndElem::StaticStatisticElemText->setFont(0, 14, 0, 0, 0, 0, 0);
	SXMainWndElem::StaticStatisticElemCreate = SXGUICrStatic("", 410, 58, 250, 15, SXMainWndElem::MainWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticStatisticElemCreate->setColorBrush(RGB(220, 220, 220));
	SXMainWndElem::StaticStatisticElemCreate->setFont(0, 14, 0, 0, 0, 0, 0);
	SXMainWndElem::StaticStatisticElemDelete = SXGUICrStatic("", 410, 73, 250, 15, SXMainWndElem::MainWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticStatisticElemDelete->setColorBrush(RGB(220, 220, 220));
	SXMainWndElem::StaticStatisticElemDelete->setFont(0, 14, 0, 0, 0, 0, 0);
	SXMainWndElem::StaticStatisticElemCurrent = SXGUICrStatic("", 410, 88, 250, 15, SXMainWndElem::MainWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticStatisticElemCurrent->setColorBrush(RGB(220, 220, 220));
	SXMainWndElem::StaticStatisticElemCurrent->setFont(0, 14, 0, 0, 0, 0, 0);


	int tmpPosX = 5;
	int tmpPosY = 160;

	//установка статиков для отображения параметров
	SXMainWndElem::StaticParamPosX = SXGUICrStatic("PosX:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamPosX->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::StaticParamPosX->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamPosX = SXGUICrEdit("PosX:", 90, tmpPosY, 155, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamPosX->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamPosX->modifyStyle(ES_NUMBER, 0);
	SXMainWndElem::EditParamPosX->addHandler(InputInfoEdit, WM_KEYDOWN);

	tmpPosY += 20;


	SXMainWndElem::StaticParamPosY = SXGUICrStatic("PosY:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamPosY->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::StaticParamPosY->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamPosY = SXGUICrEdit("PosY:", 90, tmpPosY, 155, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamPosY->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamPosY->modifyStyle(ES_NUMBER, 0);
	SXMainWndElem::EditParamPosY->addHandler(InputInfoEdit, WM_KEYDOWN);

	tmpPosY += 20;

	SXMainWndElem::StaticParamWidth = SXGUICrStatic("Width:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamWidth->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::StaticParamWidth->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamWidth = SXGUICrEdit("Width:", 90, tmpPosY, 155, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamWidth->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamWidth->modifyStyle(ES_NUMBER, 0);
	SXMainWndElem::EditParamWidth->addHandler(InputInfoEdit, WM_KEYDOWN);

	tmpPosY += 20;

	SXMainWndElem::StaticParamHeight = SXGUICrStatic("Height:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamHeight->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::StaticParamHeight->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamHeight = SXGUICrEdit("Height:", 90, tmpPosY, 155, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamHeight->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamHeight->modifyStyle(ES_NUMBER, 0);
	SXMainWndElem::EditParamHeight->addHandler(InputInfoEdit, WM_KEYDOWN);

	tmpPosY += 20;

	SXMainWndElem::StaticParamCaption = SXGUICrStatic("Caption:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamCaption->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::StaticParamCaption->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamCaption = SXGUICrEdit("Caption:", 90, tmpPosY, 155, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamCaption->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamCaption->addHandler(InputInfoEdit, WM_KEYDOWN);

	tmpPosY += 20;

	SXMainWndElem::StaticParamVarName = SXGUICrStatic("Variable name:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamVarName->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::StaticParamCaption->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamVarName = SXGUICrEdit("name", 90, tmpPosY, 155, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamCaption->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamVarName->addHandler(InputInfoEdit, WM_KEYDOWN);

	tmpPosY += 20;

	SXMainWndElem::StaticParamColorText = SXGUICrStatic("Color text:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamColorText->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::StaticParamColorText->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamColorTextR = SXGUICrEdit("0", 90, tmpPosY, 45, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamColorTextR->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextR->modifyStyle(ES_NUMBER, 0);
	SXMainWndElem::EditParamColorTextR->addHandler(InputInfoEdit, WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextR->addHandler(InitColorText, WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextR->addHandler(InputToEditColor, WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextR->addHandler(InputToEditColor, WM_KEYUP);

	SXMainWndElem::EditParamColorTextG = SXGUICrEdit("0", 145, tmpPosY, 45, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamColorTextG->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextG->modifyStyle(ES_NUMBER, 0);
	SXMainWndElem::EditParamColorTextG->addHandler(InputInfoEdit, WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextG->addHandler(InitColorText, WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextG->addHandler(InputToEditColor, WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextG->addHandler(InputToEditColor, WM_KEYUP);

	SXMainWndElem::EditParamColorTextB = SXGUICrEdit("0", 200, tmpPosY, 45, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamColorTextB->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextB->modifyStyle(ES_NUMBER, 0);
	SXMainWndElem::EditParamColorTextB->addHandler(InputInfoEdit, WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextB->addHandler(InitColorText, WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextB->addHandler(InputToEditColor, WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextB->addHandler(InputToEditColor, WM_KEYUP);

	tmpPosY += 20;

	SXMainWndElem::StaticParamColorBKText = SXGUICrStatic("Color bk text:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamColorBKText->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::StaticParamColorBKText->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamColorTextBKR = SXGUICrEdit("0", 90, tmpPosY, 45, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamColorTextBKR->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextBKR->modifyStyle(ES_NUMBER, 0);
	SXMainWndElem::EditParamColorTextBKR->addHandler(InputInfoEdit, WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKR->addHandler(InitColorText, WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextBKR->addHandler(InputToEditColor, WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKR->addHandler(InputToEditColor, WM_KEYUP);

	SXMainWndElem::EditParamColorTextBKG = SXGUICrEdit("0", 145, tmpPosY, 45, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamColorTextBKG->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextBKG->modifyStyle(ES_NUMBER, 0);
	SXMainWndElem::EditParamColorTextBKG->addHandler(InputInfoEdit, WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKG->addHandler(InitColorText, WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextBKG->addHandler(InputToEditColor, WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKG->addHandler(InputToEditColor, WM_KEYUP);

	SXMainWndElem::EditParamColorTextBKB = SXGUICrEdit("0", 200, tmpPosY, 45, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamColorTextBKB->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorTextBKB->modifyStyle(ES_NUMBER, 0);
	SXMainWndElem::EditParamColorTextBKB->addHandler(InputInfoEdit, WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKB->addHandler(InitColorText, WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorTextBKB->addHandler(InputToEditColor, WM_KEYDOWN);
	SXMainWndElem::EditParamColorTextBKB->addHandler(InputToEditColor, WM_KEYUP);

	tmpPosY += 20;

	SXMainWndElem::StaticParamTransparentText = SXGUICrStatic("Alpha bk text:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamTransparentText->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::StaticParamTransparentText->Font(0,15,0,0,0,0);

	SXMainWndElem::ComboBoxParamTransparentText = SXGUICrComboBox(90, tmpPosY - 2, 155, 70, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::ComboBoxParamTransparentText->Font(0,15,0,0,0,0);
	SendMessage(SXMainWndElem::ComboBoxParamTransparentText->getHWND(), CB_SETITEMHEIGHT, -1, 14);
	SXMainWndElem::ComboBoxParamTransparentText->addItem("true");
	SXMainWndElem::ComboBoxParamTransparentText->addItem("false");
	SXMainWndElem::ComboBoxParamTransparentText->setSel(1);
	//SXMainWndElem::ComboBoxParamTransparentText->addHandler(InputInfoComboBox,WM_LBUTTONUP);
	SXMainWndElem::ComboBoxParamTransparentText->addHandler(InputInfoComboBox, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	tmpPosY += 20;

	SXMainWndElem::StaticParamColorBK = SXGUICrStatic("Color bk:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamColorBK->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::StaticParamColorBK->Font(0,15,0,0,0,0);

	SXMainWndElem::EditParamColorBKR = SXGUICrEdit("0", 90, tmpPosY + 1, 45, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamColorBKR->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorBKR->modifyStyle(ES_NUMBER, 0);
	SXMainWndElem::EditParamColorBKR->addHandler(InputInfoEdit, WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKR->addHandler(InitColorText, WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorBKR->addHandler(InputToEditColor, WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKR->addHandler(InputToEditColor, WM_KEYUP);

	SXMainWndElem::EditParamColorBKG = SXGUICrEdit("0", 145, tmpPosY + 1, 45, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamColorBKG->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorBKG->modifyStyle(ES_NUMBER, 0);
	SXMainWndElem::EditParamColorBKG->addHandler(InputInfoEdit, WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKG->addHandler(InitColorText, WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorBKG->addHandler(InputToEditColor, WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKG->addHandler(InputToEditColor, WM_KEYUP);

	SXMainWndElem::EditParamColorBKB = SXGUICrEdit("0", 200, tmpPosY + 1, 45, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamColorBKB->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamColorBKB->modifyStyle(ES_NUMBER, 0);
	SXMainWndElem::EditParamColorBKB->addHandler(InputInfoEdit, WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKB->addHandler(InitColorText, WM_LBUTTONDBLCLK);
	SXMainWndElem::EditParamColorBKB->addHandler(InputToEditColor, WM_KEYDOWN);
	SXMainWndElem::EditParamColorBKB->addHandler(InputToEditColor, WM_KEYUP);

	tmpPosY += 20;

	SXMainWndElem::StaticParamFont = SXGUICrStatic("Font:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamFont->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::StaticParamFont->Font(0,15,0,0,0,0);
	SXMainWndElem::ButtonParamSelectFont = SXGUICrButton("View font", 90, tmpPosY, 155, 15, SXGUI_BUTTON_IMAGE_NONE, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::ButtonParamSelectFont->Font(0,13,0,0,0,0);
	SXMainWndElem::ButtonParamSelectFont->addHandler(InitFont, WM_LBUTTONUP);

	tmpPosY += 20;

	SXMainWndElem::StaticParamFontParent = SXGUICrStatic("Regulation Font:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamFontParent->setColorBrush(RGB(220, 220, 220));

	SXMainWndElem::ComboBoxParamParentFont = SXGUICrComboBox(90, tmpPosY - 1, 155, 70, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SendMessage(SXMainWndElem::ComboBoxParamParentFont->getHWND(), CB_SETITEMHEIGHT, -1, 15);
	SXMainWndElem::ComboBoxParamParentFont->addItem("Parent");
	SXMainWndElem::ComboBoxParamParentFont->addItem("Default gui");
	SXMainWndElem::ComboBoxParamParentFont->addItem("User selected");
	SXMainWndElem::ComboBoxParamParentFont->setSel(1);
	SXMainWndElem::ComboBoxParamParentFont->addHandler(InputInfoComboBox, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	tmpPosY += 20;

	SXMainWndElem::StaticParamHintText = SXGUICrStatic("Hint text:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamHintText->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::StaticParamHintText->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamHintText = SXGUICrEdit("", 90, tmpPosY + 2, 155, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::EditParamHintText->Font(0,15,0,0,0,0);
	SXMainWndElem::EditParamHintText->addHandler(InputInfoEdit, WM_KEYDOWN);

	tmpPosY += 20;

	SXMainWndElem::StaticParamHintVisible = SXGUICrStatic("Hint visible:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamHintVisible->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::StaticParamHintVisible->Font(0,15,0,0,0,0);

	SXMainWndElem::ComboBoxParamHintVisible = SXGUICrComboBox(90, tmpPosY, 155, 70, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::ComboBoxParamHintVisible->Font(0,15,0,0,0,0);
	SendMessage(SXMainWndElem::ComboBoxParamHintVisible->getHWND(), CB_SETITEMHEIGHT, -1, 15);
	SXMainWndElem::ComboBoxParamHintVisible->addItem("true");
	SXMainWndElem::ComboBoxParamHintVisible->addItem("false");
	SXMainWndElem::ComboBoxParamHintVisible->setSel(1);
	SXMainWndElem::ComboBoxParamHintVisible->addHandler(InputInfoComboBox, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	tmpPosY += 20;

	SXMainWndElem::StaticParamVisible = SXGUICrStatic("Visible:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamVisible->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::StaticParamVisible->Font(0,15,0,0,0,0);
	SXMainWndElem::ComboBoxParamVisible = SXGUICrComboBox(90, tmpPosY, 155, 70, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::ComboBoxParamVisible->Font(0,15,0,0,0,0);
	SendMessage(SXMainWndElem::ComboBoxParamVisible->getHWND(), CB_SETITEMHEIGHT, -1, 15);
	SXMainWndElem::ComboBoxParamVisible->addItem("true");
	SXMainWndElem::ComboBoxParamVisible->addItem("false");
	SXMainWndElem::ComboBoxParamVisible->setSel(0);
	SXMainWndElem::ComboBoxParamVisible->addHandler(InputInfoComboBox, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	tmpPosY += 20;

	SXMainWndElem::StaticParamEnabled = SXGUICrStatic("Enabled:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamEnabled->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::StaticParamEnabled->Font(0,15,0,0,0,0);
	SXMainWndElem::ComboBoxParamEnabled = SXGUICrComboBox(90, tmpPosY, 155, 70, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	//SXMainWndElem::ComboBoxParamEnabled->Font(0,15,0,0,0,0);
	SXMainWndElem::ComboBoxParamEnabled->addItem("true");
	SXMainWndElem::ComboBoxParamEnabled->addItem("false");
	SXMainWndElem::ComboBoxParamEnabled->setSel(0);
	SXMainWndElem::ComboBoxParamEnabled->addHandler(InputInfoComboBox, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	tmpPosY += 20;

	SXMainWndElem::StaticParamImgBLoadImg = SXGUICrStatic("Image:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamImgBLoadImg->setColorBrush(RGB(220, 220, 220));
	SXMainWndElem::ButtomParamImgBLoadImg = SXGUICrButton("LoadImg", 90, tmpPosY, 155, 15, SXGUI_BUTTON_IMAGE_NONE, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::ButtomParamImgBLoadImg->addHandler(InitImageForButtonImg, WM_LBUTTONUP);

	tmpPosY += 20;

	SXMainWndElem::StaticParamWinMenu = SXGUICrStatic("Menu window:", 5, tmpPosY, 80, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0);
	SXMainWndElem::StaticParamWinMenu->setColorBrush(RGB(220, 220, 220));
	SXMainWndElem::CheckBoxParamWinMenu = SXGUICrCheckBox("Enable", 90, tmpPosY, 155, 15, SXMainWndElem::ParamWnd->getHWND(), 0, 0, false);
	SXMainWndElem::CheckBoxParamWinMenu->setColorBrush(RGB(220, 220, 220));
	//SXMainWndElem::CheckBoxParamWinMenu->addHandler(InitWindowMenu,WM_LBUTTONUP);

	SXMainWndElem::WndLog = SXGUICrBaseWndEx(
		"WndLog", "WndLog",  
		0, wrect.bottom - 220, 500, 220,
		0, 0, CreateSolidBrush(RGB(220, 220, 220)),
		WS_EX_TOOLWINDOW, CS_HREDRAW | CS_VREDRAW, WS_CAPTION | WS_MINIMIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_OVERLAPPED,
		0, WndProcAllDefault);
	SXMainWndElem::WndLog->setVisible(true);
	SXMainWndElem::WndLog->addHandler(MinimuzeWinInsteadClose, WM_CLOSE, 0, 0, 0, 0, true);
	RECT crWndLog;
	GetClientRect(SXMainWndElem::WndLog->getHWND(), &crWndLog);
	SXMainWndElem::ListBoxLog = SXGUICrListBox( crWndLog.left, crWndLog.top, crWndLog.right, crWndLog.bottom, SXMainWndElem::WndLog->getHWND(), 0, 0, false);
	//SXMainWndElem::ListBoxLog->Font(0,14,0,0,0,0);
	InLog("%s", "Log created, run cycle ...");

	//SXMainWndElem::JobMainWnd->getHWND();

	SXMainWndElem::ArrMainWnd[0] = SXMainWndElem::MainWnd->getHWND();
	SXMainWndElem::ArrMainWnd[1] = SXMainWndElem::JobMainWnd->getHWND();
	SXMainWndElem::ArrMainWnd[2] = SXMainWndElem::ParamWnd->getHWND();
	SXMainWndElem::ArrMainWnd[3] = SXMainWndElem::WndLog->getHWND();

	SXNameSapce::InitAllElements();
	SXNameSapce::OutputGUIInFile->setVisible(false);
	AboutSXWinCreator::InitAllElements();
	AboutSXWinCreator::JobWindow->setVisible(false);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (GetMessage(&msg, 0, 0, 0) != 0)
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);
		Render();

	}

	return msg.wParam;
}