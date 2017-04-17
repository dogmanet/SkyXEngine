#include "TabSkins.h"

TabSkins::TabSkins(TabManager * tm):EditorTab(tm)
{
	m_pTabBtn = SXGUICrButtonEx("Skins", 379, 648, 100, 20, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pTabBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pTabBtn->GAlign = {false, true, true, false};
	m_pTabBtn->SetUserPtr(this);
	m_pTabBtn->AddHandler(EditorTab::TabBtnHandler, WM_LBUTTONUP);
	

	m_pRoot = SXGUICrGroupBox("Skins", 279, 480, 1016, 167, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pRoot->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pRoot->SetColorText(0, 0, 0);
	m_pRoot->SetColorTextBk(255, 255, 255);
	m_pRoot->SetTransparentTextBk(true);
	m_pRoot->SetColorBrush(255, 255, 255);
	m_pRoot->GAlign = {false, true, true, true};
	SkinsList = SXGUICrListBox("", 7, 16, 335, 124, m_pRoot->GetHWND(), 0, 0, false);
	SkinsList->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinsList->SetColorText(0, 0, 0);
	SkinsList->SetColorTextBk(255, 255, 255);
	SkinsList->SetTransparentTextBk(true);
	SkinsList->SetColorBrush(255, 255, 255);
	SkinsList->GAlign = {1, 1, 1, 0};
	SkinNewBtn = SXGUICrButton("New", 7, 143, 79, 20, 0, m_pRoot->GetHWND(), 0, 0);
	SkinNewBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinNewBtn->GAlign = {1, 1, 1, 0};
	SkinRenameBtn = SXGUICrButton("Rename", 90, 143, 78, 20, 0, m_pRoot->GetHWND(), 0, 0);
	SkinRenameBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinRenameBtn->GAlign = {1, 1, 1, 0};
	SkinRemoveBtn = SXGUICrButton("Remove", 174, 143, 77, 20, 0, m_pRoot->GetHWND(), 0, 0);
	SkinRemoveBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinRemoveBtn->GAlign = {1, 1, 1, 0};
	SkinDuplicateBtn = SXGUICrButton("Duplicate", 256, 143, 85, 20, 0, m_pRoot->GetHWND(), 0, 0);
	SkinDuplicateBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinDuplicateBtn->GAlign = {1, 1, 1, 0};
	SkinMatsGB = SXGUICrGroupBox("Materials", 347, 16, 661, 143, m_pRoot->GetHWND(), 0, 0);
	SkinMatsGB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinMatsGB->SetColorText(0, 0, 0);
	SkinMatsGB->SetColorTextBk(255, 255, 255);
	SkinMatsGB->SetTransparentTextBk(true);
	SkinMatsGB->SetColorBrush(255, 255, 255);
	SkinMatsGB->GAlign = {1, 1, 1, 1};
	SkinMatsList = SXGUICrListBox("", 9, 15, 294, 121, SkinMatsGB->GetHWND(), 0, 0, false);
	SkinMatsList->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinMatsList->SetColorText(0, 0, 0);
	SkinMatsList->SetColorTextBk(255, 255, 255);
	SkinMatsList->SetTransparentTextBk(true);
	SkinMatsList->SetColorBrush(255, 255, 255);
	SkinMatsList->GAlign = {1, 1, 1, 0};
	SkinMatPropsGB = SXGUICrGroupBox("Props", 310, 15, 342, 121, SkinMatsGB->GetHWND(), 0, 0);
	SkinMatPropsGB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinMatPropsGB->SetColorText(0, 0, 0);
	SkinMatPropsGB->SetColorTextBk(255, 255, 255);
	SkinMatPropsGB->SetTransparentTextBk(true);
	SkinMatPropsGB->SetColorBrush(255, 255, 255);
	SkinMatPropsGB->GAlign = {1, 1, 1, 1};
	SkinMatFile = SXGUICrEdit("Material", 10, 40, 288, 20, SkinMatPropsGB->GetHWND(), 0, 0);
	SkinMatFile->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinMatFile->SetColorText(0, 0, 0);
	SkinMatFile->SetColorTextBk(255, 255, 255);
	SkinMatFile->SetTransparentTextBk(true);
	SkinMatFile->SetColorBrush(255, 255, 255);
	SkinMatFile->GAlign = {1, 1, 0, 0};
	Static6 = SXGUICrStatic("Material", 10, 17, 100, 20, SkinMatPropsGB->GetHWND(), 0, 0);
	Static6->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static6->SetColorText(0, 0, 0);
	Static6->SetColorTextBk(255, 255, 255);
	Static6->SetTransparentTextBk(true);
	Static6->SetColorBrush(255, 255, 255);
	Static6->GAlign = {1, 1, 0, 0};
	SkinMatBrowse = SXGUICrButton("...", 302, 39, 31, 20, 0, SkinMatPropsGB->GetHWND(), 0, 0);
	SkinMatBrowse->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinMatBrowse->GAlign = {1, 1, 0, 0};
	SkinMatReset = SXGUICrButton("Reset to default", 234, 92, 100, 20, 0, SkinMatPropsGB->GetHWND(), 0, 0);
	SkinMatReset->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SkinMatReset->GAlign = {0, 0, 1, 1};

	Show(false);
	m_pTabBtn->Enable(0);

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
