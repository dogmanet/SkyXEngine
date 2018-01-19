
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "TabControllers.h"

TabControllers::TabControllers(TabManager * tm):EditorTab(tm)
{
	m_pTabBtn = SXGUICrButtonEx("Controllers", 679, 648, 100, 20, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pTabBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pTabBtn->GAlign = {false, true, true, false};
	m_pTabBtn->SetUserPtr(this);
	m_pTabBtn->AddHandler(EditorTab::TabBtnHandler, WM_LBUTTONUP);

	m_pRoot = SXGUICrGroupBox("Controllers", 279, 480, 1016, 167, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pRoot->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pRoot->SetColorText(0, 0, 0);
	m_pRoot->SetColorTextBk(255, 255, 255);
	m_pRoot->SetTransparentTextBk(true);
	m_pRoot->SetColorBrush(255, 255, 255);
	m_pRoot->GAlign = {0, 1, 1, 1};

	CtlsList = SXGUICrListBox("", 8, 16, 150, 145, m_pRoot->GetHWND(), 0, 0, false);
	CtlsList->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsList->SetColorText(0, 0, 0);
	CtlsList->SetColorTextBk(255, 255, 255);
	CtlsList->SetTransparentTextBk(true);
	CtlsList->SetColorBrush(255, 255, 255);
	CtlsList->GAlign = {1, 1, 1, 0};

	CtlsNewBtn = SXGUICrButton("New", 8, 143, 50, 20, 0, m_pRoot->GetHWND(), 0, 0);
	CtlsNewBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsNewBtn->GAlign = {0, 1, 1, 0};

	CtlsRenameBtn = SXGUICrButton("Rename", 58, 143, 50, 20, 0, m_pRoot->GetHWND(), 0, 0);
	CtlsRenameBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsRenameBtn->GAlign = {0, 1, 1, 0};

	CtlsRemove = SXGUICrButton("Remove", 108, 143, 50, 20, 0, m_pRoot->GetHWND(), 0, 0);
	CtlsRemove->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsRemove->GAlign = {0, 1, 1, 0};

	m_pstBones = SXGUICrStatic("Bones", 160, 16, 50, 16, m_pRoot->GetHWND(), 0, 0);

	//CtlsBonesGB = SXGUICrGroupBox("Bones", 160, 16, 298, 145, m_pRoot->GetHWND(), 0, 0);
	//CtlsBonesGB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	//CtlsBonesGB->SetColorText(0, 0, 0);
	//CtlsBonesGB->SetColorTextBk(255, 255, 255);
	//CtlsBonesGB->SetTransparentTextBk(true);
	//CtlsBonesGB->SetColorBrush(255, 255, 255);
	//CtlsBonesGB->GAlign = {1, 1, 0, 0};

	CtlsBoneList = SXGUICrListBox("", 160, 29, 150, 114, m_pRoot->GetHWND(), 0, 0, false);
	CtlsBoneList->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsBoneList->SetColorText(0, 0, 0);
	CtlsBoneList->SetColorTextBk(255, 255, 255);
	CtlsBoneList->SetTransparentTextBk(true);
	CtlsBoneList->SetColorBrush(255, 255, 255);
	CtlsBoneList->GAlign = {1, 1, 1, 0};

	CtlsBoneAddBtn = SXGUICrButton("Add", 160, 143, 75, 20, 0, m_pRoot->GetHWND(), 0, 0);
	CtlsBoneAddBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsBoneAddBtn->GAlign = {0, 1, 1, 0};

	CtlsBoneRemoveBtn = SXGUICrButton("Remove", 235, 143, 75, 20, 0, m_pRoot->GetHWND(), 0, 0);
	CtlsBoneRemoveBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsBoneRemoveBtn->GAlign = {0, 1, 1, 0};


	CtlsPropsGB = SXGUICrGroupBox("Props", 315, 15, 693, 146, m_pRoot->GetHWND(), 0, 0);
	CtlsPropsGB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropsGB->SetColorText(0, 0, 0);
	CtlsPropsGB->SetColorTextBk(255, 255, 255);
	CtlsPropsGB->SetTransparentTextBk(true);
	CtlsPropsGB->SetColorBrush(255, 255, 255);
	CtlsPropsGB->GAlign = {1, 1, 0, 0};
	CtlsPropCurTrack = SXGUICrTrackBar("", 7, 16, 286, 20, CtlsPropsGB->GetHWND(), 0, 0);
	CtlsPropCurTrack->GAlign = {1, 1, 0, 0};
	CtlsPropCur = SXGUICrEdit("1.0", 297, 16, 84, 20, CtlsPropsGB->GetHWND(), 0, 0);
	CtlsPropCur->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropCur->SetColorText(0, 0, 0);
	CtlsPropCur->SetColorTextBk(255, 255, 255);
	CtlsPropCur->SetTransparentTextBk(true);
	CtlsPropCur->SetColorBrush(255, 255, 255);
	CtlsPropCur->GAlign = {1, 1, 0, 0};


	CtlsPropTranslationGB = SXGUICrGroupBox("Translation From", 6, 38, 316, 50, CtlsPropsGB->GetHWND(), 0, 0);
	CtlsPropTranslationGB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTranslationGB->SetColorText(0, 0, 0);
	CtlsPropTranslationGB->SetColorTextBk(255, 255, 255);
	CtlsPropTranslationGB->SetTransparentTextBk(true);
	CtlsPropTranslationGB->SetColorBrush(255, 255, 255);
	CtlsPropTranslationGB->GAlign = {1, 1, 0, 0};

	CtlsPropTrXCB = SXGUICrCheckBox("x", 6, 19, 30, 20, CtlsPropTranslationGB->GetHWND(), 0, 0, false);
	CtlsPropTrXCB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrXCB->SetColorText(0, 0, 0);
	CtlsPropTrXCB->SetColorTextBk(255, 255, 255);
	CtlsPropTrXCB->SetTransparentTextBk(true);
	CtlsPropTrXCB->SetColorBrush(255, 255, 255);
	CtlsPropTrXCB->GAlign = {1, 1, 0, 0};
	CtlsPropTrYCB = SXGUICrCheckBox("y", 109, 19, 30, 20, CtlsPropTranslationGB->GetHWND(), 0, 0, false);
	CtlsPropTrYCB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrYCB->SetColorText(0, 0, 0);
	CtlsPropTrYCB->SetColorTextBk(255, 255, 255);
	CtlsPropTrYCB->SetTransparentTextBk(true);
	CtlsPropTrYCB->SetColorBrush(255, 255, 255);
	CtlsPropTrYCB->GAlign = {1, 1, 0, 0};
	CtlsPropTrZCB = SXGUICrCheckBox("z", 212, 19, 30, 20, CtlsPropTranslationGB->GetHWND(), 0, 0, false);
	CtlsPropTrZCB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrZCB->SetColorText(0, 0, 0);
	CtlsPropTrZCB->SetColorTextBk(255, 255, 255);
	CtlsPropTrZCB->SetTransparentTextBk(true);
	CtlsPropTrZCB->SetColorBrush(255, 255, 255);
	CtlsPropTrZCB->GAlign = {1, 1, 0, 0};

	CtlsPropTrX = SXGUICrEdit("0.0", 33, 19, 72, 20, CtlsPropTranslationGB->GetHWND(), 0, 0);
	CtlsPropTrX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrX->SetColorText(0, 0, 0);
	CtlsPropTrX->SetColorTextBk(255, 255, 255);
	CtlsPropTrX->SetTransparentTextBk(true);
	CtlsPropTrX->SetColorBrush(255, 255, 255);
	CtlsPropTrX->GAlign = {1, 1, 0, 0};
	CtlsPropTrY = SXGUICrEdit("0.0", 137, 19, 72, 20, CtlsPropTranslationGB->GetHWND(), 0, 0);
	CtlsPropTrY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrY->SetColorText(0, 0, 0);
	CtlsPropTrY->SetColorTextBk(255, 255, 255);
	CtlsPropTrY->SetTransparentTextBk(true);
	CtlsPropTrY->SetColorBrush(255, 255, 255);
	CtlsPropTrY->GAlign = {1, 1, 0, 0};
	CtlsPropTrZ = SXGUICrEdit("0.0", 242, 19, 72, 20, CtlsPropTranslationGB->GetHWND(), 0, 0);
	CtlsPropTrZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrZ->SetColorText(0, 0, 0);
	CtlsPropTrZ->SetColorTextBk(255, 255, 255);
	CtlsPropTrZ->SetTransparentTextBk(true);
	CtlsPropTrZ->SetColorBrush(255, 255, 255);
	CtlsPropTrZ->GAlign = {1, 1, 0, 0};


	CtlsPropTrXSpin = SXGUICrUpDownEx(105, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropTranslationGB->GetHWND(), 0, 0, CtlsPropTrX->GetHWND());
	CtlsPropTrYSpin = SXGUICrUpDownEx(354, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropTranslationGB->GetHWND(), 0, 0, CtlsPropTrY->GetHWND());
	CtlsPropTrZSpin = SXGUICrUpDownEx(229, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropTranslationGB->GetHWND(), 0, 0, CtlsPropTrZ->GetHWND());
	CtlsPropTrXSpin->GAlign = {1, 1, 0, 0};
	CtlsPropTrYSpin->GAlign = {1, 1, 0, 0};
	CtlsPropTrZSpin->GAlign = {1, 1, 0, 0};
	//6, 89, 316, 50


	CtlsPropTranslationGB2 = SXGUICrGroupBox("Translation To", 6, 89, 316, 50, CtlsPropsGB->GetHWND(), 0, 0);
	CtlsPropTranslationGB2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTranslationGB2->SetColorText(0, 0, 0);
	CtlsPropTranslationGB2->SetColorTextBk(255, 255, 255);
	CtlsPropTranslationGB2->SetTransparentTextBk(true);
	CtlsPropTranslationGB2->SetColorBrush(255, 255, 255);
	CtlsPropTranslationGB2->GAlign = {1, 1, 0, 0};

	CtlsPropTrXCB2 = SXGUICrCheckBox("x", 6, 19, 30, 20, CtlsPropTranslationGB2->GetHWND(), 0, 0, false);
	CtlsPropTrXCB2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrXCB2->SetColorText(0, 0, 0);
	CtlsPropTrXCB2->SetColorTextBk(255, 255, 255);
	CtlsPropTrXCB2->SetTransparentTextBk(true);
	CtlsPropTrXCB2->SetColorBrush(255, 255, 255);
	CtlsPropTrXCB2->GAlign = {1, 1, 0, 0};
	CtlsPropTrXCB2->Enable(0);
	CtlsPropTrYCB2 = SXGUICrCheckBox("y", 109, 19, 30, 20, CtlsPropTranslationGB2->GetHWND(), 0, 0, false);
	CtlsPropTrYCB2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrYCB2->SetColorText(0, 0, 0);
	CtlsPropTrYCB2->SetColorTextBk(255, 255, 255);
	CtlsPropTrYCB2->SetTransparentTextBk(true);
	CtlsPropTrYCB2->SetColorBrush(255, 255, 255);
	CtlsPropTrYCB2->GAlign = {1, 1, 0, 0};
	CtlsPropTrYCB2->Enable(0);
	CtlsPropTrZCB2 = SXGUICrCheckBox("z", 212, 19, 30, 20, CtlsPropTranslationGB2->GetHWND(), 0, 0, false);
	CtlsPropTrZCB2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrZCB2->SetColorText(0, 0, 0);
	CtlsPropTrZCB2->SetColorTextBk(255, 255, 255);
	CtlsPropTrZCB2->SetTransparentTextBk(true);
	CtlsPropTrZCB2->SetColorBrush(255, 255, 255);
	CtlsPropTrZCB2->GAlign = {1, 1, 0, 0};
	CtlsPropTrZCB2->Enable(0);

	CtlsPropTrX2 = SXGUICrEdit("0.0", 33, 19, 72, 20, CtlsPropTranslationGB2->GetHWND(), 0, 0);
	CtlsPropTrX2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrX2->SetColorText(0, 0, 0);
	CtlsPropTrX2->SetColorTextBk(255, 255, 255);
	CtlsPropTrX2->SetTransparentTextBk(true);
	CtlsPropTrX2->SetColorBrush(255, 255, 255);
	CtlsPropTrX2->GAlign = {1, 1, 0, 0};
	CtlsPropTrY2 = SXGUICrEdit("0.0", 137, 19, 72, 20, CtlsPropTranslationGB2->GetHWND(), 0, 0);
	CtlsPropTrY2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrY2->SetColorText(0, 0, 0);
	CtlsPropTrY2->SetColorTextBk(255, 255, 255);
	CtlsPropTrY2->SetTransparentTextBk(true);
	CtlsPropTrY2->SetColorBrush(255, 255, 255);
	CtlsPropTrY2->GAlign = {1, 1, 0, 0};
	CtlsPropTrZ2 = SXGUICrEdit("0.0", 242, 19, 72, 20, CtlsPropTranslationGB2->GetHWND(), 0, 0);
	CtlsPropTrZ2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropTrZ2->SetColorText(0, 0, 0);
	CtlsPropTrZ2->SetColorTextBk(255, 255, 255);
	CtlsPropTrZ2->SetTransparentTextBk(true);
	CtlsPropTrZ2->SetColorBrush(255, 255, 255);
	CtlsPropTrZ2->GAlign = {1, 1, 0, 0};


	CtlsPropTrXSpin2 = SXGUICrUpDownEx(105, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropTranslationGB2->GetHWND(), 0, 0, CtlsPropTrX2->GetHWND());
	CtlsPropTrYSpin2 = SXGUICrUpDownEx(354, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropTranslationGB2->GetHWND(), 0, 0, CtlsPropTrY2->GetHWND());
	CtlsPropTrZSpin2 = SXGUICrUpDownEx(229, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropTranslationGB2->GetHWND(), 0, 0, CtlsPropTrZ2->GetHWND());
	CtlsPropTrXSpin2->GAlign = {1, 1, 0, 0};
	CtlsPropTrYSpin2->GAlign = {1, 1, 0, 0};
	CtlsPropTrZSpin2->GAlign = {1, 1, 0, 0};



	CtlsPropRotationGB = SXGUICrGroupBox("Rotation From", 322, 38, 316, 50, CtlsPropsGB->GetHWND(), 0, 0);
	CtlsPropRotationGB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotationGB->SetColorText(0, 0, 0);
	CtlsPropRotationGB->SetColorTextBk(255, 255, 255);
	CtlsPropRotationGB->SetTransparentTextBk(true);
	CtlsPropRotationGB->SetColorBrush(255, 255, 255);
	CtlsPropRotationGB->GAlign = {1, 1, 0, 0};

	CtlsPropRotXCB = SXGUICrCheckBox("x", 6, 19, 30, 20, CtlsPropRotationGB->GetHWND(), 0, 0, false);
	CtlsPropRotXCB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotXCB->SetColorText(0, 0, 0);
	CtlsPropRotXCB->SetColorTextBk(255, 255, 255);
	CtlsPropRotXCB->SetTransparentTextBk(true);
	CtlsPropRotXCB->SetColorBrush(255, 255, 255);
	CtlsPropRotXCB->GAlign = {1, 1, 0, 0};
	CtlsPropRotYCB = SXGUICrCheckBox("y", 109, 19, 30, 20, CtlsPropRotationGB->GetHWND(), 0, 0, false);
	CtlsPropRotYCB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotYCB->SetColorText(0, 0, 0);
	CtlsPropRotYCB->SetColorTextBk(255, 255, 255);
	CtlsPropRotYCB->SetTransparentTextBk(true);
	CtlsPropRotYCB->SetColorBrush(255, 255, 255);
	CtlsPropRotYCB->GAlign = {1, 1, 0, 0};
	CtlsPropRotZCB = SXGUICrCheckBox("z", 212, 19, 30, 20, CtlsPropRotationGB->GetHWND(), 0, 0, false);
	CtlsPropRotZCB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotZCB->SetColorText(0, 0, 0);
	CtlsPropRotZCB->SetColorTextBk(255, 255, 255);
	CtlsPropRotZCB->SetTransparentTextBk(true);
	CtlsPropRotZCB->SetColorBrush(255, 255, 255);
	CtlsPropRotZCB->GAlign = {1, 1, 0, 0};

	CtlsPropRotX = SXGUICrEdit("0.0", 33, 19, 72, 20, CtlsPropRotationGB->GetHWND(), 0, 0);
	CtlsPropRotX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotX->SetColorText(0, 0, 0);
	CtlsPropRotX->SetColorTextBk(255, 255, 255);
	CtlsPropRotX->SetTransparentTextBk(true);
	CtlsPropRotX->SetColorBrush(255, 255, 255);
	CtlsPropRotX->GAlign = {1, 1, 0, 0};
	CtlsPropRotY = SXGUICrEdit("0.0", 137, 19, 72, 20, CtlsPropRotationGB->GetHWND(), 0, 0);
	CtlsPropRotY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotY->SetColorText(0, 0, 0);
	CtlsPropRotY->SetColorTextBk(255, 255, 255);
	CtlsPropRotY->SetTransparentTextBk(true);
	CtlsPropRotY->SetColorBrush(255, 255, 255);
	CtlsPropRotY->GAlign = {1, 1, 0, 0};
	CtlsPropRotZ = SXGUICrEdit("0.0", 242, 19, 72, 20, CtlsPropRotationGB->GetHWND(), 0, 0);
	CtlsPropRotZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotZ->SetColorText(0, 0, 0);
	CtlsPropRotZ->SetColorTextBk(255, 255, 255);
	CtlsPropRotZ->SetTransparentTextBk(true);
	CtlsPropRotZ->SetColorBrush(255, 255, 255);
	CtlsPropRotZ->GAlign = {1, 1, 0, 0};

	CtlsPropRotXSpin = SXGUICrUpDownEx(105, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropRotationGB->GetHWND(), 0, 0, CtlsPropRotX->GetHWND());
	CtlsPropRotYSpin = SXGUICrUpDownEx(229, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropRotationGB->GetHWND(), 0, 0, CtlsPropRotY->GetHWND());
	CtlsPropRotZSpin = SXGUICrUpDownEx(354, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropRotationGB->GetHWND(), 0, 0, CtlsPropRotZ->GetHWND());

	CtlsPropRotXSpin->GAlign = {1, 1, 0, 0};
	CtlsPropRotYSpin->GAlign = {1, 1, 0, 0};
	CtlsPropRotZSpin->GAlign = {1, 1, 0, 0};




	CtlsPropRotationGB2 = SXGUICrGroupBox("Rotation To", 322, 89, 316, 50, CtlsPropsGB->GetHWND(), 0, 0);
	CtlsPropRotationGB2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotationGB2->SetColorText(0, 0, 0);
	CtlsPropRotationGB2->SetColorTextBk(255, 255, 255);
	CtlsPropRotationGB2->SetTransparentTextBk(true);
	CtlsPropRotationGB2->SetColorBrush(255, 255, 255);
	CtlsPropRotationGB2->GAlign = {1, 1, 0, 0};

	CtlsPropRotXCB2 = SXGUICrCheckBox("x", 6, 19, 30, 20, CtlsPropRotationGB2->GetHWND(), 0, 0, false);
	CtlsPropRotXCB2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotXCB2->SetColorText(0, 0, 0);
	CtlsPropRotXCB2->SetColorTextBk(255, 255, 255);
	CtlsPropRotXCB2->SetTransparentTextBk(true);
	CtlsPropRotXCB2->SetColorBrush(255, 255, 255);
	CtlsPropRotXCB2->GAlign = {1, 1, 0, 0};
	CtlsPropRotXCB2->Enable(0);
	CtlsPropRotYCB2 = SXGUICrCheckBox("y", 109, 19, 30, 20, CtlsPropRotationGB2->GetHWND(), 0, 0, false);
	CtlsPropRotYCB2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotYCB2->SetColorText(0, 0, 0);
	CtlsPropRotYCB2->SetColorTextBk(255, 255, 255);
	CtlsPropRotYCB2->SetTransparentTextBk(true);
	CtlsPropRotYCB2->SetColorBrush(255, 255, 255);
	CtlsPropRotYCB2->GAlign = {1, 1, 0, 0};
	CtlsPropRotYCB2->Enable(0);
	CtlsPropRotZCB2 = SXGUICrCheckBox("z", 212, 19, 30, 20, CtlsPropRotationGB2->GetHWND(), 0, 0, false);
	CtlsPropRotZCB2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotZCB2->SetColorText(0, 0, 0);
	CtlsPropRotZCB2->SetColorTextBk(255, 255, 255);
	CtlsPropRotZCB2->SetTransparentTextBk(true);
	CtlsPropRotZCB2->SetColorBrush(255, 255, 255);
	CtlsPropRotZCB2->GAlign = {1, 1, 0, 0};
	CtlsPropRotZCB2->Enable(0);

	CtlsPropRotX2 = SXGUICrEdit("0.0", 33, 19, 72, 20, CtlsPropRotationGB2->GetHWND(), 0, 0);
	CtlsPropRotX2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotX2->SetColorText(0, 0, 0);
	CtlsPropRotX2->SetColorTextBk(255, 255, 255);
	CtlsPropRotX2->SetTransparentTextBk(true);
	CtlsPropRotX2->SetColorBrush(255, 255, 255);
	CtlsPropRotX2->GAlign = {1, 1, 0, 0};
	CtlsPropRotY2 = SXGUICrEdit("0.0", 137, 19, 72, 20, CtlsPropRotationGB2->GetHWND(), 0, 0);
	CtlsPropRotY2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotY2->SetColorText(0, 0, 0);
	CtlsPropRotY2->SetColorTextBk(255, 255, 255);
	CtlsPropRotY2->SetTransparentTextBk(true);
	CtlsPropRotY2->SetColorBrush(255, 255, 255);
	CtlsPropRotY2->GAlign = {1, 1, 0, 0};
	CtlsPropRotZ2 = SXGUICrEdit("0.0", 242, 19, 72, 20, CtlsPropRotationGB2->GetHWND(), 0, 0);
	CtlsPropRotZ2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CtlsPropRotZ2->SetColorText(0, 0, 0);
	CtlsPropRotZ2->SetColorTextBk(255, 255, 255);
	CtlsPropRotZ2->SetTransparentTextBk(true);
	CtlsPropRotZ2->SetColorBrush(255, 255, 255);
	CtlsPropRotZ2->GAlign = {1, 1, 0, 0};

	CtlsPropRotXSpin2 = SXGUICrUpDownEx(105, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropRotationGB2->GetHWND(), 0, 0, CtlsPropRotX2->GetHWND());
	CtlsPropRotYSpin2 = SXGUICrUpDownEx(229, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropRotationGB2->GetHWND(), 0, 0, CtlsPropRotY2->GetHWND());
	CtlsPropRotZSpin2 = SXGUICrUpDownEx(354, 19, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, CtlsPropRotationGB2->GetHWND(), 0, 0, CtlsPropRotZ2->GetHWND());

	CtlsPropRotXSpin2->GAlign = {1, 1, 0, 0};
	CtlsPropRotYSpin2->GAlign = {1, 1, 0, 0};
	CtlsPropRotZSpin2->GAlign = {1, 1, 0, 0};

	Show(false);
	m_pTabBtn->Enable(0);
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
