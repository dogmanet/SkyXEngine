
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "TabHitboxes.h"
#include <anim/ModelFile.h>

void TabHitboxes::Show(bool bShow)
{
	EditorTab::Show(bShow);
	m_bShown = bShow;
}

TabHitboxes::TabHitboxes(TabManager * tm):EditorTab(tm)
{
	m_pTabBtn = SXGUICrButtonEx("Hitboxes", 780, 648, 100, 20, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, m_pTM->GetBaseWnd()->getHWND(), 0, 0);
	m_pTabBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pTabBtn->setFollowParentSides(false, true, true, false);
	m_pTabBtn->setUserPtr(this);
	m_pTabBtn->addHandler(EditorTab::TabBtnHandler, WM_LBUTTONUP);

	m_pRoot = SXGUICrGroupBox("Hitboxes", 279, 480, 1016, 167, m_pTM->GetBaseWnd()->getHWND(), 0, 0);
	m_pRoot->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pRoot->setColorText(RGB(0, 0, 0));
	m_pRoot->setColorTextBk(RGB(255, 255, 255));
	m_pRoot->setTransparentTextBk(true);
	m_pRoot->setColorBrush(RGB(255, 255, 255));
	m_pRoot->setFollowParentSides(false, true, true, true);
	m_pRoot->addHandler(Tools::ProxyCommandProc, WM_COMMAND);

	HBList = SXGUICrListBoxEx(3, 12, 236, 135, 0, WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | WS_VSCROLL | WS_BORDER | LBS_NOTIFY/* | LBS_SORT*/, m_pRoot->getHWND(), 0, IDC_HB_LB);
	HBList->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	HBList->setColorText(RGB(0, 0, 0));
	HBList->setColorTextBk(RGB(255, 255, 255));
	HBList->setTransparentTextBk(true);
	HBList->setColorBrush(RGB(255, 255, 255));

	BtnAdd = SXGUICrButton("Add", 5, 139, 130, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, IDC_HB_ADD);
	BtnAdd->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);

	BtnDel = SXGUICrButton("Remove", 138, 139, 100, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, IDC_HB_DEL);
	BtnDel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);


	PropsGB = SXGUICrGroupBox("Props", 242, 10, 768, 150, m_pRoot->getHWND(), 0, 0);
	PropsGB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	PropsGB->setColorText(RGB(0, 0, 0));
	PropsGB->setColorTextBk(RGB(255, 255, 255));
	PropsGB->setTransparentTextBk(true);
	PropsGB->setColorBrush(RGB(255, 255, 255));
	PropsGB->addHandler(Tools::ProxyCommandProc, WM_COMMAND);

	Static1 = SXGUICrStatic("Name:", 6, 16, 50, 20, PropsGB->getHWND(), 0, 0);
	Static1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static1->setColorText(RGB(0, 0, 0));
	Static1->setColorTextBk(RGB(255, 255, 255));
	Static1->setTransparentTextBk(true);
	Static1->setColorBrush(RGB(255, 255, 255));

	Static2 = SXGUICrStatic("Bone:", 6, 38, 50, 20, PropsGB->getHWND(), 0, 0);
	Static2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static2->setColorText(RGB(0, 0, 0));
	Static2->setColorTextBk(RGB(255, 255, 255));
	Static2->setTransparentTextBk(true);
	Static2->setColorBrush(RGB(255, 255, 255));

	Static3 = SXGUICrStatic("Bodypart:", 6, 60, 50, 20, PropsGB->getHWND(), 0, 0);
	Static3->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static3->setColorText(RGB(0, 0, 0));
	Static3->setColorTextBk(RGB(255, 255, 255));
	Static3->setTransparentTextBk(true);
	Static3->setColorBrush(RGB(255, 255, 255));

	EdName = SXGUICrEdit("", 57, 16, 100, 20, PropsGB->getHWND(), 0, IDC_HB_NAME);
	EdName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdName->setColorText(RGB(0, 0, 0));
	EdName->setColorTextBk(RGB(255, 255, 255));
	EdName->setTransparentTextBk(true);
	EdName->setColorBrush(RGB(255, 255, 255));

	CBBone = SXGUICrComboBoxEx(57, 37, 100, 300, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, PropsGB->getHWND(), 0, IDC_HB_BONE);
	CBBone->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CBBone->setColorText(RGB(0, 0, 0));
	CBBone->setColorTextBk(RGB(255, 255, 255));
	CBBone->setTransparentTextBk(true);
	CBBone->setColorBrush(RGB(255, 255, 255));

	CBBodyPart = SXGUICrComboBoxEx(57, 59, 100, 300, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, PropsGB->getHWND(), 0, IDC_HB_BODYPART);
	CBBodyPart->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CBBodyPart->setColorText(RGB(0, 0, 0));
	CBBodyPart->setColorTextBk(RGB(255, 255, 255));
	CBBodyPart->setTransparentTextBk(true);
	CBBodyPart->setColorBrush(RGB(255, 255, 255));

	Static4 = SXGUICrStatic("Position", 172, 15, 241, 20, PropsGB->getHWND(), 0, 0);
	Static4->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static4->setColorText(RGB(0, 0, 0));
	Static4->setColorTextBk(RGB(255, 255, 255));
	Static4->setTransparentTextBk(true);
	Static4->setColorBrush(RGB(255, 255, 255));

	Static5 = SXGUICrStatic("X", 172, 35, 15, 20, PropsGB->getHWND(), 0, 0);
	Static5->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static5->setColorText(RGB(0, 0, 0));
	Static5->setColorTextBk(RGB(255, 255, 255));
	Static5->setTransparentTextBk(true);
	Static5->setColorBrush(RGB(255, 255, 255));

	EdPosX = SXGUICrEdit("0.0", 187, 35, 60, 20, PropsGB->getHWND(), 0, IDC_HB_ED_POS_X);
	EdPosX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdPosX->setColorText(RGB(0, 0, 0));
	EdPosX->setColorTextBk(RGB(255, 255, 255));
	EdPosX->setTransparentTextBk(true);
	EdPosX->setColorBrush(RGB(255, 255, 255));

	Static6 = SXGUICrStatic("Y", 256, 35, 15, 20, PropsGB->getHWND(), 0, 0);
	Static6->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static6->setColorText(RGB(0, 0, 0));
	Static6->setColorTextBk(RGB(255, 255, 255));
	Static6->setTransparentTextBk(true);
	Static6->setColorBrush(RGB(255, 255, 255));

	EdPosY = SXGUICrEdit("0.0", 271, 35, 60, 20, PropsGB->getHWND(), 0, IDC_HB_ED_POS_Y);
	EdPosY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdPosY->setColorText(RGB(0, 0, 0));
	EdPosY->setColorTextBk(RGB(255, 255, 255));
	EdPosY->setTransparentTextBk(true);
	EdPosY->setColorBrush(RGB(255, 255, 255));

	Static7 = SXGUICrStatic("Z", 338, 35, 15, 20, PropsGB->getHWND(), 0, 0);
	Static7->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static7->setColorText(RGB(0, 0, 0));
	Static7->setColorTextBk(RGB(255, 255, 255));
	Static7->setTransparentTextBk(true);
	Static7->setColorBrush(RGB(255, 255, 255));

	EdPosZ = SXGUICrEdit("0.0", 353, 35, 60, 20, PropsGB->getHWND(), 0, IDC_HB_ED_POS_Z);
	EdPosZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdPosZ->setColorText(RGB(0, 0, 0));
	EdPosZ->setColorTextBk(RGB(255, 255, 255));
	EdPosZ->setTransparentTextBk(true);
	EdPosZ->setColorBrush(RGB(255, 255, 255));

	Static8 = SXGUICrStatic("Rotation", 171, 59, 241, 20, PropsGB->getHWND(), 0, 0);
	Static8->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static8->setColorText(RGB(0, 0, 0));
	Static8->setColorTextBk(RGB(255, 255, 255));
	Static8->setTransparentTextBk(true);
	Static8->setColorBrush(RGB(255, 255, 255));

	Static9 = SXGUICrStatic("X", 172, 79, 15, 20, PropsGB->getHWND(), 0, 0);
	Static9->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static9->setColorText(RGB(0, 0, 0));
	Static9->setColorTextBk(RGB(255, 255, 255));
	Static9->setTransparentTextBk(true);
	Static9->setColorBrush(RGB(255, 255, 255));

	Static10 = SXGUICrStatic("Y", 256, 79, 15, 20, PropsGB->getHWND(), 0, 0);
	Static10->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static10->setColorText(RGB(0, 0, 0));
	Static10->setColorTextBk(RGB(255, 255, 255));
	Static10->setTransparentTextBk(true);
	Static10->setColorBrush(RGB(255, 255, 255));

	Static11 = SXGUICrStatic("Z", 338, 79, 15, 20, PropsGB->getHWND(), 0, 0);
	Static11->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static11->setColorText(RGB(0, 0, 0));
	Static11->setColorTextBk(RGB(255, 255, 255));
	Static11->setTransparentTextBk(true);
	Static11->setColorBrush(RGB(255, 255, 255));

	EdRotX = SXGUICrEdit("0.0", 187, 79, 60, 20, PropsGB->getHWND(), 0, IDC_HB_ED_ROT_X);
	EdRotX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdRotX->setColorText(RGB(0, 0, 0));
	EdRotX->setColorTextBk(RGB(255, 255, 255));
	EdRotX->setTransparentTextBk(true);
	EdRotX->setColorBrush(RGB(255, 255, 255));

	EdRotY = SXGUICrEdit("0.0", 271, 79, 60, 20, PropsGB->getHWND(), 0, IDC_HB_ED_ROT_Y);
	EdRotY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdRotY->setColorText(RGB(0, 0, 0));
	EdRotY->setColorTextBk(RGB(255, 255, 255));
	EdRotY->setTransparentTextBk(true);
	EdRotY->setColorBrush(RGB(255, 255, 255));

	EdRotZ = SXGUICrEdit("0.0", 353, 79, 60, 20, PropsGB->getHWND(), 0, IDC_HB_ED_ROT_Z);
	EdRotZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdRotZ->setColorText(RGB(0, 0, 0));
	EdRotZ->setColorTextBk(RGB(255, 255, 255));
	EdRotZ->setTransparentTextBk(true);
	EdRotZ->setColorBrush(RGB(255, 255, 255));

	Static16 = SXGUICrStatic("L", 172, 104, 15, 20, PropsGB->getHWND(), 0, 0);
	Static16->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static16->setColorText(RGB(0, 0, 0));
	Static16->setColorTextBk(RGB(255, 255, 255));
	Static16->setTransparentTextBk(true);
	Static16->setColorBrush(RGB(255, 255, 255));

	Static13 = SXGUICrStatic("W", 256, 104, 15, 20, PropsGB->getHWND(), 0, 0);
	Static13->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static13->setColorText(RGB(0, 0, 0));
	Static13->setColorTextBk(RGB(255, 255, 255));
	Static13->setTransparentTextBk(true);
	Static13->setColorBrush(RGB(255, 255, 255));

	Static14 = SXGUICrStatic("H", 338, 104, 15, 20, PropsGB->getHWND(), 0, 0);
	Static14->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static14->setColorText(RGB(0, 0, 0));
	Static14->setColorTextBk(RGB(255, 255, 255));
	Static14->setTransparentTextBk(true);
	Static14->setColorBrush(RGB(255, 255, 255));

	EdL = SXGUICrEdit("0.0", 187, 104, 60, 20, PropsGB->getHWND(), 0, IDC_HB_ED_L);
	EdL->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdL->setColorText(RGB(0, 0, 0));
	EdL->setColorTextBk(RGB(255, 255, 255));
	EdL->setTransparentTextBk(true);
	EdL->setColorBrush(RGB(255, 255, 255));

	EdW = SXGUICrEdit("0.0", 271, 104, 60, 20, PropsGB->getHWND(), 0, IDC_HB_ED_W);
	EdW->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdW->setColorText(RGB(0, 0, 0));
	EdW->setColorTextBk(RGB(255, 255, 255));
	EdW->setTransparentTextBk(true);
	EdW->setColorBrush(RGB(255, 255, 255));

	EdH = SXGUICrEdit("0.0", 353, 104, 60, 20, PropsGB->getHWND(), 0, IDC_HB_ED_H);
	EdH->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdH->setColorText(RGB(0, 0, 0));
	EdH->setColorTextBk(RGB(255, 255, 255));
	EdH->setTransparentTextBk(true);
	EdH->setColorBrush(RGB(255, 255, 255));

	Static15 = SXGUICrStatic("Type:", 6, 104, 50, 20, PropsGB->getHWND(), 0, 0);
	Static15->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static15->setColorText(RGB(0, 0, 0));
	Static15->setColorTextBk(RGB(255, 255, 255));
	Static15->setTransparentTextBk(true);
	Static15->setColorBrush(RGB(255, 255, 255));

	CBType = SXGUICrComboBoxEx(57, 104, 100, 300, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, PropsGB->getHWND(), 0, IDC_HB_TYPE);
	CBType->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CBType->setColorText(RGB(0, 0, 0));
	CBType->setColorTextBk(RGB(255, 255, 255));
	CBType->setTransparentTextBk(true);
	CBType->setColorBrush(RGB(255, 255, 255));
	
	RBTranslate = SXGUICrRadioButton("Translate", 417, 15, 100, 20, PropsGB->getHWND(), 0, IDC_HB_TRANS);
	RBTranslate->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	RBTranslate->setColorText(RGB(0, 0, 0));
	RBTranslate->setColorTextBk(RGB(255, 255, 255));
	RBTranslate->setTransparentTextBk(true);
	RBTranslate->setColorBrush(RGB(255, 255, 255));

	RBRotate = SXGUICrRadioButton("Rotate", 417, 59, 100, 20, PropsGB->getHWND(), 0, IDC_HB_ROT);
	RBRotate->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	RBRotate->setColorText(RGB(0, 0, 0));
	RBRotate->setColorTextBk(RGB(255, 255, 255));
	RBRotate->setTransparentTextBk(true);
	RBRotate->setColorBrush(RGB(255, 255, 255));

	RBScale = SXGUICrRadioButton("Scale", 417, 104, 100, 20, PropsGB->getHWND(), 0, IDC_HB_SCALE);
	RBScale->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	RBScale->setColorText(RGB(0, 0, 0));
	RBScale->setColorTextBk(RGB(255, 255, 255));
	RBScale->setTransparentTextBk(true);
	RBScale->setColorBrush(RGB(255, 255, 255));



	CBType->addItem("Box");
	CBType->setItemData(CBType->getCount() - 1, HT_BOX);
	CBType->addItem("Cylinder");
	CBType->setItemData(CBType->getCount() - 1, HT_CYLINDER);
	CBType->addItem("Capsule");
	CBType->setItemData(CBType->getCount() - 1, HT_CAPSULE);
	CBType->addItem("Elipsoid");
	CBType->setItemData(CBType->getCount() - 1, HT_ELIPSOID);

	CBType->setSel(0);

	CBBodyPart->addItem("Default");
	CBBodyPart->setItemData(CBBodyPart->getCount() - 1, HBP_DEFAULT);
	CBBodyPart->addItem("Head");
	CBBodyPart->setItemData(CBBodyPart->getCount() - 1, HBP_HEAD);
	CBBodyPart->addItem("Chest");
	CBBodyPart->setItemData(CBBodyPart->getCount() - 1, HBP_CHEST);
	CBBodyPart->addItem("Stomach");
	CBBodyPart->setItemData(CBBodyPart->getCount() - 1, HBP_STOMACH);
	CBBodyPart->addItem("Left arm");
	CBBodyPart->setItemData(CBBodyPart->getCount() - 1, HBP_LEFTARM);
	CBBodyPart->addItem("Right arm");
	CBBodyPart->setItemData(CBBodyPart->getCount() - 1, HBP_RIGHTARM);
	CBBodyPart->addItem("Left leg");
	CBBodyPart->setItemData(CBBodyPart->getCount() - 1, HBP_LEFTLEG);
	CBBodyPart->addItem("Right leg");
	CBBodyPart->setItemData(CBBodyPart->getCount() - 1, HBP_RIGHTLEG);
	
	CBBodyPart->setSel(0);

	Show(false);
}

TabHitboxes::~TabHitboxes()
{
	mem_delete(HBList);
	mem_delete(BtnAdd);
	mem_delete(BtnDel);
	mem_delete(PropsGB);
	mem_delete(Static1);
	mem_delete(Static2);
	mem_delete(Static3);
	mem_delete(EdName);
	mem_delete(CBBone);
	mem_delete(CBBodyPart);
	mem_delete(Static4);
	mem_delete(Static5);
	mem_delete(EdPosX);
	mem_delete(Static6);
	mem_delete(EdPosY);
	mem_delete(Static7);
	mem_delete(EdPosZ);
	mem_delete(Static8);
	mem_delete(Static9);
	mem_delete(Static10);
	mem_delete(Static11);
	mem_delete(EdRotX);
	mem_delete(EdRotY);
	mem_delete(EdRotZ);
	mem_delete(Static13);
	mem_delete(Static14);
	mem_delete(EdL);
	mem_delete(EdW);
	mem_delete(EdH);
	mem_delete(Static15);
	mem_delete(CBType);
	mem_delete(Static16);
	mem_delete(RBTranslate);
	mem_delete(RBRotate);
	mem_delete(RBScale);
}
