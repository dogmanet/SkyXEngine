#include "TabHitboxes.h"
#include <sxanim/ModelFile.h>

void TabHitboxes::Show(bool bShow)
{
	EditorTab::Show(bShow);
	m_bShown = bShow;
}

TabHitboxes::TabHitboxes(TabManager * tm):EditorTab(tm)
{
	m_pTabBtn = SXGUICrButtonEx("Hitboxes", 780, 648, 100, 20, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pTabBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pTabBtn->GAlign = {false, true, true, false};
	m_pTabBtn->SetUserPtr(this);
	m_pTabBtn->AddHandler(EditorTab::TabBtnHandler, WM_LBUTTONUP);

	m_pRoot = SXGUICrGroupBox("Hitboxes", 279, 480, 1016, 167, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pRoot->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pRoot->SetColorText(0, 0, 0);
	m_pRoot->SetColorTextBk(255, 255, 255);
	m_pRoot->SetTransparentTextBk(true);
	m_pRoot->SetColorBrush(255, 255, 255);
	m_pRoot->GAlign = {false, true, true, true};
	m_pRoot->AddHandler(Tools::ProxyCommandProc, WM_COMMAND);

	HBList = SXGUICrListBoxEx("", 3, 12, 236, 135, 0, WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | WS_VSCROLL | WS_BORDER | LBS_NOTIFY/* | LBS_SORT*/, m_pRoot->GetHWND(), 0, IDC_HB_LB);
	HBList->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	HBList->SetColorText(0, 0, 0);
	HBList->SetColorTextBk(255, 255, 255);
	HBList->SetTransparentTextBk(true);
	HBList->SetColorBrush(255, 255, 255);

	BtnAdd = SXGUICrButton("Add", 5, 139, 130, 20, 0, m_pRoot->GetHWND(), 0, IDC_HB_ADD);
	BtnAdd->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);

	BtnDel = SXGUICrButton("Remove", 138, 139, 100, 20, 0, m_pRoot->GetHWND(), 0, IDC_HB_DEL);
	BtnDel->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);


	PropsGB = SXGUICrGroupBox("Props", 242, 10, 768, 150, m_pRoot->GetHWND(), 0, 0);
	PropsGB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	PropsGB->SetColorText(0, 0, 0);
	PropsGB->SetColorTextBk(255, 255, 255);
	PropsGB->SetTransparentTextBk(true);
	PropsGB->SetColorBrush(255, 255, 255);
	PropsGB->AddHandler(Tools::ProxyCommandProc, WM_COMMAND);

	Static1 = SXGUICrStatic("Name:", 6, 16, 50, 20, PropsGB->GetHWND(), 0, 0);
	Static1->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static1->SetColorText(0, 0, 0);
	Static1->SetColorTextBk(255, 255, 255);
	Static1->SetTransparentTextBk(true);
	Static1->SetColorBrush(255, 255, 255);

	Static2 = SXGUICrStatic("Bone:", 6, 38, 50, 20, PropsGB->GetHWND(), 0, 0);
	Static2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static2->SetColorText(0, 0, 0);
	Static2->SetColorTextBk(255, 255, 255);
	Static2->SetTransparentTextBk(true);
	Static2->SetColorBrush(255, 255, 255);

	Static3 = SXGUICrStatic("Bodypart:", 6, 60, 50, 20, PropsGB->GetHWND(), 0, 0);
	Static3->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static3->SetColorText(0, 0, 0);
	Static3->SetColorTextBk(255, 255, 255);
	Static3->SetTransparentTextBk(true);
	Static3->SetColorBrush(255, 255, 255);

	EdName = SXGUICrEdit("", 57, 16, 100, 20, PropsGB->GetHWND(), 0, IDC_HB_NAME);
	EdName->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdName->SetColorText(0, 0, 0);
	EdName->SetColorTextBk(255, 255, 255);
	EdName->SetTransparentTextBk(true);
	EdName->SetColorBrush(255, 255, 255);

	CBBone = SXGUICrComboBoxEx("", 57, 37, 100, 300, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, PropsGB->GetHWND(), 0, IDC_HB_BONE);
	CBBone->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CBBone->SetColorText(0, 0, 0);
	CBBone->SetColorTextBk(255, 255, 255);
	CBBone->SetTransparentTextBk(true);
	CBBone->SetColorBrush(255, 255, 255);

	CBBodyPart = SXGUICrComboBoxEx("", 57, 59, 100, 300, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, PropsGB->GetHWND(), 0, IDC_HB_BODYPART);
	CBBodyPart->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CBBodyPart->SetColorText(0, 0, 0);
	CBBodyPart->SetColorTextBk(255, 255, 255);
	CBBodyPart->SetTransparentTextBk(true);
	CBBodyPart->SetColorBrush(255, 255, 255);

	Static4 = SXGUICrStatic("Position", 172, 15, 241, 20, PropsGB->GetHWND(), 0, 0);
	Static4->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static4->SetColorText(0, 0, 0);
	Static4->SetColorTextBk(255, 255, 255);
	Static4->SetTransparentTextBk(true);
	Static4->SetColorBrush(255, 255, 255);

	Static5 = SXGUICrStatic("X", 172, 35, 15, 20, PropsGB->GetHWND(), 0, 0);
	Static5->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static5->SetColorText(0, 0, 0);
	Static5->SetColorTextBk(255, 255, 255);
	Static5->SetTransparentTextBk(true);
	Static5->SetColorBrush(255, 255, 255);

	EdPosX = SXGUICrEdit("0.0", 187, 35, 60, 20, PropsGB->GetHWND(), 0, IDC_HB_ED_POS_X);
	EdPosX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdPosX->SetColorText(0, 0, 0);
	EdPosX->SetColorTextBk(255, 255, 255);
	EdPosX->SetTransparentTextBk(true);
	EdPosX->SetColorBrush(255, 255, 255);

	Static6 = SXGUICrStatic("Y", 256, 35, 15, 20, PropsGB->GetHWND(), 0, 0);
	Static6->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static6->SetColorText(0, 0, 0);
	Static6->SetColorTextBk(255, 255, 255);
	Static6->SetTransparentTextBk(true);
	Static6->SetColorBrush(255, 255, 255);

	EdPosY = SXGUICrEdit("0.0", 271, 35, 60, 20, PropsGB->GetHWND(), 0, IDC_HB_ED_POS_Y);
	EdPosY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdPosY->SetColorText(0, 0, 0);
	EdPosY->SetColorTextBk(255, 255, 255);
	EdPosY->SetTransparentTextBk(true);
	EdPosY->SetColorBrush(255, 255, 255);

	Static7 = SXGUICrStatic("Z", 338, 35, 15, 20, PropsGB->GetHWND(), 0, 0);
	Static7->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static7->SetColorText(0, 0, 0);
	Static7->SetColorTextBk(255, 255, 255);
	Static7->SetTransparentTextBk(true);
	Static7->SetColorBrush(255, 255, 255);

	EdPosZ = SXGUICrEdit("0.0", 353, 35, 60, 20, PropsGB->GetHWND(), 0, IDC_HB_ED_POS_Z);
	EdPosZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdPosZ->SetColorText(0, 0, 0);
	EdPosZ->SetColorTextBk(255, 255, 255);
	EdPosZ->SetTransparentTextBk(true);
	EdPosZ->SetColorBrush(255, 255, 255);

	Static8 = SXGUICrStatic("Rotation", 171, 59, 241, 20, PropsGB->GetHWND(), 0, 0);
	Static8->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static8->SetColorText(0, 0, 0);
	Static8->SetColorTextBk(255, 255, 255);
	Static8->SetTransparentTextBk(true);
	Static8->SetColorBrush(255, 255, 255);

	Static9 = SXGUICrStatic("X", 172, 79, 15, 20, PropsGB->GetHWND(), 0, 0);
	Static9->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static9->SetColorText(0, 0, 0);
	Static9->SetColorTextBk(255, 255, 255);
	Static9->SetTransparentTextBk(true);
	Static9->SetColorBrush(255, 255, 255);

	Static10 = SXGUICrStatic("Y", 256, 79, 15, 20, PropsGB->GetHWND(), 0, 0);
	Static10->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static10->SetColorText(0, 0, 0);
	Static10->SetColorTextBk(255, 255, 255);
	Static10->SetTransparentTextBk(true);
	Static10->SetColorBrush(255, 255, 255);

	Static11 = SXGUICrStatic("Z", 338, 79, 15, 20, PropsGB->GetHWND(), 0, 0);
	Static11->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static11->SetColorText(0, 0, 0);
	Static11->SetColorTextBk(255, 255, 255);
	Static11->SetTransparentTextBk(true);
	Static11->SetColorBrush(255, 255, 255);

	EdRotX = SXGUICrEdit("0.0", 187, 79, 60, 20, PropsGB->GetHWND(), 0, IDC_HB_ED_ROT_X);
	EdRotX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdRotX->SetColorText(0, 0, 0);
	EdRotX->SetColorTextBk(255, 255, 255);
	EdRotX->SetTransparentTextBk(true);
	EdRotX->SetColorBrush(255, 255, 255);

	EdRotY = SXGUICrEdit("0.0", 271, 79, 60, 20, PropsGB->GetHWND(), 0, IDC_HB_ED_ROT_Y);
	EdRotY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdRotY->SetColorText(0, 0, 0);
	EdRotY->SetColorTextBk(255, 255, 255);
	EdRotY->SetTransparentTextBk(true);
	EdRotY->SetColorBrush(255, 255, 255);

	EdRotZ = SXGUICrEdit("0.0", 353, 79, 60, 20, PropsGB->GetHWND(), 0, IDC_HB_ED_ROT_Z);
	EdRotZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdRotZ->SetColorText(0, 0, 0);
	EdRotZ->SetColorTextBk(255, 255, 255);
	EdRotZ->SetTransparentTextBk(true);
	EdRotZ->SetColorBrush(255, 255, 255);

	Static16 = SXGUICrStatic("L", 172, 104, 15, 20, PropsGB->GetHWND(), 0, 0);
	Static16->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static16->SetColorText(0, 0, 0);
	Static16->SetColorTextBk(255, 255, 255);
	Static16->SetTransparentTextBk(true);
	Static16->SetColorBrush(255, 255, 255);

	Static13 = SXGUICrStatic("W", 256, 104, 15, 20, PropsGB->GetHWND(), 0, 0);
	Static13->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static13->SetColorText(0, 0, 0);
	Static13->SetColorTextBk(255, 255, 255);
	Static13->SetTransparentTextBk(true);
	Static13->SetColorBrush(255, 255, 255);

	Static14 = SXGUICrStatic("H", 338, 104, 15, 20, PropsGB->GetHWND(), 0, 0);
	Static14->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static14->SetColorText(0, 0, 0);
	Static14->SetColorTextBk(255, 255, 255);
	Static14->SetTransparentTextBk(true);
	Static14->SetColorBrush(255, 255, 255);

	EdL = SXGUICrEdit("0.0", 187, 104, 60, 20, PropsGB->GetHWND(), 0, IDC_HB_ED_L);
	EdL->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdL->SetColorText(0, 0, 0);
	EdL->SetColorTextBk(255, 255, 255);
	EdL->SetTransparentTextBk(true);
	EdL->SetColorBrush(255, 255, 255);

	EdW = SXGUICrEdit("0.0", 271, 104, 60, 20, PropsGB->GetHWND(), 0, IDC_HB_ED_W);
	EdW->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdW->SetColorText(0, 0, 0);
	EdW->SetColorTextBk(255, 255, 255);
	EdW->SetTransparentTextBk(true);
	EdW->SetColorBrush(255, 255, 255);

	EdH = SXGUICrEdit("0.0", 353, 104, 60, 20, PropsGB->GetHWND(), 0, IDC_HB_ED_H);
	EdH->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	EdH->SetColorText(0, 0, 0);
	EdH->SetColorTextBk(255, 255, 255);
	EdH->SetTransparentTextBk(true);
	EdH->SetColorBrush(255, 255, 255);

	Static15 = SXGUICrStatic("Type:", 6, 104, 50, 20, PropsGB->GetHWND(), 0, 0);
	Static15->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static15->SetColorText(0, 0, 0);
	Static15->SetColorTextBk(255, 255, 255);
	Static15->SetTransparentTextBk(true);
	Static15->SetColorBrush(255, 255, 255);

	CBType = SXGUICrComboBoxEx("", 57, 104, 100, 300, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, PropsGB->GetHWND(), 0, IDC_HB_TYPE);
	CBType->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CBType->SetColorText(0, 0, 0);
	CBType->SetColorTextBk(255, 255, 255);
	CBType->SetTransparentTextBk(true);
	CBType->SetColorBrush(255, 255, 255);
	
	RBTranslate = SXGUICrRadioButton("Translate", 417, 15, 100, 20, PropsGB->GetHWND(), 0, IDC_HB_TRANS);
	RBTranslate->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	RBTranslate->SetColorText(0, 0, 0);
	RBTranslate->SetColorTextBk(255, 255, 255);
	RBTranslate->SetTransparentTextBk(true);
	RBTranslate->SetColorBrush(255, 255, 255);

	RBRotate = SXGUICrRadioButton("Rotate", 417, 59, 100, 20, PropsGB->GetHWND(), 0, IDC_HB_ROT);
	RBRotate->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	RBRotate->SetColorText(0, 0, 0);
	RBRotate->SetColorTextBk(255, 255, 255);
	RBRotate->SetTransparentTextBk(true);
	RBRotate->SetColorBrush(255, 255, 255);

	RBScale = SXGUICrRadioButton("Scale", 417, 104, 100, 20, PropsGB->GetHWND(), 0, IDC_HB_SCALE);
	RBScale->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	RBScale->SetColorText(0, 0, 0);
	RBScale->SetColorTextBk(255, 255, 255);
	RBScale->SetTransparentTextBk(true);
	RBScale->SetColorBrush(255, 255, 255);



	CBType->AddItem("Box");
	CBType->SetItemData(CBType->GetCount() - 1, HT_BOX);
	CBType->AddItem("Cylinder");
	CBType->SetItemData(CBType->GetCount() - 1, HT_CYLINDER);
	CBType->AddItem("Capsule");
	CBType->SetItemData(CBType->GetCount() - 1, HT_CAPSULE);
	CBType->AddItem("Elipsoid");
	CBType->SetItemData(CBType->GetCount() - 1, HT_ELIPSOID);

	CBType->SetSel(0);

	CBBodyPart->AddItem("Default");
	CBBodyPart->SetItemData(CBBodyPart->GetCount() - 1, HBP_DEFAULT);
	CBBodyPart->AddItem("Head");
	CBBodyPart->SetItemData(CBBodyPart->GetCount() - 1, HBP_HEAD);
	CBBodyPart->AddItem("Chest");
	CBBodyPart->SetItemData(CBBodyPart->GetCount() - 1, HBP_CHEST);
	CBBodyPart->AddItem("Stomach");
	CBBodyPart->SetItemData(CBBodyPart->GetCount() - 1, HBP_STOMACH);
	CBBodyPart->AddItem("Left arm");
	CBBodyPart->SetItemData(CBBodyPart->GetCount() - 1, HBP_LEFTARM);
	CBBodyPart->AddItem("Right arm");
	CBBodyPart->SetItemData(CBBodyPart->GetCount() - 1, HBP_RIGHTARM);
	CBBodyPart->AddItem("Left leg");
	CBBodyPart->SetItemData(CBBodyPart->GetCount() - 1, HBP_LEFTLEG);
	CBBodyPart->AddItem("Right leg");
	CBBodyPart->SetItemData(CBBodyPart->GetCount() - 1, HBP_RIGHTLEG);
	
	CBBodyPart->SetSel(0);

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
