
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_TOOLBAR_H
#define __SXGUI_TOOLBAR_H

#include "base.h"


class CGUIToolBar : public CGUIComponent, public virtual ISXGUIToolBar
{
public:
	CGUIToolBar();
	CGUIToolBar(const char* caption, WORD x, WORD y, WORD width, WORD heigth, WORD width_element, WORD heigth_element, HWND parent, WNDPROC handler, ID idObj);
	
	~CGUIToolBar();

	void Release(){ mem_del(this); }


	void AddButton(WORD num,WORD id,const char* hint,WORD id_resource,DWORD mask);
	void AddSeparator(WORD num);

private:
	HIMAGELIST HimList;
};

#endif