
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "TabControllers.h"

TabControllers::TabControllers(TabManager * tm):EditorTab(tm)
{
	m_pTabBtn = SXGUICrButtonEx("Controllers", 679, 648, 100, 20, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, m_pTM->GetBaseWnd()->getHWND(), 0, 0);
	m_pTabBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pTabBtn->setFollowParentSides(false, true, true, false);
	m_pTabBtn->setUserPtr(this);
	m_pTabBtn->addHandler(EditorTab::TabBtnHandler, WM_LBUTTONUP);

	m_pRoot = SXGUICrGroupBox("Controllers", 279, 480, 1016, 167, m_pTM->GetBaseWnd()->getHWND(), 0, 0);
	m_pRoot->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pRoot->setColorText(RGB(0, 0, 0));
	m_pRoot->setColorTextBk(RGB(255, 255, 255));
	m_pRoot->setTransparentTextBk(true);
	m_pRoot->setColorBrush(RGB(255, 255, 255));
	m_pRoot->setFollowParentSides(0, 1, 1, 1);

	CtlsList = SXGUICrListBox(8, 16, 150, 145, m_pRoot->getHWND(), 0, 0, false);
	CtlsList->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsList->setColorText(RGB(0, 0, 0));
	CtlsList->setColorTextBk(RGB(255, 255, 255));
	CtlsList->setTransparentTextBk(true);
	CtlsList->setColorBrush(RGB(255, 255, 255));
	CtlsList->setFollowParentSides(1, 1, 1, 0);

	CtlsNewBtn = SXGUICrButton("New", 8, 143, 50, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, 0);
	CtlsNewBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsNewBtn->setFollowParentSides(0, 1, 1, 0);

	CtlsRenameBtn = SXGUICrButton("Rename", 58, 143, 50, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, 0);
	CtlsRenameBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsRenameBtn->setFollowParentSides(0, 1, 1, 0);

	CtlsRemove = SXGUICrButton("Remove", 108, 143, 50, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, 0);
	CtlsRemove->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsRemove->setFollowParentSides(0, 1, 1, 0);

	m_pstBones = SXGUICrStatic("Bones", 160, 16, 50, 16, m_pRoot->getHWND(), 0, 0);

	//CtlsBonesGB = SXGUICrGroupBox("Bones", 160, 16, 298, 145, m_pRoot->getHWND(), 0, 0);
	//CtlsBonesGB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	//CtlsBonesGB->setColorText(RGB(0, 0, 0));
	//CtlsBonesGB->setColorTextBk(RGB(255, 255, 255));
	//CtlsBonesGB->setTransparentTextBk(true);
	//CtlsBonesGB->setColorBrush(RGB(255, 255, 255));
	//CtlsBonesGB->setFollowParentSides(1, 1, 0, 0);

	CtlsBoneList = SXGUICrListBox(160, 29, 150, 114, m_pRoot->getHWND(), 0, 0, false);
	CtlsBoneList->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsBoneList->setColorText(RGB(0, 0, 0));
	CtlsBoneList->setColorTextBk(RGB(255, 255, 255));
	CtlsBoneList->setTransparentTextBk(true);
	CtlsBoneList->setColorBrush(RGB(255, 255, 255));
	CtlsBoneList->setFollowParentSides(1, 1, 1, 0);

	CtlsBoneAddBtn = SXGUICrButton("Add", 160, 143, 75, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, 0);
	CtlsBoneAddBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsBoneAddBtn->setFollowParentSides(0, 1, 1, 0);

	CtlsBoneRemoveBtn = SXGUICrButton("Remove", 235, 143, 75, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, 0);
	CtlsBoneRemoveBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsBoneRemoveBtn->setFollowParentSides(0, 1, 1, 0);


	CtlsPropsGB = SXGUICrGroupBox("Props", 315, 15, 693, 146, m_pRoot->getHWND(), 0, 0);
	CtlsPropsGB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropsGB->setColorText(RGB(0, 0, 0));
	CtlsPropsGB->setColorTextBk(RGB(255, 255, 255));
	CtlsPropsGB->setTransparentTextBk(true);
	CtlsPropsGB->setColorBrush(RGB(255, 255, 255));
	CtlsPropsGB->setFollowParentSides(1, 1, 0, 0);
	CtlsPropCurTrack = SXGUICrTrackBar("", 7, 16, 286, 20, CtlsPropsGB->getHWND(), 0, 0);
	CtlsPropCurTrack->setFollowParentSides(1, 1, 0, 0);
	CtlsPropCur = SXGUICrEdit("1.0", 297, 16, 84, 20, CtlsPropsGB->getHWND(), 0, 0);
	CtlsPropCur->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropCur->setColorText(RGB(0, 0, 0));
	CtlsPropCur->setColorTextBk(RGB(255, 255, 255));
	CtlsPropCur->setTransparentTextBk(true);
	CtlsPropCur->setColorBrush(RGB(255, 255, 255));
	CtlsPropCur->setFollowParentSides(1, 1, 0, 0);


	CtlsPropTranslationGB = SXGUICrGroupBox("Translation From", 6, 38, 316, 50, CtlsPropsGB->getHWND(), 0, 0);
	CtlsPropTranslationGB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTranslationGB->setColorText(RGB(0, 0, 0));
	CtlsPropTranslationGB->setColorTextBk(RGB(255, 255, 255));
	CtlsPropTranslationGB->setTransparentTextBk(true);
	CtlsPropTranslationGB->setColorBrush(RGB(255, 255, 255));
	CtlsPropTranslationGB->setFollowParentSides(1, 1, 0, 0);

	CtlsPropTrXCB = SXGUICrCheckBox("x", 6, 19, 30, 20, CtlsPropTranslationGB->getHWND(), 0, 0, false);
	CtlsPropTrXCB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrXCB->setColorText(RGB(0, 0, 0));
	CtlsPropTrXCB->setColorTextBk(RGB(255, 255, 255));
	CtlsPropTrXCB->setTransparentTextBk(true);
	CtlsPropTrXCB->setColorBrush(RGB(255, 255, 255));
	CtlsPropTrXCB->setFollowParentSides(1, 1, 0, 0);
	CtlsPropTrYCB = SXGUICrCheckBox("y", 109, 19, 30, 20, CtlsPropTranslationGB->getHWND(), 0, 0, false);
	CtlsPropTrYCB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrYCB->setColorText(RGB(0, 0, 0));
	CtlsPropTrYCB->setColorTextBk(RGB(255, 255, 255));
	CtlsPropTrYCB->setTransparentTextBk(true);
	CtlsPropTrYCB->setColorBrush(RGB(255, 255, 255));
	CtlsPropTrYCB->setFollowParentSides(1, 1, 0, 0);
	CtlsPropTrZCB = SXGUICrCheckBox("z", 212, 19, 30, 20, CtlsPropTranslationGB->getHWND(), 0, 0, false);
	CtlsPropTrZCB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrZCB->setColorText(RGB(0, 0, 0));
	CtlsPropTrZCB->setColorTextBk(RGB(255, 255, 255));
	CtlsPropTrZCB->setTransparentTextBk(true);
	CtlsPropTrZCB->setColorBrush(RGB(255, 255, 255));
	CtlsPropTrZCB->setFollowParentSides(1, 1, 0, 0);

	CtlsPropTrX = SXGUICrEdit("0.0", 33, 19, 72, 20, CtlsPropTranslationGB->getHWND(), 0, 0);
	CtlsPropTrX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrX->setColorText(RGB(0, 0, 0));
	CtlsPropTrX->setColorTextBk(RGB(255, 255, 255));
	CtlsPropTrX->setTransparentTextBk(true);
	CtlsPropTrX->setColorBrush(RGB(255, 255, 255));
	CtlsPropTrX->setFollowParentSides(1, 1, 0, 0);
	CtlsPropTrY = SXGUICrEdit("0.0", 137, 19, 72, 20, CtlsPropTranslationGB->getHWND(), 0, 0);
	CtlsPropTrY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrY->setColorText(RGB(0, 0, 0));
	CtlsPropTrY->setColorTextBk(RGB(255, 255, 255));
	CtlsPropTrY->setTransparentTextBk(true);
	CtlsPropTrY->setColorBrush(RGB(255, 255, 255));
	CtlsPropTrY->setFollowParentSides(1, 1, 0, 0);
	CtlsPropTrZ = SXGUICrEdit("0.0", 242, 19, 72, 20, CtlsPropTranslationGB->getHWND(), 0, 0);
	CtlsPropTrZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrZ->setColorText(RGB(0, 0, 0));
	CtlsPropTrZ->setColorTextBk(RGB(255, 255, 255));
	CtlsPropTrZ->setTransparentTextBk(true);
	CtlsPropTrZ->setColorBrush(RGB(255, 255, 255));
	CtlsPropTrZ->setFollowParentSides(1, 1, 0, 0);


	CtlsPropTrXSpin = SXGUICrUpDownEx(105, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropTranslationGB->getHWND(), 0, 0, CtlsPropTrX->getHWND());
	CtlsPropTrYSpin = SXGUICrUpDownEx(354, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropTranslationGB->getHWND(), 0, 0, CtlsPropTrY->getHWND());
	CtlsPropTrZSpin = SXGUICrUpDownEx(229, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropTranslationGB->getHWND(), 0, 0, CtlsPropTrZ->getHWND());
	CtlsPropTrXSpin->setFollowParentSides(1, 1, 0, 0);
	CtlsPropTrYSpin->setFollowParentSides(1, 1, 0, 0);
	CtlsPropTrZSpin->setFollowParentSides(1, 1, 0, 0);
	//6, 89, 316, 50


	CtlsPropTranslationGB2 = SXGUICrGroupBox("Translation To", 6, 89, 316, 50, CtlsPropsGB->getHWND(), 0, 0);
	CtlsPropTranslationGB2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTranslationGB2->setColorText(RGB(0, 0, 0));
	CtlsPropTranslationGB2->setColorTextBk(RGB(255, 255, 255));
	CtlsPropTranslationGB2->setTransparentTextBk(true);
	CtlsPropTranslationGB2->setColorBrush(RGB(255, 255, 255));
	CtlsPropTranslationGB2->setFollowParentSides(1, 1, 0, 0);

	CtlsPropTrXCB2 = SXGUICrCheckBox("x", 6, 19, 30, 20, CtlsPropTranslationGB2->getHWND(), 0, 0, false);
	CtlsPropTrXCB2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrXCB2->setColorText(RGB(0, 0, 0));
	CtlsPropTrXCB2->setColorTextBk(RGB(255, 255, 255));
	CtlsPropTrXCB2->setTransparentTextBk(true);
	CtlsPropTrXCB2->setColorBrush(RGB(255, 255, 255));
	CtlsPropTrXCB2->setFollowParentSides(1, 1, 0, 0);
	CtlsPropTrXCB2->setEnable(0);
	CtlsPropTrYCB2 = SXGUICrCheckBox("y", 109, 19, 30, 20, CtlsPropTranslationGB2->getHWND(), 0, 0, false);
	CtlsPropTrYCB2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrYCB2->setColorText(RGB(0, 0, 0));
	CtlsPropTrYCB2->setColorTextBk(RGB(255, 255, 255));
	CtlsPropTrYCB2->setTransparentTextBk(true);
	CtlsPropTrYCB2->setColorBrush(RGB(255, 255, 255));
	CtlsPropTrYCB2->setFollowParentSides(1, 1, 0, 0);
	CtlsPropTrYCB2->setEnable(0);
	CtlsPropTrZCB2 = SXGUICrCheckBox("z", 212, 19, 30, 20, CtlsPropTranslationGB2->getHWND(), 0, 0, false);
	CtlsPropTrZCB2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrZCB2->setColorText(RGB(0, 0, 0));
	CtlsPropTrZCB2->setColorTextBk(RGB(255, 255, 255));
	CtlsPropTrZCB2->setTransparentTextBk(true);
	CtlsPropTrZCB2->setColorBrush(RGB(255, 255, 255));
	CtlsPropTrZCB2->setFollowParentSides(1, 1, 0, 0);
	CtlsPropTrZCB2->setEnable(0);

	CtlsPropTrX2 = SXGUICrEdit("0.0", 33, 19, 72, 20, CtlsPropTranslationGB2->getHWND(), 0, 0);
	CtlsPropTrX2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrX2->setColorText(RGB(0, 0, 0));
	CtlsPropTrX2->setColorTextBk(RGB(255, 255, 255));
	CtlsPropTrX2->setTransparentTextBk(true);
	CtlsPropTrX2->setColorBrush(RGB(255, 255, 255));
	CtlsPropTrX2->setFollowParentSides(1, 1, 0, 0);
	CtlsPropTrY2 = SXGUICrEdit("0.0", 137, 19, 72, 20, CtlsPropTranslationGB2->getHWND(), 0, 0);
	CtlsPropTrY2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrY2->setColorText(RGB(0, 0, 0));
	CtlsPropTrY2->setColorTextBk(RGB(255, 255, 255));
	CtlsPropTrY2->setTransparentTextBk(true);
	CtlsPropTrY2->setColorBrush(RGB(255, 255, 255));
	CtlsPropTrY2->setFollowParentSides(1, 1, 0, 0);
	CtlsPropTrZ2 = SXGUICrEdit("0.0", 242, 19, 72, 20, CtlsPropTranslationGB2->getHWND(), 0, 0);
	CtlsPropTrZ2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrZ2->setColorText(RGB(0, 0, 0));
	CtlsPropTrZ2->setColorTextBk(RGB(255, 255, 255));
	CtlsPropTrZ2->setTransparentTextBk(true);
	CtlsPropTrZ2->setColorBrush(RGB(255, 255, 255));
	CtlsPropTrZ2->setFollowParentSides(1, 1, 0, 0);


	CtlsPropTrXSpin2 = SXGUICrUpDownEx(105, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropTranslationGB2->getHWND(), 0, 0, CtlsPropTrX2->getHWND());
	CtlsPropTrYSpin2 = SXGUICrUpDownEx(354, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropTranslationGB2->getHWND(), 0, 0, CtlsPropTrY2->getHWND());
	CtlsPropTrZSpin2 = SXGUICrUpDownEx(229, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropTranslationGB2->getHWND(), 0, 0, CtlsPropTrZ2->getHWND());
	CtlsPropTrXSpin2->setFollowParentSides(1, 1, 0, 0);
	CtlsPropTrYSpin2->setFollowParentSides(1, 1, 0, 0);
	CtlsPropTrZSpin2->setFollowParentSides(1, 1, 0, 0);



	CtlsPropRotationGB = SXGUICrGroupBox("Rotation From", 322, 38, 316, 50, CtlsPropsGB->getHWND(), 0, 0);
	CtlsPropRotationGB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotationGB->setColorText(RGB(0, 0, 0));
	CtlsPropRotationGB->setColorTextBk(RGB(255, 255, 255));
	CtlsPropRotationGB->setTransparentTextBk(true);
	CtlsPropRotationGB->setColorBrush(RGB(255, 255, 255));
	CtlsPropRotationGB->setFollowParentSides(1, 1, 0, 0);

	CtlsPropRotXCB = SXGUICrCheckBox("x", 6, 19, 30, 20, CtlsPropRotationGB->getHWND(), 0, 0, false);
	CtlsPropRotXCB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotXCB->setColorText(RGB(0, 0, 0));
	CtlsPropRotXCB->setColorTextBk(RGB(255, 255, 255));
	CtlsPropRotXCB->setTransparentTextBk(true);
	CtlsPropRotXCB->setColorBrush(RGB(255, 255, 255));
	CtlsPropRotXCB->setFollowParentSides(1, 1, 0, 0);
	CtlsPropRotYCB = SXGUICrCheckBox("y", 109, 19, 30, 20, CtlsPropRotationGB->getHWND(), 0, 0, false);
	CtlsPropRotYCB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotYCB->setColorText(RGB(0, 0, 0));
	CtlsPropRotYCB->setColorTextBk(RGB(255, 255, 255));
	CtlsPropRotYCB->setTransparentTextBk(true);
	CtlsPropRotYCB->setColorBrush(RGB(255, 255, 255));
	CtlsPropRotYCB->setFollowParentSides(1, 1, 0, 0);
	CtlsPropRotZCB = SXGUICrCheckBox("z", 212, 19, 30, 20, CtlsPropRotationGB->getHWND(), 0, 0, false);
	CtlsPropRotZCB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotZCB->setColorText(RGB(0, 0, 0));
	CtlsPropRotZCB->setColorTextBk(RGB(255, 255, 255));
	CtlsPropRotZCB->setTransparentTextBk(true);
	CtlsPropRotZCB->setColorBrush(RGB(255, 255, 255));
	CtlsPropRotZCB->setFollowParentSides(1, 1, 0, 0);

	CtlsPropRotX = SXGUICrEdit("0.0", 33, 19, 72, 20, CtlsPropRotationGB->getHWND(), 0, 0);
	CtlsPropRotX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotX->setColorText(RGB(0, 0, 0));
	CtlsPropRotX->setColorTextBk(RGB(255, 255, 255));
	CtlsPropRotX->setTransparentTextBk(true);
	CtlsPropRotX->setColorBrush(RGB(255, 255, 255));
	CtlsPropRotX->setFollowParentSides(1, 1, 0, 0);
	CtlsPropRotY = SXGUICrEdit("0.0", 137, 19, 72, 20, CtlsPropRotationGB->getHWND(), 0, 0);
	CtlsPropRotY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotY->setColorText(RGB(0, 0, 0));
	CtlsPropRotY->setColorTextBk(RGB(255, 255, 255));
	CtlsPropRotY->setTransparentTextBk(true);
	CtlsPropRotY->setColorBrush(RGB(255, 255, 255));
	CtlsPropRotY->setFollowParentSides(1, 1, 0, 0);
	CtlsPropRotZ = SXGUICrEdit("0.0", 242, 19, 72, 20, CtlsPropRotationGB->getHWND(), 0, 0);
	CtlsPropRotZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotZ->setColorText(RGB(0, 0, 0));
	CtlsPropRotZ->setColorTextBk(RGB(255, 255, 255));
	CtlsPropRotZ->setTransparentTextBk(true);
	CtlsPropRotZ->setColorBrush(RGB(255, 255, 255));
	CtlsPropRotZ->setFollowParentSides(1, 1, 0, 0);

	CtlsPropRotXSpin = SXGUICrUpDownEx(105, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropRotationGB->getHWND(), 0, 0, CtlsPropRotX->getHWND());
	CtlsPropRotYSpin = SXGUICrUpDownEx(229, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropRotationGB->getHWND(), 0, 0, CtlsPropRotY->getHWND());
	CtlsPropRotZSpin = SXGUICrUpDownEx(354, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropRotationGB->getHWND(), 0, 0, CtlsPropRotZ->getHWND());

	CtlsPropRotXSpin->setFollowParentSides(1, 1, 0, 0);
	CtlsPropRotYSpin->setFollowParentSides(1, 1, 0, 0);
	CtlsPropRotZSpin->setFollowParentSides(1, 1, 0, 0);




	CtlsPropRotationGB2 = SXGUICrGroupBox("Rotation To", 322, 89, 316, 50, CtlsPropsGB->getHWND(), 0, 0);
	CtlsPropRotationGB2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotationGB2->setColorText(RGB(0, 0, 0));
	CtlsPropRotationGB2->setColorTextBk(RGB(255, 255, 255));
	CtlsPropRotationGB2->setTransparentTextBk(true);
	CtlsPropRotationGB2->setColorBrush(RGB(255, 255, 255));
	CtlsPropRotationGB2->setFollowParentSides(1, 1, 0, 0);

	CtlsPropRotXCB2 = SXGUICrCheckBox("x", 6, 19, 30, 20, CtlsPropRotationGB2->getHWND(), 0, 0, false);
	CtlsPropRotXCB2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotXCB2->setColorText(RGB(0, 0, 0));
	CtlsPropRotXCB2->setColorTextBk(RGB(255, 255, 255));
	CtlsPropRotXCB2->setTransparentTextBk(true);
	CtlsPropRotXCB2->setColorBrush(RGB(255, 255, 255));
	CtlsPropRotXCB2->setFollowParentSides(1, 1, 0, 0);
	CtlsPropRotXCB2->setEnable(0);
	CtlsPropRotYCB2 = SXGUICrCheckBox("y", 109, 19, 30, 20, CtlsPropRotationGB2->getHWND(), 0, 0, false);
	CtlsPropRotYCB2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotYCB2->setColorText(RGB(0, 0, 0));
	CtlsPropRotYCB2->setColorTextBk(RGB(255, 255, 255));
	CtlsPropRotYCB2->setTransparentTextBk(true);
	CtlsPropRotYCB2->setColorBrush(RGB(255, 255, 255));
	CtlsPropRotYCB2->setFollowParentSides(1, 1, 0, 0);
	CtlsPropRotYCB2->setEnable(0);
	CtlsPropRotZCB2 = SXGUICrCheckBox("z", 212, 19, 30, 20, CtlsPropRotationGB2->getHWND(), 0, 0, false);
	CtlsPropRotZCB2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotZCB2->setColorText(RGB(0, 0, 0));
	CtlsPropRotZCB2->setColorTextBk(RGB(255, 255, 255));
	CtlsPropRotZCB2->setTransparentTextBk(true);
	CtlsPropRotZCB2->setColorBrush(RGB(255, 255, 255));
	CtlsPropRotZCB2->setFollowParentSides(1, 1, 0, 0);
	CtlsPropRotZCB2->setEnable(0);

	CtlsPropRotX2 = SXGUICrEdit("0.0", 33, 19, 72, 20, CtlsPropRotationGB2->getHWND(), 0, 0);
	CtlsPropRotX2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotX2->setColorText(RGB(0, 0, 0));
	CtlsPropRotX2->setColorTextBk(RGB(255, 255, 255));
	CtlsPropRotX2->setTransparentTextBk(true);
	CtlsPropRotX2->setColorBrush(RGB(255, 255, 255));
	CtlsPropRotX2->setFollowParentSides(1, 1, 0, 0);
	CtlsPropRotY2 = SXGUICrEdit("0.0", 137, 19, 72, 20, CtlsPropRotationGB2->getHWND(), 0, 0);
	CtlsPropRotY2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotY2->setColorText(RGB(0, 0, 0));
	CtlsPropRotY2->setColorTextBk(RGB(255, 255, 255));
	CtlsPropRotY2->setTransparentTextBk(true);
	CtlsPropRotY2->setColorBrush(RGB(255, 255, 255));
	CtlsPropRotY2->setFollowParentSides(1, 1, 0, 0);
	CtlsPropRotZ2 = SXGUICrEdit("0.0", 242, 19, 72, 20, CtlsPropRotationGB2->getHWND(), 0, 0);
	CtlsPropRotZ2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotZ2->setColorText(RGB(0, 0, 0));
	CtlsPropRotZ2->setColorTextBk(RGB(255, 255, 255));
	CtlsPropRotZ2->setTransparentTextBk(true);
	CtlsPropRotZ2->setColorBrush(RGB(255, 255, 255));
	CtlsPropRotZ2->setFollowParentSides(1, 1, 0, 0);

	CtlsPropRotXSpin2 = SXGUICrUpDownEx(105, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropRotationGB2->getHWND(), 0, 0, CtlsPropRotX2->getHWND());
	CtlsPropRotYSpin2 = SXGUICrUpDownEx(229, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropRotationGB2->getHWND(), 0, 0, CtlsPropRotY2->getHWND());
	CtlsPropRotZSpin2 = SXGUICrUpDownEx(354, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropRotationGB2->getHWND(), 0, 0, CtlsPropRotZ2->getHWND());

	CtlsPropRotXSpin2->setFollowParentSides(1, 1, 0, 0);
	CtlsPropRotYSpin2->setFollowParentSides(1, 1, 0, 0);
	CtlsPropRotZSpin2->setFollowParentSides(1, 1, 0, 0);

	Show(false);
	m_pTabBtn->setEnable(0);
}

TabControllers::~TabControllers()
{
	mem_delete(CtlsList);
	mem_delete(CtlsNewBtn);
	mem_delete(CtlsRenameBtn);
	mem_delete(CtlsRemove);
	//mem_delete(CtlsBonesGB);
	mem_delete(CtlsBoneList);
	mem_delete(CtlsBoneAddBtn);
	mem_delete(CtlsBoneRemoveBtn);
	mem_delete(CtlsPropsGB);
	mem_delete(CtlsPropCurTrack);
	mem_delete(CtlsPropCur);

	mem_delete(CtlsPropTranslationGB);
	mem_delete(CtlsPropTrXCB);
	mem_delete(CtlsPropTrYCB);
	mem_delete(CtlsPropTrZCB);
	mem_delete(CtlsPropTrX);
	mem_delete(CtlsPropTrY);
	mem_delete(CtlsPropTrZ);
	mem_delete(CtlsPropTrXSpin);
	mem_delete(CtlsPropTrYSpin);
	mem_delete(CtlsPropTrZSpin);

	mem_delete(CtlsPropRotationGB);
	mem_delete(CtlsPropRotXCB);
	mem_delete(CtlsPropRotYCB);
	mem_delete(CtlsPropRotZCB);
	mem_delete(CtlsPropRotX);
	mem_delete(CtlsPropRotY);
	mem_delete(CtlsPropRotZ);
	mem_delete(CtlsPropRotXSpin);
	mem_delete(CtlsPropRotYSpin);
	mem_delete(CtlsPropRotZSpin);

	mem_delete(CtlsPropTranslationGB2);
	mem_delete(CtlsPropTrXCB2);
	mem_delete(CtlsPropTrYCB2);
	mem_delete(CtlsPropTrZCB2);
	mem_delete(CtlsPropTrX2);
	mem_delete(CtlsPropTrY2);
	mem_delete(CtlsPropTrZ2);
	mem_delete(CtlsPropTrXSpin2);
	mem_delete(CtlsPropTrYSpin2);
	mem_delete(CtlsPropTrZSpin2);

	mem_delete(CtlsPropRotationGB2);
	mem_delete(CtlsPropRotXCB2);
	mem_delete(CtlsPropRotYCB2);
	mem_delete(CtlsPropRotZCB2);
	mem_delete(CtlsPropRotX2);
	mem_delete(CtlsPropRotY2);
	mem_delete(CtlsPropRotZ2);
	mem_delete(CtlsPropRotXSpin2);
	mem_delete(CtlsPropRotYSpin2);
	mem_delete(CtlsPropRotZSpin2);

	mem_delete(m_pstBones);
}
