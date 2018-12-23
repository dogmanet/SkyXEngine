
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "toolbar.h"


CGUIToolBar::CGUIToolBar()
{

}

CGUIToolBar::CGUIToolBar(const char* caption, WORD x, WORD y, WORD width, WORD heigth, WORD width_element, WORD heigth_element, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							0,
							TOOLBARCLASSNAME,
							caption,
							(parent != 0 ? WS_CHILD : 0) | TBSTYLE_FLAT | WS_BORDER | CCS_NOMOVEY | CCS_NORESIZE | TBSTYLE_TOOLTIPS | TBSTYLE_WRAPABLE,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(this->getHWND(), parent, handler, idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();

	HimList = ImageList_Create (width_element, heigth_element, ILC_COLOR24 | ILC_MASK, 1, 256);
	SendMessage(this->getHWND(), TB_SETIMAGELIST, 0, (LPARAM)HimList);
	ShowWindow(this->getHWND(), SW_SHOW);
}

CGUIToolBar::~CGUIToolBar()
{
	ImageList_Destroy(HimList);
}

void CGUIToolBar::AddButton(WORD num,WORD id,const char* hint,WORD id_resource,DWORD mask)
{
	TBBUTTON tbb[1];
	tbb[0].iBitmap = num;
	tbb[0].idCommand = id;
	tbb[0].fsState = TBSTATE_ENABLED;
	tbb[0].fsStyle = TBSTYLE_BUTTON;
	tbb[0].dwData = 0;
	tbb[0].iString = (INT_PTR)hint;

	HBITMAP HBitmap = LoadBitmap (GetModuleHandle(NULL), MAKEINTRESOURCE(id_resource));
    ImageList_AddMasked (HimList, HBitmap, mask);

	SendMessage (this->getHWND(), TB_ADDBUTTONS, 1, (LPARAM)&tbb);
	SendMessage (this->getHWND(), TB_SETMAXTEXTROWS, 0, 0);
	SendMessage(this->getHWND(), TB_ADDSTRING, 0, (LPARAM)hint);
	ShowWindow (this->getHWND(), SW_SHOW);
}

void CGUIToolBar::AddSeparator(WORD num)
{
	TBBUTTON tbb[1];
	tbb[0].iBitmap = num;
	tbb[0].idCommand = 0;
	tbb[0].fsState = TBSTATE_ENABLED;
	tbb[0].fsStyle = TBSTYLE_SEP;
	tbb[0].dwData = 0;

	SendMessage (this->getHWND(), TB_ADDBUTTONS, 1, (LPARAM)&tbb);
	SendMessage (this->getHWND(), TB_SETMAXTEXTROWS, 0, 0);
	ShowWindow (this->getHWND(), SW_SHOW);
}