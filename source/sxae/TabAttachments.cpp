
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "TabAttachments.h"

TabAttachments::TabAttachments(TabManager * tm):EditorTab(tm)
{
	m_pTabBtn = SXGUICrButtonEx("Attachments", 279, 648, 100, 20, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pTabBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pTabBtn->GAlign = {false, true, true, false};
	m_pTabBtn->SetUserPtr(this);
	m_pTabBtn->AddHandler(EditorTab::TabBtnHandler, WM_LBUTTONUP);

	m_pRoot = SXGUICrGroupBox("Attachments", 279, 480, 1016, 167, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pRoot->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pRoot->SetColorText(0, 0, 0);
	m_pRoot->SetColorTextBk(255, 255, 255);
	m_pRoot->SetTransparentTextBk(true);
	m_pRoot->SetColorBrush(255, 255, 255);
	m_pRoot->GAlign = {false, true, true, true};
	m_pRoot->AddHandler(Tools::ProxyCommandProc, WM_COMMAND);

	AttachmentsList = SXGUICrListBoxEx("", 10, 15, 312, 127, 0, WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | WS_VSCROLL | WS_BORDER | LBS_NOTIFY/* | LBS_SORT*/, m_pRoot->GetHWND(), 0, IDC_PT_LB);
	AttachmentsList->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachmentsList->SetColorText(0, 0, 0);
	AttachmentsList->SetColorTextBk(255, 255, 255);
	AttachmentsList->SetTransparentTextBk(true);
	AttachmentsList->SetColorBrush(255, 255, 255);
	AttachmentsList->GAlign = {true, true, true, false};

	AttachNew = SXGUICrButton("New", 11, 144, 100, 20, 0, m_pRoot->GetHWND(), 0, IDC_PT_ADD);
	AttachNew->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachNew->GAlign = {true, true, true, false};

	AttachRename = SXGUICrButton("Rename", 116, 144, 100, 20, 0, m_pRoot->GetHWND(), 0, IDC_PT_REN);
	AttachRename->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachRename->GAlign = {true, true, true, false};

	AttachRemove = SXGUICrButton("Remove", 220, 144, 100, 20, 0, m_pRoot->GetHWND(), 0, IDC_PT_DEL);
	AttachRemove->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachRemove->GAlign = {true, true, true, false};

	AttachPropsGB = SXGUICrGroupBox("Props", 331, 14, 677, 144, m_pRoot->GetHWND(), 0, 0);
	AttachPropsGB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachPropsGB->SetColorText(0, 0, 0);
	AttachPropsGB->SetColorTextBk(255, 255, 255);
	AttachPropsGB->SetTransparentTextBk(true);
	AttachPropsGB->SetColorBrush(255, 255, 255);
	AttachPropsGB->GAlign = {false, true, true, true};
	AttachPropsGB->AddHandler(Tools::ProxyCommandProc, WM_COMMAND);

	AttachBoneRB = SXGUICrRadioButton("Bone attachment", 17, 19, 100, 20, AttachPropsGB->GetHWND(), 0, IDC_ATTACH_RB_BONE);
	AttachBoneRB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachBoneRB->SetColorText(0, 0, 0);
	AttachBoneRB->SetColorTextBk(255, 255, 255);
	AttachBoneRB->SetTransparentTextBk(true);
	AttachBoneRB->SetColorBrush(255, 255, 255);
	AttachBoneRB->GAlign = {true, true, true, false};

	AttachSkinRB = SXGUICrRadioButton("Skin attachment", 17, 39, 100, 20, AttachPropsGB->GetHWND(), 0, IDC_ATTACH_RB_SKIN);
	AttachSkinRB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachSkinRB->SetColorText(0, 0, 0);
	AttachSkinRB->SetColorTextBk(255, 255, 255);
	AttachSkinRB->SetTransparentTextBk(true);
	AttachSkinRB->SetColorBrush(255, 255, 255);
	AttachSkinRB->GAlign = {true, true, true, false};

	AttachHideCB = SXGUICrCheckBox("Hide", 17, 62, 100, 20, AttachPropsGB->GetHWND(), 0, IDC_PT_CB_HIDDEN, false);
	AttachHideCB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachHideCB->SetColorText(0, 0, 0);
	AttachHideCB->SetColorTextBk(255, 255, 255);
	AttachHideCB->SetTransparentTextBk(true);
	AttachHideCB->SetColorBrush(255, 255, 255);
	AttachHideCB->GAlign = {true, true, true, false};

	AttachEnaCollisionCB = SXGUICrCheckBox("Enable collisions", 17, 82, 100, 20, AttachPropsGB->GetHWND(), 0, IDC_PT_CB_COLLIDE, false);
	AttachEnaCollisionCB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachEnaCollisionCB->SetColorText(0, 0, 0);
	AttachEnaCollisionCB->SetColorTextBk(255, 255, 255);
	AttachEnaCollisionCB->SetTransparentTextBk(true);
	AttachEnaCollisionCB->SetColorBrush(255, 255, 255);
	AttachEnaCollisionCB->GAlign = {true, true, true, false};

	AttachEnaRaytraceCB = SXGUICrCheckBox("Enable raytrace", 17, 102, 100, 20, AttachPropsGB->GetHWND(), 0, IDC_PT_CB_RAYTRACE, false);
	AttachEnaRaytraceCB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachEnaRaytraceCB->SetColorText(0, 0, 0);
	AttachEnaRaytraceCB->SetColorTextBk(255, 255, 255);
	AttachEnaRaytraceCB->SetTransparentTextBk(true);
	AttachEnaRaytraceCB->SetColorBrush(255, 255, 255);
	AttachEnaRaytraceCB->GAlign = {true, true, true, false};

	AttachBone = SXGUICrComboBoxEx("", 141, 18, 350, 350, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, AttachPropsGB->GetHWND(), 0, 0);
	AttachBone->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachBone->SetColorText(0, 0, 0);
	AttachBone->SetColorTextBk(255, 255, 255);
	AttachBone->SetTransparentTextBk(true);
	AttachBone->SetColorBrush(255, 255, 255);
	AttachBone->GAlign = {true, true, true, false};
	AttachBone->Enable(0);

	AttachXshift = SXGUICrEdit("x-shift", 143, 42, 90, 20, AttachPropsGB->GetHWND(), 0, 0);
	AttachXshift->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachXshift->SetColorText(0, 0, 0);
	AttachXshift->SetColorTextBk(255, 255, 255);
	AttachXshift->SetTransparentTextBk(true);
	AttachXshift->SetColorBrush(255, 255, 255);
	AttachXshift->GAlign = {true, true, true, false};
	AttachXshiftSpin = SXGUICrUpDown(233, 42, 17, 20, AttachPropsGB->GetHWND(), 0, 0, 0, 0);
	AttachXshiftSpin->GAlign = {true, true, true, false};
	AttachYshift = SXGUICrEdit("y-shift", 270, 42, 87, 20, AttachPropsGB->GetHWND(), 0, 0);
	AttachYshift->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachYshift->SetColorText(0, 0, 0);
	AttachYshift->SetColorTextBk(255, 255, 255);
	AttachYshift->SetTransparentTextBk(true);
	AttachYshift->SetColorBrush(255, 255, 255);
	AttachYshift->GAlign = {true, true, true, false};
	AttachYshiftSpin = SXGUICrUpDown(357, 42, 17, 20, AttachPropsGB->GetHWND(), 0, 0, 0, 0);
	AttachYshiftSpin->GAlign = {true, true, true, false};
	AttachZshift = SXGUICrEdit("z-shift", 392, 42, 81, 20, AttachPropsGB->GetHWND(), 0, 0);
	AttachZshift->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachZshift->SetColorText(0, 0, 0);
	AttachZshift->SetColorTextBk(255, 255, 255);
	AttachZshift->SetTransparentTextBk(true);
	AttachZshift->SetColorBrush(255, 255, 255);
	AttachZshift->GAlign = {true, true, true, false};
	AttachZshiftSpin = SXGUICrUpDown(473, 42, 17, 20, AttachPropsGB->GetHWND(), 0, 0, 0, 0);
	AttachZshiftSpin->GAlign = {true, true, true, false};

	Static5 = SXGUICrStatic("File", 142, 80, 38, 20, AttachPropsGB->GetHWND(), 0, 0);
	Static5->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static5->SetColorText(0, 0, 0);
	Static5->SetColorTextBk(255, 255, 255);
	Static5->SetTransparentTextBk(true);
	Static5->SetColorBrush(255, 255, 255);
	Static5->GAlign = {true, true, true, false};

	AttachFileField = SXGUICrEdit("file", 182, 80, 279, 20, AttachPropsGB->GetHWND(), 0, 0);
	AttachFileField->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachFileField->SetColorText(0, 0, 0);
	AttachFileField->SetColorTextBk(255, 255, 255);
	AttachFileField->SetTransparentTextBk(true);
	AttachFileField->SetColorBrush(255, 255, 255);
	AttachFileField->GAlign = {true, true, true, false};
	SendMessage(AttachFileField->GetHWND(), EM_SETREADONLY, TRUE, NULL);

	AttachFileBrowse = SXGUICrButton("...", 464, 80, 27, 20, 0, AttachPropsGB->GetHWND(), 0, IDC_PT_BROWSE);
	AttachFileBrowse->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachFileBrowse->GAlign = {true, true, true, false};

	AttachFileApply = SXGUICrButton("Apply", 364, 102, 127, 20, 0, AttachPropsGB->GetHWND(), 0, IDC_PT_APPLY);
	AttachFileApply->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AttachFileApply->GAlign = {true, false, true, true};

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
