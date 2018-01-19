
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __EDITOR_TAB_H
#define __EDITOR_TAB_H

#include "TabManager.h"
#include <SXGUIWinApi/sxgui.h>
#include <sxae/resource.h>
#include "EMessages.h"
#include "Tools.h"


class EditorTab
{
public:
	EditorTab(TabManager * tm);
	virtual ~EditorTab();

	void Show();
	virtual void Show(bool bShow);

	static LRESULT TabBtnHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool m_bShow;
	TabManager * m_pTM;

	ISXGUIGroupBox * m_pRoot;
	ISXGUIButton * m_pTabBtn;
};

#endif
