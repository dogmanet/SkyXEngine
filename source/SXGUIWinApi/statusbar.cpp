
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "statusbar.h"


CGUIStatusBar::CGUIStatusBar()
{

}

CGUIStatusBar::CGUIStatusBar(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							exstyle,
							STATUSCLASSNAME,
							caption,
							style,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
	
	AlignReSizing = SXGUI_STATUSBAR_ALIGN_RESIZE_PERCENT;
	ArrWidth = 0;
	ArrCoef = 0;
	CountArr = 0;

	m_aFollowParentSide[SXGUI_SIDE_TOP] = false;
	m_aFollowParentSide[SXGUI_SIDE_BOTTOM] = false;
	m_aFollowParentSide[SXGUI_SIDE_RIGHT] = false;
	m_aFollowParentSide[SXGUI_SIDE_LEFT] = false;
}

CGUIStatusBar::CGUIStatusBar(const char* caption, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							0,
							STATUSCLASSNAME,
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | TBS_AUTOTICKS | SBARS_SIZEGRIP | CCS_BOTTOM,
							CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
	
	AlignReSizing = SXGUI_STATUSBAR_ALIGN_RESIZE_PERCENT;
	ArrWidth = 0;
	ArrCoef = 0;
	CountArr = 0;

	m_aFollowParentSide[SXGUI_SIDE_TOP] = false;
	m_aFollowParentSide[SXGUI_SIDE_BOTTOM] = false;
	m_aFollowParentSide[SXGUI_SIDE_RIGHT] = false;
	m_aFollowParentSide[SXGUI_SIDE_LEFT] = false;

	::GetClientRect(this->getHWND(),&this->OldRect);
}

CGUIStatusBar::~CGUIStatusBar()
{
	mem_delete_a(ArrCoef);
	mem_delete_a(ArrWidth);
}

bool CGUIStatusBar::setPartsCount(int count, int *arr)
{
	CountArr = count;
	mem_delete_a(ArrWidth);
	ArrWidth = 0;
	ArrWidth = arr;

	comCoef();

		if(SendMessage(this->getHWND(),SB_SETPARTS,count,(LPARAM)arr))
			return true;
		else
			return false;
}

void CGUIStatusBar::comCoef()
{
	WORD GWidth = 0;
	RECT rect;
	::GetClientRect(this->getHWND(),&rect);
	GWidth = rect.right;
	float OnePercent = 100.0 / float(GWidth);

	mem_delete_a(ArrCoef);
	ArrCoef = new float[CountArr];

		for(int i=0;i<CountArr;i++)
		{
			ArrCoef[i] = OnePercent *  ((ArrWidth[i] != -1 ? ArrWidth[i] : GWidth) - (i > 0 ? ArrWidth[i-1] : 0));
		}
}

bool CGUIStatusBar::setPartText(int pos, const char* text)
{
	SendMessage(this->getHWND(),SB_SETTEXT,pos,(LPARAM)text);
	return true;
}

int CGUIStatusBar::getPartsCount(int **ppArr)
{
	int iCount = SendMessage(this->getHWND(), SB_GETPARTS, 0, 0);
	int *aParts = new int[iCount];

	int tmpcp = SendMessage(this->getHWND(), SB_GETPARTS, iCount, (LPARAM)aParts);

	if (ppArr != 0)
		*ppArr = aParts;
	else
	{
		mem_delete_a(aParts);
	}

	return iCount;
}

bool CGUIStatusBar::getPartText(int pos, char* buf, int len)
{
	int CountSym = SendMessage(this->getHWND(), SB_GETTEXTLENGTH, pos, 0);
	if (len < CountSym)
		return false;

	if (!SendMessage(this->getHWND(), SB_GETTEXT, pos, (LPARAM)buf))
		return false;

	return true;
}

void CGUIStatusBar::setAlignReSize(SXGUI_STATUSBAR_ALIGN_RESIZE alignrs)
{
	AlignReSizing = alignrs;
}

SXGUI_STATUSBAR_ALIGN_RESIZE CGUIStatusBar::getAlignReSize()
{
	return AlignReSizing;
}

void CGUIStatusBar::update()
{
	//RECT OldRect;
	RECT NewRect;
	//::GetClientRect(this->getHWND(),&this->OldRect);
	SendMessage(this->getHWND(),WM_SIZE,0,0);
	::GetClientRect(this->getHWND(),&NewRect);

	int width = NewRect.right - OldRect.right;
		/*if(width != 0)
			MessageBox(0,ToPointChar(width),0,0);*/
	int *Arr;
	
	WORD CountParts = getPartsCount(&Arr);
	int *NewArr = new int[CountParts];

	bool UpdateOldRect = true;

	WORD tmpCountParts = (Arr[CountParts-1] == -1 ? CountParts - 1 : CountParts);
	if (AlignReSizing == SXGUI_STATUSBAR_ALIGN_RESIZE_PERCENT)
		{
				for(int i=0;i<tmpCountParts;i++)
				{
					float coef = ArrCoef[i] / 100.0;
					//MessageBox(0,ToPointChar(coef),0,0);
					float part_width = Arr[i] - (i > 0 ? Arr[i-1] : 0);
					float new_part_width = part_width + (int(width * coef));
					//MessageBox(0,ToPointChar(ToString(width) + "|" + ToString(coef)),ToPointChar(new_part_width),0);
					NewArr[i] = new_part_width + (i > 0 ? NewArr[i-1] : 0);
						if(Arr[i] == NewArr[i])
							UpdateOldRect = false;
							//MessageBox(0,ToPointChar(ToString(Arr[i]) + "|" + ToString(NewArr[i])),0,0);
				}
		}
		else if (AlignReSizing == SXGUI_STATUSBAR_ALIGN_RESIZE_EQUALLY)
		{
				for(int i=0;i<tmpCountParts;i++)
					NewArr[i] = Arr[i] + (width / CountParts);
		}
		else
		{
				for(int i=0;i<tmpCountParts;i++)
					NewArr[i] = Arr[i];
		}

		if(Arr[CountParts-1] == -1)
			NewArr[CountParts-1] = Arr[CountParts-1];

		if(UpdateOldRect)
			::GetClientRect(this->getHWND(),&this->OldRect);

	SendMessage(this->getHWND(),SB_SETPARTS,CountParts,(LPARAM)NewArr);
	mem_delete_a(Arr);
	mem_delete_a(NewArr);
}

void CGUIStatusBar::updateSize()
{
	update();
}