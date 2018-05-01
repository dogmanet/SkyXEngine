
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "TabAttachments.h"

TabAttachments::TabAttachments(TabManager * tm):EditorTab(tm)
{
	m_pTabBtn = SXGUICrButtonEx("Attachments", 279, 648, 100, 20, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, m_pTM->GetBaseWnd()->getHWND(), 0, 0);
	m_pTabBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pTabBtn->setFollowParentSides(false, true, true, false);
	m_pTabBtn->setUserPtr(this);
	m_pTabBtn->addHandler(EditorTab::TabBtnHandler, WM_LBUTTONUP);

	m_pRoot = SXGUICrGroupBox("Attachments", 279, 480, 1016, 167, m_pTM->GetBaseWnd()->getHWND(), 0, 0);
	m_pRoot->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pRoot->setColorText(RGB(0, 0, 0));
	m_pRoot->setColorTextBk(RGB(255, 255, 255));
	m_pRoot->setTransparentTextBk(true);
	m_pRoot->setColorBrush(RGB(255, 255, 255));
	m_pRoot->setFollowParentSides(false, true, true, true);
	m_pRoot->addHandler(Tools::ProxyCommandProc, WM_COMMAND);

	AttachmentsList = SXGUICrListBoxEx(10, 15, 312, 127, 0, WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | WS_VSCROLL | WS_BORDER | LBS_NOTIFY/* | LBS_SORT*/, m_pRoot->getHWND(), 0, IDC_PT_LB);
	AttachmentsList->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachmentsList->setColorText(RGB(0, 0, 0));
	AttachmentsList->setColorTextBk(RGB(255, 255, 255));
	AttachmentsList->setTransparentTextBk(true);
	AttachmentsList->setColorBrush(RGB(255, 255, 255));
	AttachmentsList->setFollowParentSides(true, true, true, false);

	AttachNew = SXGUICrButton("New", 11, 144, 100, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, IDC_PT_ADD);
	AttachNew->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachNew->setFollowParentSides(true, true, true, false);

	AttachRename = SXGUICrButton("Rename", 116, 144, 100, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, IDC_PT_REN);
	AttachRename->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachRename->setFollowParentSides(true, true, true, false);

	AttachRemove = SXGUICrButton("Remove", 220, 144, 100, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, IDC_PT_DEL);
	AttachRemove->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachRemove->setFollowParentSides(true, true, true, false);

	AttachPropsGB = SXGUICrGroupBox("Props", 331, 14, 677, 144, m_pRoot->getHWND(), 0, 0);
	AttachPropsGB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachPropsGB->setColorText(RGB(0, 0, 0));
	AttachPropsGB->setColorTextBk(RGB(255, 255, 255));
	AttachPropsGB->setTransparentTextBk(true);
	AttachPropsGB->setColorBrush(RGB(255, 255, 255));
	AttachPropsGB->setFollowParentSides(false, true, true, true);
	AttachPropsGB->addHandler(Tools::ProxyCommandProc, WM_COMMAND);

	AttachBoneRB = SXGUICrRadioButton("Bone attachment", 17, 19, 100, 20, AttachPropsGB->getHWND(), 0, IDC_ATTACH_RB_BONE);
	AttachBoneRB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachBoneRB->setColorText(RGB(0, 0, 0));
	AttachBoneRB->setColorTextBk(RGB(255, 255, 255));
	AttachBoneRB->setTransparentTextBk(true);
	AttachBoneRB->setColorBrush(RGB(255, 255, 255));
	AttachBoneRB->setFollowParentSides(true, true, true, false);

	AttachSkinRB = SXGUICrRadioButton("Skin attachment", 17, 39, 100, 20, AttachPropsGB->getHWND(), 0, IDC_ATTACH_RB_SKIN);
	AttachSkinRB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachSkinRB->setColorText(RGB(0, 0, 0));
	AttachSkinRB->setColorTextBk(RGB(255, 255, 255));
	AttachSkinRB->setTransparentTextBk(true);
	AttachSkinRB->setColorBrush(RGB(255, 255, 255));
	AttachSkinRB->setFollowParentSides(true, true, true, false);

	AttachHideCB = SXGUICrCheckBox("Hide", 17, 62, 100, 20, AttachPropsGB->getHWND(), 0, IDC_PT_CB_HIDDEN, false);
	AttachHideCB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachHideCB->setColorText(RGB(0, 0, 0));
	AttachHideCB->setColorTextBk(RGB(255, 255, 255));
	AttachHideCB->setTransparentTextBk(true);
	AttachHideCB->setColorBrush(RGB(255, 255, 255));
	AttachHideCB->setFollowParentSides(true, true, true, false);

	AttachEnaCollisionCB = SXGUICrCheckBox("Enable collisions", 17, 82, 100, 20, AttachPropsGB->getHWND(), 0, IDC_PT_CB_COLLIDE, false);
	AttachEnaCollisionCB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachEnaCollisionCB->setColorText(RGB(0, 0, 0));
	AttachEnaCollisionCB->setColorTextBk(RGB(255, 255, 255));
	AttachEnaCollisionCB->setTransparentTextBk(true);
	AttachEnaCollisionCB->setColorBrush(RGB(255, 255, 255));
	AttachEnaCollisionCB->setFollowParentSides(true, true, true, false);

	AttachEnaRaytraceCB = SXGUICrCheckBox("Enable raytrace", 17, 102, 100, 20, AttachPropsGB->getHWND(), 0, IDC_PT_CB_RAYTRACE, false);
	AttachEnaRaytraceCB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachEnaRaytraceCB->setColorText(RGB(0, 0, 0));
	AttachEnaRaytraceCB->setColorTextBk(RGB(255, 255, 255));
	AttachEnaRaytraceCB->setTransparentTextBk(true);
	AttachEnaRaytraceCB->setColorBrush(RGB(255, 255, 255));
	AttachEnaRaytraceCB->setFollowParentSides(true, true, true, false);

	AttachBone = SXGUICrComboBoxEx(141, 18, 350, 350, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, AttachPropsGB->getHWND(), 0, 0);
	AttachBone->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachBone->setColorText(RGB(0, 0, 0));
	AttachBone->setColorTextBk(RGB(255, 255, 255));
	AttachBone->setTransparentTextBk(true);
	AttachBone->setColorBrush(RGB(255, 255, 255));
	AttachBone->setFollowParentSides(true, true, true, false);
	AttachBone->setEnable(0);

	AttachXshift = SXGUICrEdit("x-shift", 143, 42, 90, 20, AttachPropsGB->getHWND(), 0, 0);
	AttachXshift->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachXshift->setColorText(RGB(0, 0, 0));
	AttachXshift->setColorTextBk(RGB(255, 255, 255));
	AttachXshift->setTransparentTextBk(true);
	AttachXshift->setColorBrush(RGB(255, 255, 255));
	AttachXshift->setFollowParentSides(true, true, true, false);
	AttachXshiftSpin = SXGUICrUpDown(233, 42, 17, 20, AttachPropsGB->getHWND(), 0, 0, 0, 0);
	AttachXshiftSpin->setFollowParentSides(true, true, true, false);
	AttachYshift = SXGUICrEdit("y-shift", 270, 42, 87, 20, AttachPropsGB->getHWND(), 0, 0);
	AttachYshift->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachYshift->setColorText(RGB(0, 0, 0));
	AttachYshift->setColorTextBk(RGB(255, 255, 255));
	AttachYshift->setTransparentTextBk(true);
	AttachYshift->setColorBrush(RGB(255, 255, 255));
	AttachYshift->setFollowParentSides(true, true, true, false);
	AttachYshiftSpin = SXGUICrUpDown(357, 42, 17, 20, AttachPropsGB->getHWND(), 0, 0, 0, 0);
	AttachYshiftSpin->setFollowParentSides(true, true, true, false);
	AttachZshift = SXGUICrEdit("z-shift", 392, 42, 81, 20, AttachPropsGB->getHWND(), 0, 0);
	AttachZshift->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachZshift->setColorText(RGB(0, 0, 0));
	AttachZshift->setColorTextBk(RGB(255, 255, 255));
	AttachZshift->setTransparentTextBk(true);
	AttachZshift->setColorBrush(RGB(255, 255, 255));
	AttachZshift->setFollowParentSides(true, true, true, false);
	AttachZshiftSpin = SXGUICrUpDown(473, 42, 17, 20, AttachPropsGB->getHWND(), 0, 0, 0, 0);
	AttachZshiftSpin->setFollowParentSides(true, true, true, false);

	Static5 = SXGUICrStatic("File", 142, 80, 38, 20, AttachPropsGB->getHWND(), 0, 0);
	Static5->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static5->setColorText(RGB(0, 0, 0));
	Static5->setColorTextBk(RGB(255, 255, 255));
	Static5->setTransparentTextBk(true);
	Static5->setColorBrush(RGB(255, 255, 255));
	Static5->setFollowParentSides(true, true, true, false);

	AttachFileField = SXGUICrEdit("file", 182, 80, 279, 20, AttachPropsGB->getHWND(), 0, 0);
	AttachFileField->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachFileField->setColorText(RGB(0, 0, 0));
	AttachFileField->setColorTextBk(RGB(255, 255, 255));
	AttachFileField->setTransparentTextBk(true);
	AttachFileField->setColorBrush(RGB(255, 255, 255));
	AttachFileField->setFollowParentSides(true, true, true, false);
	SendMessage(AttachFileField->getHWND(), EM_SETREADONLY, TRUE, NULL);

	AttachFileBrowse = SXGUICrButton("...", 464, 80, 27, 20, SXGUI_BUTTON_IMAGE_NONE, AttachPropsGB->getHWND(), 0, IDC_PT_BROWSE);
	AttachFileBrowse->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachFileBrowse->setFollowParentSides(true, true, true, false);

	AttachFileApply = SXGUICrButton("Apply", 364, 102, 127, 20, SXGUI_BUTTON_IMAGE_NONE, AttachPropsGB->getHWND(), 0, IDC_PT_APPLY);
	AttachFileApply->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachFileApply->setFollowParentSides(true, false, true, true);

	Show(false);
}

TabAttachments::~TabAttachments()
{
	mem_delete(AttachmentsList);
	mem_delete(AttachNew);
	mem_delete(AttachRename);
	mem_delete(AttachRemove);
	mem_delete(AttachPropsGB);
	mem_delete(AttachBoneRB);
	mem_delete(AttachSkinRB);
	mem_delete(AttachHideCB);
	mem_delete(AttachEnaCollisionCB);
	mem_delete(AttachEnaRaytraceCB);
	mem_delete(AttachBone);
	mem_delete(AttachXshift);
	mem_delete(AttachXshiftSpin);
	mem_delete(AttachYshift);
	mem_delete(AttachYshiftSpin);
	mem_delete(AttachZshift);
	mem_delete(AttachZshiftSpin);
	mem_delete(Static5);
	mem_delete(AttachFileField);
	mem_delete(AttachFileBrowse);
	mem_delete(AttachFileApply);
}
