
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __TAB_MANAGER_H
#define __TAB_MANAGER_H

class EditorTab;
struct ISXGUIBaseWnd;

class TabManager
{
public:
	TabManager(ISXGUIBaseWnd * wnd);
	~TabManager();

	EditorTab * m_pTabAttachments;
	EditorTab * m_pTabSkins;
	EditorTab * m_pTabActivities;
	EditorTab * m_pTabAnimation;
	EditorTab * m_pTabControllers;
	EditorTab * m_pTabFiles;
	EditorTab * m_pTabHitboxes;

	void ShowTab(EditorTab * pTab);
	ISXGUIBaseWnd * GetBaseWnd();
protected:
	EditorTab * m_pCurTab;
	ISXGUIBaseWnd * m_pBaseWnd;
};

#endif
