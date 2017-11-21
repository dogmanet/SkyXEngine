
#include "SXGUI_status_bar.h"

SXGUIStatusBar::SXGUIStatusBar()
{

}

SXGUIStatusBar::SXGUIStatusBar(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							exstyle,
							STATUSCLASSNAME,
							caption,
							style,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
	
	AlignReSizing = SXGUI_STATUSBAR_ALIGNRS_PERCENT;
	ArrWidth = 0;
	ArrCoef = 0;
	CountArr = 0;

	this->GAlign.left = false;
	this->GAlign.top = false;
	this->GAlign.right = false;
	this->GAlign.bottom = false;
}

SXGUIStatusBar::SXGUIStatusBar(const char* caption,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							0,
							STATUSCLASSNAME,
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | TBS_AUTOTICKS | SBARS_SIZEGRIP | CCS_BOTTOM,
							CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
	
	AlignReSizing = SXGUI_STATUSBAR_ALIGNRS_PERCENT;
	ArrWidth = 0;
	ArrCoef = 0;
	CountArr = 0;

	this->GAlign.left = false;
	this->GAlign.top = false;
	this->GAlign.right = false;
	this->GAlign.bottom = false;

	::GetClientRect(this->GetHWND(),&this->OldRect);
}

SXGUIStatusBar::~SXGUIStatusBar()
{
	mem_delete_a(ArrCoef);
	mem_delete_a(ArrWidth);
}

bool SXGUIStatusBar::SetCountParts(int count, int *arr)
{
	CountArr = count;
	mem_delete_a(ArrWidth);
	ArrWidth = 0;
	ArrWidth = arr;

	this->ComCoef();

		if(SendMessage(this->GetHWND(),SB_SETPARTS,count,(LPARAM)arr))
			return true;
		else
			return false;
}

void SXGUIStatusBar::ComCoef()
{
	WORD GWidth = 0;
	RECT rect;
	::GetClientRect(this->GetHWND(),&rect);
	GWidth = rect.right;
	float OnePercent = 100.0 / float(GWidth);

	mem_delete_a(ArrCoef);
	ArrCoef = new float[CountArr];

		for(int i=0;i<CountArr;i++)
		{
			ArrCoef[i] = OnePercent *  ((ArrWidth[i] != -1 ? ArrWidth[i] : GWidth) - (i > 0 ? ArrWidth[i-1] : 0));
		}
}

bool SXGUIStatusBar::SetTextParts(int pos, const char* text)
{
	SendMessage(this->GetHWND(),SB_SETTEXT,pos,(LPARAM)text);
	return true;
}

int SXGUIStatusBar::GetCountParts(int** arr)
{
	int CountParts = SendMessage(this->GetHWND(), SB_GETPARTS, 0, 0);
	int *parts = new int[CountParts];

	int tmpcp = SendMessage(this->GetHWND(), SB_GETPARTS, CountParts, (LPARAM)parts);

		if(arr != 0)
			*arr = parts;
		else
		{
			mem_delete_a(parts);
		}
	return CountParts;
}

bool SXGUIStatusBar::GetTextParts(int pos, char* buf, int len)
{
	int CountSym = SendMessage(this->GetHWND(), SB_GETTEXTLENGTH, pos, 0);
	if (len < CountSym)
		return false;

	if (!SendMessage(this->GetHWND(), SB_GETTEXT, pos, (LPARAM)buf))
		return false;

	return true;
}

void SXGUIStatusBar::SetAlignRS(int alignrs)
{
	AlignReSizing = alignrs;
}

int SXGUIStatusBar::GetAlignRS()
{
	return AlignReSizing;
}

void SXGUIStatusBar::Update()
{
	//RECT OldRect;
	RECT NewRect;
	//::GetClientRect(this->GetHWND(),&this->OldRect);
	SendMessage(this->GetHWND(),WM_SIZE,0,0);
	::GetClientRect(this->GetHWND(),&NewRect);

	int width = NewRect.right - OldRect.right;
		/*if(width != 0)
			MessageBox(0,ToPointChar(width),0,0);*/
	int *Arr;
	
	WORD CountParts = this->GetCountParts(&Arr);
	int *NewArr = new int[CountParts];

	bool UpdateOldRect = true;

	WORD tmpCountParts = (Arr[CountParts-1] == -1 ? CountParts - 1 : CountParts);
		if (AlignReSizing == SXGUI_STATUSBAR_ALIGNRS_PERCENT)
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
		else if (AlignReSizing == SXGUI_STATUSBAR_ALIGNRS_EQUALLY)
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
			::GetClientRect(this->GetHWND(),&this->OldRect);

	SendMessage(this->GetHWND(),SB_SETPARTS,CountParts,(LPARAM)NewArr);
	mem_delete_a(Arr);
	mem_delete_a(NewArr);
}

void SXGUIStatusBar::UpdateSize()
{
	this->Update();
}