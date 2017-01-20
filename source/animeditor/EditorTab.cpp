
#include "EditorTab.h"

EditorTab::EditorTab(TabManager * tm):
m_bShow(false),
m_pTM(tm)
{
}

EditorTab::~EditorTab()
{
	mem_delete(m_pTabBtn);
	mem_delete(m_pRoot);
}

void EditorTab::Show()
{
	m_pTM->ShowTab(this);
}


void EditorTab::Show(bool bShow)
{
	m_pRoot->Visible(bShow);

	m_pTabBtn->Enable(!bShow);
	SendMessage(m_pTabBtn->GetHWND(), BM_SETCHECK, bShow ? BST_CHECKED : BST_UNCHECKED, 0);
}

LRESULT EditorTab::TabBtnHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_LBUTTONUP)
	{
		ISXGUIComponent * cmp = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
		//cmp->Enable(false);
		EditorTab * tab = (EditorTab*)cmp->GetUserPtr();
		tab->Show();
	}
	return(0);
}