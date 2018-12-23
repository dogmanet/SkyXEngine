
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "TabSkins.h"

TabSkins::TabSkins(TabManager * tm):EditorTab(tm)
{
	m_pTabBtn = SXGUICrButtonEx("Skins", 379, 648, 100, 20, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, m_pTM->GetBaseWnd()->getHWND(), 0, 0);
	m_pTabBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pTabBtn->setFollowParentSides(false, true, true, false);
	m_pTabBtn->setUserPtr(this);
	m_pTabBtn->addHandler(EditorTab::TabBtnHandler, WM_LBUTTONUP);
	

	m_pRoot = SXGUICrGroupBox("Skins", 279, 480, 1016, 167, m_pTM->GetBaseWnd()->getHWND(), 0, 0);
	m_pRoot->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pRoot->setColorText(RGB(0, 0, 0));
	m_pRoot->setColorTextBk(RGB(255, 255, 255));
	m_pRoot->setTransparentTextBk(true);
	m_pRoot->setColorBrush(RGB(255, 255, 255));
	m_pRoot->setFollowParentSides(false, true, true, true);
	SkinsList = SXGUICrListBox(7, 16, 335, 124, m_pRoot->getHWND(), 0, 0, false);
	SkinsList->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinsList->setColorText(RGB(0, 0, 0));
	SkinsList->setColorTextBk(RGB(255, 255, 255));
	SkinsList->setTransparentTextBk(true);
	SkinsList->setColorBrush(RGB(255, 255, 255));
	SkinsList->setFollowParentSides(1, 1, 1, 0);
	SkinNewBtn = SXGUICrButton("New", 7, 143, 79, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, 0);
	SkinNewBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinNewBtn->setFollowParentSides(1, 1, 1, 0);
	SkinRenameBtn = SXGUICrButton("Rename", 90, 143, 78, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, 0);
	SkinRenameBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinRenameBtn->setFollowParentSides(1, 1, 1, 0);
	SkinRemoveBtn = SXGUICrButton("Remove", 174, 143, 77, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, 0);
	SkinRemoveBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinRemoveBtn->setFollowParentSides(1, 1, 1, 0);
	SkinDuplicateBtn = SXGUICrButton("Duplicate", 256, 143, 85, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, 0);
	SkinDuplicateBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinDuplicateBtn->setFollowParentSides(1, 1, 1, 0);
	SkinMatsGB = SXGUICrGroupBox("Materials", 347, 16, 661, 143, m_pRoot->getHWND(), 0, 0);
	SkinMatsGB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinMatsGB->setColorText(RGB(0, 0, 0));
	SkinMatsGB->setColorTextBk(RGB(255, 255, 255));
	SkinMatsGB->setTransparentTextBk(true);
	SkinMatsGB->setColorBrush(RGB(255, 255, 255));
	SkinMatsGB->setFollowParentSides(1, 1, 1, 1);
	SkinMatsList = SXGUICrListBox(9, 15, 294, 121, SkinMatsGB->getHWND(), 0, 0, false);
	SkinMatsList->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinMatsList->setColorText(RGB(0, 0, 0));
	SkinMatsList->setColorTextBk(RGB(255, 255, 255));
	SkinMatsList->setTransparentTextBk(true);
	SkinMatsList->setColorBrush(RGB(255, 255, 255));
	SkinMatsList->setFollowParentSides(1, 1, 1, 0);
	SkinMatPropsGB = SXGUICrGroupBox("Props", 310, 15, 342, 121, SkinMatsGB->getHWND(), 0, 0);
	SkinMatPropsGB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinMatPropsGB->setColorText(RGB(0, 0, 0));
	SkinMatPropsGB->setColorTextBk(RGB(255, 255, 255));
	SkinMatPropsGB->setTransparentTextBk(true);
	SkinMatPropsGB->setColorBrush(RGB(255, 255, 255));
	SkinMatPropsGB->setFollowParentSides(1, 1, 1, 1);
	SkinMatFile = SXGUICrEdit("Material", 10, 40, 288, 20, SkinMatPropsGB->getHWND(), 0, 0);
	SkinMatFile->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinMatFile->setColorText(RGB(0, 0, 0));
	SkinMatFile->setColorTextBk(RGB(255, 255, 255));
	SkinMatFile->setTransparentTextBk(true);
	SkinMatFile->setColorBrush(RGB(255, 255, 255));
	SkinMatFile->setFollowParentSides(1, 1, 0, 0);
	Static6 = SXGUICrStatic("Material", 10, 17, 100, 20, SkinMatPropsGB->getHWND(), 0, 0);
	Static6->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static6->setColorText(RGB(0, 0, 0));
	Static6->setColorTextBk(RGB(255, 255, 255));
	Static6->setTransparentTextBk(true);
	Static6->setColorBrush(RGB(255, 255, 255));
	Static6->setFollowParentSides(1, 1, 0, 0);
	SkinMatBrowse = SXGUICrButton("...", 302, 39, 31, 20, SXGUI_BUTTON_IMAGE_NONE, SkinMatPropsGB->getHWND(), 0, 0);
	SkinMatBrowse->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinMatBrowse->setFollowParentSides(1, 1, 0, 0);
	SkinMatReset = SXGUICrButton("Reset to default", 234, 92, 100, 20, SXGUI_BUTTON_IMAGE_NONE, SkinMatPropsGB->getHWND(), 0, 0);
	SkinMatReset->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinMatReset->setFollowParentSides(0, 0, 1, 1);

	Show(false);
	m_pTabBtn->setEnable(0);

}

TabSkins::~TabSkins()
{
	mem_delete(SkinsList);
	mem_delete(SkinNewBtn);
	mem_delete(SkinRenameBtn);
	mem_delete(SkinRemoveBtn);
	mem_delete(SkinDuplicateBtn);
	mem_delete(SkinMatsGB);
	mem_delete(SkinMatsList);
	mem_delete(SkinMatPropsGB);
	mem_delete(SkinMatFile);
	mem_delete(Static6);
	mem_delete(SkinMatBrowse);
	mem_delete(SkinMatReset);
}
