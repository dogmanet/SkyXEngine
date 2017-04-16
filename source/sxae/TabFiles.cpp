#include "TabFiles.h"

TabFiles::TabFiles(TabManager * tm):EditorTab(tm)
{
	m_pTabBtn = SXGUICrButtonEx("Files", 780, 648, 100, 20, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pTabBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pTabBtn->GAlign = {false, true, true, false};
	m_pTabBtn->SetUserPtr(this);
	m_pTabBtn->AddHandler(EditorTab::TabBtnHandler, WM_LBUTTONUP);

	m_pRoot = SXGUICrGroupBox("Files", 279, 480, 1016, 167, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pRoot->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pRoot->SetColorText(0, 0, 0);
	m_pRoot->SetColorTextBk(255, 255, 255);
	m_pRoot->SetTransparentTextBk(true);
	m_pRoot->SetColorBrush(255, 255, 255);
	m_pRoot->GAlign = {false, true, true, true};
	FileList = SXGUICrListBox("", 6, 15, 1002, 140, m_pRoot->GetHWND(), 0, 0, false);
	FileList->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	FileList->SetColorText(0, 0, 0);
	FileList->SetColorTextBk(255, 255, 255);
	FileList->SetTransparentTextBk(true);
	FileList->SetColorBrush(255, 255, 255);
	FileList->GAlign = {true, true, true, true};
	FileAddBtn = SXGUICrButton("Add file", 6, 144, 100, 20, 0, m_pRoot->GetHWND(), 0, 0);
	FileAddBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	FileAddBtn->GAlign = {false, true, true, false};
	FileRemoveBtn = SXGUICrButton("Remove selected", 108, 144, 100, 20, 0, m_pRoot->GetHWND(), 0, 0);
	FileRemoveBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	FileRemoveBtn->GAlign = {false, true, true, false};

	Show(false);
}

TabFiles::~TabFiles()
{
	mem_delete(FileList);
	mem_delete(FileAddBtn);
	mem_delete(FileRemoveBtn);
}
