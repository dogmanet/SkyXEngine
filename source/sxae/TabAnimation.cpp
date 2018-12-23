
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "TabAnimation.h"

TabAnimation::TabAnimation(TabManager * tm):EditorTab(tm)
{
	m_pTabBtn = SXGUICrButtonEx("Animation", 579, 648, 100, 20, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, m_pTM->GetBaseWnd()->getHWND(), 0, 0);
	m_pTabBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pTabBtn->setFollowParentSides(false, true, true, false);
	m_pTabBtn->setUserPtr(this);
	m_pTabBtn->addHandler(EditorTab::TabBtnHandler, WM_LBUTTONUP);

	m_pRoot = SXGUICrGroupBox("Animation", 279, 480, 1016, 167, m_pTM->GetBaseWnd()->getHWND(), 0, 0);
	m_pRoot->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pRoot->setColorText(RGB(0, 0, 0));
	m_pRoot->setColorTextBk(RGB(255, 255, 255));
	m_pRoot->setTransparentTextBk(true);
	m_pRoot->setColorBrush(RGB(255, 255, 255));
	m_pRoot->setFollowParentSides(false, true, true, true);
	m_pRoot->addHandler(Tools::ProxyCommandProc, WM_COMMAND);

	AnimCtlGB = SXGUICrGroupBox("Control", 6, 15, 500, 145, m_pRoot->getHWND(), 0, 0);
	AnimCtlGB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimCtlGB->setColorText(RGB(0, 0, 0));
	AnimCtlGB->setColorTextBk(RGB(255, 255, 255));
	AnimCtlGB->setTransparentTextBk(true);
	AnimCtlGB->setColorBrush(RGB(255, 255, 255));
	AnimCtlGB->setFollowParentSides(1, 1, 1, 0);
	AnimCtlGB->addHandler(Tools::ProxyCommandProc, WM_COMMAND);

	Static7 = SXGUICrStatic("Progress:", 7, 15, 52, 20, AnimCtlGB->getHWND(), 0, 0);
	Static7->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static7->setColorText(RGB(0, 0, 0));
	Static7->setColorTextBk(RGB(255, 255, 255));
	Static7->setTransparentTextBk(true);
	Static7->setColorBrush(RGB(255, 255, 255));
	Static7->setFollowParentSides(1, 1, 0, 0);

	AnimCtlProgress = SXGUICrEdit("Progress", 65, 15, 100, 20, AnimCtlGB->getHWND(), 0, IDC_ANIM_PROGRESS);
	AnimCtlProgress->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimCtlProgress->setColorText(RGB(0, 0, 0));
	AnimCtlProgress->setColorTextBk(RGB(255, 255, 255));
	AnimCtlProgress->setTransparentTextBk(true);
	AnimCtlProgress->setColorBrush(RGB(255, 255, 255));
	AnimCtlProgress->setFollowParentSides(1, 1, 0, 0);
	AnimCtlProgress->modifyStyle(0, 0);

	AnimCtlPlayBtn = SXGUICrButton("Play", 178, 15, 61, 20, SXGUI_BUTTON_IMAGE_NONE, AnimCtlGB->getHWND(), 0, IDC_ANIM_PLAY);
	AnimCtlPlayBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimCtlPlayBtn->setFollowParentSides(1, 1, 0, 0);
	AnimCtlPlayBtn->setEnable(0);
	
	AnimCtlPauseBtn = SXGUICrButton("Pause", 240, 15, 59, 20, SXGUI_BUTTON_IMAGE_NONE, AnimCtlGB->getHWND(), 0, IDC_ANIM_PAUSE);
	AnimCtlPauseBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimCtlPauseBtn->setFollowParentSides(1, 1, 0, 0);
	AnimCtlPauseBtn->setEnable(0);

	AnimCtlProgressTrack = SXGUICrTrackBar("", 6, 43, 489, 20, AnimCtlGB->getHWND(), 0, IDC_ANIM_TB);
	AnimCtlProgressTrack->setFollowParentSides(1, 1, 0, 0);
	AnimCtlProgressTrack->setMinMax(0, 1000);
	AnimCtlProgressTrack->setTickFrequency(0);
	AnimCtlProgressTrack->setTick(500);

	AnimPropsGB = SXGUICrGroupBox("Props", 509, 15, 500, 145, m_pRoot->getHWND(), 0, 0);
	AnimPropsGB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimPropsGB->setColorText(RGB(0, 0, 0));
	AnimPropsGB->setColorTextBk(RGB(255, 255, 255));
	AnimPropsGB->setTransparentTextBk(true);
	AnimPropsGB->setColorBrush(RGB(255, 255, 255));
	AnimPropsGB->setFollowParentSides(1, 1, 1, 1);
	AnimPropsGB->addHandler(Tools::ProxyCommandProc, WM_COMMAND);

	AnimPropLoopCB = SXGUICrCheckBox("Loop animation", 10, 88, 100, 20, AnimPropsGB->getHWND(), 0, IDC_ANIM_LOOPED, false);
	AnimPropLoopCB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimPropLoopCB->setColorText(RGB(0, 0, 0));
	AnimPropLoopCB->setColorTextBk(RGB(255, 255, 255));
	AnimPropLoopCB->setTransparentTextBk(true);
	AnimPropLoopCB->setColorBrush(RGB(255, 255, 255));
	AnimPropLoopCB->setFollowParentSides(1, 1, 0, 0);
	AnimPropLoopCB->modifyStyle(BS_NOTIFY, 0);

	Static8 = SXGUICrStatic("Activity", 10, 42, 43, 20, AnimPropsGB->getHWND(), 0, 0);
	Static8->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static8->setColorText(RGB(0, 0, 0));
	Static8->setColorTextBk(RGB(255, 255, 255));
	Static8->setTransparentTextBk(true);
	Static8->setColorBrush(RGB(255, 255, 255));
	Static8->setFollowParentSides(1, 1, 0, 0);

	AnimPropSpeed = SXGUICrEdit("Speed", 54, 65, 246, 20, AnimPropsGB->getHWND(), 0, IDC_ANIM_SPEED);
	AnimPropSpeed->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimPropSpeed->setColorText(RGB(0, 0, 0));
	AnimPropSpeed->setColorTextBk(RGB(255, 255, 255));
	AnimPropSpeed->setTransparentTextBk(true);
	AnimPropSpeed->setColorBrush(RGB(255, 255, 255));
	AnimPropSpeed->setFollowParentSides(1, 1, 0, 0);

	AnimPropSpeedSpin = SXGUICrUpDownEx(283, 65, 17, 20, 0, WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_SETBUDDYINT | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS, AnimPropsGB->getHWND(), 0, 0, AnimPropSpeed->getHWND());
	AnimPropSpeedSpin->setFollowParentSides(1, 1, 0, 0);

	Static9 = SXGUICrStatic("Name", 10, 19, 43, 20, AnimPropsGB->getHWND(), 0, 0);
	Static9->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static9->setColorText(RGB(0, 0, 0));
	Static9->setColorTextBk(RGB(255, 255, 255));
	Static9->setTransparentTextBk(true);
	Static9->setColorBrush(RGB(255, 255, 255));
	Static9->setFollowParentSides(1, 1, 0, 0);

	AnimPropName = SXGUICrEdit("Name", 54, 19, 246, 20, AnimPropsGB->getHWND(), 0, IDC_ANIM_NAME);
	AnimPropName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimPropName->setColorText(RGB(0, 0, 0));
	AnimPropName->setColorTextBk(RGB(255, 255, 255));
	AnimPropName->setTransparentTextBk(true);
	AnimPropName->setColorBrush(RGB(255, 255, 255));
	AnimPropName->setFollowParentSides(1, 1, 0, 0);

	Static10 = SXGUICrStatic("Speed", 10, 65, 43, 20, AnimPropsGB->getHWND(), 0, 0);
	Static10->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static10->setColorText(RGB(0, 0, 0));
	Static10->setColorTextBk(RGB(255, 255, 255));
	Static10->setTransparentTextBk(true);
	Static10->setColorBrush(RGB(255, 255, 255));
	Static10->setFollowParentSides(1, 1, 0, 0);

	AnimPropActCmb = SXGUICrComboBoxEx(54, 42, 123, 210, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, AnimPropsGB->getHWND(), 0, IDC_ANIM_ACT_NAME);
	AnimPropActCmb->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimPropActCmb->setColorText(RGB(0, 0, 0));
	AnimPropActCmb->setColorTextBk(RGB(255, 255, 255));
	AnimPropActCmb->setTransparentTextBk(true);
	AnimPropActCmb->setColorBrush(RGB(255, 255, 255));
	AnimPropActCmb->setFollowParentSides(1, 1, 0, 0);

	Static11 = SXGUICrStatic("Chance", 184, 42, 44, 20, AnimPropsGB->getHWND(), 0, 0);
	Static11->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static11->setColorText(RGB(0, 0, 0));
	Static11->setColorTextBk(RGB(255, 255, 255));
	Static11->setTransparentTextBk(true);
	Static11->setColorBrush(RGB(255, 255, 255));
	Static11->setFollowParentSides(1, 1, 0, 0);

	AnimPropActChance = SXGUICrEdit("Chance", 229, 42, 71, 20, AnimPropsGB->getHWND(), 0, IDC_ANIM_ACT_CNANCE);
	AnimPropActChance->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimPropActChance->setColorText(RGB(0, 0, 0));
	AnimPropActChance->setColorTextBk(RGB(255, 255, 255));
	AnimPropActChance->setTransparentTextBk(true);
	AnimPropActChance->setColorBrush(RGB(255, 255, 255));
	AnimPropActChance->setFollowParentSides(1, 1, 0, 0);

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
