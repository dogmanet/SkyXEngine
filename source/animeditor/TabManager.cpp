#include "TabManager.h"

#include "TabAttachments.h"
#include "TabSkins.h"
#include "TabActivities.h"
#include "TabAnimation.h"
#include "TabControllers.h"
#include "TabFiles.h"

TabManager::TabManager(ISXGUIBaseWnd * wnd):
m_pBaseWnd(wnd)
{
	m_pCurTab = m_pTabAttachments = new TabAttachments(this);
	m_pTabSkins = new TabSkins(this);
	m_pTabActivities = new TabActivities(this);
	m_pTabAnimation = new TabAnimation(this);
	m_pTabControllers = new TabControllers(this);
	//m_pTabFiles = new TabFiles(this);

	m_pCurTab->Show();
}

TabManager::~TabManager()
{
	mem_delete(m_pTabAttachments);
	mem_delete(m_pTabSkins);
	mem_delete(m_pTabActivities);
	mem_delete(m_pTabAnimation);
	mem_delete(m_pTabControllers);
	//mem_delete(m_pTabFiles);
}

void TabManager::ShowTab(EditorTab * pTab)
{
	m_pCurTab->Show(false);
	m_pCurTab = pTab;
	m_pCurTab->Show(true);
}


ISXGUIBaseWnd * TabManager::GetBaseWnd()
{
	return(m_pBaseWnd);
}