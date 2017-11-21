#include "TabAnimation.h"

TabAnimation::TabAnimation(TabManager * tm):EditorTab(tm)
{
	m_pTabBtn = SXGUICrButtonEx("Animation", 579, 648, 100, 20, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pTabBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pTabBtn->GAlign = {false, true, true, false};
	m_pTabBtn->SetUserPtr(this);
	m_pTabBtn->AddHandler(EditorTab::TabBtnHandler, WM_LBUTTONUP);

	m_pRoot = SXGUICrGroupBox("Animation", 279, 480, 1016, 167, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pRoot->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pRoot->SetColorText(0, 0, 0);
	m_pRoot->SetColorTextBk(255, 255, 255);
	m_pRoot->SetTransparentTextBk(true);
	m_pRoot->SetColorBrush(255, 255, 255);
	m_pRoot->GAlign = {false, true, true, true};
	m_pRoot->AddHandler(Tools::ProxyCommandProc, WM_COMMAND);

	AnimCtlGB = SXGUICrGroupBox("Control", 6, 15, 500, 145, m_pRoot->GetHWND(), 0, 0);
	AnimCtlGB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimCtlGB->SetColorText(0, 0, 0);
	AnimCtlGB->SetColorTextBk(255, 255, 255);
	AnimCtlGB->SetTransparentTextBk(true);
	AnimCtlGB->SetColorBrush(255, 255, 255);
	AnimCtlGB->GAlign = {1, 1, 1, 0};
	AnimCtlGB->AddHandler(Tools::ProxyCommandProc, WM_COMMAND);

	Static7 = SXGUICrStatic("Progress:", 7, 15, 52, 20, AnimCtlGB->GetHWND(), 0, 0);
	Static7->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static7->SetColorText(0, 0, 0);
	Static7->SetColorTextBk(255, 255, 255);
	Static7->SetTransparentTextBk(true);
	Static7->SetColorBrush(255, 255, 255);
	Static7->GAlign = {1, 1, 0, 0};

	AnimCtlProgress = SXGUICrEdit("Progress", 65, 15, 100, 20, AnimCtlGB->GetHWND(), 0, IDC_ANIM_PROGRESS);
	AnimCtlProgress->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimCtlProgress->SetColorText(0, 0, 0);
	AnimCtlProgress->SetColorTextBk(255, 255, 255);
	AnimCtlProgress->SetTransparentTextBk(true);
	AnimCtlProgress->SetColorBrush(255, 255, 255);
	AnimCtlProgress->GAlign = {1, 1, 0, 0};
	AnimCtlProgress->ModifyStyle(0, 0);

	AnimCtlPlayBtn = SXGUICrButton("Play", 178, 15, 61, 20, 0, AnimCtlGB->GetHWND(), 0, IDC_ANIM_PLAY);
	AnimCtlPlayBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimCtlPlayBtn->GAlign = {1, 1, 0, 0};
	AnimCtlPlayBtn->Enable(0);
	
	AnimCtlPauseBtn = SXGUICrButton("Pause", 240, 15, 59, 20, 0, AnimCtlGB->GetHWND(), 0, IDC_ANIM_PAUSE);
	AnimCtlPauseBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimCtlPauseBtn->GAlign = {1, 1, 0, 0};
	AnimCtlPauseBtn->Enable(0);

	AnimCtlProgressTrack = SXGUICrTrackBar("", 6, 43, 489, 20, AnimCtlGB->GetHWND(), 0, IDC_ANIM_TB);
	AnimCtlProgressTrack->GAlign = {1, 1, 0, 0};
	AnimCtlProgressTrack->SetMinMax(0, 1000);
	AnimCtlProgressTrack->SetTickFrequency(0);
	AnimCtlProgressTrack->SetTick(500);

	AnimPropsGB = SXGUICrGroupBox("Props", 509, 15, 500, 145, m_pRoot->GetHWND(), 0, 0);
	AnimPropsGB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimPropsGB->SetColorText(0, 0, 0);
	AnimPropsGB->SetColorTextBk(255, 255, 255);
	AnimPropsGB->SetTransparentTextBk(true);
	AnimPropsGB->SetColorBrush(255, 255, 255);
	AnimPropsGB->GAlign = {1, 1, 1, 1};
	AnimPropsGB->AddHandler(Tools::ProxyCommandProc, WM_COMMAND);

	AnimPropLoopCB = SXGUICrCheckBox("Loop animation", 10, 88, 100, 20, AnimPropsGB->GetHWND(), 0, IDC_ANIM_LOOPED, false);
	AnimPropLoopCB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimPropLoopCB->SetColorText(0, 0, 0);
	AnimPropLoopCB->SetColorTextBk(255, 255, 255);
	AnimPropLoopCB->SetTransparentTextBk(true);
	AnimPropLoopCB->SetColorBrush(255, 255, 255);
	AnimPropLoopCB->GAlign = {1, 1, 0, 0};
	AnimPropLoopCB->ModifyStyle(BS_NOTIFY, 0);

	Static8 = SXGUICrStatic("Activity", 10, 42, 43, 20, AnimPropsGB->GetHWND(), 0, 0);
	Static8->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static8->SetColorText(0, 0, 0);
	Static8->SetColorTextBk(255, 255, 255);
	Static8->SetTransparentTextBk(true);
	Static8->SetColorBrush(255, 255, 255);
	Static8->GAlign = {1, 1, 0, 0};

	AnimPropSpeed = SXGUICrEdit("Speed", 54, 65, 246, 20, AnimPropsGB->GetHWND(), 0, IDC_ANIM_SPEED);
	AnimPropSpeed->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimPropSpeed->SetColorText(0, 0, 0);
	AnimPropSpeed->SetColorTextBk(255, 255, 255);
	AnimPropSpeed->SetTransparentTextBk(true);
	AnimPropSpeed->SetColorBrush(255, 255, 255);
	AnimPropSpeed->GAlign = {1, 1, 0, 0};

	AnimPropSpeedSpin = SXGUICrUpDownEx(283, 65, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_SETBUDDYINT | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, AnimPropsGB->GetHWND(), 0, 0, AnimPropSpeed->GetHWND());
	AnimPropSpeedSpin->GAlign = {1, 1, 0, 0};

	Static9 = SXGUICrStatic("Name", 10, 19, 43, 20, AnimPropsGB->GetHWND(), 0, 0);
	Static9->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static9->SetColorText(0, 0, 0);
	Static9->SetColorTextBk(255, 255, 255);
	Static9->SetTransparentTextBk(true);
	Static9->SetColorBrush(255, 255, 255);
	Static9->GAlign = {1, 1, 0, 0};

	AnimPropName = SXGUICrEdit("Name", 54, 19, 246, 20, AnimPropsGB->GetHWND(), 0, IDC_ANIM_NAME);
	AnimPropName->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimPropName->SetColorText(0, 0, 0);
	AnimPropName->SetColorTextBk(255, 255, 255);
	AnimPropName->SetTransparentTextBk(true);
	AnimPropName->SetColorBrush(255, 255, 255);
	AnimPropName->GAlign = {1, 1, 0, 0};

	Static10 = SXGUICrStatic("Speed", 10, 65, 43, 20, AnimPropsGB->GetHWND(), 0, 0);
	Static10->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static10->SetColorText(0, 0, 0);
	Static10->SetColorTextBk(255, 255, 255);
	Static10->SetTransparentTextBk(true);
	Static10->SetColorBrush(255, 255, 255);
	Static10->GAlign = {1, 1, 0, 0};

	AnimPropActCmb = SXGUICrComboBoxEx("", 54, 42, 123, 210, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, AnimPropsGB->GetHWND(), 0, IDC_ANIM_ACT_NAME);
	AnimPropActCmb->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimPropActCmb->SetColorText(0, 0, 0);
	AnimPropActCmb->SetColorTextBk(255, 255, 255);
	AnimPropActCmb->SetTransparentTextBk(true);
	AnimPropActCmb->SetColorBrush(255, 255, 255);
	AnimPropActCmb->GAlign = {1, 1, 0, 0};

	Static11 = SXGUICrStatic("Chance", 184, 42, 44, 20, AnimPropsGB->GetHWND(), 0, 0);
	Static11->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static11->SetColorText(0, 0, 0);
	Static11->SetColorTextBk(255, 255, 255);
	Static11->SetTransparentTextBk(true);
	Static11->SetColorBrush(255, 255, 255);
	Static11->GAlign = {1, 1, 0, 0};

	AnimPropActChance = SXGUICrEdit("Chance", 229, 42, 71, 20, AnimPropsGB->GetHWND(), 0, IDC_ANIM_ACT_CNANCE);
	AnimPropActChance->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimPropActChance->SetColorText(0, 0, 0);
	AnimPropActChance->SetColorTextBk(255, 255, 255);
	AnimPropActChance->SetTransparentTextBk(true);
	AnimPropActChance->SetColorBrush(255, 255, 255);
	AnimPropActChance->GAlign = {1, 1, 0, 0};

	Show(false);

}

TabAnimation::~TabAnimation()
{
	mem_delete(AnimCtlGB);
	mem_delete(Static7);
	mem_delete(AnimCtlProgress);
	mem_delete(AnimCtlPlayBtn);
	mem_delete(AnimCtlPauseBtn);
	mem_delete(AnimCtlProgressTrack);
	mem_delete(AnimPropsGB);
	mem_delete(AnimPropLoopCB);
	mem_delete(Static8);
	mem_delete(AnimPropSpeed);
	mem_delete(AnimPropSpeedSpin);
	mem_delete(Static9);
	mem_delete(AnimPropName);
	mem_delete(Static10);
	mem_delete(AnimPropActCmb);
	mem_delete(Static11);
	mem_delete(AnimPropActChance);
}
