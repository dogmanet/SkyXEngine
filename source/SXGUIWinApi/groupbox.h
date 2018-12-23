
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_GROUP_BOX_H
#define __SXGUI_GROUP_BOX_H

#include "base.h"


class CGUIGroupBox : public CGUITextual, public virtual ISXGUIGroupBox
{
public:
	CGUIGroupBox();
	~CGUIGroupBox();
	void Release(){ mem_del(this); }
	CGUIGroupBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj);
	CGUIGroupBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj);

//private:
	char* StrText;
	int LenStrText;
};

#endif