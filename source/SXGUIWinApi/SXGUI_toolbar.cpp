
#include "SXGUI_toolbar.h"

SXGUIToolBar::SXGUIToolBar()
{

}

SXGUIToolBar::SXGUIToolBar(const char* caption,WORD x,WORD y,WORD width,WORD heigth,WORD width_element,WORD heigth_element,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							0,
							TOOLBARCLASSNAME,
							caption,
							(parent != 0 ? WS_CHILD : 0) | TBSTYLE_FLAT | WS_BORDER | CCS_NOMOVEY | CCS_NORESIZE | TBSTYLE_TOOLTIPS | TBSTYLE_WRAPABLE,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,handler);
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();

	HimList = ImageList_Create (width_element, heigth_element, ILC_COLOR24 | ILC_MASK, 1, 256);
	SendMessage(this->GetHWND(), TB_SETIMAGELIST, 0, (LPARAM)HimList);
	ShowWindow(this->GetHWND(), SW_SHOW);
}

SXGUIToolBar::~SXGUIToolBar()
{
	ImageList_Destroy(HimList);
}

void SXGUIToolBar::AddButton(WORD num,WORD id,const char* hint,WORD id_resource,DWORD mask)
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

	SendMessage (this->GetHWND(), TB_ADDBUTTONS, 1, (LPARAM)&tbb);
	SendMessage (this->GetHWND(), TB_SETMAXTEXTROWS, 0, 0);
	SendMessage(this->GetHWND(), TB_ADDSTRING, 0, (LPARAM)hint);
	ShowWindow (this->GetHWND(), SW_SHOW);
}

void SXGUIToolBar::AddSeparator(WORD num)
{
	TBBUTTON tbb[1];
	tbb[0].iBitmap = num;
	tbb[0].idCommand = 0;
	tbb[0].fsState = TBSTATE_ENABLED;
	tbb[0].fsStyle = TBSTYLE_SEP;
	tbb[0].dwData = 0;

	SendMessage (this->GetHWND(), TB_ADDBUTTONS, 1, (LPARAM)&tbb);
	SendMessage (this->GetHWND(), TB_SETMAXTEXTROWS, 0, 0);
	ShowWindow (this->GetHWND(), SW_SHOW);
}