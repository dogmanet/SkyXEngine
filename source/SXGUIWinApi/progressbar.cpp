
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "progressbar.h"


CGUIProgressBar::CGUIProgressBar()
{

}

CGUIProgressBar::CGUIProgressBar(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							exstyle,
							PROGRESS_CLASS,
							0,
							style,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	m_lpfnPrevProc = DefWindowProc;
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

CGUIProgressBar::CGUIProgressBar(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj, bool vertical, bool smooth)
{
	m_hWindow = CreateWindowEx(
							0,
							PROGRESS_CLASS,
							0,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | (smooth ? PBS_SMOOTH : 0) | (vertical ? PBS_VERTICAL : 0),
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	//SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	ISXGUIComponent* component = dynamic_cast<ISXGUIComponent*>(this);
	SetWindowLong(getHWND(), GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

int CGUIProgressBar::getPos()
{
	return SendMessage(this->getHWND(),PBM_GETPOS,0,0);
}

int CGUIProgressBar::setPos(int pos)
{
	return SendMessage(this->getHWND(),PBM_SETPOS,WPARAM(pos),0);
}

int CGUIProgressBar::getStep()
{
	int step = SendMessage(this->getHWND(),PBM_SETSTEP,WPARAM(0),0);

	SendMessage(this->getHWND(),PBM_SETSTEP,WPARAM(step),0);
	return step;
}

int CGUIProgressBar::setStep(int step)
{
	return SendMessage(this->getHWND(),PBM_SETSTEP,WPARAM(step),0);
}

int CGUIProgressBar::setPosSteps(int steps)
{
	return SendMessage(this->getHWND(),PBM_DELTAPOS,WPARAM(steps),0);
}

int CGUIProgressBar::setOneStep()
{
	return SendMessage(this->getHWND(),PBM_STEPIT,0,0);
}

bool CGUIProgressBar::setMinMax(int min,int max)
{
	return SendMessage(this->getHWND(),PBM_SETRANGE32,WPARAM(min),LPARAM(max));
}


int CGUIProgressBar::getMin()
{
	return SendMessage(this->getHWND(),PBM_GETRANGE,WPARAM(1),0);
}

int CGUIProgressBar::getMax()
{
	return SendMessage(this->getHWND(),PBM_GETRANGE,WPARAM(0),0);
}

bool CGUIProgressBar::setColorBar(DWORD dwColor)
{
	return SendMessage(this->getHWND(), PBM_SETBARCOLOR, 0, LPARAM(dwColor));
}

bool CGUIProgressBar::setColorBk(DWORD dwColor)
{
	return SendMessage(this->getHWND(), PBM_SETBKCOLOR, 1, LPARAM(dwColor));
}

DWORD CGUIProgressBar::getColorBar()
{
	COLORREF color = SendMessage(this->getHWND(),PBM_SETBARCOLOR,0,0);
	return color;
}

DWORD CGUIProgressBar::getColorBk()
{
	COLORREF color = SendMessage(this->getHWND(),PBM_SETBKCOLOR,0,0);
	return color;
}