#ifndef EditorTab_H
#define EditorTab_H

#include "TabManager.h"
#include <SXGUIWinApi/sxguielements.h>
#include "../../SkyXEngine/animeditor/resource.h"


class EditorTab
{
public:
	EditorTab(TabManager * tm);
	virtual ~EditorTab();

	void Show();
	void Show(bool bShow);

	static LRESULT TabBtnHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool m_bShow;
	TabManager * m_pTM;

	ISXGUIGroupBox * m_pRoot;
	ISXGUIButton * m_pTabBtn;
};

#endif
