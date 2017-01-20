#include "TabActivities.h"

TabActivities::TabActivities(TabManager * tm):EditorTab(tm)
{
	m_pTabBtn = SXGUICrButtonEx("Activities", 479, 648, 100, 20, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pTabBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pTabBtn->GAlign = {false, true, true, false};
	m_pTabBtn->SetUserPtr(this);
	m_pTabBtn->AddHandler(EditorTab::TabBtnHandler, WM_LBUTTONUP);

	m_pRoot = SXGUICrGroupBox("Activities", 279, 480, 1016, 167, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pRoot->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pRoot->SetColorText(0, 0, 0);
	m_pRoot->SetColorTextBk(255, 255, 255);
	m_pRoot->SetTransparentTextBk(true);
	m_pRoot->SetColorBrush(255, 255, 255);
	m_pRoot->GAlign = {0, 1, 1, 1};
	ActList = SXGUICrListBox("", 6, 16, 1003, 122, m_pRoot->GetHWND(), 0, 0, false);
	ActList->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	ActList->SetColorText(0, 0, 0);
	ActList->SetColorTextBk(255, 255, 255);
	ActList->SetTransparentTextBk(true);
	ActList->SetColorBrush(255, 255, 255);
	ActList->GAlign = {1, 1, 1, 1};
	ActNewBtn = SXGUICrButton("New", 7, 140, 100, 20, 0, m_pRoot->GetHWND(), 0, 0);
	ActNewBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	ActNewBtn->GAlign = {0, 1, 1, 0};
	ActRenameBtn = SXGUICrButton("Rename", 110, 140, 100, 20, 0, m_pRoot->GetHWND(), 0, 0);
	ActRenameBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	ActRenameBtn->GAlign = {0, 1, 1, 0};
	ActRemoveBtn = SXGUICrButton("Remove", 213, 140, 100, 20, 0, m_pRoot->GetHWND(), 0, 0);
	ActRemoveBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	ActRemoveBtn->GAlign = {0, 1, 1, 0};

	Show(false);

}

TabActivities::~TabActivities()
{
	mem_delete(ActList);
	mem_delete(ActNewBtn);
	mem_delete(ActRenameBtn);
	mem_delete(ActRemoveBtn);
}
