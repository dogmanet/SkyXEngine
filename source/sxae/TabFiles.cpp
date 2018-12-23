
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "TabFiles.h"

TabFiles::TabFiles(TabManager * tm):EditorTab(tm)
{
	m_pTabBtn = SXGUICrButtonEx("Files", 780, 648, 100, 20, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, m_pTM->GetBaseWnd()->getHWND(), 0, 0);
	m_pTabBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pTabBtn->setFollowParentSides(false, true, true, false);
	m_pTabBtn->setUserPtr(this);
	m_pTabBtn->addHandler(EditorTab::TabBtnHandler, WM_LBUTTONUP);

	m_pRoot = SXGUICrGroupBox("Files", 279, 480, 1016, 167, m_pTM->GetBaseWnd()->getHWND(), 0, 0);
	m_pRoot->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pRoot->setColorText(RGB(0, 0, 0));
	m_pRoot->setColorTextBk(RGB(255, 255, 255));
	m_pRoot->setTransparentTextBk(true);
	m_pRoot->setColorBrush(RGB(255, 255, 255));
	m_pRoot->setFollowParentSides(false, true, true, true);
	FileList = SXGUICrListBox(6, 15, 1002, 140, m_pRoot->getHWND(), 0, 0, false);
	FileList->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	FileList->setColorText(RGB(0, 0, 0));
	FileList->setColorTextBk(RGB(255, 255, 255));
	FileList->setTransparentTextBk(true);
	FileList->setColorBrush(RGB(255, 255, 255));
	FileList->setFollowParentSides(true, true, true, true);
	FileAddBtn = SXGUICrButton("Add file", 6, 144, 100, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, 0);
	FileAddBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	FileAddBtn->setFollowParentSides(false, true, true, false);
	FileRemoveBtn = SXGUICrButton("Remove selected", 108, 144, 100, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, 0);
	FileRemoveBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	FileRemoveBtn->setFollowParentSides(false, true, true, false);

	Show(false);
}

TabFiles::~TabFiles()
{
	mem_delete(FileList);
	mem_delete(FileAddBtn);
	mem_delete(FileRemoveBtn);
}
