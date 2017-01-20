#ifndef TabManager_H
#define TabManager_H

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

	void ShowTab(EditorTab * pTab);
	ISXGUIBaseWnd * GetBaseWnd();
protected:
	EditorTab * m_pCurTab;
	ISXGUIBaseWnd * m_pBaseWnd;
};

#endif
